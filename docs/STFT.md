
# segmented input
```cpp
int segmentSize = 8;
int segmentIndex= 0;
int overlap = 4;
int fftSize = 512;
float inputSegment[segmentSize];
float windowedInput[overlap][fftSize];
float window[fftSize];

float processInput(float inputSample){
  inputSegment[segmentIndex] = inputSample;
  segmentIndex++;
  if(segmentIndex >= segmentSize){
    segmentIndex = 0;//reset index
    for(int i = 0; i < overlap; i++){
      int initialIndex = i * hopSize;
      for(int j = 0; j < segmentSize; j++){
        windowedInput[i][initialIndex + j] = 
            inputSegment[j] * window[initialIndex + j];
      }
    }
  }
}
```