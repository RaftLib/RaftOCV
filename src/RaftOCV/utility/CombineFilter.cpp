
#include <src/RaftOCV/utility/Metadata.h>
#include <opencv2/core/mat.hpp>
#include "CombineFilter.h"

CombineFilter::CombineFilter() {
    output.addPort< MetadataEnvelope<cv::UMat> > ("0");
}

void CombineFilter::OnMatch(MetadataEnvelope<std::pair<cv::UMat, cv::UMat>> &in) {

    auto& A = in.first;
    auto& B = in.second;

    if(A.empty() || B.empty())
        return;

    MetadataEnvelope<cv::UMat> C;
    C.create(std::max(A.size().height, B.size().height),
             A.size().width + B.size().width, A.type());

    cv::UMat left_roi(C, cv::Rect(0, 0, A.size().width, A.size().height));
    A.copyTo(left_roi);
    cv::UMat right_roi(C, cv::Rect(A.size().width, 0, B.size().width, B.size().height));
    B.copyTo(right_roi);

    output["0"].push(C);
}
