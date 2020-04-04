#ifndef MicroBenchmark_hpp
#define MicroBenchmark_hpp

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm> 
#include <cmath> 
#include <limits>

class MicroBenchmark{
  public:
  MicroBenchmark(std::string eventName = "event", long int numberOfIterations = 1000);
  ~MicroBenchmark();
  void initialize(std::string eventName, long int numberOfIterations);
  void startTiming();//start next event timing
  void stopTiming();//end event timing
  void pauseTimer();//pause timer mid-event
  void resumeTimer();//resume timer mid-event
  void printAllResults();
  //void saveResultsToFile(std::string filePath);
  void printHighlites();
  bool getCompleteFlag();

  private:
  void calculateResults();
  void calculateAverage();//in microseconds
  void findFastestInstance();
  void findSlowestInstance();
  double average;
  double slowest;
  double fastest;
  double range;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> pauseStartTime;
  long long pauseTime;//in microseconds
  std::vector<long long> timerResults;
  long numberOfIterations;
  long currentIteration;
  std::string timerName;
  bool completeFlag;
  bool foundStopCall;
};
#endif 