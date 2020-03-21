
# Making STFT

The STFT (Short-Time Fourier Transform) is a common audio tool used to convert a time domain input signal into a frequency domain signal. For efficiency, this analysis uses a Fast Fourier Transform which requires that the analysis window be a size in samples exponent of 2. Window sizes 64,128,256,512,1024,2048,4096 are common. A higher window size provies higher spectral resolution at the cost of accuracy in high frequency (smearing); This signal, once converted to the frequency domain, may be modified by modifying 'bin' values. Only the bottom half of the bins are useful; there are 'windowSize/2' useful bins. 

This class should feature a single-sample input, a method of modifying bins if analysis is ready, and a single-sample output. After the input, samples are multiplied by a window value. FFTs with high overlap have more to calculate. Care should be taken when handling the input and output stream to avoid scattered memory reads and writes.

## Handling the Input Stream

There seems to exist to differening methods to efficiently handle the input stream. The input can be windowed then stored, or the windowing can occur all at once just before analysis. Windowing before storing is a bit more straightforward to look at, but is memory bound (requires overlap * fftSize floats just for the inputStream). Handling the windowing just before analysis and only storing unaltered input releaves this memory burden, and does not scale exponentially with (fftSize * overlap).

### Winowing Before Storing


#### Segmenting Input for Memory Alignment


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

### Storing Before Windowing