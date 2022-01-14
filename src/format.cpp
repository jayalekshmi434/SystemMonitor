#include <string>
#include<cmath>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::AddZero(int t) {
    if (t==0) 
        return "00";    
    else if (t<10) 
        return "0" + std::to_string(t);
    else 
        return std::to_string(t);
}

string Format::ElapsedTime(long seconds) {  
    long hour = floor(seconds / 3600);
    long minute = floor((seconds - hour*3600)/ 60);
    long second = seconds -hour*3600 -minute*60;
    string time;
    time= Format::AddZero(hour)+":" + Format::AddZero(minute)+":"+Format::AddZero(second);
    return time;


   }