// BlobDetector.cpp
//
#include <opencv2/opencv.hpp>
using namespace cv;
#include "BlobDetector.h"
#define LZZ_INLINE inline
BlobDetector::BlobDetector ()
{
    //minThreshold + n * thresholdStep < maxThreshold ,n=0,1,2...
    pDefaultBLOB.thresholdStep = 27;//9
    pDefaultBLOB.minThreshold = 10;//10
    pDefaultBLOB.maxThreshold = 145;//145
    pDefaultBLOB.minRepeatability = 2;
    pDefaultBLOB.minDistBetweenBlobs = 20; //fast slide ->45
                                           //note to note distance is about 50
                                            //use 50 is a good idea
    pDefaultBLOB.filterByColor = false;
    pDefaultBLOB.blobColor = 0;

    pDefaultBLOB.filterByArea = true;
    pDefaultBLOB.minArea = 200;
    pDefaultBLOB.maxArea = 10000;
    pDefaultBLOB.filterByCircularity = false;
    pDefaultBLOB.filterByInertia = false;
    pDefaultBLOB.filterByConvexity = false;


//       for (int i = 0; i<65536; i++)
//           palette.push_back(Vec3b((uchar)rand(), (uchar)rand(), (uchar)rand()));
    pBLOB.push_back(pDefaultBLOB);

    b = SimpleBlobDetector::create(*pBLOB.begin());
    sbd = b.dynamicCast<SimpleBlobDetector>();
}

void BlobDetector::debug_draw(Mat &src)
{
    char sizeTxt[7] = "";
    //Circle(src, center, radius, color, thickness=1, lineType=8, shift=0)
    for (k = keyImg.begin(); k != keyImg.end(); k++)
    {
        circle(src,k->pt, k->size, 255,3);
        sprintf(sizeTxt,"%d",k-keyImg.begin());
        putText(src,sizeTxt ,k->pt, FONT_HERSHEY_DUPLEX,2,Scalar(255,255,255));
    }
    imshow("result",src);
}

#undef LZZ_INLINE
