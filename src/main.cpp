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
int getID(int &ID)
{
    ID = (ID+1)%MAX_TOUCH;
    return ID;
}
int main(int argc, const char** argv)
{
    /*
    vector <blob_t> blob_prev;
    vector <blob_t> blob_tmp;
    vector <blob_t> blob_cur;
    blob_t b,asdf;
    b.x = 100;
    b.y = 300;
    b.size = 10;
    blob_prev.push_back(b);
    b.x = 200;
    b.y = 300;
    b.size = 20;
    blob_prev.push_back(b);
    b.x = 300;
    b.y = 300;
    b.size = 30;
    blob_prev.push_back(b);


    b.x = 110;
    b.y = 300;
    b.size = 10;
    blob_cur.push_back(b);
    b.x = 200;
    b.y = 310;
    b.size = 20;
    blob_cur.push_back(b);
    b.x = 400;
    b.y = 300;
    b.size = 40;
    blob_cur.push_back(b);


    blob_tmp = blob_prev;

    int cnt = 0;

   //0 1 playing 2 off 3 on
    for(unsigned int i = 0;i<blob_tmp.size();i++)
    {
        for(unsigned int j = 0;j < blob_cur.size(); j++)
        {
            cnt++;
            //playing state
            if( (blob_cur[j].x- blob_tmp[i].x ) < GROUP_RANGE_X)
            {
                blob_cur.erase (blob_cur.begin()+j);
                blob_tmp.erase(blob_tmp.begin()+i);
                break;
            }
        }
    }
    //note off
    for(unsigned int i = 0;i<blob_tmp.size();i++)
    {

    }
    //note on
    for(unsigned int i = 0;i<blob_cur.size();i++)
    {
    }
*/
/*
    Mat prev_frame = imread("prev.jpg");
    Mat cur_frame = imread("current.jpg");
    Mat bg = imread("bg.jpg");
    cvtColor(bg,bg,CV_BGR2GRAY);
    cvtColor(prev_frame,prev_frame,CV_BGR2GRAY);
    cvtColor(cur_frame,cur_frame,CV_BGR2GRAY);
    BlobDetector blobdtr;
    blobdtr.resetBackground(bg);
    //blobdtr.detect(prev_frame);
    blobdtr.detect(cur_frame);
    blobdtr.detect(prev_frame);
    imshow("prev",prev_frame);
    imshow("cur",cur_frame);
    waitKey();
    */


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
