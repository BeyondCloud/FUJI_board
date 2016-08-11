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
    blob_t blob;
    vector<blob_t> blobs;
private:
    void debug_draw(Mat &src);
    SimpleBlobDetector::Params pDefaultBLOB;
    vector <SimpleBlobDetector::Params> pBLOB;
    vector <SimpleBlobDetector::Params>::iterator itBLOB;
    vector <KeyPoint> keyImg;
    Mat backGround;
    Ptr <Feature2D> b;
    Ptr <SimpleBlobDetector> sbd;
};
LZZ_INLINE void BlobDetector::detect (Mat & src)
{
	src -= backGround;
	sbd->detect(src, keyImg, Mat());
	blobs.clear();
	for (unsigned int i = 0; i < keyImg.size(); i++)
    {
        if(i<MAX_TOUCH)
        {
            blob.x = keyImg[i].pt.x;
			blob.y = keyImg[i].pt.y;
			blob.size = (int)keyImg[i].size;
			blobs.push_back(blob);
        }
	}


}
#undef LZZ_INLINE
#endif
