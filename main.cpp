#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <iconv.h>
#include <sstream>
#include <string>
#include <string.h>

using namespace std;
using namespace cv;

float get_time_ms(struct timeval start, struct timeval end)
{
    double start_us = start.tv_sec * 1000000 + start.tv_usec;
    double end_us = end.tv_sec * 1000000 + end.tv_usec;
    float time_ms = (end_us - start_us) / 1000;
    return time_ms;
}

int main()
{
    string gsurl = "rtspsrc location=rtsp://admin:nanchao.org@192.168.31.60:554/Streaming/Channels/101 latency=0 ! rtph264depay ! h264parse ! mppvideodec ! videoconvert ! appsink";  // 也可以使用 rgaconvert
//    string gsurl = "rtspsrc location=rtsp://admin:nanchao.org@192.168.31.60:554/Streaming/Channels/101 latency=0 ! rtph264depay ! h264parse ! mppvideodec ! videoconvert ! video/x-raw,format=(string)BGR  ! appsink sync=false";  // 也可以使用 rgaconvert
//    string gsurl = "rtspsrc location=rtsp://192.168.31.148:8554/mystream latency=0 ! rtph264depay ! h264parse ! mppvideodec ! videoconvert ! video/x-raw,format=(string)BGR  ! appsink sync=false";  // 也可以使用 rgaconvert
    VideoCapture cap = VideoCapture(gsurl,cv::CAP_GSTREAMER);
    if(!cap.isOpened())
    {
        std::cout<<"cannot open captrue..."<<std::endl;
        return 0;
    }

    int fps = cap.get(5);
    cout<<"fps:"<<fps<<endl;
    Mat frame[10];
    bool readreturn = false;
    int counter = 0;
    float time_ms;
    struct timeval start, end;
    while(1)
    {  
        gettimeofday(&start, NULL);
        readreturn = cap.read(frame[counter]);
        counter++;
        gettimeofday(&end, NULL);
        time_ms = get_time_ms(start, end);
        printf("frame #%d: time per frame: %.2fms\n", counter, time_ms);

//        printf("before write\n");
//        imwrite("RTSP.jpg",frame);
//        printf("done\n");
//        exit(0);
        //imshow("RTSP",frame);
//	sleep(20);
        //if (cvWaitKey(30) == 27) 
        //{
        //    cout << "Esc key is pressed by user" << endl;
        //    break;
        //}
    }

    cap.release();
    return 0;
}
