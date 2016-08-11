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
      void blob2midi(vector<blob_t> &blobs);

      int  getID();
      Mat Img;
      int valid_row_cnt;
      note_t **note_tbl;
    private:
      int ID;
      note_t onStrike[MAX_TOUCH];
      Midi_IO midi_io;
      static bool (valid_y) [CLIP_HEIGHT];
      bool isNoteOn[MAX_TOUCH];
      bool exist_valid_row;
      vector <blob_t> blobs_prev;
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
LZZ_INLINE void SynthMgr::blob2midi(vector<blob_t> &blobs)
{
    bool isPlaying;
    //playing state
    cout<<"size"<<blobs.size()<<endl;
    for(unsigned int i = 0;i<blobs.size();i++)
    {
        isPlaying = false;
        for(unsigned int j = 0;j < blobs_prev.size(); j++)
        {
        //Note playing
            if( abs(blobs_prev[j].x- blobs[i].x ) < GROUP_RANGE_X)
            {
                isPlaying = true;
                int chnl = blobs_prev[j].ID;
                int x = blobs_prev[j].x;
                int y = blobs_prev[j].y;
                int bend = note_tbl[y][x].bend;
                int volume = min(blobs_prev[j].size,127);
                //clamp bend value between 0~127
                bend = max(min(127,64+(bend-onStrike[chnl].bend)),0);
                midi_io.pitchBend(chnl,0,bend);
                midi_io.setExpression(chnl,volume);
                blobs_prev.erase(blobs_prev.begin()+j);
                break;
            }
        }
        //Note ON
        if(!isPlaying)
        {
            blobs.back().ID = getID();
            int chnl = blobs.back().ID;
            int x = blobs.back().x;
            int y = blobs.back().y;
            int velocity = min(blobs.back().size,127);
            onStrike[chnl].tone = note_tbl[y][x].tone;
            onStrike[chnl].bend = note_tbl[y][x].bend;
            midi_io.setExpression(chnl,velocity);
            midi_io.pitchBend(chnl,0,64);
            midi_io.noteOn(chnl,onStrike[chnl].tone,velocity);
        }
    }
    //note off
    for(unsigned int i = 0;i<blobs_prev.size();i++)
    {
        int chnl = blobs_prev.back().ID;
        midi_io.noteOff(chnl,onStrike[chnl].tone,blobs_prev.back().size);
    }
    blobs_prev = blobs;
//    for(int chnl = 0;chnl < MAX_TOUCH;chnl++)
//    {
//        if(isNoteOn[chnl] == false)
//        {
//            int velocity = min(b[chnl].size,127);
//            //note on event
//            if(velocity != 0)
//            {
//                isNoteOn[chnl] = true;
//                onStrike[chnl].tone = note_tbl[b[chnl].y][b[chnl].x].tone;
//                onStrike[chnl].bend = note_tbl[b[chnl].y][b[chnl].x].bend;
//                midi_io.setExpression(chnl,velocity);
//                midi_io.pitchBend(chnl,0,64);
//                midi_io.noteOn(chnl,onStrike[chnl].tone,velocity);
//                /*
//                cout << "channel: " << chnl << " tone: "
//                 << onStrike[chnl].tone << " bend: "<< onStrike[chnl].bend
//                 <<" velocity: " << velocity<<" is sended"<< endl;
//                */
//            }
//        }
//        else
//        {
//            int volume = min(b[chnl].size,127);
//            cout<<"volume"<<volume<<endl;
//            //note off event
//            if(volume == 0)
//            {
//
//                midi_io.noteOff(chnl,onStrike[chnl].tone,volume);
//                isNoteOn[chnl] = false;
//            }
//            //note playing event
//            else
//            {
//                int bend = note_tbl[b[chnl].y][b[chnl].x].bend;
//                //clamp bend value between 0~127
//                bend = max(min(127,64+(bend-onStrike[chnl].bend)),0);
//                midi_io.pitchBend(chnl,0,bend);
//                midi_io.setExpression(chnl,volume);
//            }
//
//        }
//    }
}
LZZ_INLINE int SynthMgr::getID()
{
    ID = (ID+1)%MAX_TOUCH;
    cout<<"ID"<<ID<<endl;
    return ID;
}
#undef LZZ_INLINE
#endif
