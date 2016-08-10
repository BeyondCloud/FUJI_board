// FeyeMgr.h
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "config.h"
#ifndef LZZ_FeyeMgr_h
#define LZZ_FeyeMgr_h
#define LZZ_INLINE inline
using namespace cv;
class FeyeMgr
{
    struct feye_t
    {
        Point orig;
        Point center;
        int r;
    };
public:
    FeyeMgr (int src_h,int src_w);
    void fillCircle (Mat &img, Point center, int r);
    void defishNcrop (Mat & gray_scale_image);
    Mat defish_Img;
private:
    void feye_tbl_create ();
    uchar * src_ptr;
    uchar * dst_ptr;
    Mat map_x,map_y;
    feye_t feye;
};
LZZ_INLINE void FeyeMgr::defishNcrop (Mat & src)
{
    dst_ptr = defish_Img.ptr<uchar>(0);
    for (int  j =CLIP_ORIGIN_Y; j <CLIP_ORIGIN_Y + CLIP_HEIGHT; j++)
    {
        for (int  i = CLIP_ORIGIN_X; i <CLIP_ORIGIN_X + CLIP_WIDTH; i++)
        {
            src_ptr = src.ptr<uchar>(map_y.at<int>(j,i));
            *dst_ptr++ = src_ptr[map_x.at<int>(j,i)];
        }
    }
}
#undef LZZ_INLINE
#endif
