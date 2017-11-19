
#include <src/utility/Metadata.h>
#include <opencv2/core/mat.hpp>
#include "CombineFilter.h"

CombineFilter::CombineFilter() {
    output.addPort< MetadataEnvelope<cv::Mat> > ("0");
}

void CombineFilter::OnMatch(MetadataEnvelope<std::pair<cv::Mat, cv::Mat>> &in) {

    auto& A = in.first;
    auto& B = in.second;

    if(A.empty() || B.empty())
        return;

    MetadataEnvelope<cv::Mat> C;
    C.create(std::max(A.size().height, B.size().height),
             A.size().width + B.size().width, A.type());

    cv::Mat left_roi(C, cv::Rect(0, 0, A.size().width, A.size().height));
    A.copyTo(left_roi);
    cv::Mat right_roi(C, cv::Rect(A.size().width, 0, B.size().width, B.size().height));
    B.copyTo(right_roi);

    output["0"].push(C);
}
