#include <string>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;
using std::setw;
using std::setfill;

// DOne: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::ostringstream time;
    int h, m, s;
    h = seconds / 3600;
    m = seconds % 3600 / 60;
    s = seconds % 3600 % 60;
    time << setw(2) << setfill('0') << to_string(h) << ':' 
        << setw(2) << setfill('0') << to_string(m) << ":" 
        << setw(2) << setfill('0') << to_string(s);
    return time.str(); 
}