// SynthMgr.cpp
//

#include "SynthMgr.h"
#include "../config.h"
#define LZZ_INLINE inline
using namespace std;
using namespace cv;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int threshold_value = 120;
int threshold_type = 0;
SynthMgr::SynthMgr ()
    {
        Img = imread("line.jpg");

        note = new note_t*[Img.rows];
        for (int i = 0 ; i < Img.rows ; i++)
            note[i] = new note_t[Img.cols];
        cvtColor(Img ,Img, COLOR_RGB2GRAY);
        threshold( Img,Img, threshold_value, max_BINARY_value,threshold_type );
        record_valid_rows(Img,valid_y);
        const float bpk = 64.0/(float)BEND_RANGE_PM;
        for (int  y = 0; y <Img.rows ; y++)
        {
            if(valid_y[y])
            {
                int current_tone = LOWEST_TONE;
                bool bw_switch = (bool)Img.at<uchar>(y,0);
                int cnt=0;
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
                           note[y][x-cnt].tone = current_tone;
                           //48~80
                           note[y][x-cnt].bend =(int)(current_bend+bpk*(1.0-(double)cnt/key_width));
                           cnt--;
                       }
                       current_bend += bpk;
                       current_tone ++;
                    }
                }
            }
        }
    }
bool (SynthMgr::valid_y) [CLIP_WIDTH] = {false};
#undef LZZ_INLINE
