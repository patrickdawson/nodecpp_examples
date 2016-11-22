#include "rainfall.h"

using namespace std;

sample::sample() {}
sample::sample(string d, double r)
: date(d)
, rainfall(r)
{}

double avg_rainfall(location &loc)
{
    double total = 0;
    for (const auto &sample : loc.samples)
    {
        total += sample.rainfall;
    }
    return total / loc.samples.size();
}