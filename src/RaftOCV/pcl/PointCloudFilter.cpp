#include "PointCloudFilter.h"

#undef restore

#include "pcl/common/common_headers.h"
#include "pcl/io/io.h"
#include "pcl/visualization/pcl_visualizer.h"
#include <pcl/io/pcd_io.h>
#include "pcl/point_cloud.h"
#include "pcl/visualization/cloud_viewer.h"

raft::kstatus PointCloudFilter::run() {
    return raft::proceed;
}

PointCloudFilter::PointCloudFilter() {

}
