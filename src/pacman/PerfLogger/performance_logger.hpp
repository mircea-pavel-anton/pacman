#ifndef PERFORMANCE_LOGGER_HPP
#define PERFORMANCE_LOGGER_HPP

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <iostream>

#ifdef DEBUG
    #define PERFLOGGER_START_JOB(job_name) PerfLogger::getInstance()->startJob(job_name);
    #define PERFLOGGER_STOP_JOB(job_name) PerfLogger::getInstance()->stopJob(job_name);
    #define PERFLOGGER_COMMIT_METRICS() PerfLogger::getInstance()->commit();
#else
    #define PERFLOGGER_START_JOB(job_name) do {} while(false);
    #define PERFLOGGER_STOP_JOB(job_name) do {} while(false);
    #define PERFLOGGER_COMMIT_METRICS(job_name) do {} while(false);
#endif
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
