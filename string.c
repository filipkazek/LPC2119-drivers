#include "string.h"

void CopyString(char pcSource[], char pcDestination[]){
	
	unsigned char ucCharacterCounter;
	
	for (ucCharacterCounter=0;(pcSource[ucCharacterCounter]!=NULL);ucCharacterCounter++){
		pcDestination[ucCharacterCounter]=pcSource[ucCharacterCounter];
	}
	pcDestination[ucCharacterCounter]=pcSource[ucCharacterCounter];
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[]){
	
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter=0;(pcStr1[ucCharacterCounter]!=NULL)&&(pcStr2[ucCharacterCounter]!=NULL);ucCharacterCounter++){
		if(pcStr1[ucCharacterCounter]!=pcStr2[ucCharacterCounter]){
			return NOTEQUAL;
		}	
	}
	if(pcStr1[ucCharacterCounter]!=pcStr2[ucCharacterCounter]){
		return NOTEQUAL;
	}	
	return EQUAL;
}	

void AppendString (char pcSourceStr[], char pcDestinationStr[]){
	
	unsigned char ucCharacterCounter;
	
	for (ucCharacterCounter=0;pcDestinationStr[ucCharacterCounter]!=NULL;ucCharacterCounter++){}
	CopyString(pcSourceStr,pcDestinationStr+ucCharacterCounter);
}

void ReplaceCharactersInString(char pcString[], char cOldChar,char cNewChar){
	
	unsigned char ucCharacterCounter;
	
	for (ucCharacterCounter=0;pcString[ucCharacterCounter]!=NULL;ucCharacterCounter ++){
		if(pcString[ucCharacterCounter]==cOldChar){
			pcString[ucCharacterCounter]=cNewChar;
		}
	}	
}

void UIntToHexStr (unsigned int uiValue, char pcStr[]){
       
	unsigned char ucBierzacaTetrada;
	unsigned char ucLicznikTetrad;
 
	pcStr[0] = '0';
	pcStr[1] = 'x';
	pcStr[6] = NULL;
 
	for(ucLicznikTetrad = 0; ucLicznikTetrad < 4; ucLicznikTetrad++){
		ucBierzacaTetrada = (uiValue >> (ucLicznikTetrad*4)) & 0xF;
		if(ucBierzacaTetrada < 10){
			pcStr[5 - ucLicznikTetrad] = '0' + ucBierzacaTetrada;
		}
		else{
			pcStr[5 - ucLicznikTetrad] = 'A' + ucBierzacaTetrada - 10;
		}
	}
}

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue){
	
	char cZnakBiezacy;
	unsigned char ucLicznikZnaku;
	*puiValue=0;
	
	if((pcStr[0] != '0')||(pcStr[1] != 'x')||(pcStr[2] == NULL)){
		return (ERROR);
	}
	for(ucLicznikZnaku=2; pcStr[ucLicznikZnaku]!=NULL; ucLicznikZnaku++){
		cZnakBiezacy = pcStr[ucLicznikZnaku];
		if (ucLicznikZnaku>=6){
			return (ERROR);
		}
		*puiValue= *puiValue<<4;
		if((cZnakBiezacy>='0')&&(cZnakBiezacy<='9')){
			*puiValue= *puiValue + (cZnakBiezacy-'0');
		}else if(((cZnakBiezacy>='a')&&(cZnakBiezacy<='f')) || ((cZnakBiezacy>='A')&&(cZnakBiezacy<='F'))){
			*puiValue= *puiValue + (cZnakBiezacy-'a'+10);		
		}else{
			return (ERROR);
		}
	}
	return (OK);
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]){

	unsigned char ucLicznikZnakow;

	for (ucLicznikZnakow=0; pcDestinationStr[ucLicznikZnakow]!=NULL; ucLicznikZnakow++){}
	UIntToHexStr(uiValue, pcDestinationStr+ucLicznikZnakow);
}
