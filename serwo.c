#include "led.h"
#include "keyboard.h"
#include "timer_interrupts.h"
#include <LPC21xx.H>
#include "serwo.h"
#define DETECTOR_bm (1<<10)



struct Servo sServo;

void DetectorInit(void){
  IO0DIR = IO0DIR&(~(DETECTOR_bm));
}


enum DetectorState {ACTIVE, INACTIVE};

enum DetectorState eReadDetector(){
  if((IO0PIN&DETECTOR_bm) == 0){
    return ACTIVE;
  }
  else{
    return INACTIVE;
  }
}


void Automat(void){
    unsigned int uiOffset = 0;
    switch(sServo.eState){
      
      case CALLIB:
        
        if(eReadDetector() == INACTIVE){
          LedStepLeft();
          sServo.eState = CALLIB;
        }
        else{
          sServo.uiCurrentPosition = 0;
          sServo.uiDesiredPosition = 0;
          sServo.eState = OFFSET;
        }
        break;
        
        
      case OFFSET:
        
        if(sServo.uiCurrentPosition == uiOffset){
          sServo.uiCurrentPosition = 0;
          sServo.uiDesiredPosition = 0;
          sServo.eState = IDLE;
        }
        else{
          LedStepLeft();
          sServo.uiCurrentPosition++;
        }
        break;
        
      case IDLE:
        
        if(sServo.uiCurrentPosition != sServo.uiDesiredPosition){ 
          sServo.eState = IN_PROGRESS;
        }
        else{
          sServo.eState = IDLE;
        }
        break;

      case IN_PROGRESS:
        
        if(sServo.uiCurrentPosition < sServo.uiDesiredPosition){
          
          LedStepRight();
          sServo.uiCurrentPosition++;
          sServo.eState = IN_PROGRESS;
          
        }
        else if(sServo.uiCurrentPosition>sServo.uiDesiredPosition){
          
          LedStepLeft();
          sServo.uiCurrentPosition--;
          sServo.eState = IN_PROGRESS;
          
        }
        else{
          sServo.eState = IDLE;
        }
        break;
      }
    }
    
    
void ServoInit(unsigned int uiServoFrequency){
  
  sServo.eState = CALLIB;
  LedInit();
 // Timer0Interrupts_Init((1000000/uiServoFrequency), &Automat);
  
}


void ServoCallib(void){
  sServo.eState = CALLIB;
}

    
void ServoGoTo(unsigned int uiPosition){
  sServo.uiDesiredPosition = uiPosition;
}
