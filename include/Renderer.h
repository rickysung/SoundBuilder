#include <vector>
using namespace std;

class Renderer
{
public:
    Renderer(int);
    const int SampleRate;
    int Length;
    vector<double> RenderredData;
    double* ResizeCurve(const double* ins, int size, int expanded_size);
private:
};