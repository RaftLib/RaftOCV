
#include "pcl/PointCloudViewer.h"

PointCloudViewer::PointCloudViewer(const std::string &name) : viewer(name) {
    input.addPort<pcl::PointCloud<pcl::PointXYZRGB>::Ptr>("pointcloud");
}

raft::kstatus PointCloudViewer::run() {
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr pc;
    input["pointcloud"].pop(pc);

    viewer.showCloud(pc);
    return raft::proceed;
}
