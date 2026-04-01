#define NULL 0
#define TERMINATOR '\r'
#define DELIMITER_CHAR 0x20

enum CompResult { NOTEQUAL, EQUAL };
enum Result { OK, ERROR };

void CopyString(char pcSource[], char pcDestination[]);
void AppendString (char pcSourceStr[], char pcDestinationStr[]);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[], char cOldChar,char cNewChar);
void UIntToHexStr (unsigned int uiValue, char pcStr[]);
enum CompResult eCompareString(char pcStr1[], char pcStr2[]);
enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
