#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string name, line;
  double mem_used, mem_free, mem_total, temp;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()) {
    while(getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> temp;
      if(name == "MemTotal:")
      {
        mem_total = temp;
      } else if (name == "MemFree:")
      {
        mem_free = temp;
      }
    }
  }
  mem_used = mem_total - mem_free;
  return mem_used / mem_total;
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() { 
  double time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> time;
  }
  return time; 
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies(); 
}

// Done: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long jiffies;
  string line;
  string utime, stime, cutime, cstime;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  if(file.is_open()) {
    std::getline(file, line);
    std::istringstream linestream(line);
    // skip first 13 entries in stream 
    int n=13;
    string skip;
    for(int i=0; i < n; i++) {
      linestream >> skip;
    }
    linestream >> utime >> stime >> cutime >> cstime;
    jiffies = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
  }
  return jiffies;
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> jiffies = CpuUtilization();
  long active_jif = stol(jiffies[kUser_]) + stol(jiffies[kNice_]) + stol(jiffies[kSystem_]) + stol(jiffies[kIRQ_]) + stol(jiffies[kSoftIRQ_]) + stol(jiffies[kSteal_]);
  return active_jif; 
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  long idle_jif = stol(jiffies[kIdle_]) + stol(jiffies[kIOwait_]);
  return idle_jif;
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  vector<string> v;
  std::ifstream file(kProcDirectory + kStatFilename);
  if(file.is_open()) {
    std::getline(file, line);
    std::istringstream filestream(line);
    filestream >> cpu >> user >> nice >> system >> idle >>  iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    v = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  }
  return v; 
} 

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int processes;
  string name, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> processes;
      if(name == "processes"){
        break;
      }
    }
  } 
  return processes; 
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int processes;
  string name, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> processes;
      if(name == "procs_running"){
        break;
      }
    }
  } 
  return processes; 
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream file(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(file.is_open()) {
    std::getline(file, cmd);
  }
  return cmd;  
}

// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  long ram; 
  string line, name;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  if(file.is_open()) {
    while(std::getline(file, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if(name == "VmSize:") {
        linestream >> ram;
        break;
      }
    }
    ram = ram / 1000;
  }
  return to_string(ram); 
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, name, uid;  
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if(name == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid; 
}

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string name, x, file_uid;
  string line;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> x >> file_uid;
      if(file_uid == uid) {
        break;
      }
    }
  }
  return name;
}

// Done: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptime; 
  string skip, line;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  if(file.is_open()) {
    std::getline(file, line);
    std::istringstream linestream(line);
    int n = 21;
    for(int i = 0; i < n; i++) {
      linestream >> skip;
    }
    linestream >> uptime;
  } 
  return uptime;
}