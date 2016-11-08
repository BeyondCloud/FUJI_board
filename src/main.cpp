#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <stdio.h>
//#include <conio.h>
//#include <windows.h>
#include <time.h>
#include <math.h>
#include "myStruct.h"
#include "SynthMgr.h"
#include "FeyeMgr.h"
#include "BlobDetector.h"
#include <ncurses.h>
#include "raspicam_cv.h"

using namespace std;
using namespace cv;

//void sleep(unsigned int mseconds)
//{
//    clock_t goal = mseconds + clock();
//    while (goal > clock());
//}



void setCamParam(raspicam::RaspiCam_Cv &Camera ) {
    Camera.set ( CV_CAP_PROP_FRAME_WIDTH  ,1280 );
    Camera.set ( CV_CAP_PROP_FRAME_HEIGHT ,960  );
    Camera.set ( CV_CAP_PROP_BRIGHTNESS   ,80   );
    Camera.set ( CV_CAP_PROP_CONTRAST     ,100  );
    Camera.set ( CV_CAP_PROP_SATURATION   ,0    );
    Camera.set ( CV_CAP_PROP_GAIN         ,100  );
    Camera.set ( CV_CAP_PROP_FORMAT, CV_8UC1 );  //grayscale
    //Camera.set ( CV_CAP_PROP_EXPOSURE, getParamVal ( "-ss",argc,argv )  );
    //Camera.set ( CV_CAP_PROP_WHITE_BALANCE_RED_V,getParamVal ( "-wb_r",argc,argv )     );
    //Camera.set ( CV_CAP_PROP_WHITE_BALANCE_BLUE_U,getParamVal ( "-wb_b",argc,argv )     );
}
int main(int argc, const char** argv)
{

    raspicam::RaspiCam_Cv Camera;
    setCamParam(Camera );
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}

    Mat frame;
    FeyeMgr feyeMgr(960,1280);
    SynthMgr synthMgr;
    BlobDetector blobdtr;
    Mat background = imread("bg.jpg");
    cvtColor(background,background,CV_BGR2GRAY);
    if(background.channels() != 1)
    {
        cerr<<"bad background channel size"<<background.channels()<<", expect 1 channel";
        return -1;
    }
    if(background.cols != CLIP_WIDTH ||background.rows != CLIP_HEIGHT )
    {
        cerr<<"Error background image size"<<endl;
        return -1;
    }

//    cvtColor(bg,bg,CV_BGR2GRAY);

//    Camera.grab();
//    Camera.retrieve (frame);
//    feyeMgr.defishNcrop(frame);
//    imwrite ("test.jpg",feyeMgr.defish_Img) ;

    bool is_running = true;

    while(is_running)
    {

        waitKey(2);
        Camera.grab();
        Camera.retrieve (frame);
        feyeMgr.defishNcrop(frame);
        feyeMgr.defish_Img -= background;
        blobdtr.detect(feyeMgr.defish_Img);
        cout<<"blb size:"<<blobdtr.blobs.size()<<endl;
        synthMgr.blob2midi(blobdtr.blobs);

        //imshow("orig",feyeMgr.defish_Img);

//        switch (getch())
//        {
//            case 'e':
//                is_running = false;
//                break;
//            case 's':
//                blobdtr.resetBackground(feyeMgr.defish_Img);
//                break;
//            case 'o':
//                for(int i = 0; i < MAX_TOUCH; i++)
//                synthMgr.midi_io.allNoteOff(i);
//                break;
//        }

    }

    return 0;
}
