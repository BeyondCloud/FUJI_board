
// SynthMgr.cpp
//
#include <iostream>

#include "SynthMgr.h"
#include "config.h"

#define LZZ_INLINE inline
using namespace std;
using namespace cv;
SynthMgr::SynthMgr ()
{

    for(int id=0;id<MAX_TOUCH;id++)
    {
        ID_queue.push(id);
        prev_blobs[id].ID = -1;
    }

    Img = imread("line.jpg");
    if(!Img.data)
    {
        cout << "Error: except line image input\n";
        cout << "Fail to create tone table\n";
        return;
    }
    cvtColor(Img,Img,COLOR_RGB2GRAY);
    threshold(Img,Img,120,255,0);//thres value,max,type
    exist_valid_row = false;
    record_valid_rows(Img,valid_y);
    if(!exist_valid_row)
    {
        cout<<"no valid row detected\n";
        cout<<"fail to create tone table\n";
        return;
    }
    //2D note array
    note_tbl = new note_t*[Img.rows];
    for (int i = 0 ; i < Img.rows ; i++)
        note_tbl[i] = new note_t[Img.cols];

    //create tone table with size Img.rows*Img.cols
    //only valid row will be initialized
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
    int ref;
    //handle invalid rows
    //search downward until valid row found
    for (int  from = 0; from <Img.rows ; from++)
    {
        if(!valid_y[from])
        {
            ref=from;
            while(++ref < Img.rows && !valid_y[ref]){}
            for(int i = from; i<ref;i++)
            {
                for (int  x = 0; x <Img.cols ; x++)
                {
                    note_tbl[i][x].tone = note_tbl[ref][x].tone;
                    note_tbl[i][x].bend = note_tbl[ref][x].bend;
                }
                valid_y[i] = true;
            }

        }
    }

}
bool (SynthMgr::valid_y) [CLIP_HEIGHT] = {false};

#undef LZZ_INLINE
