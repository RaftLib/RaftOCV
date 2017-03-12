#include <raft>

class ConvertColorFilter : public raft::kernel {
    int16_t ConversionType;
public:
    ConvertColorFilter(int16_t ConversionType);

    raft::kstatus run() override;
};


