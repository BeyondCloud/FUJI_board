// BlobDetector.h
//

#include <opencv2/opencv.hpp>
#include <vector>
#include "myStruct.h"
#include "config.h"

#ifndef LZZ_BlobDetector_h
#define LZZ_BlobDetector_h
#define LZZ_INLINE inline
using namespace cv;
using namespace std;

class BlobDetector
{

public:
    BlobDetector ();
    void resetBackground (Mat &src);
    void detect (Mat & src);
   // bool blob_increase_order(const blob_t l,const blob_t r);
    blob_t blob;
    vector<blob_t> blobs;
private:
    void debug_draw(Mat &src);
    SimpleBlobDetector::Params pDefaultBLOB;
    vector <SimpleBlobDetector::Params> pBLOB;
    vector <SimpleBlobDetector::Params>::iterator itBLOB;
    vector <KeyPoint> keyImg;
    vector <KeyPoint>::iterator k;
    Mat backGround;
    Ptr <Feature2D> b;
    Ptr <SimpleBlobDetector> sbd;
};
LZZ_INLINE void BlobDetector::detect (Mat & src)
{
	src -= backGround;
	sbd->detect(src, keyImg, Mat());
	blobs.clear();
	for (k = keyImg.begin(); k != keyImg.end(); k++)
    {
            blob.x = k->pt.x;
			blob.y = k->pt.y;
			blob.size = (int)k->size;
			blobs.push_back(blob);

	}

}
#undef LZZ_INLINE
#endif
