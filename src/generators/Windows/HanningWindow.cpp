#include "pedal/Windows.hpp"

HanningWindow::HanningWindow(){

}
HanningWindow::~HanningWindow(){
  delete[] currentBlock;
}

float HanningWindow::generateSample(){
  
}

float* HanningWindow::generateBlock(){
  if(currentBlock == nullptr){
    currentBlock = new float[pdlSettings::bufferSize];
  }
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    currentBlock[i] = generateSample();
  }
  return currentBlock;
}