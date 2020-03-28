
# Making STFT

The STFT (Short-Time Fourier Transform) is a common audio tool used to convert a time domain input signal into a frequency domain signal. For efficiency, this analysis uses a Fast Fourier Transform which requires that the analysis window be a size in samples exponent of 2. Window sizes 64,128,256,512,1024,2048,4096 are common. A higher window size provies higher spectral resolution at the cost of accuracy in high frequency (smearing); This signal, once converted to the frequency domain, may be modified by modifying 'bin' values. Only the bottom half of the bins are useful; there are 'windowSize/2' useful bins. 

This class should feature a single-sample input, a method of modifying bins if analysis is ready, and a single-sample output. After the input, samples are multiplied by a window value. FFTs with high overlap have more to calculate. Care should be taken when handling the input and output stream to avoid scattered memory reads and writes.

## Handling the Input Stream

I can think of two methods to efficiently handle the input stream. The input can be windowed then stored, or the windowing can occur all at once just before analysis. Windowing before storing is a bit more straightforward to look at, but is memory bound (requires overlap * fftSize floats just for the inputStream). Handling the windowing just before analysis and only storing unaltered input releaves this memory burden, and does not scale exponentially with (fftSize * overlap).

### Windowing Before Storing

This method requires storing a full FFT size array for each overlap
```cpp
float windowedInput[overlap][fftSize];
```
Other than this pitfall, it is fairly straight forward. For each input sample, independantly scale that input sample by the correct window value, then store it in the correct place in the 2D array.

```cpp
int fftSize = 512;
int overlap = 4;
float windowedInput[overlap][fftSize]
float window[fftSize]

float processInput(float inputSample){
  for(int i = 0; i < overlap; i++){
    int index = overlap * hopSize;
    windowedInput[i][index] = inputSample * window[index];
  }
}
```

However, we should expect this to perform poorly due to sequential scattered memory reads and writes. 'index' jumps in each itteration by hopSize. This jump causes scattered read by calling window[index], and a scattered memory write by calling windowedInput[i][index] =. There are two ways to mitigate this issue. The window could be rearranged for better memory access. This puts the hopSize skipping in the data itself, allowing for incremental access in the previous function.
```cpp
void calculateWindow(){
  for(int i = 0; i < fftSize; i += overlap){//for every 'overlap' samples in FFT
    //calculate each overlap's hanning segment in order
    for(int j = 0; j < overlap; j++){
      float phase = (i + (hopSize * j))/(float)fftSize;//phase in window 0 to 1
      window[i + j] = hanning::getSampleFromPhase(phase);
    }
  }
}
```
This is an odd way to store a window, but it would make for better accessing. Also, this does nothing to fix the scattered memory writes on the same line. I propose the following:

#### Segmenting Input for Memory Alignment
One could collect a series of input samples and perform this calculate at once on a series of samples for better memmory access/storage.


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
        windowedInput[i][initialIndex + j] =  inputSegment[j] * window[initialIndex + j];
      }
    }
  }
}
```
It will be interesting to see the performance results of varying segment sizes.

### Storing Before Windowing

Alternatively, the input stream can be stored without windowing. This leaves the windowing calculations to a single sample call. In addition, this call occurs on the same sample as the FFT. However, if the FFT size is equal to or less than the audioCallback bufer size, the work load of each callback will be constant and this is a non-issue. However, if the FFT size is greater than the audioCallback, the workload may be in one buffer call but not the other. If large windows are desired, and constant callback workloads is desired, the higher memory option may be desireable. 

How much memory is needed for the input? Clearly we no longer need to store a copy of the input for each overlap. The analysis must occur every 'hopSize' samples. At this time, STFT must have the previous 'fftSize' samples. Inconveniently, it is crucial that these samples be in order for the analysis to be accurate, so a circular buffer is out of the question.

Consider the case of overlap=4 and fftSize=512
```cpp
         |-------|

overlap 0:[   ][ |][   ][   ][   ][   ][   ][   ]

overlap 1:][   ][| ][   ][   ][   ][   ][   ][   ]

overlap 2: ][   ][   ][   ][   ][   ][   ][   ][   ]

overlap 3:  ][   ][   ][   ][   ][   ][   ][   ][   ]

