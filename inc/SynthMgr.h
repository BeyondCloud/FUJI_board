// SynthMgr.h
//
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../config.h"
#ifndef LZZ_SynthMgr_h
#define LZZ_SynthMgr_h
#define LZZ_INLINE inline
extern int threshold_value;
extern int threshold_type;
using namespace cv;

class SynthMgr
{
    struct note_t
    {
    int tone;
    int bend;
    };
    public:
      SynthMgr ();
      template <typename T, size_t N>
      void record_valid_rows (Mat & Img, T (& valid_y) [N]);
      Mat Img;
      note_t **note;
    private:
      static bool (valid_y) [CLIP_WIDTH];
};
template <typename T, size_t N>
void SynthMgr::record_valid_rows (Mat & Img, T (& valid_y) [N])
{
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
            valid_y[y] = true;
        }
        key_cnt = 0;
        pixel_cnt = 0;
    }
}
#undef LZZ_INLINE
#endif
