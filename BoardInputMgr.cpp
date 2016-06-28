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
void noteOff(int channel ,int tone,int volume)
{
    message[0] = 128 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
    message[1] = tone; 
    message[2] = volume;
    midiout->sendMessage( &message );
}
void pitchBend(int channel ,int LSB,int MSB)
{
    message[0] = 224 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
    message[1] = LSB; 
    message[2] = MSB;  //64 menas no bend
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
void selectInstrument(int channel,int instrument)
{
    // Send out a series of MIDI messages.
    // Program change: 192, instrument , N/A
    message.push_back( 192 + channel );
    message.push_back( 16 ); //select instrument 1~128 see wiki
    //third byte is not used for program change
    midiout->sendMessage( &message );
}
void update_input(blob (&b)[MAX_TOUCH])
{
	for(int i=0 ;i<MAX_TOUCH;i++ )
	{
		if(b[i].size)
		{
			int key = xPositionToKey(b[i].x);
			int tone_t = key + LOWEST_TONE;
			int key_width = keyJunctionPos[key+1] - keyJunctionPos[key];
			int bend = 16384*( b[i].x - keyJunctionPos[key] )/key_width;
			noteOn(i,tone_t,blobSizeToVolume(b[i].size));		
			pitchBend(i,(bend & 127),bend>>7);
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
    selectInstrument(1,16);
    setMainVolume(1,100);
	
	blob b[MAX_TOUCH];
	b[0].x = 0;
    b[0].size = 90;
	b[1].size = 0;
	b[2].size = 0;
	b[3].size = 0;
	b[4].size = 0;
	update_input(b);
	for(int i=0 ;i<=18;i++)
    {
	  b[0].x = i;
      update_input(b);
	  sleep( 100 ); // Platform-dependent ... see example in tests directory.
    }
	sleep(3000);
	b[0].size = 0;
	update_input(b);
	
/*

    noteOn(1,(int)Tone::C4,100);
    sleep(1000);
    setPitchBendRange(1,12,0);

    for(int i=64 ;i<=127;i++)
    {
      pitchBend(1,0,i);
      sleep( 20 ); // Platform-dependent ... see example in tests directory.
    }
    sleep(1000);
    pitchBend(1,0,64);
    noteOff(1,(int)Tone::C4,100);
*/
    // Clean up
    cleanup:
    delete midiout;
    return 0;
	
}
