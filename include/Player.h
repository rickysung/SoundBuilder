#include <algorithm>
#include <vector>
#include <string>
#include "Renderer.h"
#include "Instruments.h"
using namespace std;
class Bar;
class Line;
class Phrase;
class Note;
class NoteGroup;

enum class WaveConst : int
{
    SampleRate = 44100
};

enum class NoteIndicator
{
    Accent, 
    Slur, 
    Attack, 
    Staccato, 
    Fermata,
    Legato,
    Trill, 
    FortePiano, 
    Sforzando    
};

enum class PhraseIndicator
{
    Crescendo, 
    Decrescendo, 
    Ritardando, 
    Accelerando, 
    Stringendo    
};

enum class DynamicIndicator
{
    Pianisisimo, 
    Pianisimo, 
    Piano, 
    MezzoPiano, 
    MezzoForte,
    Forte, 
    Fortisimo,
    Fortisisimo
};

enum class NoteLength : int
{
    Whole = 32, 
	
	HalfDotDot = 28,
	HalfDot = 24,
    Half = 16, 
	
	QuaterDotDot = 14,
	QuaterDot = 12,
    Quater = 8, 

	EightDotDot = 7,
	EightDot = 6,
    Eighth = 4,

	SixteenthDot = 3,
    Sixteenth = 2,

	HalfSixteenth = 1
};

class PlayCurve
{
    public:
    PlayCurve(double, double (*pf)(double, double, double));
    double* Curve;
    const int Length;
};
class Bar
{
	public:
		NoteLength UnitNote;
		Line** PlayLine;
		const int InsNum;
		int UnitCount;
		int division;
		double UnitLength;
		
		Bar(int ic, double ul = Bar::UnitLength_Default, NoteLength un = Bar::UnitNote_Default);


		static NoteLength UnitNote_Default;
		static double UnitLength_Default;
};
class Line
{
public:
	Line(Bar*);
	vector<Note*> Notes;
	void add(Note* note);
	void add(vector<Note*> note, NoteLength);
	Bar* Parent;
};

class Phrase
{
    public:
    virtual double getLength() = 0;
    virtual Renderer* getRenderredSound(Instrument&) = 0;
    virtual ~Phrase(){};
};
class Note : public Phrase
{
public:
    explicit Note(NoteGroup*, NoteLength len, double freq, NoteIndicator playtype = NoteIndicator::Legato);
    
    double frequency;
    NoteLength length;
    NoteIndicator PlayType;
    PlayCurve AmplitudeCurve;
    PlayCurve FrequencyCurve;
    double getLength();
    Renderer* getRenderredSound(Instrument&);
    
	double realLength;
	double rhythmicHeight;
	double harmonicHeight;
	Line* myLine;
	int logicalLocation;
	int logicalLength;
private:
    static double LinearLine(double, double, double);
};
class NoteGroup : public Phrase
{
public:
    string Name;
    explicit NoteGroup(string);
    
    double getLength();
    Renderer* getRenderredSound(Instrument&);
    PhraseIndicator phraseType;
    DynamicIndicator dynamicType;
    void add(Phrase* obj);
private:
    vector <Phrase*> phrases_;
};