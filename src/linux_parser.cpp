#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
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
   string line , key , value;
   float memFree=0;
   float  memTotal=0;
   std::ifstream stream(kProcDirectory + kMeminfoFilename);
   if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memTotal=std::stof(value);
        }
        if(key == "MemFree"){
          memFree=std::stof(value);
        }

     
      }
    }
  }
   return (memTotal-memFree)/memTotal;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string upTime;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> upTime ) {
        return std::stol(upTime);
      }
    }
  }
  return 1;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
   vector<string> cpuVec = CpuUtilization();
  long jiffies = 0.0;
  for(int i = 0; i < 10; i++){
    jiffies =jiffies+ stol(cpuVec[i]);
  }
  return jiffies;
   }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
   long idle = IdleJiffies();
   long total=Jiffies();
   return total - idle;

}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
   vector<string> cpuVec = CpuUtilization();
 // idlealltime = idletime + ioWait
  return (stol(cpuVec[3]) + stol(cpuVec[4]));
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::vector<std::string> pData(10);
  std::string key;
  std::string line;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> pData[0] >> pData[1] >> pData[2] >> pData[3] >> pData[4] >> pData[5] >> pData[6] >> pData[7] >> pData[8] >> pData[9]) {
        if(key=="cpu") {
          return pData;
        }
      }
    }
  }
  return {};
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string key,value;;
  std::string line;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key>> value) {
        if(key=="processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  string key,value,line;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key>> value) {
        if(key=="procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
string line;   
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) +kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return "0";
 }

// TODO: Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 

  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  string key, value, line;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return value;
        }
      }
    }
  }
  return "0"; 
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  string key, line, number;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> number) {
        if (key == "Uid") {
          return number;
        }
      }
    }
  }

  return "not found"; 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {

   string line,uid,x, user,id;
   uid = Uid(pid);

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> id) {
        if (id == uid) {
          return user;
        }
      }
    }
  }
  return "0"; 
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string starttime,line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=0;i<22;i++){
      linestream >> starttime;
    }
    return std::stol(starttime) / sysconf(_SC_CLK_TCK);
  }
  return 0;
 }
