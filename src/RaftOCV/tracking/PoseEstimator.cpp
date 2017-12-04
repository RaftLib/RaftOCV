#include "PoseEstimator.h"

#include <opencv2/opencv.hpp>
#include "src/RaftOCV/utility/Metadata.h"

// OpenPose dependencies
#include <openpose/core/headers.hpp>
#include <openpose/filestream/headers.hpp>
#include <openpose/gui/headers.hpp>
#include <openpose/pose/headers.hpp>
#include <openpose/utilities/headers.hpp>

struct HumanPoseEstimationKernel_p {
    op::PoseExtractorCaffe poseExtractorCaffe;
    op::CvMatToOpInput cvMatToOpInput;
    op::CvMatToOpOutput cvMatToOpOutput;
    op::OpOutputToCvMat opOutputToCvMat;

    op::ScaleAndSizeExtractor scaleAndSizeExtractor;

    op::PoseCpuRenderer poseRenderer;

    HumanPoseEstimationKernel_p(const op::PoseModel poseModel, const std::string& modelFolder, const int gpuId,
                                const std::vector<op::HeatMapType>& heatMapTypes = {},
                                const op::ScaleMode heatMapScale = op::ScaleMode::ZeroToOne,
                                const bool enableGoogleLogging = true) :
            poseExtractorCaffe(poseModel, modelFolder, gpuId, heatMapTypes, heatMapScale, enableGoogleLogging),
             scaleAndSizeExtractor(op::flagsToPoint("368x368"),
                                   op::flagsToPoint("-1x-1"), 1, .3),
            poseRenderer(poseModel, (float)0.05, true, (float)0.6)
    {

    }

    bool hasInit = false;

    cv::Mat run(const cv::Mat& inputImage) {

        if(hasInit == false) {
            hasInit = true;
            poseExtractorCaffe.initializationOnThread();
            poseRenderer.initializationOnThread();
        }
        const op::Point<int> imageSize{inputImage.cols, inputImage.rows};
        // Step 2 - Get desired scale sizes
        std::vector<double> scaleInputToNetInputs;
        std::vector<op::Point<int>> netInputSizes;
        double scaleInputToOutput;
        op::Point<int> outputResolution;
        std::tie(scaleInputToNetInputs, netInputSizes, scaleInputToOutput, outputResolution)
                = scaleAndSizeExtractor.extract(imageSize);
        // Step 3 - Format input image to OpenPose input and output formats
        const auto netInputArray = cvMatToOpInput.createArray(inputImage, scaleInputToNetInputs, netInputSizes);
        auto outputArray = cvMatToOpOutput.createArray(inputImage, scaleInputToOutput, outputResolution);
        // Step 4 - Estimate poseKeypoints
        poseExtractorCaffe.forwardPass(netInputArray, imageSize, scaleInputToNetInputs);
        const auto poseKeypoints = poseExtractorCaffe.getPoseKeypoints();
        // Step 5 - Render poseKeypoints
        poseRenderer.renderPose(outputArray, poseKeypoints, scaleInputToOutput);
        // Step 6 - OpenPose output format to cv::Mat
        return opOutputToCvMat.formatToCvMat(outputArray);
    }
};

raft::kstatus HumanPoseEstimationKernel::run() {
    MetadataEnvelope<cv::Mat> img_in;
    input["0"].pop(img_in);

    MetadataEnvelope<cv::Mat> out(img_in.Metadata());

    out = p->run(img_in);

    output["0"].push(out);
    return raft::proceed;
}

HumanPoseEstimationKernel::HumanPoseEstimationKernel() : p(new HumanPoseEstimationKernel_p(op::flagsToPoseModel("COCO"),
                                                                                           "/home/justin/source/RaftOCV/.cget-bin/0.1/packages/OpenPose_HEAD/models/",
                                                                                           0, {}, op::ScaleMode::ZeroToOne, false)){
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    output.addPort<MetadataEnvelope<cv::Mat>>("0");
}

HumanPoseEstimationKernel::~HumanPoseEstimationKernel() {

}

