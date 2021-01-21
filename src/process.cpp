#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Done: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
// Average cpu usage since the process has started 
float Process::CpuUtilization() { 
    //total process time in seconds 
    float proc_total_time = LinuxParser::ActiveJiffies(pid_) / float(sysconf(_SC_CLK_TCK));
    //elapsed time since process has started in seconds
    float proc_time_frame = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_) / float(sysconf(_SC_CLK_TCK)));
    if(proc_time_frame != 0) {
      cpu_util_ = proc_total_time / proc_time_frame;
      if(cpu_util_ >= 1) {
        cpu_util_ = 1.;
      }
    } else {
        cpu_util_ = 0;
    }
    return cpu_util_; 
}

// Done: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Done: Return this process's memory utilization
string Process::Ram() { 
  if(LinuxParser::Ram(pid_)!= "") {
    ram_ = std::stoi(LinuxParser::Ram(pid_));
  } else {
    ram_ = 0;
  }
  return LinuxParser::Ram(pid_); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Done: Return the age of this process (in seconds)  
long int Process::UpTime() { 
  long int uptime = LinuxParser::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
  return uptime; 
}

// Done: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 

  return this->cpu_util_ > a.cpu_util_;   
  //return this->ram_ > a.ram_; 

}