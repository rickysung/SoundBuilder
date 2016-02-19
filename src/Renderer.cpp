#include "Renderer.h"
Renderer::Renderer(int samplesize) : SampleRate(samplesize) {}
double* Renderer::ResizeCurve(const double* ins, const int size, const int expanded_size)
{
    int i;
    int idx1;
    int idx2;
    double x1;
    double x2;
    double y1;
    double y2;
    double y;
    double magSize = (double)expanded_size/size;
    double* resized = new double[expanded_size];
    for(i=0 ; i<expanded_size ; i++)
    {
        idx1 = int(i/magSize);
        idx2 = idx1 + 1;
        x1 = idx1 * magSize;
        x2 = idx2 * magSize;
        y1 = ins[idx1];
        y2 = ins[idx2];
        y = (y2 - y1) / (x2 - x1) * (i - x1) + y1;
        resized[i] = y;
    }
    return resized;
}