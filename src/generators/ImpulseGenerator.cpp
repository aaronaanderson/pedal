#include "pedal/ImpulseGenerator.hpp"

//Constructors and Deconstructors=========
ImpulseGenerator::ImpulseGenerator(){

}
ImpulseGenerator::ImpulseGenerator(float initialFrequency){

}
ImpulseGenerator::~ImpulseGenerator(){
    delete[] currentBlock;
}
//core functionality======================
float ImpulseGenerator::generateSample(){

}
float* ImpulseGenerator::generateBlock(){

}
//Getters and Setters
void ImpulseGenerator::setFrequency(float newFrequency){

}
void ImpulseGenerator::setMaskChance(float newMaskChance){

}
void ImpulseGenerator::setDeviation(float newDeviation){

}
float ImpulseGenerator::getSample(){return currentSample;}
float* ImpulseGenerator::getBlock(){return currentBlock;}
float ImpulseGenerator::getFrequency(){return frequency;}
float ImpulseGenerator::getMaskChance(){return maskChance;}
float ImpulseGenerator::getDeviation(){return deviation;}
