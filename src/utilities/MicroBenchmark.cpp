#include "pedal/MicroBenchmark.hpp"

MicroBenchmark::MicroBenchmark(std::string eventName, long int numberOfIterations){
  completeFlag = false;
  currentIteration = 0;
  numberOfIterations = std::max(numberOfIterations, static_cast<long int>(1));
  initialize(eventName, numberOfIterations);
}
MicroBenchmark::~MicroBenchmark(){
  calculateResults();
  printHighlites();
  printAllResults();
}
void MicroBenchmark::initialize(std::string eventName, long int numberOfIterations){
  timerResults.resize(numberOfIterations);
}
void MicroBenchmark::startTiming(){
  if(!foundStopCall){
    stopTiming();
  }
  pauseTime = 0;
  startTime = std::chrono::high_resolution_clock::now();
  startTime = std::chrono::time_point_cast<std::chrono::microseconds>(startTime);
  foundStopCall = false;
}
void MicroBenchmark::stopTiming(){
  //get the current time
  endTime = std::chrono::high_resolution_clock::now();
  //cast time to microseconds
  endTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTime);
  if(currentIteration < numberOfIterations){
  timerResults[currentIteration] = (endTime.time_since_epoch().count() - startTime.time_since_epoch().count()) - pauseTime;
  }
  currentIteration++;
}
void MicroBenchmark::pauseTimer(){
  pauseStartTime = std::chrono::high_resolution_clock::now();
  pauseStartTime = std::chrono::time_point_cast<std::chrono::microseconds>(pauseStartTime);
}
void MicroBenchmark::resumeTimer(){
  //now - pauseStartTime
  pauseTime += std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() -
               pauseStartTime.time_since_epoch().count();
}
void MicroBenchmark::printHighlites(){
  std::string report = timerName;
  report += ": Number Of Iterations ";
  report += std::to_string(numberOfIterations);
  report += ": average performance ";
  report += std::to_string(average * 1000.0) + "(us)";
  if(average >= 1000.0){report += std::to_string(average) + "(ms)";}
  report += ": slowest performance ";
  report += std::to_string(slowest * 1000.0);
  if(slowest >= 1000.0){report += std::to_string(slowest * 0.001) + "(ms)";}
  report += ": fastest performance ";
  report += std::to_string(fastest);
  if(fastest >= 1000.0){report += std::to_string(fastest * 0.001) + "(ms)";}
  report += ": range ";
  report += std::to_string(range * 1000.0);
  if(range >= 1000.0){report += std::to_string(range * 0.001) + "ms";}
  std::cout << report << std::endl;
}
void MicroBenchmark::printAllResults(){
  std::string results = timerName;
  std::cout << timerName << std::endl;
  for(int i = 0; i < numberOfIterations; i++){
    std::string result = std::to_string(i) + ": ";
    result += std::to_string(timerResults[i]);
    if(timerResults[i] > 1000){
      results += "  " + std::to_string(timerResults[i] * 0.0001f) + "(ms)";
    }
    std::cout << result << std::endl;
  }
}
//private functions
void MicroBenchmark::calculateAverage(){
  double reciprocal = 1.0/static_cast<double>(numberOfIterations);
  double runningTotal = 0.0f;
  for(int i = 0; i < numberOfIterations; i++){
    runningTotal += timerResults[i] * reciprocal;
  }
  average = runningTotal;
}
void MicroBenchmark::findFastestInstance(){
  fastest = std::numeric_limits<double>::max();
  for(int i = 0; i < numberOfIterations; i++){
    if(timerResults[i] < fastest){
      fastest = timerResults[i];
    }
  }
}
void MicroBenchmark::findSlowestInstance(){
  slowest = 0.0;
  for(int i = 0; i < numberOfIterations; i++){
    if(timerResults[i] > slowest){
      slowest = timerResults[i];
    }
  }
}
void MicroBenchmark::calculateResults(){
  calculateAverage();
  findFastestInstance();
  findSlowestInstance();
  range = fastest - slowest;
}