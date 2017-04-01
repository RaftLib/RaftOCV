#include "StereoSubgraph.h"

StereoSubgraph::StereoSubgraph() : undistortR(true), undistortL(false), dup(1), stereoDup(0), rightOut(1), leftOut(1) {

}

StereoSubgraph::StereoSubgraph(const StereoCalibrationResults &r) : undistortR(true, r), undistortL(false, r), dup(1), stereoDup(0), rightOut(1), leftOut(1) {
    stereo.SetCalibration(r);
}

StereoSubgraph::StereoSubgraph(const std::string &r) : StereoSubgraph(StereoCalibrationResults(r)) {

}

auto StereoSubgraph::in(const char *name) -> decltype(split[name]) {
    return split[name];
}

auto StereoSubgraph::newImageOutput() -> decltype(dup["0"]) {
    return dup[std::to_string(dup.addPort())];
}

auto StereoSubgraph::newStereoOutput() -> decltype(stereoDup["0"]) {
    return stereoDup[std::to_string(stereoDup.addPort())];
}

void StereoSubgraph::connect(raft::map &m) {
    m += split["0"] >> rightOut["0"]["0"] >> undistortR["0"]["0"] >> combine["0"];
    m += split["1"] >> leftOut["0"]["0"]  >> undistortL["0"]["0"] >> combine["1"];
    m += combine >> dup["0"]["0"] >> stereo >> stereoDup["0"];
}

auto StereoSubgraph::newLeftOutput() -> decltype(leftOut["0"]) {
    return leftOut[std::to_string(leftOut.addPort())];
}

auto StereoSubgraph::newRightOutput() -> decltype(rightOut["0"]) {
    return rightOut[std::to_string(rightOut.addPort())];
}

auto StereoSubgraph::pointCloudOutput() -> decltype(stereo["pointcloud"]) {
    return stereo.newPointcloudOutput();
}
