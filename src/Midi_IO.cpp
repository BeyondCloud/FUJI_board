// Midi_IO.cpp
//

#include "Midi_IO.h"
#include "config.h"
#define LZZ_INLINE inline
Midi_IO::Midi_IO ()
{
	midiout = new RtMidiOut();
    openPort( 0 );
    selectInstrument(0,16);
    for(int chnl =0;chnl<MAX_TOUCH;chnl++)
    {
        setMainVolume(chnl,127);
        setExpression(chnl,127);
    }
    setPitchBendRange(0,BEND_RANGE_PM,0);
}
void Midi_IO::openPort (int port)
{
	unsigned int nPorts = midiout->getPortCount();
	if ( nPorts == 0 )
		std::cout << "No ports available!\n";
	else
	{
		std::cout << "Port " << port << " opened\n";
		midiout->openPort( port );
	}
}
void Midi_IO::setPitchBendRange (int channel, int semitones, int cents)
{
	std::cout << "Channel "<< channel <<" pitch bend range set to +-"<< semitones <<"\n";
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
		std::cout << "Channel "<< channel << " main volume set to "<< volume <<"\n";
		message[0] = 176 + channel; //10110000
		message[1] = 7;  //7 means control main volume
		message[2] = volume;//0~127
		midiout->sendMessage( &message );
}
void Midi_IO::setExpression (int channel, int volume)
{
		std::cout << "Channel "<< channel <<" expression set to +-"<< volume <<"\n";
		// Control Change: 176, 7, 100 (volume)
		message[0] = 176 + channel; //10110000
		message[1] = 11;
		message[2] = volume;
		midiout->sendMessage( &message );
}
void Midi_IO::selectInstrument (int channel, int instrument)
{
		std::cout << "Channel "<< channel <<" select instrument "<< instrument <<"\n";
		// Send out a series of MIDI messages.
		// Program change: 192, instrument , N/A
		message.push_back( 192 + channel );
		message.push_back( instrument ); //select instrument 1~128 see wiki
		//third byte is not used for program change
		midiout->sendMessage( &message );
}
#undef LZZ_INLINE
