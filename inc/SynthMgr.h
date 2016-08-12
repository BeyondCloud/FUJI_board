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
    struct blobGreater
    {
        bool operator()( const blob_t& l, const blob_t& r ) const {
            return l.x < r.x;
        }
    };
    public:
      SynthMgr ();
      template <typename T, size_t N>
      void record_valid_rows (Mat & Img, T (& valid_y) [N]);
      void blob2midi(vector<blob_t> &blobs);
      Midi_IO midi_io;


      int  getID();
      void noteOn( blob_t &cur_blob);
      void noteOff(blob_t &prev_blob);
      void notePlay(const int ID,blob_t &cur_blob);

      Mat Img;
      int valid_row_cnt;
      note_t **note_tbl;
    private:
      int ID;
      note_t noteDown[MAX_TOUCH];
      static bool (valid_y) [CLIP_HEIGHT];
      bool exist_valid_row;
      vector <blob_t> prev_blobs;
      vector<blob_t>::reverse_iterator cur_blob_rev_it;
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
//WARNING
//All blob input should be sort in ascending x order
LZZ_INLINE void SynthMgr::blob2midi(vector<blob_t> &blobs)
{
    sort(blobs.begin(),blobs.end(),blobGreater());
    cur_blob_rev_it = blobs.rbegin();
    while(!prev_blobs.empty())
    {
        while(cur_blob_rev_it != blobs.rend())
        {
            int delX = cur_blob_rev_it->x - prev_blobs.back().x;
            if(abs(delX) < BLOB_SLIDE_RANGE)
            {
                notePlay(prev_blobs.back().ID,*cur_blob_rev_it);
                prev_blobs.pop_back();
                ++cur_blob_rev_it;
                break;
            }
            else
            {
                if(delX < 0)
                {
                    noteOff(prev_blobs.back());
                    prev_blobs.pop_back();
                    break;
                }
                else
                {
                    noteOn(*cur_blob_rev_it);
                    ++cur_blob_rev_it;
                }
            }
        }
        //when current blobs done,note off the rest of previous blobs to break while loop
        if(cur_blob_rev_it == blobs.rend())
        {
            for(unsigned int i = 0;i<prev_blobs.size();i++)
                noteOff(prev_blobs[i]);
            break;
        }
    }
    for (vector<blob_t>::reverse_iterator it = cur_blob_rev_it; it != blobs.rend();++it )
    {
        noteOn(*it);
    }
    prev_blobs=blobs;
}
LZZ_INLINE void SynthMgr::noteOn( blob_t &cur_blob)
{
    cur_blob.ID = getID();
    int chnl = cur_blob.ID;
    if(noteDown[chnl].tone == -1)
    {
        int x = cur_blob.x;
        int y = cur_blob.y;
        noteDown[chnl].tone = note_tbl[y][x].tone;
        noteDown[chnl].bend = note_tbl[y][x].bend;
        int velocity = min(cur_blob.size,127);
        midi_io.setExpression(chnl,velocity);
        midi_io.pitchBend(chnl,0,64);
        midi_io.noteOn(chnl,noteDown[chnl].tone,velocity);
        cout<<chnl <<" noteOn"<<endl;
    }
}
LZZ_INLINE void SynthMgr::noteOff(blob_t &prev_blob)
{
    int chnl = prev_blob.ID;
    midi_io.noteOff(chnl,noteDown[chnl].tone,prev_blob.size);
    noteDown[chnl].tone  = -1;
    cout<<chnl <<" noteOff"<<endl;
}
LZZ_INLINE void SynthMgr::notePlay(const int chnl,blob_t &cur_blob)
{
    cur_blob.ID = chnl;
    int bend = note_tbl[(cur_blob.y)][(cur_blob.x)].bend;
    int volume = min(cur_blob.size,127);
    //clamp bend value between 0~127
    bend = max(min(127,64+(bend-noteDown[chnl].bend)),0);
    midi_io.pitchBend(chnl,0,bend);
    midi_io.setExpression(chnl,volume);

}
LZZ_INLINE int SynthMgr::getID()
{
    ID = (ID+1)%CHANNEL_USED;
    cout<<"ID "<<ID<<" create"<<endl;
    return ID;
}
#undef LZZ_INLINE
#endif
