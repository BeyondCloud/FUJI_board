// midiout.cpp
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "RtMidi.h"
#include <time.h>
#define MAX_TOUCH 5
#define KEYBOARD_WIDTH 540
#define WIGHT_KEY_NUM 17
#define TOTAL_KEY_NUM 29
#define LOWEST_TONE 48
#define VOLUME_SCALE 1
//PM : plus or minus
#define BEND_RANGE_PM 12
//8192 or 64
#define BEND_RESOLUTION 64
const float WHIGHT_KEY_LENGTH = KEYBOARD_WIDTH/WIGHT_KEY_NUM; 
RtMidiOut *midiout = new RtMidiOut();
std::vector<unsigned char> message;
enum class Tone {C0,C0s,D0,D0s,E0,F0,F0s,G0,G0s,A0,A0s,B0,
		 C1,C1s,D1,D1s,E1,F1,F1s,G1,G1s,A1,A1s,B1,
		 C2,C2s,D2,D2s,E2,F2,F2s,G2,G2s,A2,A2s,B2,
		 C3,C3s,D3,D3s,E3,F3,F3s,G3,G3s,A3,A3s,B3,
		 C4,C4s,D4,D4s,E4,F4,F4s,G4,G4s,A4,A4s,B4,
		 C5,C5s,D5,D5s,E5,F5,F5s,G5,G5s,A5,A5s,B5,
		 C6,C6s,D6,D6s,E6,F6,F6s,G6,G6s,A6,A6s,B6,
		 C7,C7s,D7,D7s,E7,F7,F7s,G7,G7s,A7,A7s,B7,
		 C8,C8s,D8,D8s,E8,F8,F8s,G8,G8s,A8,A8s,B8,
		 C9,C9s,D9,D9s,E9,F9,F9s,G9,G9s,A9,A9s,B9,
		 C10,C10s,D10,D10s,E10,F10,F10s,G10,G10s,A10,A10s,B10,
		};
int keyJunctionPos[TOTAL_KEY_NUM];
//bend width per key 
int bpk  = BEND_RESOLUTION/BEND_RANGE_PM;
int atkKey[MAX_TOUCH] = {-1};
int stnKey[MAX_TOUCH] = {-1};
	
//unsigned int_8 total_key_num_log_ceil = ceil(log2(TOTAL_KEY_NUM));
// C3 = 0~18, C3s = 18~36...
void initKeyJunctionPosition()
{
    keyJunctionPos[0] = 0;
    for(int i = 1;i < TOTAL_KEY_NUM;i++)
	keyJunctionPos[i] = keyJunctionPos[i-1]+18;
}
struct blob {
    int x;
    int y;
    int size;
};
struct note{
    int x;
    int y;
    int size;
};
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
//return key ID 
int xPositionToKey(int x)
{
	int end = TOTAL_KEY_NUM;
	int start = 0;
	int mid;
   //binary search
   while(end - start != 1)
   {
	   //fast start + floor((end-start)/2)
	   mid = start + ((end-start)>>1);
	   if( keyJunctionPos[mid] > x)
		   end = mid;
	   else 
		   start = mid;
   }
    return start;
}
unsigned int blobSizeToVolume(int blobsize)
{
    unsigned int volume = blobsize * VOLUME_SCALE ;
    //fast version to determine if volume < 128
    return ((volume >> 7)==0)? volume : 127 ;
}
void noteOn(int channel ,int tone,int volume)
{
    message[0] = 144 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
    message[1] = tone; 
    message[2] = volume;
    midiout->sendMessage( &message );
}
void noteOff(int channel ,int tone,int velocity)
{
    message[0] = 128 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
    message[1] = tone; 
	//velocity can be used for aftertouch supported synthesizer
    message[2] = velocity; 
    midiout->sendMessage( &message ); 
}
void pitchBend(int channel ,int LSB,int MSB)
{
    message[0] = 224 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
    message[1] = LSB; 
    message[2] = MSB;  //64 means no bend
    midiout->sendMessage( &message );
}
void setPitchBendRange(int channel,int semitones,int cents)
{
    message[0] = 176 + channel; 
    message[1] = 101; 
    message[2] = 0; 
    midiout->sendMessage( &message );

    message[0] = 176 + channel; 
    message[1] = 100;  
    message[2] = 0; 
    midiout->sendMessage( &message );
    //coarse -> ex: Bx 06 02 : set bend range +-2 semitone
    message[0] = 176 + channel;
    message[1] = 6; 
    message[2] = semitones;  
    midiout->sendMessage( &message );
    //fine 
    message[0] = 176 + channel; 
    message[1] = 38;  
    message[2] = cents; 
    midiout->sendMessage( &message );
  
}
inline void setMainVolume(int channel,int volume)
{
    // Control Change: 176, 7, 100 (volume)
    message[0] = 176 + channel; //10110000
    message[1] = 7;  //7 means control main volume
    message.push_back( volume );
    midiout->sendMessage( &message );
}
inline void setExpression(int channel,int volume)
{
    // Control Change: 176, 7, 100 (volume)
    message[0] = 176 + channel; //10110000
    message[1] = 11;  
    message[2] = volume; 
 
	midiout->sendMessage( &message );
}
void selectInstrument(int channel,int instrument)
{
    // Send out a series of MIDI messages.
    // Program change: 192, instrument , N/A
    message.push_back( 192 + channel );
    message.push_back( instrument ); //select instrument 1~128 see wiki
    //third byte is not used for program change
    midiout->sendMessage( &message );
}
template <typename T>
inline T clip(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}
template <typename T>
inline T interpolator(const T& n, const T& delx, const T& dely) {
  return int((float)n/(float)delx*(float)dely);
}

