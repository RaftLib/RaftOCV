#include <opencv2/opencv.hpp>
#include "DisplayFrameSink.h"
#include "Metadata.h"
#include "condition_variable"

static struct WindowThread {
    std::thread thread;
    bool shutDown = false;

    std::function<void()> runF;
    std::mutex lock;
    std::condition_variable cv;

    void run(std::function<void()> f) {
        std::unique_lock<std::mutex> l(lock);
        runF = f;
        cv.notify_all();
    }
    void _run(std::function<void()> f) {
        f();
    }
    ~WindowThread() {
        shutDown = true;
        cv.notify_all();
        if(thread.joinable())
            thread.join();
    }
    WindowThread() {
        thread = std::thread([&]{
           std::unique_lock<std::mutex> l(lock);
           while(shutDown == false) {
               cv.wait_for(l, std::chrono::milliseconds(40));
               if(runF) {
                   runF();
                   runF = nullptr;
               }
               cv::waitKey(1);
           }
        });
    }
} windowThread;

raft::kstatus DisplayFrameSink::run() {
    auto &img_in = input["0"].template peek<MetadataEnvelope<cv::Mat>>();
    auto mat = img_in.clone();

    windowThread.run([this, mat] {
        cv::imshow(frameName, mat);
        //cv::displayOverlay(frameName, std::to_string(img_in.Metadata().originId));
    });

    input["0"].unpeek();
    input["0"].recycle(1);

    return raft::proceed;
}

DisplayFrameSink::DisplayFrameSink(const std::string &frameName) : frameName(frameName) {
    input.addPort<MetadataEnvelope<cv::Mat>>("0");
    windowThread.run([this]{ cv::namedWindow(this->frameName, cv::WINDOW_AUTOSIZE | cv::WINDOW_KEEPRATIO | CV_GUI_EXPANDED); });
}
