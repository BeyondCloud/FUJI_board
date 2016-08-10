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
    blob_t blob[MAX_TOUCH];

private:
    void debug_draw(Mat &src);
    SimpleBlobDetector::Params pDefaultBLOB;
    vector< Vec3b >  palette;
    vector <SimpleBlobDetector::Params> pBLOB;
    vector <SimpleBlobDetector::Params>::iterator itBLOB;
    vector <KeyPoint> keyImg;
    vector <KeyPoint>::iterator k;
//    vector< Vec3b >  palette;
    Mat backGround;
    Mat debug_Img;
    Ptr <Feature2D> b;
    Ptr <SimpleBlobDetector> sbd;
};
LZZ_INLINE void BlobDetector::detect (Mat & src)
{
	src -= backGround;
	sbd->detect(src, keyImg, Mat());
//       drawKeypoints(src, keyImg, result);
//        char sizeTxt[7] = "";
	//cv::Scalar c = cv::Scalar(0,255,0); // text color of circle size

	//iterate over each detected center and draw circle
	//Circle(src, center, radius, color, thickness=1, lineType=8, shift=0)
	int index=0;
	for(int i = 0 ; i < MAX_TOUCH;i++)
		blob[i].size = 0;
	for (k = keyImg.begin(); k != keyImg.end(); k++)
	{
//            circle(src, k->pt, (int)k->size, palette[i % 65536],3);
//            sprintf(sizeTxt,"%d", (int)k->size);
//            putText(src,sizeTxt ,k->pt, FONT_HERSHEY_DUPLEX,2,Scalar(255,255,255));
		index   = k-keyImg.begin();
	   if(index < MAX_TOUCH && index >=0)
		{
			blob[index].x = k->pt.x;
			cout<<"x"<<blob[index].x<<endl;
			blob[index].y = k->pt.y;
			blob[index].size = (int)k->size;
		}
	}
	      debug_draw(src);

}
#undef LZZ_INLINE
#endif
