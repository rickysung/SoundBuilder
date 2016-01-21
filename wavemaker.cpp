#include <stdio.h>
#include <hash_map>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "Headers/make_wav.h"
#define SAMPLERATE 44100
#define DURATION 5
#define CHANNEL 1 
#define BITRATE 16
#define CHORD_NUM 3
#define PI 3.141592
using namespace stdext;

char abc = "B,5B,B,(C5.B,).A,|(B,4G,12)|D5DD(E5.D).C|(D4B,8)G4|F4E4D4C4|(B,8B,2)z2z4|";
int freq[] = {880, 988, 523, 587, 659, 698, 783};

hash_map<char,double> notes;
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
    initHeader(fp);
    initSound();
    SoundRendering();

/*	for(i=0 ; i<SAMPLERATE * DURATION * BITRATE / 8 ; i++)
	{
		int16_t y[CHANNEL] = {0,};
		for(j=0 ; j<CHANNEL ; j++)
		{
			for(k=0 ; k<CHORD_NUM ; k++)
			{
				y[j] +=(int16_t)(10000 * sin(2 * PI * i * freq[k] / SAMPLERATE));
			}
			fwrite(&y[j], sizeof(int16_t), 1, fp);
		}
	}
	fclose(fp);*/
	return 1;
}
