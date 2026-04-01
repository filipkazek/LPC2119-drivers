#include <LPC210X.H>
#include "string.h"

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

#define RECIEVER_SIZE 20
#define TERMINATOR '\r'
#define TRANSMITER_SIZE 30

////////////// Zmienne globalne ////////////

enum eRecieverStatus {EMPTY, READY, OVERFLOW};
enum eTransmiterStatus {FREE, BUSY};

struct RecieverBuffer{
	char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr;
	enum eRecieverStatus eStatus;
};

struct TransmiterBuffer{
	char cData[TRANSMITER_SIZE];
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char cCharCtr;
};

struct TransmiterBuffer sTBuffer;
struct RecieverBuffer sBuffer;

///////////////////////////////////////////
char Transmitter_GetCharacterFromBuffer(){	
		if(sTBuffer.fLastCharacter){
			sTBuffer.eStatus = FREE;
			return NULL;
	} else if (sTBuffer.cData[sTBuffer.cCharCtr] == NULL){
			sTBuffer.fLastCharacter = 1;
			return TERMINATOR;
	} else {
			return sTBuffer.cData[sTBuffer.cCharCtr++];
		}
}

void Reciever_PutCharacterToBuffer(char cCharacter){
		if(sBuffer.ucCharCtr == RECIEVER_SIZE){
			sBuffer.eStatus = OVERFLOW;
			sBuffer.ucCharCtr = 0;
		} else if(cCharacter == TERMINATOR)
		{
			sBuffer.cData[sBuffer.ucCharCtr] = NULL;
			sBuffer.eStatus = READY;
			sBuffer.ucCharCtr = 0;
		} else {
			sBuffer.cData[sBuffer.ucCharCtr] = cCharacter;
			sBuffer.ucCharCtr++;
	}
}

void Transmiter_SendString(char *cString){
	sTBuffer.fLastCharacter = 0;
	sTBuffer.eStatus = BUSY;
	sTBuffer.cCharCtr = 1;
	CopyString(cString, sTBuffer.cData);

	U0THR = sTBuffer.cData[0];
}
enum eTransmiterStatus Transmiter_GetStatus(void){
	return sTBuffer.eStatus;
}

__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
      Reciever_PutCharacterToBuffer(U0RBR);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
			
      if(sTBuffer.eStatus == BUSY) U0THR = Transmitter_GetCharacterFromBuffer();
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}




enum eRecieverStatus eReciever_GetStatus(void){
	return sBuffer.eStatus;
}

void Reciever_GetStringCopy(char * ucDestination){
		CopyString(sBuffer.cData, ucDestination);
		sBuffer.eStatus = EMPTY;
	}		
////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | 0x05;                                     // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = (((15000000)/16)/uiBaudRate);                      // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE;               // wlaczamy interupt recive data available

   // INT
   VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
		
	 sBuffer.eStatus = EMPTY;
	 sBuffer.ucCharCtr = 0;

}

