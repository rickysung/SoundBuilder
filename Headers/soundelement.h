class SoundWave{
    private:
        int freq;
        int volume;
        int16_t waveshape[];
        int16_t wavenoise[];
        void set_waveshape(int16_t* original_wave, int guide_freq);
    public:
        SoundWave(int d, int f, int v)
        {
            dur = d;
            freq = f;
            volume = v;
        }
};
class SoundSequence{
    SoundWave *seq;
}
