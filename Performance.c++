#include "Performance.h"
#include <FL/Fl.H>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

void printMemoryUsage() {
    std::ifstream status_file("/proc/self/status");
    std::string line;
    while (std::getline(status_file, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            std::istringstream iss(line);
            std::string key, unit;
            long value_kb;
            iss >> key >> value_kb >> unit;
            std::cout << "Memory (RSS): " << value_kb * 1024 << " bytes\n";
            break;
        }
    }
}

void printCPUUsage() {
    std::ifstream stat_file("/proc/self/stat");
    std::string dummy;
    long utime, stime;
    
    for (int i = 0; i < 13; i++) stat_file >> dummy;
    stat_file >> utime >> stime;

    long ticks_per_sec = sysconf(_SC_CLK_TCK);
    
    stat_file.clear();
    stat_file.seekg(0);
    long starttime;
    for (int i = 0; i < 21; i++) stat_file >> dummy;
    stat_file >> starttime;

    std::ifstream uptime_file("/proc/uptime");
    double uptime;
    uptime_file >> uptime;

    double seconds = uptime - (starttime / (double)ticks_per_sec);
    double cpu_percent = 100.0 * ((utime + stime) / (double)ticks_per_sec) / seconds;

    std::cout << "CPU usage: " << cpu_percent << " %\n";
}

void monitorCallback(void* data) {
    printMemoryUsage();
    printCPUUsage();
    Fl::repeat_timeout(2.0, monitorCallback, data);
}