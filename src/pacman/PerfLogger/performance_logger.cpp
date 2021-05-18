#include "performance_logger.hpp"

PerfLogger *PerfLogger::instance = nullptr;

PerfLogger::PerfLogger() { 
    start_times = {};
    durations = {};
}
PerfLogger::~PerfLogger() { /* nothing to do here */ }

void PerfLogger::commit() {
    std::ofstream csv("performance.csv");

    for (auto &entry : durations) {
        csv << entry.first << "; ";

        for (int metric : entry.second) {
            csv << metric << "; ";
        }

        csv << std::endl;
    }

    csv.close();
}

PerfLogger *PerfLogger::getInstance() { 
    if (instance == nullptr) instance = new PerfLogger();
    return instance;
}

void PerfLogger::startJob(const std::string &job_name) {
    auto start = std::chrono::high_resolution_clock::now();

    auto iterator = start_times.find(job_name);
    if (iterator == start_times.end()) {
        std::cout << "Job \"" << job_name << "\" started" << std::endl;
        start_times[job_name] = start; 
    } else {
        std::cout << "ERROR: Job \"" << job_name << "\" was already started!" << std::endl;
    }
}

void PerfLogger::stopJob(const std::string &job_name) {
    using namespace std::chrono;
    auto stop = high_resolution_clock::now();

    auto iterator = start_times.find(job_name);
    if (iterator == start_times.end()) {
        std::cout << "ERROR: Job \"" << job_name << "\" was never started!" << std::endl;
    } else {
        auto duration = duration_cast<microseconds>(stop - start_times[job_name]);
        durations[job_name].push_back(duration.count());
        std::cout << "Job \"" << job_name << "\" ended and logged." << std::endl;
        start_times.erase(iterator);
    }
}