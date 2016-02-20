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
SampleSound::SampleSound(int length, double* data) : SampleSize(length)
{
    int i;
    SampleData = data;
    for(i=0 ; i<length ; i++)
    {
        SampleData[i] = sin(2*3.14159*i/length) + 0.3 * sin(8*3.14159*i/length);
    }
}
NoiseSound::NoiseSound(int length, double* data) : NoiseSize(length)
{
    int i;
    NoiseData = data;
    for(i=0 ; i<length ; i++)
    {
        NoiseData[i] = sin(440 * 2 * 3.14159 * i/length);
    }
}
Instrument::Instrument(int length, double* sound, double* noise) : Sample(length, sound), Noise(length, sound)
{
    
}
Bar::Bar(int ic, double ul, NoteLength un) : UnitCount(0), UnitLength(ul), UnitNote(un), division(1), InsNum(ic)
{
	PlayLine = new Line*[ic];
	int i;
	for (i = 0; i < ic; i++)
	{
		PlayLine[i] = new Line((this));
	}
}
Line::Line(Bar* p)
{
	Parent = p;
}
int gcd(int p, int q)
{
	if(q==0) return p;
	return gcd(q,p%q);
}
void Line::add(Note* note)
{
	int divider = static_cast<int>(Bar::UnitNote_Default);
	int multiplier = static_cast<int>(note->length);
	int g = gcd(divider, multiplier);
	multiplier /= g;
	divider /= g;
	int l = divider*Parent->division / gcd(divider, Parent->division);
	int mag;
	mag = l/Parent->division;
	Parent->UnitCount *= mag;
	for(vector<Note*>::const_iterator  it = Notes.begin(); it != Notes.end() ; ++it)
	{
		(*it)->logicalLength *= mag;
	}
	multiplier *= l/divider;
	Parent->division = l;
	note->myLine = (this);
	note->logicalLength = multiplier;
	note->realLength = Bar::UnitLength_Default * multiplier / Parent->division;
	note->logicalLocation = (*this).Parent->UnitCount;
	(*this).Parent->UnitCount += multiplier;
	Notes.push_back(note);
}
void Line::add(vector<Note*> note, NoteLength realLength)
{
	int divider = static_cast<int>(Bar::UnitNote_Default);
	int multiplier = static_cast<int>(realLength);
	int sum = 0;
	int n = static_cast<int>(note.size());
	int tmp_gcd;
	int total_gcd;
	int g = gcd(divider, multiplier);
	int i;
	multiplier/=g;
	divider/=g;
	total_gcd = static_cast<int>(note[0]->length);
	for(i=0 ; i<n ; i++)
	{
		total_gcd = gcd(total_gcd, static_cast<int>(note[i]->length));
		sum += static_cast<int>(note[i]->length);
	}
	sum/= total_gcd;
	tmp_gcd = gcd(sum, multiplier);
	multiplier = multiplier * sum / tmp_gcd;
	divider = divider * sum / tmp_gcd;
	for(i=0 ; i<n ; i++)
	{
		note[i]->myLine = (this);
		note[i]->logicalLength = static_cast<int>(note[i]->length)*multiplier / (sum*total_gcd);
		note[i]->realLength = Bar::UnitLength_Default * note[i]->logicalLength / divider;
		note[i]->logicalLocation = (*this).Parent->UnitCount;
		(*this).Parent->UnitCount += static_cast<int>(note[i]->length)*multiplier / (sum*total_gcd);
		Notes.push_back(note[i]);
	}
	Parent->division = divider;
}
NoteLength Bar::UnitNote_Default;
double Bar::UnitLength_Default;

Renderer* Note::getRenderredSound(Instrument& inst)
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
        soundlength = static_cast<int>(static_cast<double>(WaveConst::SampleRate) * 2);
        break;
    case NoteLength::Half:
        soundlength = static_cast<int>(static_cast<double>(WaveConst::SampleRate) * 1);
        break;
    case NoteLength::Quater:
        soundlength = static_cast<int>(static_cast<double>(WaveConst::SampleRate) * 0.5);
        break;
    case NoteLength::Eighth:
        soundlength = static_cast<int>(static_cast<double>(WaveConst::SampleRate) * 0.25);
        break;
    case NoteLength::Sixteenth:
        soundlength = static_cast<int>(static_cast<double>(WaveConst::SampleRate) * 0.125);
        break;
    }
    if(frequency == 0)
        resizedLength = 1;
    else
        resizedLength = static_cast<int>(static_cast<double>(WaveConst::SampleRate)/frequency);
    resized = sound->ResizeCurve(inst.Sample.SampleData, inst.Sample.SampleSize, resizedLength);
    for(i=0 ; i<soundlength ; )
    {
        for(j=0 ; j<resizedLength && i<soundlength ; i++, j++)
        {
            sounddata.push_back((resized[(j+phase)%resizedLength]));
        }
    }
    return sound;
}
Renderer* NoteGroup::getRenderredSound(Instrument& inst)
{
    Renderer* sound = new Renderer(44100);
    vector<double>& sounddata = sound->RenderredData;
    for(vector<Phrase*>::const_iterator it = phrases_.begin(); it != phrases_.end() ; ++it)
    {
		Renderer* child = (*it)->getRenderredSound(inst);
        sounddata.insert(sounddata.end(), child->RenderredData.begin(), child->RenderredData.end());
    }
    return sound;
}

Note::Note(NoteGroup* p, NoteLength len, double freq, NoteIndicator playtype) : AmplitudeCurve(static_cast<double>(len), Note::LinearLine), FrequencyCurve(static_cast<double>(len), Note::LinearLine)
{
    length = len;
    frequency = freq;
    PlayType = playtype;
	p->add((this));
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