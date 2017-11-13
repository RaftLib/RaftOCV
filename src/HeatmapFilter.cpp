#include <opencv2/imgproc.hpp>
#include "HeatmapFilter.h"
#include "Metadata.h"

HeatmapFilter::HeatmapFilter(void)
{
	input.addPort<MetadataEnvelope<cv::Mat>>("0");
	output.addPort<MetadataEnvelope<cv::Mat>>("0");
}


HeatmapFilter::~HeatmapFilter(void)
{
}

raft::kstatus HeatmapFilter::run() {
	auto &img = input["0"].template peek<MetadataEnvelope<cv::Mat>>();

	double nMin, nMax;
	cv::minMaxLoc( img, &nMin, &nMax);
	minVal = .99 * minVal + .01 * nMin;
	maxVal = .99 * maxVal + .01 * nMax;

	auto &out = output["0"].template allocate<MetadataEnvelope<cv::Mat>>();
	out.create(img.rows, img.cols, CV_8UC1);
	//-- 4. Display it as a CV_8UC1 image
	img.convertTo( out, CV_8UC1, 255/(maxVal - minVal));

	input["0"].unpeek();
	input["0"].recycle(1);

	cv::applyColorMap(out, out, cv::COLORMAP_JET);
	output["0"].send();

	return raft::proceed;
}
