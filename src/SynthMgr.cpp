// SynthMgr.cpp
//
#include <iostream>

#include "SynthMgr.h"
#include "../config.h"

#define LZZ_INLINE inline
using namespace std;
using namespace cv;
SynthMgr::SynthMgr ()
{
    for(int chnl = 0;chnl < MAX_TOUCH;chnl++)
        isNoteOn[chnl] = false;
    Img = imread("line.jpg");
    if(!Img.data)
    {
        cout << "Error: except line image input\n";
        cout << "Fail to create tone table\n";
        return;
    }
    cvtColor(Img,Img,COLOR_RGB2GRAY);
    threshold(Img,Img,120,255,0);//thres value,max,type
    record_valid_rows(Img,valid_y);

    //2D note array
    note_tbl = new note_t*[Img.rows];
    for (int i = 0 ; i < Img.rows ; i++)
        note_tbl[i] = new note_t[Img.cols];

    //create tone table with size Img.rows*Img.cols
    const float bpk = 64.0/(float)BEND_RANGE_PM;
    for (int  y = 0; y <Img.rows ; y++)
    {
        if(valid_y[y])
        {
            int current_tone = LOWEST_TONE;
            int cnt=0;
            bool bw_switch = (bool)Img.at<uchar>(y,0);
            float current_bend = 0;
            for (int  x = 1; x <Img.cols ; x++)
            {

                bool current_pixel = (bool)Img.at<uchar>(y,x);
                cnt++;
                if(current_pixel != bw_switch || x == Img.cols-1)
                {
                    bw_switch = current_pixel;
                    double key_width = (double)cnt;
                    while(cnt != -1 )
                   {
                       note_tbl[y][x-cnt].tone = current_tone;
                       //48~80
                       note_tbl[y][x-cnt].bend =(int)(current_bend+bpk*(1.0-(double)cnt/key_width));
                       cnt--;
                   }
                   current_bend += bpk;
                   current_tone ++;
                }
            }
        }
    }
}


bool (SynthMgr::valid_y) [CLIP_HEIGHT] = {false};
#undef LZZ_INLINE
