#include <string>
#include <vector>

class sample
{
public:
    sample();
    sample(std::string d, double r);
    std::string date;
    double rainfall;
};

class location
{
public:
    double longitude;
    double latitude;
    std::vector<sample> samples;
};

class rain_result {
public:
    float median;
    float mean;
    float standard_deviation;
    int n;
};

bool operator<(const sample &s1, const sample &s2);

// Will return the average (arithmetic mean) rainfall for the give location
double avg_rainfall(location &loc); // code in rainfall.cc
rain_result calc_rain_stats(location &loc); // code in rainfall.cc
