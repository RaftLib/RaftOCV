#include <opencv2/core/core.hpp>
#include <pcl/pcl_base.h>
#include <pcl/visualization/cloud_viewer.h>
#include "StereoBMFilter.h"

struct StereoBMFilter_p {
    StereoBMFilter& t;
    std::shared_ptr<StereoCalibrationResults> calib;

    cv::Ptr<cv::StereoBM> sbm;
    cv::Size imageSize = { 0, 0 };
    cv::Mat depth;
    cv::Mat left, right;

    StereoBMFilter_p(StereoBMFilter &t) : t(t), sbm(cv::StereoBM::create(160, 21)) {
        sbm->setPreFilterSize(5);
        sbm->setPreFilterCap(25);
        sbm->setMinDisparity(1);
        sbm->setTextureThreshold(1);
        sbm->setUniquenessRatio(5);
        sbm->setSpeckleWindowSize(500);
        sbm->setSpeckleRange(20);
        sbm->setDisp12MaxDiff(1);
    }

    raft::kstatus run() {
        if(t.hasCalibInput && t.input["calib"].size()) {
            t.input["calib"].pop(calib);
        }

        auto &in = t.input["0"].template peek<cv::Mat>();

        cv::Size s(in.cols / 2, in.rows);
        if(s != imageSize) {
            imageSize = s;

            depth = cv::Mat(s.height, s.width, CV_16S );

            left.create(imageSize, CV_8UC1);
            right.create(imageSize, CV_8UC1);
        }

        cv::Mat right_roi(in, cv::Rect(0, 0, s.width,s.height));
        cv::Mat left_roi(in, cv::Rect(s.width, 0, s.width,s.height));

        cv::cvtColor(left_roi, left, cv::COLOR_RGB2GRAY);
        cv::cvtColor(right_roi, right, cv::COLOR_RGB2GRAY);

        t.input["0"].unpeek();
        t.input["0"].recycle(1);

        sbm->compute(left, right, depth);

        auto &out = t.output["0"].template allocate<MetadataEnvelope<cv::Mat>>();
        out = depth.clone();
        t.output["0"].send();


        if(calib && t.hasPCOutput) {
            cv::Mat points, points1, disp8;
            cv::normalize(depth, disp8, 0, 255, CV_MINMAX, CV_8U);
            cv::reprojectImageTo3D(disp8, points, calib->Q, true);

            pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);

            for (int rows = 0; rows < points.rows; ++rows) {
                for (int cols = 0; cols < points.cols; ++cols) {
                    cv::Point3f point = points.at<cv::Point3f>(rows, cols);
                    double dist = cv::norm(point);
                    if(std::isfinite(dist) && dist < 5) {
                        pcl::PointXYZRGB pcl_point_rgb;
                        pcl_point_rgb.x = point.x;    // rgb PointCloud
                        pcl_point_rgb.y = point.y;
                        pcl_point_rgb.z = point.z;

                        cv::Vec3b intensity = right_roi.at<cv::Vec3b>(rows, cols); //BGR
                        uint32_t rgb = (static_cast<uint32_t>(intensity[2]) << 16 |
                                        static_cast<uint32_t>(intensity[1]) << 8 | static_cast<uint32_t>(intensity[0]));
                        pcl_point_rgb.rgb = *reinterpret_cast<float *>(&rgb);

                        cloud_xyzrgb->push_back(pcl_point_rgb);
                    }
                }
            }

            t.output["pointcloud"].push(cloud_xyzrgb);
        }

        return raft::proceed;
    }
};

StereoBMFilter::StereoBMFilter() : p(new StereoBMFilter_p(*this)) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}

StereoBMFilter::~StereoBMFilter() {

}

raft::kstatus StereoBMFilter::run() {
    return p->run();
}

auto StereoBMFilter::newCalibInput() -> decltype(input["calib"]) {
    hasCalibInput = true;
    input.addPort<std::shared_ptr<CalibrationResults>>("calib");
    return input["calib"];
}

auto StereoBMFilter::newPointcloudOutput() -> decltype((*this)["pointcloud"]) {
    hasPCOutput = true;
    output.addPort<pcl::PointCloud<pcl::PointXYZRGB>::Ptr>("pointcloud");
    return (*this)["pointcloud"];
}

void StereoBMFilter::SetCalibration(const StereoCalibrationResults &results) {
    p->calib = std::make_shared<StereoCalibrationResults>(results);
}
