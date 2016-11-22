#include "rainfall.h"
#include <algorithm>
#include <cmath>

using namespace std;

sample::sample() {}
sample::sample(string d, double r)
: date(d)
, rainfall(r)
{}

double avg_rainfall(location &loc) {
    double total = 0;
    for (const auto &sample : loc.samples)
    {
        total += sample.rainfall;
    }
    return total / loc.samples.size();
}

rain_result calc_rain_stats(location &loc) {
    rain_result result;
    double ss = 0;
    double total = 0;

    result.n = loc.samples.size();

    for (const auto &sample : loc.samples)
    {
        total += sample.rainfall;
    }
    result.mean = total / loc.samples.size();

    for (const auto &sample : loc.samples)
    {
        ss += pow(sample.rainfall - result.mean, 2);
    }
    result.standard_deviation = sqrt(ss / (result.n - 1));

    sort(loc.samples.begin(), loc.samples.end());
    if (result.n % 2 == 0)
    {
        result.median = (loc.samples[result.n / 2 - 1].rainfall + loc.samples[result.n / 2].rainfall) / 2;
    }
    else
    {
        result.median = loc.samples[result.n / 2].rainfall;
    }
    return result;
}

bool operator<(const sample &s1, const sample &s2)
{
    return s1.rainfall < s2.rainfall;
}