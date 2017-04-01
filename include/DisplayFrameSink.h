#include <raft>

class DisplayFrameSink : public raft::kernel {
public:
    raft::kstatus run() override;
    DisplayFrameSink(const std::string& frameName);
    size_t width = 0, height = 0;
    const std::string& FrameName() const;
private:
    std::string frameName;
};


