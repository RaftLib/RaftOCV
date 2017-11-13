#include <opencv/cv.hpp>
#include "StabalizeVideo.h"

#include "Metadata.h"

using namespace cv;
using namespace std;

class Tracker {
    Mat             prevGray;
public:
    bool            freshStart;
    Mat_<float>     rigidTransform;
    vector<Point2f> trackedFeatures;
    vector<Point2f> prevTrackedFeatures;

    Tracker():freshStart(true) {
        rigidTransform = Mat::eye(3,3,CV_32FC1); //affine 2x3 in a 3x3 matrix
    }

    bool processImage(Mat& img) {
        Mat gray; cvtColor(img,gray,CV_BGR2GRAY);
        vector<Point2f> corners;
        if(trackedFeatures.size() < 200) {
            goodFeaturesToTrack(gray,corners,300,0.01,10);
            cout << "found " << corners.size() << " features\n";
            for (int i = 0; i < corners.size(); ++i) {
                trackedFeatures.push_back(corners[i]);
            }
        }

        if(!prevGray.empty()) {
            vector<uchar> status; vector<float> errors;
            calcOpticalFlowPyrLK(prevGray,gray,trackedFeatures,corners,status,errors,Size(10,10));

            if(countNonZero(status) < status.size() * 0.8) {
                cout << "cataclysmic error \n";
                rigidTransform = Mat::eye(3,3,CV_32FC1);
                trackedFeatures.clear();
                prevGray.release();
                freshStart = true;
                return false;
            } else
                freshStart = false;

            Mat_<float> newRigidTransform = estimateRigidTransform(trackedFeatures,corners,false);
            if(newRigidTransform.cols == 0) {
                return false;
            }

            Mat_<float> nrt33 = Mat_<float>::eye(3,3);
            newRigidTransform.copyTo(nrt33.rowRange(0,2));
            rigidTransform *= nrt33;

            prevTrackedFeatures.clear();
            for (int i = 0; i < status.size(); ++i) {
                if(status[i]) {
                    prevTrackedFeatures.push_back(trackedFeatures[i]);
                }
            }
            trackedFeatures.clear();
            for (int i = 0; i < status.size(); ++i) {
                if(status[i]) {
                    trackedFeatures.push_back(corners[i]);
                }
            }
        }
        gray.copyTo(prevGray);
        return true;
    }
};

struct StabalizeVideo_p {
    StabalizeVideo& self;

    cv::Mat prev;
    std::vector<cv::Point2f> points[2];
    std::vector<uchar> status;
    std::vector<float> err;

    cv::Size subPixWinSize = {10,10}, winSize = {31,31};

    cv::TermCriteria termcrit = {cv::TermCriteria::COUNT|cv::TermCriteria::EPS, 20, 0.03};
    const int MAX_COUNT = 5000;

    Tracker tracker;

    raft::kstatus run() {
        MetadataEnvelope<cv::Mat> img_in;
        self.input["0"].pop(img_in);

        MetadataEnvelope<cv::Mat> out(img_in.Metadata());

        {
            auto& oldTracked = tracker.prevTrackedFeatures;

            auto newMat = img_in.clone();
            if(!tracker.processImage(newMat)) {
                return raft::stop;
            }

            cv::Mat invTrans = tracker.rigidTransform.inv(cv::DECOMP_SVD);

            if(self.hasVideoOut) {
                auto &newTracked = tracker.trackedFeatures;
                warpAffine(newMat, out, invTrans.rowRange(0, 2), out.size());
            }
        }

        if(out.cols && self.hasVideoOut) {
            self.output["0"].push(out);
        }
        if(tracker.rigidTransform.cols && self.hasTxOut) {
            self.output["tx"].push(tracker.rigidTransform.clone());
        }
        return raft::proceed;
    }

    StabalizeVideo_p(StabalizeVideo &self) : self(self) {}
};

StabalizeVideo::StabalizeVideo() : p(new StabalizeVideo_p(*this)) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
}

raft::kstatus StabalizeVideo::run() {
    return p->run();
}

StabalizeVideo::~StabalizeVideo() {

}

raft::kernel &StabalizeVideo::videoOut() {
    if(!hasVideoOut) {
        hasVideoOut = true;
        output.addPort<MetadataEnvelope<cv::Mat>>("0");
    }
    return (*this)["0"];
}

raft::kernel &StabalizeVideo::txOut() {
    if(!hasTxOut) {
        hasTxOut = true;
        output.addPort<cv::Mat>("tx");
    }
    return (*this)["tx"];
}

StabalizeVideo &StabalizeVideo::operator[](const std::string &&portname) {
    raft::kernel::operator[](std::string(portname));
    return *this;
}

