// Midi_IO.cpp
//

#include "Midi_IO.h"
#include "config.h"
#define LZZ_INLINE inline

Midi_IO::Midi_IO ()
{
	midiout = new RtMidiOut();
    midiout->openPort( 0 );
    for(int chnl =0;chnl<1;chnl++)
    {
        selectInstrument(chnl,16);
        setMainVolume(chnl,50);
        setExpression(chnl,127);
        setPitchBendRange(chnl,BEND_RANGE_PM,0);
    }
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
void Midi_IO::setPitchBendRange (int chnl, int semitones, int cents)
{
	std::cout << "Channel "<< chnl <<" pitch bend range set to +-"<< semitones <<"\n";
    message.clear();
    message.push_back(176+chnl);
    message.push_back(101);
    message.push_back(0);
    midiout->sendMessage( &message );
    message.clear();
    message.push_back(176+chnl);
    message.push_back(100);
    message.push_back(0);
    midiout->sendMessage( &message );
    message.clear();
    message.push_back(176+chnl);
    message.push_back(6);
    message.push_back(semitones);
    midiout->sendMessage( &message );
    message.clear();
    message.push_back(176+chnl);
    message.push_back(38);
    message.push_back(cents);
    midiout->sendMessage( &message );

}
void Midi_IO::setMainVolume (int chnl,int volume)
{
    message.clear();
    message.push_back(176+chnl);
    message.push_back(7);
    message.push_back(volume);
    midiout->sendMessage( &message );
    std::cout <<"channel "<<chnl<< " main volume set to "<< volume <<"\n";
}
void Midi_IO::setExpression (int chnl, int volume)
{
    message.clear();
    std::cout << "Channel "<< chnl <<" expression set to "<< volume <<"\n";
    message.push_back(176+chnl);
    message.push_back(11);
    message.push_back(volume);
    midiout->sendMessage( &message );
}
void Midi_IO::selectInstrument (int channel, int instrument)
{
    message.clear();
    // Program change: 192, instrument , N/A
    message.push_back( 192 + channel );
    message.push_back( instrument ); //select instrument 1~128 see wiki
    //third byte is not used for program change
    midiout->sendMessage( &message );
    std::cout << "Channel "<< channel <<" select instrument "<< instrument <<"\n";
}
void Midi_IO::cleanUp()
{
    delete midiout;
}
#undef LZZ_INLINE

