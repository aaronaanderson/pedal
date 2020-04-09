#ifndef MIDIEvent_hpp
#define MIDIEvent_hpp

#include <vector>
class MIDIEvent{
  public:
  MIDIEvent();
  MIDIEvent(std::vector<unsigned char>* bytes);
  enum class EventTypes{
    NOTE_ON, 
    NOTE_OFF, 
    CONTROL_CHANGE,
    PITCH_BEND, 
    POLYPHONIC_AFTERTOUCH,
    MONOPHONIC_AFTERTOUCH,
    PROGRAM_CHANGE,
    UNSUPPORTED  
  };
  void setFromBytes(std::vector<unsigned char>* inputBytes);
  int getChannel();
  int getNoteNumber();
  int getVelocity();
  int getControlNumber();
  int getControlValue();
  EventTypes getEventType();
  private:
  int channel;
  int noteNumber, velocity;
  int controlNumber, controlValue;
  EventTypes eventType;
  std::vector<unsigned char> bytes;
};
#endif