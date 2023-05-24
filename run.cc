#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define GSTREAMER 1 
//#define GUI
#define RTSP


#ifdef RTSP
#define LOC_SRC     "rtspsrc"
#define LOC_CONTENT "rtsp://admin:nanchao.org@192.168.31.107:554/Streaming/Channels/101"
#define LOC_PARAM   "latency=0"
#else
#define LOC_SRC     "filesrc"
#define LOC_CONTENT "test.mp4"
#define LOC_PARAM   ""
#endif


float get_time_ms(struct timeval start, struct timeval end)
{
    double start_us = start.tv_sec * 1000000 + start.tv_usec;
    double end_us = end.tv_sec * 1000000 + end.tv_usec;
    float time_ms = (end_us - start_us) / 1000;
    return time_ms;
}


int main(int argc, const char *argv[])
{
#ifdef GSTREAMER
    char gsurl[200];
    sprintf(gsurl, "%s location=%s %s ! rtph264depay ! h264parse ! mppvideodec ! videoconvert ! video/x-raw,format=BGR ! appsink",
            LOC_SRC, LOC_CONTENT, LOC_PARAM);
    printf("%s\n", gsurl);
    VideoCapture cap = VideoCapture(gsurl, CAP_GSTREAMER);
#else
    VideoCapture cap = VideoCapture(LOC_CONTENT, CAP_ANY);
#endif

    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    float fps = cap.get(CAP_PROP_FPS);
    printf("basic = %dx%d @ %.1f\n", width, height, fps);

    float time_ms;
    struct timeval start, end;
    char fps_log[100];
    int counter = 0;
    int key;
    Mat frame;
    bool ret = false;
    while (cap.isOpened()) {
        gettimeofday(&start, NULL);

        // read and count frame
        ret = cap.read(frame);
        if (!ret) {
            printf("done\n");
            break;
        }

        // Process
        usleep(1 / fps * 1000000); // convert s to us
        counter += 1;
        if (counter == 1) {
            imwrite("frame_1.jpg", frame);
        }

#ifdef GUI
        // Control
        key = waitKey(1) & 0xFF;
        if (key == (int)(' ')) {
            printf("pause\n");
            key = waitKey(0) && 0xFF;
        }
        if (key == (int)('q')) {
            printf("quit\n");
            break;
        }
#endif

        // FPS
        gettimeofday(&end, NULL);
        time_ms = get_time_ms(start, end);
        sprintf(fps_log, "FPS = %.1f", 1 / time_ms * 1000);
        putText(frame, fps_log, Point(30, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 3);
        printf("Frame #%d = %.2fms (acutal FPS = %.1f)\n", counter, time_ms, 1 / time_ms * 1000);

#ifdef GUI
        // Show
        imshow("frame", frame);
#endif
    }

    printf("----------\n");
    printf("total frames: %d\n", counter);

    // Release
    cap.release();
#ifdef GUI
    destroyAllWindows();
#endif

    return 0;
}
