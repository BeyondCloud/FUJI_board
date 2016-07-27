// Midi_IO.cpp
//

#include "Midi_IO.h"
#define LZZ_INLINE inline
Midi_IO::Midi_IO ()
{
		midiout = new RtMidiOut();
}
void Midi_IO::openPort (int port)
{
	unsigned int nPorts = midiout->getPortCount();
	if ( nPorts == 0 ) 
		std::cout << "No ports available!\n";
	else
		midiout->openPort( port );	
}
void Midi_IO::setPitchBendRange (int channel, int semitones, int cents)
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
void Midi_IO::setMainVolume (int channel, int volume)
{
		// Control Change: 176, 7, 100 (volume)
		message[0] = 176 + channel; //10110000
		message[1] = 7;  //7 means control main volume
		message.push_back( volume );
		midiout->sendMessage( &message );
}
void Midi_IO::setExpression (int channel, int volume)
{
		// Control Change: 176, 7, 100 (volume)
		message[0] = 176 + channel; //10110000
		message[1] = 11;  
		message[2] = volume; 
		midiout->sendMessage( &message );
}
void Midi_IO::selectInstrument (int channel, int instrument)
{
		// Send out a series of MIDI messages.
		// Program change: 192, instrument , N/A
		message.push_back( 192 + channel );
		message.push_back( instrument ); //select instrument 1~128 see wiki
		//third byte is not used for program change
		midiout->sendMessage( &message );
}
#undef LZZ_INLINE
