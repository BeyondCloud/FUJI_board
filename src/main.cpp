#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "myStruct.h"
#include "SynthMgr.h"
#include "FeyeMgr.h"
#include "BlobDetector.h"

using namespace std;
using namespace cv;
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

VideoCapture cap(0); // open the camera 1
void cap_init()
{
    cap.set(CV_CAP_PROP_FOURCC ,CV_FOURCC('M', 'J', 'P', 'G') );
    cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);
}
void drawLine( Mat img, Point start, Point end )
{
  int thickness = 2;
  int lineType = 8;
  line( img,
        start,
        end,
        Scalar( 0, 0, 0 ),
        thickness,
        lineType );
}
int main(int argc, const char** argv)
{
    //test camera
//    Mat frame = imread("orig.png");
//    if(!frame.data)
//    {
//        cout<<"read error";
//        return -1;
//    }
//
//    cvtColor(frame,frame,CV_BGR2GRAY);
//    circle(frame, Point(FeyeCenterX,FeyeCenterY), FeyeRadius, Scalar(255,255,255), 0);
//    imshow("orig",frame);
//    FeyeMgr feyeMgr(720,1280);
//    feyeMgr.defishNcrop(frame);
//    imshow("cali",feyeMgr.defish_Img);
//    waitKey(0);


//    Mat frame;
//    cap_init();
//    FeyeMgr feyeMgr(cap.get(CV_CAP_PROP_FRAME_HEIGHT),cap.get(CV_CAP_PROP_FRAME_WIDTH));
//    bool is_running = true;
//    while(is_running)
//    {
//        waitKey(2);
//        cap>>frame;
//        cvtColor(frame,frame,CV_BGR2GRAY);
//        //drawLine(frame,Point(FeyeCenterX,0),Point(FeyeCenterX,720));
//        //drawLine(frame,Point(0,FeyeCenterY),Point(1280,FeyeCenterY));
//        feyeMgr.defishNcrop(frame);
//        //imshow("orig",frame);
//        imshow("orig",feyeMgr.defish_Img);
//        if (_kbhit() )
//        {
//            switch (_getch())
//            {
//                case 'e':
//                    is_running = false;
//                    break;
//            }
//        }
//    }



    Mat frame;
    cap_init();
    FeyeMgr feyeMgr(cap.get(CV_CAP_PROP_FRAME_HEIGHT),cap.get(CV_CAP_PROP_FRAME_WIDTH));
    SynthMgr synthMgr;
    BlobDetector blobdtr;
    bool is_running = true;
    Mat bg = imread("bg.jpg");
    cvtColor(bg,bg,CV_BGR2GRAY);
    blobdtr.resetBackground(bg);
    while(is_running)
    {
        waitKey(2);
        cap>>frame;
        cvtColor(frame,frame,CV_BGR2GRAY);
        feyeMgr.defishNcrop(frame);
        blobdtr.detect(feyeMgr.defish_Img);
        synthMgr.blob2midi(blobdtr.blobs);
        imshow("orig",feyeMgr.defish_Img);
        if (_kbhit() )
        {
            switch (_getch())
            {
                case 'e':
                    is_running = false;
                    break;
                case 's':
                    blobdtr.resetBackground(feyeMgr.defish_Img);
                    break;
                case 'o':
                    for(int i = 0; i < CHANNEL_USED; i++)
                    synthMgr.midi_io.allNoteOff(i);
                    break;
            }
        }
    }

    return 0;
}
