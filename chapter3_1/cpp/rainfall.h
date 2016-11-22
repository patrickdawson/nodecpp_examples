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

// Will return the average (arithmetic mean) rainfall for the give location
double avg_rainfall(location &loc); // code in rainfall.cc
