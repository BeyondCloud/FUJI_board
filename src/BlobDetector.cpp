// BlobDetector.cpp
//
#include <opencv2/opencv.hpp>
using namespace cv;
#include "BlobDetector.h"
#define LZZ_INLINE inline
BlobDetector::BlobDetector ()
    {
        //minThreshold + n * thresholdStep < maxThreshold ,n=0,1,2...
        pDefaultBLOB.thresholdStep = 20;//9
        pDefaultBLOB.minThreshold = 10;//10
        pDefaultBLOB.maxThreshold = 70;//145
        pDefaultBLOB.minRepeatability = 2;
        pDefaultBLOB.minDistBetweenBlobs = 10;

        pDefaultBLOB.filterByColor = false;
        pDefaultBLOB.blobColor = 0;

        pDefaultBLOB.filterByArea = true;
        pDefaultBLOB.minArea = 40;
        pDefaultBLOB.maxArea = 600;

        pDefaultBLOB.filterByCircularity = false;
        pDefaultBLOB.minCircularity = 0.9f;
        pDefaultBLOB.maxCircularity = (float)1e37;

        pDefaultBLOB.filterByInertia = false;
        pDefaultBLOB.minInertiaRatio = 0.1f;
        pDefaultBLOB.maxInertiaRatio = (float)1e37;

        pDefaultBLOB.filterByConvexity = false;
        pDefaultBLOB.minConvexity = 0.95f;
        pDefaultBLOB.maxConvexity = (float)1e37;
        backGround = Mat(CLIP_HEIGHT,CLIP_WIDTH,CV_8UC1);
//       for (int i = 0; i<65536; i++)
//           palette.push_back(Vec3b((uchar)rand(), (uchar)rand(), (uchar)rand()));
        pBLOB.push_back(pDefaultBLOB);
        b = SimpleBlobDetector::create(*pBLOB.begin());
        sbd = b.dynamicCast<SimpleBlobDetector>();
    }
void BlobDetector::resetBackground (Mat &src)
    {
        imshow("background",src);
        backGround = src;
    }
#undef LZZ_INLINE
