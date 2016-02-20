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

Instrument sinwave(1000, SampleSound::Sinusoidal(1000), SampleSound::Sinusoidal(1000));
vector<Bar*> BarList;
NoteGroup Root("Root");
void BuildPhrase()
{
	Bar* tmp = new Bar(1);
	Bar* tmp2 = new Bar(1);
	NoteGroup* g = new NoteGroup("phrase1");
	NoteGroup* g2 = new NoteGroup("phrase1");
	vector<Note*> MultiNotes;
		
	tmp->PlayLine[0]->add(new Note(g, NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->PlayLine[0]->add(new Note(g, NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->PlayLine[0]->add(new Note(g, NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->PlayLine[0]->add(new Note(g, NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->PlayLine[0]->add(new Note(g, NoteLength::Eighth, 440, NoteIndicator::Legato));
	tmp->PlayLine[0]->add(new Note(g2, NoteLength::Eighth, 440, NoteIndicator::Legato));
	
	MultiNotes.push_back(new Note(g2, NoteLength::Sixteenth, 445, NoteIndicator::Legato));
	MultiNotes.push_back(new Note(g2, NoteLength::Sixteenth, 445, NoteIndicator::Legato));
	MultiNotes.push_back(new Note(g2, NoteLength::Sixteenth, 445, NoteIndicator::Legato));
	MultiNotes.push_back(new Note(g2, NoteLength::Sixteenth, 445, NoteIndicator::Legato));
	tmp2->PlayLine[0]->add(MultiNotes, NoteLength::EightDot);
	tmp2->PlayLine[0]->add(new Note(g2, NoteLength::EightDot, 440, NoteIndicator::Legato));
	Root.add(g);
	Root.add(g2);

}
void initSound()
{
    
}
void initHeader(FILE *fout)
{
	WAVE_HEADER header;
	Renderer* sequence = Root.getRenderredSound(sinwave);
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
	header.Data.ChunkSize = static_cast<unsigned int>(sequence->RenderredData.size()) * CHANNEL * BITRATE / 8;

	fwrite(&header, sizeof(header), 1, fout);
}
int main(void)
{
	FILE *fp = fopen("waveout.wav","wb");
	Bar::UnitLength_Default = 100;
	Bar::UnitNote_Default = NoteLength::Quater;
	BuildPhrase();
    initHeader(fp);
    initSound();
	return 1;
}