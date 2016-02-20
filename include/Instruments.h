class SampleSound
{
public:
	static double* Sinusoidal(double);
    private:
    public:
    SampleSound(int, double*);
    const int SampleSize;
    double* SampleData;
};

class NoiseSound
{
    public:
    NoiseSound(int, double*);
    const int NoiseSize;
    double* NoiseData;
};

class Instrument
{
    public:
    Instrument(int l, double*, double*);
    SampleSound Sample;
    NoiseSound Noise;
};