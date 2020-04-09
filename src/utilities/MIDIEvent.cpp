#include "pedal/MIDIEvent.hpp"
MIDIEvent::MIDIEvent(){
  clear();
}
MIDIEvent::MIDIEvent(std::vector<unsigned char>* inputBytes){
  setFromBytes(inputBytes);
}
void MIDIEvent::setFromBytes(std::vector<unsigned char>* inputBytes){
  
  for(int i = 0; i < inputBytes->size(); i++){
    bytes.push_back(inputBytes->at(i));
  }
  //https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message
  unsigned char firstByte = bytes[0];
  unsigned char eventTypeBinary = firstByte & 0b11110000;//mask the last 4 bits
  channel =  firstByte & 0b00001111;//mask the last four bits
  switch(eventTypeBinary){
    case 0b10000000://note off
      eventType = EventTypes::NOTE_OFF;
      //the first byte in the array is the note
      noteNumber = bytes[1] & 0b01111111;//mask the first value
      noteVelocity = bytes[2] & 0b01111111;//mask the first value
    break;
    case 0b10010000://note on
      eventType = EventTypes::NOTE_ON;
      noteNumber = bytes[1] & 0b01111111;//mask the first value
      noteVelocity = bytes[2] & 0b01111111;//mask the first value
    break;
    case 0b10100000://Poly Aftertouch
      eventType = EventTypes::POLYPHONIC_AFTERTOUCH;
    break;
    case 0b10110000://control change
      eventType = EventTypes::CONTROL_CHANGE;
      controlNumber = bytes[1] & 0b01111111;
      controlValue = bytes[2] & 0b01111111;
    break;
    case 0b11000000://program change
      eventType = EventTypes::PROGRAM_CHANGE; 
    break;
    case 0b11010000://mono Aftertouch
      eventType = EventTypes::POLYPHONIC_AFTERTOUCH;
    break;
    case 0b11100000://pitch bend change
      eventType = EventTypes::PITCH_BEND;
      controlValue = (int)(bytes[1] << 7) + (int)bytes[2];
    break;
    default:
      eventType = EventTypes::UNSUPPORTED;
    break;
  }
}
void MIDIEvent::clear(){
  bytes.clear();
  channel = 0;
  noteNumber = noteVelocity = 0;
  controlNumber = controlValue = 0;
  eventType = EventTypes::UNSUPPORTED;
}
int MIDIEvent::getChannel(){return channel;}
int MIDIEvent::getNoteNumber(){return noteNumber;}
int MIDIEvent::getNoteVelocity(){return noteVelocity;}
int MIDIEvent::getControlNumber(){return controlNumber;}
int MIDIEvent::getControlValue(){return controlValue;}
MIDIEvent::EventTypes MIDIEvent::getEventType(){return eventType;}