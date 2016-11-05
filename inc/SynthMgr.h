
// SynthMgr.h
//
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "config.h"
#include "myStruct.h"
#include "Midi_IO.h"
#include "Double_llist.h"

#include <queue>
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

      void noteOn( blob_t &cur_blob);
      void noteOff(blob_t &prev_blob);
      void notePlay(blob_t &cur_blob);

      Mat Img;
      int valid_row_cnt;
      note_t **note_tbl; //tone table
    private:
      Double_llist<blob_t> playing_note;
      queue<int> ID_queue;
      note_t noteDown[MAX_TOUCH];
      static bool (valid_y) [CLIP_HEIGHT];
      bool exist_valid_row;
      blob_t prev_blobs[MAX_TOUCH];
      vector<blob_t>::iterator cur_it;
      vector<blob_t>::iterator prev_it;
      vector<blob_t>::iterator blob_it;


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
    if(playing_note.go_tail())
    {
        //note playing
        while(playing_note.cur().ID != -1) //-1 means already note off
        {
            for(blob_it =blobs.begin();blob_it!=blobs.end();blob_it++)
            {
                if(abs(blob_it->x - playing_note.cur().x )<BLOB_SLIDE_RANGE &&
                   abs(blob_it->y-playing_note.cur().y )<BLOB_SLIDE_RANGE )
                {
                    blob_it->ID = playing_note.cur().ID;
                    playing_note.set_cur(*blob_it);
                    notePlay(playing_note.cur());
                    blob_it->x = X_FAR_AWAY;
                }
            }

        }
        //other note off
    }

    for(blob_it =blobs.begin();blob_it!=blobs.end();blob_it++)
    {
        if(blob_it->x != X_FAR_AWAY)
        {
            playing_note.push_back(*blob_it);
        }
    }
        //when note on , access ID from ID queue



//    sort(blobs.begin(),blobs.end(),blobGreater());
//    cur_it = blobs.begin();
//    for(int i=0;i<MAX_TOUCH;i++)
//    {
//        if(prev_blobs[i].ID != -1)
//        {
//           // cout<<"ID: "<<prev_blobs.begin()->ID<<endl;
//            while(cur_it != blobs.end())
//            {
//                int delX = cur_it->x - prev_blobs[i].x;
//                if(abs(delX) < BLOB_SLIDE_RANGE)
//                {
//                    notePlay(i,*cur_it);
//                    break;
//                }
//                else if(delX>0)
//                {
//                    noteOff(prev_blobs[i]);
//                    break;
//                }
//                ++cur_it;
//            }
//            if(cur_it == blobs.end())
//            {
//                 noteOff(prev_blobs[i]);
//            }
//        }
//    }
//    while(cur_it != blobs.end() && !ID_queue.empty())
//    {
//        noteOn(*cur_it);
//        ++cur_it;
//    }
}
LZZ_INLINE void SynthMgr::noteOn( blob_t &cur_blob)
{
    if(ID_queue.empty())
    {
        cout<<"pool empty"<<endl;
        return;
    }
    cur_blob.ID = ID_queue.front();
    ID_queue.pop();
    int chnl = cur_blob.ID;
    int x = cur_blob.x;
    int y = cur_blob.y;
    noteDown[chnl].tone = note_tbl[y][x].tone;
    noteDown[chnl].bend = note_tbl[y][x].bend;
    int velocity = min(cur_blob.size,127);
    midi_io.setExpression(chnl,velocity);
    midi_io.pitchBend(chnl,0,64);
    midi_io.noteOn(chnl,noteDown[chnl].tone,velocity);

    prev_blobs[cur_blob.ID] = cur_blob;
    cout<<chnl <<" noteOn"<<endl;

}
LZZ_INLINE void SynthMgr::noteOff(blob_t &prev_blob)
{
    int chnl = prev_blob.ID;
    prev_blobs[chnl].ID = -1;
    ID_queue.push(chnl);
    midi_io.noteOff(chnl,noteDown[chnl].tone,prev_blob.size);
    cout<<chnl <<" noteOff"<<endl;
}
LZZ_INLINE void SynthMgr::notePlay(blob_t &cur_blob)
{
    int chnl = cur_blob.ID;
    int bend = note_tbl[(cur_blob.y)][(cur_blob.x)].bend;
    int volume = min(cur_blob.size,127);
    //clamp bend value between 0~127
    bend = max(min(127,64+(bend-noteDown[chnl].bend)),0);
    midi_io.pitchBend(chnl,0,bend);
    midi_io.setExpression(chnl,volume);
}

#undef LZZ_INLINE
#endif
