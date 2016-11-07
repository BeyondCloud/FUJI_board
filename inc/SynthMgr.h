
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
      Double_llist<blob_t> onTouch_list;
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
    //if list is not empty,go to list tail
    //determine note playing,note off,ignored note
    if(onTouch_list.go_tail())
    {
        //ID = -1 means already note off
        for(int i = 0;i<onTouch_list.size();i++)
        {
            for(blob_it =blobs.begin();blob_it!=blobs.end();blob_it++)
            {
                if(!blob_it->isMatch)
                {
                    //compare blobs with each blob in touch list
                    if(abs(blob_it->x - onTouch_list.cur().x )<BLOB_SLIDE_RANGE &&
                       abs(blob_it->y - onTouch_list.cur().y )<BLOB_SLIDE_RANGE )
                    {
                        blob_it->ID = onTouch_list.cur().ID;
                        blob_it->isMatch = true;
                        if(blob_it->ID != -1)
                        {
                            onTouch_list.set_cur(*blob_it);
                            notePlay(onTouch_list.cur());
                        }
                        break;
                    }
                }

            }
            //if blob in list does not all blob on new frame
            //note off this note
            if(blob_it==blobs.end())
            {
                //note off only when this note is playing before
                if(blob_it->ID != -1)
                    noteOff(onTouch_list.cur());
                onTouch_list.remove_N_next();
            }
            else
                onTouch_list.go_next();
        }
    }
    //only this region allow blobs to enter onTouch_list
    for(blob_it =blobs.begin();blob_it!=blobs.end();blob_it++)
    {
        if(!blob_it->isMatch)
            onTouch_list.push_back(*blob_it);
    }

    if(onTouch_list.go_tail())
    {
        for(int i = 0;i< min(MAX_TOUCH,onTouch_list.size());i++)
        {
            if(onTouch_list.cur().ID == -1)
            {
                if(ID_queue.empty())
                {
                    noteOff(onTouch_list.first_touch->obj);
                    onTouch_list.first_touch = onTouch_list.first_touch->prev;
                }
                noteOn(onTouch_list.cur());
            }
            onTouch_list.go_next();
        }
    }
}
LZZ_INLINE void SynthMgr::noteOn( blob_t &cur_blob)
{
    if(ID_queue.empty())
    {
        cout<<"pool empty"<<endl;
        return;
    }
    if(ID_queue.size() == MAX_TOUCH)
        onTouch_list.first_touch = onTouch_list.head();

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
