// BlobDetector.cpp
//

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

        for (int i = 0; i<65536; i++)
            palette.push_back(Vec3b((uchar)rand(), (uchar)rand(), (uchar)rand()));
        pBLOB.push_back(pDefaultBLOB);
        b = SimpleBlobDetector::create(*pBLOB.begin());
        sbd = b.dynamicCast<SimpleBlobDetector>();
    }
void BlobDetector::resetBackground ()
    {
        Mat bg;
        cap>>bg;
        imshow("background",bg);
        cvtColor(bg, bg, CV_BGR2GRAY);
        backGround = bg;
    }
#undef LZZ_INLINE
