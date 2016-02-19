#define _USE_MATH_DEFINES

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <vector>
#include "make_wav.h"
#include "Player.h"

using namespace std;
#define SAMPLERATE 44100
#define DURATION 5
#define CHANNEL 1 
#define BITRATE 16
#define CHORD_NUM 3
#define PI 3.141592

//Instrument sinwave(1000);
vector<Bar*> BarList;
void BuildPhrase()
{
	Bar* tmp = new Bar();
	Bar* tmp2 = new Bar();
	Note* MultiNotes[] = {new Note(NoteLength::Sixteenth, 445, NoteIndicator::Legato),
		new Note(NoteLength::Sixteenth, 446, NoteIndicator::Legato),
		new Note(NoteLength::Sixteenth, 446, NoteIndicator::Legato),
		new Note(NoteLength::Sixteenth, 447, NoteIndicator::Legato) };
	tmp->add(new Note(NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->add(new Note(NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->add(new Note(NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->add(new Note(NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->add(new Note(NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->add(new Note(NoteLength::Eighth, 440, NoteIndicator::Legato));
	
	tmp2->add(MultiNotes, 4, NoteLength::EightDot);
	tmp2->add(new Note(NoteLength::EightDot, 440, NoteIndicator::Legato));
}
void initSound()
{
    
}
void initHeader(FILE *fout)
{
	WAVE_HEADER header;
    memcpy(header.Riff.ChunkID, "RIFF", 4);
	header.Riff.ChunkSize = DURATION * SAMPLERATE * CHANNEL * BITRATE / 8 + 36;
	memcpy(header.Riff.Format, "WAVE", 4);
    memcpy(header.Fmt.ChunkID, "fmt ", 4);
	header.Fmt.ChunkSize = 0x10;
	header.Fmt.AudioFormat = WAVE_FORMAT_PCM;
	header.Fmt.NumChannels = CHANNEL;
	header.Fmt.SampleRate = SAMPLERATE;
	header.Fmt.AvgByteRate = SAMPLERATE * CHANNEL * BITRATE / 8;
    header.Fmt.BlockAlign = CHANNEL * BITRATE / 8;
	header.Fmt.BitPerSample = BITRATE;
	
	memcpy(header.Data.ChunkID, "data", 4);
	header.Data.ChunkSize = DURATION * SAMPLERATE * CHANNEL * BITRATE / 8;

	fwrite(&header, sizeof(header), 1, fout);
}
int main(void)
{
	FILE *fp = fopen("waveout.wav","wb");
	int i, j, k;
	Bar::UnitLength_Default = 100;
	Bar::UnitNote_Default = NoteLength::Quater;
	BuildPhrase();
    initHeader(fp);
    initSound();
	return 1;
}