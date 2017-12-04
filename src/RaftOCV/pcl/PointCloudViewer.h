#pragma once
#include <raft>
#undef restore
#include <pcl/visualization/cloud_viewer.h>

class PointCloudViewer : public raft::kernel {
    pcl::visualization::CloudViewer viewer;
    public:
    PointCloudViewer(const std::string& name = "Pointcloud");

    raft::kstatus run() override;
};