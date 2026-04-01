#define TRANSMITER_SIZE 30
#define RECIEVER_SIZE 20
void UART_InitWithInt(unsigned int uiBaudRate);
extern char cOdebranyZnak;

enum eRecieverStatus {EMPTY, READY, OVERFLOW};
struct RecieverBuffer{
	char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr;
	enum eRecieverStatus eStatus;
};

enum eRecieverStatus eReciever_GetStatus(void);
extern struct RecieverBuffer sBuffer;
void Reciever_GetStringCopy(char * ucDestination);

enum eTransmiterStatus {FREE, BUSY};
struct TransmiterBuffer{
	char cData[TRANSMITER_SIZE];
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char cCharCtr;
};

extern struct TransmiterBuffer sTBuffer;
char Transmitter_GetCharacterFromBuffer();
void Transmiter_SendString(char *cString);
enum eTransmiterStatus Transmiter_GetStatus(void);