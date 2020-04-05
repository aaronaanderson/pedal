#include "pedal/MicroBenchmark.hpp"

MicroBenchmark::MicroBenchmark(std::string eventName, long iterations){
  completeFlag = false;
  foundStopCall = true;
  currentIteration = 0;
  numberOfIterations = std::max(iterations, static_cast<long>(1));
  initialize(eventName, numberOfIterations);
}
MicroBenchmark::~MicroBenchmark(){
  calculateResults();
  //printHighlites();
  //printAllResults();
}
void MicroBenchmark::initialize(std::string name, long numberOfIterations){
  timerName = name;
  timerResults.resize(numberOfIterations);
}
void MicroBenchmark::startTiming(){
  if(!foundStopCall){
    stopTiming();
  }
  pauseTime = (std::chrono::duration<double>)0.0;
  startTime = std::chrono::high_resolution_clock::now();
  foundStopCall = false;
}
void MicroBenchmark::stopTiming(){
  if(!completeFlag){//if there are more events to time
    //get the current time
    foundStopCall = true;
    endTime = std::chrono::high_resolution_clock::now();
    //cast time to microseconds
    if(currentIteration < numberOfIterations){
      timerResults[currentIteration] = ((endTime - startTime) - pauseTime);
      currentIteration++;
    }else{//if there are no remaining events to time
      calculateResults();
      completeFlag = true;
    }
  }
}
void MicroBenchmark::pauseTimer(){
  pauseStartTime = std::chrono::high_resolution_clock::now();
  //pauseStartTime = std::chrono::time_point_cast<std::chrono::microseconds>(pauseStartTime);
}
void MicroBenchmark::resumeTimer(){
  //now - pauseStartTime
  pauseTime += std::chrono::high_resolution_clock::now() -
               pauseStartTime;
}
void MicroBenchmark::printAllResults(){
  std::string results = timerName;
  std::cout << timerName << std::endl;
  for(int i = 0; i < numberOfIterations; i++){
    std::string result = std::to_string(i) + " | ";
    result += std::to_string(timerResults[i].count() * 1000.0f);
    std::cout << result << std::endl;
  }
}
void MicroBenchmark::printHighlites(){
  std::string report = timerName;
  report += ": Number Of Iterations ";
  report += std::to_string(numberOfIterations);
  report += " | average performance ";
  if(average > 0.0001){
    report += std::to_string(average * 1000.0) + "(ms) ";
  }else{
    report += std::to_string(average * 1000000.0) + "(us) ";
  }
  report += " | slowest performance ";
  if(slowest > 0.00001){
    report += std::to_string(slowest * 1000.0) + "(ms) ";
  }else{
    report += std::to_string(slowest * 1000000.0) + "(us) ";
  }
  report += " | fastest performance ";
  if(fastest > 00001){
    report += std::to_string(fastest * 1000.0) + "(ms) ";
  }else{
    report += std::to_string(fastest * 1000000.0) + "(us) ";
  }
  report += " | range ";
  if(range > 0.0001){
    report += std::to_string(std::abs(range) * 1000.0) + "(ms) ";
  }else{
    report += std::to_string(std::abs(range) * 1000000.0) + "(us) ";
  } 
  std::cout << report << std::endl;
}
bool MicroBenchmark::getCompleteFlag(){return completeFlag;}
//private functions
void MicroBenchmark::calculateAverage(){
  //start with an 0.0 total
  double runningTotal = 0.0f;
  //for every event
  for(int i = 0; i < numberOfIterations; i++){
    //accumulate the result
    runningTotal += timerResults[i].count();
  }
  //the average is the accumulation / numberOfIterations
  average = runningTotal / static_cast<double>(numberOfIterations);
}
void MicroBenchmark::findFastestInstance(){
  //start with the highest possible value
  fastest = std::numeric_limits<double>::max();
  //iterate through each time, replace if lower
  for(int i = 0; i < numberOfIterations; i++){
    if(timerResults[i].count() < fastest){
      fastest = timerResults[i].count();
    }
  }
}
void MicroBenchmark::findSlowestInstance(){
  //begin with the fastest possible value
  slowest = 0.0;
  //iterate through each time, replace if higher
  for(int i = 0; i < numberOfIterations; i++){
    if(timerResults[i].count() > slowest){
      slowest = timerResults[i].count();
    }
  }
}
void MicroBenchmark::calculateResults(){
  calculateAverage();
  findFastestInstance();
  findSlowestInstance();
  range = fastest - slowest;
}