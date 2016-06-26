// midiout.cpp
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <time.h>
#define MAX_TOUCH 5
#define KEYBOARD_WIDTH 540
#define WIGHT_KEY_NUM 17
#define VOLUME_SCALE 3
const float WHIGHT_KEY_LENGTH = KEYBOARD_WIDTH/WIGHT_KEY_NUM; 
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
/*
void update_input(blob (&b)[MAX_TOUCH])
{
	int play_tone;
	for(int i=0 ;i<MAX_TOUCH;i++ )
	{
	    play_tone =(int)( b[i].x / WHIGHT_KEY_LENGTH );

	    message[0] = 144 + i; //10010000 status byte 1001->note on  0000->cannel 0
	    message[1] = 48 + play_tone;  //tone 
	    message[2] = b[i].size * VOLUME_SCALE;  //volume
	    midiout->sendMessage( &message );
	}

}
*/
int main()
{

  RtMidiOut *midiout = new RtMidiOut();
  std::vector<unsigned char> message;
  // Check available ports.
  unsigned int nPorts = midiout->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }
 // std::cout <<  midiout->getPortName(1);

  // Open first available port.
  midiout->openPort( 1 );

  // Send out a series of MIDI messages.
  // Program change: 192, instrument , N/A
  message.push_back( 192 );
  message.push_back( 21 ); //select instrument 1~128 see wiki
  //third byte is not used for program change
  midiout->sendMessage( &message );

  // Control Change: 176, 7, 100 (volume)
  message[0] = 176; //10110000
  message[1] = 7;  //7 means control main volume
  message.push_back( 100 );
  midiout->sendMessage( &message );

  
  
  message[0] = 176;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
  message[1] = 6;  
  message[2] = 4;
  midiout->sendMessage( &message );

 
  // Note On: 144, 60, 90
  message[0] = 144;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
  message[1] = 60;  //tone 
  message[2] = 90;
  midiout->sendMessage( &message );
  // NOTE THAT THE PITCH BEND MSB AND LSB'S POSITION SWAP
  sleep( 500 ); // Platform-dependent ... see example in tests directory.
  for(int i=64 ;i<=127;i++)
  {
      message[0] = 225;   //11100000 pitch bend channel 0
      message[1] = 0;  //LSB 0~127
      message[2] = i; //MSB 0~127
      midiout->sendMessage( &message );
      sleep( 20 ); // Platform-dependent ... see example in tests directory.
  }
//================
  // Note On: 144, 60, 90
  message[0] = 145;   //10010000 status byte 1001->note on  0000->cannel 0
  message[1] = 62;  //tone 
  message[2] = 90;
  midiout->sendMessage( &message );
  // Note Off: 128, 60, 40
  message[0] = 128;  //10000000 status byte 1000->note off 0000->cannel 0
  message[1] = 60;  
  message[2] = 90;
  midiout->sendMessage( &message );

  // NOTE THAT THE PITCH BEND MSB AND LSB'S POSITION SWAP
  for(int i=64 ;i<=127;i++)
  {
      message[0] = 225;   //224 = 11100000 pitch bend channel 0
      message[1] = 0;  //LSB 0~127
      message[2] = i; //MSB 0~127
      midiout->sendMessage( &message );
      sleep( 20 ); // Platform-dependent ... see example in tests directory.
  }
//================
  // Note Off: 128, 62, 40
  message[0] = 129;  //10000000 status byte 1000->note off 0000->cannel 0
  message[1] = 62;  
  message[2] = 90;
  midiout->sendMessage( &message );
  sleep( 500 ); // Platform-dependent ... see example in tests directory.
  
  message[0] = 224;   //224 = 11100000 pitch bend channel 0
  message[1] = 0;  //LSB 0~127
  message[2] = 64; //MSB 0~127
  midiout->sendMessage( &message );
  message[0] = 225;   //224 = 11100000 pitch bend channel 0
  message[1] = 0;  //LSB 0~127
  message[2] = 64; //MSB 0~127
  midiout->sendMessage( &message );




  // Clean up
 cleanup:
  delete midiout;
  return 0;
}
