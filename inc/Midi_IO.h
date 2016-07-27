// Midi_IO.h
//

#ifndef LZZ_Midi_IO_h
#define LZZ_Midi_IO_h
#define LZZ_INLINE inline
#include "RtMidi.h"

class Midi_IO
{
public:
  Midi_IO ();
  void noteOn (int channel, int tone, int volume);
  void noteOff (int channel, int tone, int velocity);
  void pitchBend (int channel, int LSB, int MSB);
  void openPort (int port);
  void setPitchBendRange (int channel, int semitones, int cents);
  void setMainVolume (int channel, int volume);
  void setExpression (int channel, int volume);
  void selectInstrument (int channel, int instrument);
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
private:
  RtMidiOut * midiout;
  std::vector <unsigned char> message;

};
LZZ_INLINE void Midi_IO::noteOn (int channel, int tone, int volume)
        {
		message[0] = 144 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
		message[1] = tone; 
		message[2] = volume;
		midiout->sendMessage( &message );
	}
LZZ_INLINE void Midi_IO::noteOff (int channel, int tone, int velocity)
        {
		message[0] = 128 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
		message[1] = tone; 
		//velocity can be used for aftertouch supported synthesizer
		message[2] = velocity; 
		midiout->sendMessage( &message ); 
	}
LZZ_INLINE void Midi_IO::pitchBend (int channel, int LSB, int MSB)
        {
		message[0] = 224 + channel;   //144 = 10010000 status byte 1001->note on  0000->cannel 0
		message[1] = LSB; 
		message[2] = MSB;  //64 means no bend
		midiout->sendMessage( &message );
	}
#undef LZZ_INLINE
#endif
