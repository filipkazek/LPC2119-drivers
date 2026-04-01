#include "uart.h"
#include "string.h"
#include "timer_interrupts.h"
#include "command_decoder.h"

char Tx[11];
char ucCalc[20];
char Rx[RECIEVER_SIZE];
unsigned char fCalc = 0;
struct Watch{
	unsigned char ucMinutes, ucSeconds, fSecChange, fMinChange;
};
struct Watch myWatch;

void WatchUpdate(){
    myWatch.ucSeconds++;
    if(myWatch.ucSeconds == 10) {
        myWatch.ucSeconds = 0;
        myWatch.ucMinutes++;
        myWatch.fMinChange = 1;
    }
    myWatch.fSecChange = 1;
}

int main (){
  UART_InitWithInt(9600);
	Timer0Interrupts_Init(1, &WatchUpdate);
	
  while(1){
		if(eReciever_GetStatus() == READY){ 
		Reciever_GetStringCopy(Rx);
		DecodeMsg(Rx);
		if(ucTokenNr >0 && asToken[0].uValue.eKeyword == CALC && asToken[1].eType == NUMBER){
			 CopyString("calc ", ucCalc);
			 AppendUIntToString(asToken[1].uValue.uiNumber*2, ucCalc);
			 fCalc = 1;
		}
		ucTokenNr=0;
	}	
		if(Transmiter_GetStatus() == FREE){
				if(fCalc){
					Transmiter_SendString(ucCalc);
					fCalc = 0;
				}
				else if(myWatch.fMinChange){
					CopyString("min ", Tx);
					AppendUIntToString((unsigned int)myWatch.ucMinutes, Tx);
					Transmiter_SendString(Tx);
					myWatch.fMinChange = 0;
			} else if(myWatch.fSecChange){
					CopyString("sec ", Tx);
					AppendUIntToString((unsigned int)myWatch.ucSeconds, Tx);
					Transmiter_SendString(Tx);
					myWatch.fSecChange = 0;
		}
	}
}
}