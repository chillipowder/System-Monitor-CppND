#include <vector>
#include <string>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stoi;
using std::cout;

// Done: Return the aggregate CPU utilization
float Processor::Utilization() { 
    active_ = LinuxParser::ActiveJiffies();
    total_ = LinuxParser::Jiffies();

    total_d = total_ - prev_total_;
    active_d = active_ - prev_active_;
    cpu_perc_ = active_d / total_d;

    //save current values in order to calculate delta next time Utilization is called
    prev_active_ = active_;
    prev_total_ = total_;

    return cpu_perc_; 
}