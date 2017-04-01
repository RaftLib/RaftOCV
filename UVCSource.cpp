#include <libv4l2.h>
#include "UVCSource.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <opencv2/core/mat.hpp>
#include "Metadata.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
    void   *start;
    size_t length;
};


static int xioctl(int fh, int request, void *arg)
{
    int r;

    do {
        r = v4l2_ioctl(fh, request, arg);
    } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

    if (r == -1) {
        fprintf(stderr, "error %d, %s\\n", errno, strerror(errno));
    }
    return r;
}

struct UVCSource_p {
    UVCSource& t;
    size_t frameId = 0;
    struct v4l2_format              fmt;
    struct v4l2_buffer              buf;
    struct v4l2_requestbuffers      req;
    fd_set                          fds;
    struct timeval                  tv;
    int                             r, fd = -1;
    unsigned int                    i, n_buffers;

    struct buffer                   *buffers;
    uint32_t frameWidth = 1748;
    uint32_t frameHeight = 408;

    UVCSource_p(const std::string& source, UVCSource &t, uint32_t* width = 0, uint32_t* height = 0) : t(t) {
        fd = v4l2_open(source.c_str(), O_RDWR | O_NONBLOCK, 0);
        if (fd < 0) {
            perror("Cannot open device");
            exit(EXIT_FAILURE);
        }

        v4l2_control c;
        c.id = V4L2_CID_EXPOSURE_AUTO;
        c.value = V4L2_EXPOSURE_AUTO;
        xioctl(fd, VIDIOC_S_CTRL, &c);

        if(width)
            frameWidth = *width;
        if(height)
            frameHeight = *height;

        CLEAR(fmt);
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width       = frameWidth;
        fmt.fmt.pix.height      = frameHeight;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
        xioctl(fd, VIDIOC_S_FMT, &fmt);
        if ((fmt.fmt.pix.width != frameWidth) || (fmt.fmt.pix.height != frameHeight)) {
            printf("Warning: driver is sending image at %dx%d\\n",
                   fmt.fmt.pix.width, fmt.fmt.pix.height);
            frameWidth = fmt.fmt.pix.width;
            frameHeight = fmt.fmt.pix.height;
        }

        if(width != 0)
            *width = frameWidth;
        if(height != 0)
            *height = frameHeight;

        CLEAR(req);
        req.count = 2;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;
        xioctl(fd, VIDIOC_REQBUFS, &req);

        buffers = (buffer *) calloc(req.count, sizeof(*buffers));
        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
            CLEAR(buf);

            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = n_buffers;

            xioctl(fd, VIDIOC_QUERYBUF, &buf);

            buffers[n_buffers].length = buf.length;
            buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
                                                 PROT_READ | PROT_WRITE, MAP_SHARED,
                                                 fd, buf.m.offset);

            if (MAP_FAILED == buffers[n_buffers].start) {
                perror("mmap");
                exit(EXIT_FAILURE);
            }
        }

        for (i = 0; i < n_buffers; ++i) {
            CLEAR(buf);
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;
            xioctl(fd, VIDIOC_QBUF, &buf);
        }

        enum v4l2_buf_type              type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        xioctl(fd, VIDIOC_STREAMON, &type);
    }

    void RunIteration() {
        do {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* Timeout. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);
        } while ((r == -1 && (errno = EINTR)));
        if (r == -1) {
            perror("select");
            return;
        }

        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        xioctl(fd, VIDIOC_DQBUF, &buf);
        //std::cerr << t.output["0"].size() << std::endl;
        //t.output[ "0" ].recycle(t.output["0"].size());

        MetadataEnvelope<cv::Mat> frame;

        cv::Mat yuv(frameHeight, frameWidth, CV_8UC2, buffers[buf.index].start);
        frame.Metadata().originId = frameId++;
        frame = yuv.clone();

        t.output[ "0" ].push(frame);

        xioctl(fd, VIDIOC_QBUF, &buf);
    }
};


raft::kstatus UVCSource::run() {
    p->RunIteration();
    return raft::proceed;
}

UVCSource::UVCSource(const std::string& source) : p(new UVCSource_p(source, *this)) {
    output.addPort < MetadataEnvelope<cv::Mat> > ("0");
}

UVCSource::UVCSource(uint32_t& width, uint32_t& height, const std::string &source) : p(new UVCSource_p(source, *this, &width, &height)) {
    output.addPort < MetadataEnvelope<cv::Mat> > ("0");
}

UVCSource::~UVCSource() {}
