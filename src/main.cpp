#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
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
int main(int argc, const char** argv)
{

    Mat frame;

    cap_init();
    FeyeMgr feyeMgr(cap.get(CV_CAP_PROP_FRAME_HEIGHT),cap.get(CV_CAP_PROP_FRAME_WIDTH));
 //   SynthMgr synthMgr;
    BlobDetector blobdtr;
    bool is_running = false;
    Mat bg = imread("bg.jpg");
    cvtColor(bg,bg,CV_BGR2GRAY);
    blobdtr.resetBackground(bg);
    while(!is_running)
    {
        waitKey(2);
        cap>>frame;
        cvtColor(frame,frame,CV_BGR2GRAY);
        feyeMgr.defishNcrop(frame);
        blobdtr.detect(feyeMgr.defish_Img);
    //    synthMgr.blob2midi(blobdtr.blob);
        imshow("orig",feyeMgr.defish_Img);
        if (_kbhit() )
        {
            switch (_getch())
            {
                case 'e':
                    is_running = true;
                    break;
                case 's':
                    blobdtr.resetBackground(feyeMgr.defish_Img);
                    break;
            }
        }
    }

/*
    while(true)
    {
        waitKey(2);
        if (_kbhit() )
        {
            switch (_getch())
            {
                case 'e':
                    cap.release();
                    cout << "Release camera\n";
                    return -1;
                    break;
            }
        }
        cap>>frame;
        cvtColor(frame,frame,CV_BGR2GRAY);
        feyeMgr.defishNcrop(frame);
        imshow("frame",feyeMgr.defish_Img);
    }
*/

    /*
    Mat frame,crop;
    BlobDetector blobDtr;
   Rect myROI(0,0,CLIP_WIDTH,CLIP_HEIGHT);
  while(true){
    cap>>frame;
    Mat(frame,myROI).copyTo(crop);
    cvtColor(crop,crop, CV_BGR2GRAY);
        if (_kbhit() )
		{
			switch (_getch())
			{
                case 's':
                    blobDtr.resetBackground(crop);
                    break;
                case 'e':
                    cap.release();
                    cout << "Release camera\n";
					return -1;
					break;
			}
		}
        blobDtr.detect(crop);
        for(int i =0;i<MAX_TOUCH;i++)
            cout<<blobDtr.blob[i].size<<"\t";
        cout<<endl;
        waitKey(2);

    }
*/
    return 0;
}
