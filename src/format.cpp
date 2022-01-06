#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) {  
    long hour = seconds / 3600;
    long minute = (seconds % 3600) / 60;
    long second = seconds % 60;
    string time;
    time= std::to_string(hour)+":" + std::to_string(minute)+":"+std::to_string(second);
    return time;
   }