```
The input stream buffer should minimally be (fftSize) + (hopSize * (overlap - 1)). However, using fftSize * 2 removes a condition check so I deem it worth of the additional 'hopSize' memory.

The input buffer is now a one dimensional array
```cpp
float inputBuffer[fftSize * 2];
```
However, this method does cause the need of a 'fftSize' array to store the windowed FFT input.
```cpp
float windowedInputSegment[fftSize];
```

Taking care not to overwrite data that is needed for the FFT analysis, an input sample can be added to the array.
```cpp
int fftSize = 512;
int overlap = 4;
int hopSize = fftSize/overlap;
int inputBufferSize = fftSize * 2;
float inputBuffer[inputBufferSize]
float windowedInputSegment[fftSize];
float window[fftSize];//pretend it is filled
int whichSegment = 0;
bool readyFlag = false;//return true if bins are safe to manipulate

bool processInput(float inputSample){
  writeIndex = writeIndex % fftSize;
  inputBuffer[writeIndex] = input;
  //place another input fftSize in the past 
  //this is the same as adding fftSize and wrapping in this case
  int redundantIndex = (writeIndex + fftSize) % inputBufferSize;
  inputBuffer[redundantIndex] = input;

  if(writeIndex % hopSize == 0){//If there are enough input samples for analysis
    //fill the windowedInputSegment with the right segment of the input
    int initialIndex = hopSize * whichSegment;
    for(int i = 0; i < fftSize; i++){
      windowedInputSegment[i] = inputBuffer[initialIndex + i] * window[i];
    }
    fft.process(windowedInputSegment);//analyse the windowed input
    readyFlag = true;
  }else{
    readyFlag = false;
  }
  return readyFlag;
}
```

This method reduces the amount of memory needed by windowing the entire analysis segment at once. The memory required for this method is fftSize * 3 compared to the other methods' fftSize * overlap + fftSize. I have chosen this to be the default method, but after benchmarking may make the window before store option available. 

## Handling the Output Stream

For the output stream, each overlap output is added to a global sum, then returned. The windowed result of each fft output must be stored until it is ready to be replaced once more with new data. Therefore, we are stuck with a fftSize*overlap array size for the output stream. Methods of reducing this are left for future exploration. 

Reversing the input segmentation from above, we can improve memory performance by doing the reverse to the output. 

#### Segmented Overlap Add output
```cpp
int fftSize = 512;
int overlap = 4;
int hopSize = fftSize/overlap;
int currentOutputIndex = 0;
int whichLayer = 0;//which overlap layer
float outputBuffer[overlap][fftSize];
int segmentSize = 8;
int segmentIndex = 0;
float outputSegment[segmentSize];//process 8 output samples at a time

float updateOutput(){
  if(readyFlag){//Ready for iFFT 
    iFFt.process(outputBuffer[whichLayer]);
    //window this result in place, then add to output
    for(int i = 0; i < fftSize; i++){
      fftOutput[whichLayer][i] *= window[i];
    }
    whichLayer++;
    whichLayer = whichLayer % overlap;
  }
  float output = 0.0f;
  if(currentOutputIndex % segmentSize == 0){
    for(int i = 0; i < overlap; i++){//for every overlap
      //look back 'hopSize*i' samples
      int overlapOffsetIndex = (currentOutputIndex - (hopSize * i) + windowSize) % windowSize;
      for(int j = 0; j < segmentSize; j++){//overlap add the next 'segmentSize' samples
        outputSegment[j] += outputBuffer[i][overlapOffset + j];
      }
    }
    output = outputSegment[0];//return the newly calculated first sample
  }else{//the values are ready for output
    output = outputSegment[currentOutputIndex % segmentSize];
  }
  currentOutputIndex++;
  currentOutputIndex = currentOutputIndex % fftSize;
  return output;
}

```

## Expected Use

To show expected behavior, I have written a small real-time convolution app below

```cpp
Buffer soundOne;//soundfile
BufferPlayer playerOne(&soundOne);//soundfile player
STFT stftOne;//stft

Buffer soundTwo;
BufferPlayer playerTwo(&soundTwo);
STFT stftTwo;

void audioCallback(float buffer, int bufferSize, int nChannels){
  for(int i = 0; i < bufferSize; i++){
    //get the sample player output
    float sampleOne = playerOne.update();
    float sampleTwo = playerTwo.update();
    //instert sample player to stft
    stftOne.updateInput(sampleOne);
    stftTwo.updateInput(sampleTwo);
    if(stftOne.isReady() && stftTwo.isReady()){//if both are ready
      for(int j = 0; j < stftOne.getNumberOfBins(); j++){//for each bin
        float newMagnitude = stftOne.getMagnitude() * stftTwo.getMagnitude();
        stftOne.setMagnitude(magnitude);
      }
    }
    float output = stftOne.updateOutput();//do processing for output
    buffer[i * nChannels + 0] = output;
    buffer[i * nChannels + 1] = output;
  }
}