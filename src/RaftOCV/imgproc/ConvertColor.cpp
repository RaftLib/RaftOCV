//
// Created by justin on 3/12/17.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "ConvertColor.h"
#include "src/RaftOCV/utility/Metadata.h"

ConvertColorFunctor::ConvertColorFunctor(int16_t ConversionType) : ConversionType(ConversionType) {
}

MetadataEnvelope<cv::UMat> ConvertColorFunctor::operator()(const MetadataEnvelope<cv::UMat> &img_in) {
    MetadataEnvelope<cv::UMat> out;
    out.metadata = img_in.Metadata();
    cv::cvtColor(img_in, out, ConversionType);

    return out;
}
