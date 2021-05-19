#include "performance_logger.hpp"

PerfLogger *PerfLogger::instance = nullptr;

PerfLogger::PerfLogger() { 
    start_times = {};
    durations = {};
}
PerfLogger::~PerfLogger() { /* nothing to do here */ }

void PerfLogger::commit() {
    std::ofstream csv("performance.csv");
    std::vector<std::vector<int64_t>> master_vector;

    // Write the column titles to the file
    for (auto &entry : durations) {
        csv << entry.first << ",";
        master_vector.push_back(entry.second);
    }
    csv << std::endl;

    // Get the max size of a vector.
    long unsigned int max_size = 0;
    for (auto &vector : master_vector) {
        if (vector.size() > max_size) max_size = vector.size();
    }

    // Add one element from each vector on each line.
    for (long unsigned int i = 0; i < max_size; i++) {
        for (auto &vector : master_vector) {
            if (vector.size() > i) csv << vector.at(i);
            csv << ",";
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