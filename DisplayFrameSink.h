#include <raft>

class DisplayFrameSink : public raft::kernel {
public:
    raft::kstatus run() override;
    DisplayFrameSink(const std::string& frameName);

private:
    std::string frameName;
};


