#include "Player.h"
#include <vector>
#include <cmath>
using namespace std;

PlayCurve::PlayCurve(double length, double (*linef)(double, double, double)) : Length((int)length * static_cast<int>(WaveConst::SampleRate))
{
    int i;
    Curve = new double[Length];
    for(i=0 ; i<Length ; i++)
    {
        Curve[i] = linef(i/Length, 1, 1);
    }
}
SampleSound::SampleSound(int length) : SampleSize(length)
{
    int i;
    SampleData = new double[SampleSize];
    for(i=0 ; i<length ; i++)
    {
        SampleData[i] = sin(2*3.14159*i/length) + 0.3 * sin(8*3.14159*i/length);
    }
}
NoiseSound::NoiseSound(int length) : NoiseSize(length)
{
    int i;
    NoiseData = new double[NoiseSize];
    for(i=0 ; i<length ; i++)
    {
        NoiseData[i] = sin(440 * 2 * 3.14159 * i/length);
    }
}
Instrument::Instrument(int length = 1000) : Sample(length), Noise(length)
{
    
}
int gcd(int p, int q)
{
	if(q==0) return p;
	return gcd(q,p%q);
}
void Bar::add(Note* note)
{
	int div = static_cast<int>(Bar::UnitNote_Default);
	int mult = static_cast<int>(note->length);
	int g = gcd(div, mult);
	mult /= g;
	div /= g;
	int l = div*division / gcd(div, division);
	int mag;
	mag = l/division;
	UnitCount *= mag;
	for(vector<Note*>::const_iterator  it = Notes.begin(); it != Notes.end() ; ++it)
	{
		(*it)->logicalLength *= mag;
	}
	mult *= l/div;
	division = l;
	note->myBar = (this);
	note->logicalLength = mult;
	(*this).UnitCount += mult;
	Notes.push_back(note);
}
Bar::Bar(double ul, NoteLength un) :  UnitCount(0), UnitLength(ul), UnitNote(un), division(1){}
void Bar::add(Note* note[], int length, NoteLength realLength)
{
	int div = static_cast<int>(Bar::UnitNote_Default);
	int mult = static_cast<int>(realLength);
	int sum = 0;
	int g = gcd(div, mult);
	int i;
	mult/=g;
	div/=g;
	g = static_cast<int>(note[0]->length);
	for(i=0 ; i<length ; i++)
	{
		g = gcd(g, static_cast<int>(note[i]->length));
		sum += static_cast<int>(note[i]->length);
	}
	sum/=g;
	mult = sum*mult / gcd(sum,mult);
	div = div*sum / gcd(sum,mult);
	for(i=0 ; i<length ; i++)
	{
		note[i]->myBar = (this);
		note[i]->logicalLength = (static_cast<int>(note[i]->length)/(sum*g))*mult;
		(*this).UnitCount += (static_cast<int>(note[i]->length)/(sum*g))*mult;
		Notes.push_back(note[i]);
	}
}
NoteLength Bar::UnitNote_Default;
double Bar::UnitLength_Default;

Renderer* Note::getRenderredSound(Instrument& inst, double val, double diff)
{
    Renderer* sound = new Renderer(static_cast<int>(WaveConst::SampleRate));
    vector<double>& sounddata = sound->RenderredData;
    int& soundlength = sound->Length;
    int resizedLength;
    double* resized;
    int phase = 0;
    double max = 0;
    int i, j;
    switch(length)
    {
    case NoteLength::Whole:
        soundlength = static_cast<int>(WaveConst::SampleRate) * 2;
        break;
    case NoteLength::Half:
        soundlength = static_cast<int>(WaveConst::SampleRate) * 1;
        break;
    case NoteLength::Quater:
        soundlength = static_cast<int>(WaveConst::SampleRate) * 0.5;
        break;
    case NoteLength::Eighth:
        soundlength = static_cast<int>(WaveConst::SampleRate) * 0.25;
        break;
    case NoteLength::Sixteenth:
        soundlength = static_cast<int>(WaveConst::SampleRate) * 0.125;
        break;
    }
    if(frequency == 0)
        resizedLength = 1;
    else
        resizedLength = static_cast<int>(WaveConst::SampleRate)/frequency;
    resized = sound->ResizeCurve(inst.Sample.SampleData, inst.Sample.SampleSize, resizedLength);
    for(i=0 ; i<resizedLength - 1 ; i++)
    {
        if((resized[i+1]-val) * (resized[i]-val)<=0 && diff * (resized[i+1] - resized[i]) >= 0)
        {
            phase = i+1;
            break;
        }
    }
    for(i=0 ; i<soundlength ; )
    {
        for(j=0 ; j<resizedLength && i<soundlength ; i++, j++)
        {
            sounddata.push_back((resized[(j+phase)%resizedLength]));
        }
    }
    return sound;
}
Renderer* NoteGroup::getRenderredSound(Instrument& inst, double val, double diff)
{
    Renderer* sound = new Renderer(44100);
    vector<double>& sounddata = sound->RenderredData;
    double endvalue = val;
    double enddiff = diff;
    for(vector<Phrase*>::const_iterator it = phrases_.begin(); it != phrases_.end() ; ++it)
    {
        Renderer* child = (*it)->getRenderredSound(inst, endvalue, enddiff);
        sounddata.insert(sounddata.end(), child->RenderredData.begin(), child->RenderredData.end());
        endvalue = *(sounddata.end()-1);
        enddiff = endvalue > (*(sounddata.end()-2)) ? 1: -1;
    }
    return sound;
}

Note::Note(NoteLength len, double freq, NoteIndicator playtype = NoteIndicator::Legato) : AmplitudeCurve(len, Note::LinearLine), FrequencyCurve(len, Note::LinearLine)
{
    length = len;
    frequency = freq;
    PlayType = playtype;
}
NoteGroup::NoteGroup(string name)
{
    Name = name;
}
double Note::LinearLine(double x, double s, double f)
{
    return s + (f-s) * x;
}
void NoteGroup::add(Phrase* obj)
{
    phrases_.push_back(obj);
}
double Note::getLength()
{
	return 1000;
}
double NoteGroup::getLength()
{
	double sum =0;
	for(vector<Phrase*>::const_iterator it = phrases_.begin() ; it != phrases_.end() ; ++it)
	{
		sum += (*it)->getLength();
	}
	return sum;
}