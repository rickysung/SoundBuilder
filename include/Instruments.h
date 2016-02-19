class SampleSound
{
    private:
    static double* Sinusoidal(double);
    public:
    SampleSound(int);
    const int SampleSize;
    double* SampleData;
};

class NoiseSound
{
    public:
    NoiseSound(int);
    const int NoiseSize;
    double* NoiseData;
};

class Instrument
{
    public:
    Instrument(int);
    SampleSound Sample;
    NoiseSound Noise;
};