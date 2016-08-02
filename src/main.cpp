#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "../myStruct.h"
#include "SynthMgr.h"
using namespace std;
using namespace cv;
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
int main(int argc, const char** argv)
{
    SynthMgr synMgr;
    blob_t blob[MAX_TOUCH];
    for(int i=0;i < MAX_TOUCH;i++)
    {
        blob[i].x=0;
        blob[i].y=0;
        blob[i].size=0;
    }
    blob[0].x=127;
    blob[0].y=89;
    blob[0].size=76;
    synMgr.blob2midi(blob);
    sleep(1000);
    blob[0].size=0;
    synMgr.blob2midi(blob);


    waitKey();
    return 0;
}