void updateInput(blob (&b)[MAX_TOUCH])
{
	for(int chnl=0 ;chnl<MAX_TOUCH;chnl++ )
	{
		if(b[chnl].size != 0)
		{

			if(atkKey[chnl]==-1)
			{
				atkKey[chnl] = xPositionToKey(b[chnl].x);
				noteOn(chnl,atkKey[chnl]+LOWEST_TONE,blobSizeToVolume(b[chnl].size));
			}
			else
			{
				stnKey[chnl] = xPositionToKey(b[chnl].x);
				//err: -bend range~ + bend range
				int err   = (stnKey[chnl] - atkKey[chnl]);
				//root : 0,bpk,2*bpk...(BEND_RANGE_PM -1 )*bpk
				int root = 63 +  bpk *  clip(err , -BEND_RANGE_PM,BEND_RANGE_PM-1);
				int shift = interpolator( b[chnl].x - keyJunctionPos[stnKey[chnl]],
										  keyJunctionPos[stnKey[chnl]+1]-keyJunctionPos[stnKey[chnl]],
					 					  bpk);
				int result = root + shift;
				pitchBend(chnl,0,result);
/* for 16384 resolution supported synthesizer
				int root  = 8191 + bpk *  clip(err , -BEND_RANGE_PM,BEND_RANGE_PM-1) ;
				//0<= shift <=bpk
				int shift = interpolator( b[chnl].x - keyJunctionPos[stnKey[chnl]],
										  keyJunctionPos[stnKey[chnl]+1]-keyJunctionPos[stnKey[chnl]],
					 					  bpk);
				int result = root + shift;
				pitchBend(chnl,( result & 127),result>>7);
*/				
/*
				//=======debug info=============
				std::cout<<(result>>7)<<"\t";
				std::cout<<(result&127)<<"\t";
				std::cout<<(root)<<"\t";
				std::cout<<(shift)<<"\t";
				std::cout<<(  b[chnl].x - keyJunctionPos[stnKey[chnl]])<<"\t";
				std::cout<<( keyJunctionPos[stnKey[chnl]+1]-keyJunctionPos[stnKey[chnl]])<<"\n";
				//=============================
*/
				}
		}
		else
		{
			
			noteOff(chnl,atkKey[chnl] + LOWEST_TONE,0);		
			if(chnl==0)
				pitchBend(chnl,0,64);
		
			atkKey[chnl] = -1;		
		}
			
	}

}

int main()
{
    initKeyJunctionPosition();
    // Check available ports.
    unsigned int nPorts = midiout->getPortCount();
    if ( nPorts == 0 ) {
		std::cout << "No ports available!\n";
		goto cleanup;
    }
    // std::cout <<  midiout->getPortName(1);
    // port 0 : MS synthesizer
    // port 1 : fuji_port

    midiout->openPort( 1 );	
    selectInstrument(0,16);
    setMainVolume(0,100);

	setPitchBendRange(0,BEND_RANGE_PM,0);

	blob b[MAX_TOUCH];
	b[0].x = 0;
    b[0].size = 90;
	b[1].size = 0;
	b[2].size = 0;
	b[3].size = 0;
	b[4].size = 0;
	updateInput(b);
	sleep(500);
	for(int i=0 ;i<=198;i++)
    {
	  b[0].x = i;
      updateInput(b);
	  sleep( 20 ); // Platform-dependent ... see example in tests directory.
    }
   	sleep(500);
   	b[0].size = 0;
    updateInput(b);


	/*
	noteOn(0,(int)Tone::A4,70);
	sleep(20);
    for(int i=0 ;i<=120;i+=10)
    {
      pitchBend(0,0,64);
      sleep( 50 ); // Platform-dependent ... see example in tests directory.
    pitchBend(0,0,76);
  sleep( 50 ); // Platform-dependent ... see example in tests directory.
    
	}
    sleep(1000);
    pitchBend(0,0,64);
    
	noteOff(0,(int)Tone::C4,70);
*/
    // Clean up
    cleanup:
    delete midiout;
    return 0;
	
}
