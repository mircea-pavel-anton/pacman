#ifndef PERFORMANCE_LOGGER_HPP
#define PERFORMANCE_LOGGER_HPP

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <iostream>

class PerfLogger {

public:
    ~PerfLogger();
    
    static PerfLogger *getInstance();

    PerfLogger(const PerfLogger&) = delete;
    PerfLogger& operator=(const PerfLogger&) = delete;

    void commit();
    void startJob(const std::string &);
    void stopJob(const std::string &);
private:
    static PerfLogger *instance;

    PerfLogger();

    std::map<const std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> start_times;
    std::map<const std::string, std::vector<int64_t>> durations;

}; //PerfLogger

#endif //PERFORMANCE_LOGGER_HPP
