// SynthMgr.h
//
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "config.h"
#include "myStruct.h"
#include "Midi_IO.h"

#include <iostream>
#include <math.h>
#ifndef LZZ_SynthMgr_h
#define LZZ_SynthMgr_h
#define LZZ_INLINE inline
using namespace cv;
using namespace std;
class SynthMgr
{

    public:
      SynthMgr ();
      template <typename T, size_t N>
      void record_valid_rows (Mat & Img, T (& valid_y) [N]);
      void blob2midi(const blob_t (&blob_tbl)[MAX_TOUCH]);
      Mat Img;
      int valid_row_cnt;
      note_t **note_tbl;
    private:
      note_t onStrike[MAX_TOUCH];
      Midi_IO midi_io;
      static bool (valid_y) [CLIP_HEIGHT];
      bool isNoteOn[MAX_TOUCH];
      bool exist_valid_row;

};
//if the rows of line image contain valid key number
//mark it as valid row
template <typename T, size_t N>
void SynthMgr::record_valid_rows (Mat & Img, T (& valid_y) [N])
{
    valid_row_cnt = 0;
    int key_cnt = 0;
    int pixel_cnt = 0;
    bool bw_switch = 0; //  black/white  true/false
    for (int  y = 0; y <Img.rows ; y++)
    {
        for (int  x = 0; x <Img.cols ; x++)
        {
            if((int)Img.at<uchar>(y,x)==0)
            {
                if(bw_switch == true)
                {
                    pixel_cnt = 0;
                    bw_switch = false;
                }
            }
            else
            {
                if(bw_switch == false)
                {
                    pixel_cnt = 0;
                    bw_switch = true;
                }
            }
            pixel_cnt++;
            if(pixel_cnt==4)
            {
                key_cnt++;
            }

        }
        if(key_cnt == KEY)
        {
            valid_row_cnt++;
            valid_y[y] = true;
            exist_valid_row = true;
        }
        key_cnt = 0;
        pixel_cnt = 0;
    }

}
LZZ_INLINE void SynthMgr::blob2midi(const blob_t (&b)[MAX_TOUCH])
{
    for(int chnl = 0;chnl < MAX_TOUCH;chnl++)
    {
        if(isNoteOn[chnl] == false)
        {
            int velocity = min(b[chnl].size,127);
            //note on event
            if(velocity != 0)
            {
                isNoteOn[chnl] = true;
                onStrike[chnl].tone = note_tbl[b[chnl].y][b[chnl].x].tone;
                onStrike[chnl].bend = note_tbl[b[chnl].y][b[chnl].x].bend;
                midi_io.setExpression(chnl,velocity);
                midi_io.pitchBend(chnl,0,64);
                midi_io.noteOn(chnl,onStrike[chnl].tone,velocity);
                /*
                cout << "channel: " << chnl << " tone: "
                 << onStrike[chnl].tone << " bend: "<< onStrike[chnl].bend
                 <<" velocity: " << velocity<<" is sended"<< endl;
                */
            }
        }
        else
        {
            int volume = min(b[chnl].size,127);
            cout<<"volume"<<volume<<endl;
            //note off event
            if(volume == 0)
            {

                midi_io.noteOff(chnl,onStrike[chnl].tone,volume);
                isNoteOn[chnl] = false;
            }
            //note playing event
            else
            {
                int bend = note_tbl[b[chnl].y][b[chnl].x].bend;
                //clamp bend value between 0~127
                bend = max(min(127,64+(bend-onStrike[chnl].bend)),0);
                midi_io.pitchBend(chnl,0,bend);
                midi_io.setExpression(chnl,volume);
            }

        }
    }
}

#undef LZZ_INLINE
#endif
