#include "pedal/WTSine.hpp"

//====================================SineTableCPP
SineTable::SineTable(){//when it is time to build a table (constructor)
  table = new float[TABLESIZE];//allocate memory for the table
  for(int i = 0; i < TABLESIZE; i++){//for every memory location
    //convert i from 0 - (TABLESIZE-1) to 0 - TWO_PI
    float phase = (i * TABLESIZE * 6.2831853072)/float(TABLESIZE);
    table[i] = sin(phase);//store the sample in the table
  }
}
//since the construct was private, an access method must be included
SineTable* SineTable::getInstance(){
  if(instance == nullptr){
    instance = new SineTable();
  }
  return instance;
}
//access to the data in the table
float* SineTable::getTable(){return table;}
//initiate the static pointer as a nullptr
SineTable* SineTable::instance = nullptr;//@kee is this necessary?


//================================Table player