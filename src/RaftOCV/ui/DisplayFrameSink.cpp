#include <opencv2/opencv.hpp>
#include <src/RaftOCV/ui/KeyListener.h>
#include "DisplayFrameSink.h"
#include "src/RaftOCV/utility/Metadata.h"
#include "condition_variable"

std::map<std::string, DisplayFrameSink*> frames;
void TileWindows() {
    auto height = 1050;
    int x,y;
    x = 0;
    y = 30;
    size_t mx = 0;
    int bx = 20;
    int by = 20;
    std::set<std::pair<int, DisplayFrameSink*>> order;
    for(auto it = frames.begin();it != frames.end();it++) {
        order.insert(std::make_pair(-it->second->height, it->second));
    }
    for(auto it = order.begin();it != order.end();it++){
        if(it->second == 0) continue;
        if(y > 0 && y + (30 + it->second->height) > height) {
            y = 30;
            x += mx + bx;
            mx = 0;
        }
        std::cerr << it->second->FrameName() << " moved to " << x << ", " << y << "("
                  << it->second->width << ", " << it->second->height << ")" <<  std::endl;
        cv::moveWindow(it->second->FrameName(), x,y);
        y += it->second->height + by;
        mx = std::max(mx, it->second->width);
    }
}


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
        cv.wait(l);
    }
    void _run(std::function<void()> f) {
        f();
    }
    ~WindowThread() {
        shutDown = true;
        cv.notify_all();
        if(thread.joinable() && thread.get_id() != std::this_thread::get_id())
            thread.join();
        else
            thread.detach();
    }
    WindowThread() {
        thread = std::thread([&]{
           std::unique_lock<std::mutex> l(lock);
           while(!shutDown) {
               cv.wait_for(l, std::chrono::milliseconds(40));

               if(shutDown)
                   break;

               auto wait = cv::waitKey(1);
               switch(wait) {
                   case 't':
                       TileWindows();
                       break;
                   case 'q':
                       exit(0);
                   case -1:
                       break;
                   default: {
                       std::lock_guard<std::mutex> l(listen_mutex);
                       for(auto& l : listeners) {
                           l->OnKey(wait);
                       }
                       std::cerr << wait << std::endl;
                   }
               }
               if(runF) {
                   runF();
                   runF = nullptr;
                   cv.notify_all();
               }
           }
        });
    }

    std::mutex listen_mutex;
    std::set<KeyListener_t*> listeners;
    void Register(KeyListener_t& listener) {
        std::lock_guard<std::mutex> l(listen_mutex);
        listeners.insert(&listener);
    }
    void Deregister(KeyListener_t& listener) {
        std::lock_guard<std::mutex> l(listen_mutex);
        listeners.erase(&listener);
    }

} windowThread;

void Register(KeyListener_t& listener) {
    windowThread.Register(listener);
}
void Deregister(KeyListener_t& listener) {
    windowThread.Deregister(listener);
}

raft::kstatus DisplayFrameSink::run() {
    MetadataEnvelope<cv::UMat> img_in;
    input["0"].pop(img_in);
    width = img_in.cols;
    height = img_in.rows;

    if(width * height == 0)
        return raft::proceed;

    windowThread.run([&] {
        if(!inited)
            cv::namedWindow(this->frameName, cv::WINDOW_AUTOSIZE | cv::WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
        inited =true;
        cv::imshow(frameName, img_in);
        //cv::displayOverlay(frameName, std::to_string(img_in.Metadata().originId));
    });

    return raft::proceed;
}

DisplayFrameSink::DisplayFrameSink(const std::string &frameName) : frameName(frameName) {
    input.addPort<MetadataEnvelope<cv::UMat>>("0");
    frames[frameName] = this;
}

const std::string& DisplayFrameSink::FrameName() const {
    return frameName;
}

