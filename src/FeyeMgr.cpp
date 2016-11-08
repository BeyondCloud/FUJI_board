// FeyeMgr.cpp
//

#include "FeyeMgr.h"
#include <iostream>
#define LZZ_INLINE inline
using namespace std;
using namespace cv;
FeyeMgr::FeyeMgr (int src_h,int src_w)
{
    defish_Img = Mat(CLIP_HEIGHT,CLIP_WIDTH,CV_8UC1);
    if(CLIP_WIDTH+CLIP_ORIGIN_X > src_w)
    {
        cout << "invalid clip window: except CLIP_WIDTH+CLIP_ORIGIN_X < src_w:"<<src_w<<"\n";
        return ;
    }
    else if(CLIP_HEIGHT+CLIP_ORIGIN_Y > src_h)
    {
        cout << "invalid clip window: except CLIP_HEIGHT+CLIP_ORIGIN_Y < src_h:"<<src_h<<"\n";
        return ;
    }
    map_x =Mat(src_h,src_w,CV_32SC1);
    map_y =Mat(src_h,src_w,CV_32SC1);
    feye_tbl_create();

}
void FeyeMgr::fillCircle (Mat &img, Point center, int r)
{
    int thickness = 1;
    int lineType = 8;
    circle( img,
         center,
         r,
         Scalar( 255, 255, 255 ),
         thickness,
         lineType );
}
void FeyeMgr::feye_tbl_create ()
{

    //set fish eye configure over here
    feye.center.x = FeyeCenterX;
    feye.center.y = FeyeCenterY;
    feye.r = FeyeRadius;
    //===================================
//  fillCircle(frame,feye.center,feye.r);

    Point feye_top_left = Point(feye.center.x-feye.r,feye.center.y-feye.r);
    cout<<"feye top left orig(x,y)=" << feye_top_left.x<<" "<<feye_top_left.y<<endl;
    bool isROI;
    for (int  y = 0; y <map_y.rows ; y++)
    {
        // normalize y coordinate to -1 ... 1
        double ny =  (y-feye.center.y)/(double)feye.r;
        double ny2 = ny*ny;
        for (int  x = 0 ; x <map_x.cols ; x++)
        {
            isROI = false;
            // x coordinate to -1 ... 1
            double nx =  (x -feye.center.x)/(double)feye.r;
            double nx2 = nx*nx;
            double r = sqrt(nx2+ny2);
            double theta = atan2(ny,nx);
            // discard pixels outside from circle
            if (0.0<=r&&r<=1.0)
            {
                double nr;
                if(x-feye.center.x < 0)
                    nr = 0.04*r*r*r*r+0.5*r*r*r-2.0*r*r + 2.5*r+0.0051 ;
                else
                    nr = 0.45*r*r*r*r+0.14*r*r*r-2.1*r*r + 2.5*r+0.0051 ;
                if (0.0 <= nr)
                {
                    double nxn = nr*cos(theta);
                    double nyn = nr*sin(theta);
                    int x2 = (int)(((nxn+1.0)*(double)feye.r)+feye_top_left.x);
                    int y2 = (int)(((nyn+1.0)*(double)feye.r)+feye_top_left.y);
                    if(0 <= y2 && y2 < map_y.rows && 0 <= x2 && x2 < map_x.cols)
                    {
                        map_x.at<int>(y,x) = x2;
                        map_y.at<int>(y,x) = y2;
                        isROI = true;
                    }
                }
            }
            if(!isROI)
            {
                map_x.at<int>(y,x) = x;
                map_y.at<int>(y,x) = y;
            }
         }
    }
    cout<<"feye table complete"<<endl;

}
#undef LZZ_INLINE
