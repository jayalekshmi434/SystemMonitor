#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
   std::vector<string> cpuVals = LinuxParser::CpuUtilization();

    long user = std::stol(cpuVals[LinuxParser::CPUStates::kUser_]);
    long nice = std::stol(cpuVals[LinuxParser::CPUStates::kNice_]);
    long system = std::stol(cpuVals[LinuxParser::CPUStates::kSystem_]);
    long idle = std::stol(cpuVals[LinuxParser::CPUStates::kIdle_]);
    long iowait = std::stol(cpuVals[LinuxParser::CPUStates::kIOwait_]);
    long irq = std::stol(cpuVals[LinuxParser::CPUStates::kIRQ_]);
    long softirq = std::stol(cpuVals[LinuxParser::CPUStates::kSoftIRQ_]);
    long steal = std::stol(cpuVals[LinuxParser::CPUStates::kSteal_]);

    long guest = std::stol(cpuVals[LinuxParser::kGuest_]);
    long guestnice = std::stol(cpuVals[LinuxParser::kGuestNice_]);

    long idleTime;
    long nonIdleTime;
    long totalTime;

  
    //Idle = idle + iowait
    idleTime=idle+iowait;
    
    //PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal

    //NonIdle = user + nice + system + irq + softirq + steal
    user = user - guest;
	nice = nice - guestnice;
    nonIdleTime= user+nice+system+irq+softirq+steal;

    //PrevTotal = PrevIdle + PrevNonIdle
    //Total = Idle + NonIdle
    totalTime=idleTime+nonIdleTime;

    //# differentiate: actual value minus the previous one
    long totald = totalTime-prevTotal;
    long idled= idleTime-prevIdle;

    //CPU_Percentage = (totald - idled)/totald

    //float cpu_util =  (float)((totald - idled)/totald);

    prevTotal=totalTime;
    prevIdle=idleTime;

    return (float)(totald-idled)/totald;
}