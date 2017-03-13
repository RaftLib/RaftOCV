#include <raft>
#include <opencv2/imgproc/types_c.h>

class ConvertColorFilter : public raft::kernel {
    int16_t ConversionType;
public:
    ConvertColorFilter(int16_t ConversionType);

    raft::kstatus run() override;
};


