/***************************************************************************************
THIS SOFTWARE/DOCUMENTATION  IS FOR GUIDANCE ONLY. ST MAKES NO WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE nor for any infringement of patents
or other rights of third parties which may result from its use.

ST MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT INCIDENTAL OR 
CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING IN CONNECTION WITH OR ARISING
FROM THE FURNISHING, PERFORMANCE, OR USE OF THIS SOFTWARE.

Specifications mentioned in this publication are subject to change without notice. 
This publication supersedes and replaces all information previously supplied. 
STMicroelectronics products are not authorized for use as critical components in 
life support devices or systems without the express written approval of 
STMicroelectronics.
***************************************************************************************/

/*_________________________________________________________________________________
 *
 *		  STMicroelectronics : Programming ToolKit
 *
 * 			     Source File : PtkToolBox.cpp
 *
 *	It Contains some functions and procedures relying on the DLLs API and that
 *	may be useful for any applications. 
 *  These functions should be easily reusable and customized by other applications.
 *  These functions mainly intends to abstract the access to the configuration 
 *  files (Get the list of hardware, programming protocols, devices, get the
 *  list of areas and mapping for each areas ...)
 *  
 *  
 *	Date : Oct 24 2001
 *
 *  Compiler : Microsoft Visual C++ 6.00
 *
  ________________________________________________________________________________*/


#include <windows.h>

#include <string.h>
#include <stdio.h>

#include "PtkAPI.h"
#include "PtkToolBox.h"

#define MAXSIZE	30000

const char c_szListDelimiters[] = "\n";
const char c_szPortDelimiters[] = ",";


// GetAliasDevice
// Each customer Device name has an internal alias name we should be used
// for configuration data base requests, this function returns the internal
// Alias name corresponding to the customer name
//
// Parameters:
//		szStdDeviceName : The device usual name (used in STVP interface)
//      szAliasDeviceName : The data base internal name get from data base 

void GetAliasDevice(const char *szStdDeviceName, char *szAliasDeviceName, int iAliasNameSize)
{
	// Get the internal alias name corresponding to the 'customer' device  
	if (!CGetKeyValue("DBCFILE.CNF","ALIAS",szStdDeviceName,szAliasDeviceName)) {
		strncpy(szAliasDeviceName,szStdDeviceName,iAliasNameSize-1);
		szAliasDeviceName[iAliasNameSize-1] = 0;
	} 
}

// GetNextListItem
//
// This function allows to get the first item string in a list, items in the list
// are separated with '\n'. The item is removed from the list
//
// Parameters
//  	strItemList : The Items list
//
// Return  : First list item

void GetNextListItem(char *szItemList, char *szFirstItem)
{
	char *ptrLF;
	// Is the list valid ?
	if (szItemList == NULL) {
		*szFirstItem = 0;
		return;
	}

	// Is there an '\n' separator
	if ((ptrLF = strchr(szItemList,'\n')) != NULL) {
		// Separate the first item from the list
		*ptrLF = 0;
		// Copy it in the destination string
		strncpy(szFirstItem,szItemList,ITEMSIZE);
		szFirstItem[ITEMSIZE-1] = 0;

		// Remove it from the list
		strcpy (szItemList,ptrLF+1);
	}
	else {
		// Is the list empty ?
		if (strlen(szItemList) == 0) {
			// Yes return an empty item
			*szFirstItem = 0;
			return;
		}
		else {
			// The list is not empty but there is no '\n' separator at the end
			// It should be the last item copy the remaining character in the 
			// destination string
			strncpy(szFirstItem,szItemList,ITEMSIZE);
			szFirstItem[ITEMSIZE-1] = 0;
			*szItemList = 0;
		}
	}
}

// TrimList
// Loop on the list of Items to remove blank and tab characters.
//
// Parameters:
// szInitialBuffer: The original list
// szDestBuffer: The output buffer fills will the new list items afters blank and tab 
//               removal. The buffer allocation is in charge of the calling function
// iDestBufferSize:Size of the output buffer
//
// Return:TRUE;

int TrimList(char *szInitialBuffer, char *szDestBuffer,unsigned int iDestBufferSize,
			 const char* szDelimiters)
{
	//parse return string and add each device to list
	char  *szTmp;
	char* pToken = strtok( szInitialBuffer, szDelimiters);
	int i;
	szDestBuffer[0] = 0;
	while((pToken!=NULL) && 
		  ( (strlen(szDestBuffer) + strlen(pToken) + 1)<iDestBufferSize))	{
		szTmp = pToken;
		// Suppress blank and tab characters at the beginning of the token
		while ( (*szTmp == ' ') || (*szTmp == '\t')) {
			szTmp++;
		}
		// and at the end of the token
		for (i=strlen(szTmp)-1;szTmp[i] == ' ' || szTmp[i] == '\t'; i--) {
			szTmp[i] = 0;
		}
		strcat(szDestBuffer,szTmp);
		strcat(szDestBuffer,"\n");
		pToken = strtok( NULL, szDelimiters);
	}
	return( TRUE);
}

/////////////////////////////////////////////////////////////////////////
//   The following functions constitute the Programming Toolkit 
//   Helper functions, they are documented in the Programming
//   Toolkit documentation
/////////////////////////////////////////////////////////////////////////

int GetHardwareList(char *szListBuffer,int iBufferSize)
{
	int any;
	char szRep[MAXSIZE];

	any = CGetList("tools.cnf","TOOLS.PROGRAMER",szRep);
	TrimList(szRep,szListBuffer,iBufferSize,c_szListDelimiters);

	return (any == 1);
}

int GetProtocolList(const char * szHardName, char *szListBuffer,int iBufferSize)
{
	int any;
	char szRep[MAXSIZE];
	char szQuery[100];
	// Get the protocol list attached to this hardware
	sprintf(szQuery,"TOOLS.PROGRAMER.%s.PROTOCOLLIST",szHardName);
	any = CGetList("tools.cnf",szQuery,szRep);
	TrimList(szRep,szListBuffer,iBufferSize,c_szListDelimiters);
	return (any == 1);
}

int GetDeviceList(const char * szHardName,const char *szProtocolName, char *szListBuffer,int iBufferSize)
{
	int any;
	char szRep[MAXSIZE];
	char szQuery[100];
	// Get the protocol list attached to this hardware
	sprintf(szQuery,"TOOLS.PROGRAMER.%s.PROTOCOLLIST.%s.DEVICELIST",szHardName,szProtocolName);
	any = CGetSection("tools.cnf",szQuery,szRep);
	TrimList(szRep,szListBuffer,iBufferSize,c_szListDelimiters);
	return (any == 1);
}


int GetPortList(const char * szHardName, char *szListBuffer,int iBufferSize)
{
	int any;
	char szRep[MAXSIZE];
	char szQuery[100];
	// Get the list of communication port attached to this hardware
	sprintf(szQuery,"TOOLS.PROGRAMER.%s",szHardName);
	any =  CGetKeyValue("TOOLS.CNF", szQuery, "Port", szRep);
	
	TrimList(szRep,szListBuffer,iBufferSize,c_szPortDelimiters);

#if 0
	//parse returned string and add each device to list
	char  *szTmp;
	char* pToken = strtok( szRep, ",");
	int i;
	szListBuffer[0] = 0;
	while((pToken!=NULL) && 
		  ( (strlen(szListBuffer) + strlen(pToken) + 1)<(unsigned int) iBufferSize))	{
		szTmp = pToken;
		// Suppress blank and tab characters at the beginning of the token
		while ( (*szTmp == ' ') || (*szTmp == '\t')) {
			szTmp++;
		}
		// and at the end of the token
		for (i=strlen(szTmp)-1;szTmp[i] == ' ' || szTmp[i] == '\t'; i--) {
			szTmp[i] = 0;
		}
		strcat(szListBuffer,szTmp);
		strcat(szListBuffer,"\n");
		pToken = strtok( NULL, ",");
	}
#endif
	return (any == 1);
}

int GetAreaList(const char *szDeviceName, char *szListBuffer,int iBufferSize)
{
	int any;
	char szRep[MAXSIZE];
	char szQuery[100];
	char szDeviceAlias[ITEMSIZE];

	// Get the internal alias corresponding to the 'customer' device name  
	GetAliasDevice(szDeviceName,szDeviceAlias,sizeof(szDeviceAlias));

	// Get the areas list attached to this hardware
	sprintf(szQuery,"DEVICE.%s.Eprcore.Tab",szDeviceAlias);
	any = CGetList("DBCFILE.CNF",szQuery,szRep);
	TrimList(szRep,szListBuffer,iBufferSize,c_szListDelimiters);
	return (any == 1);
}

int GetProtectionList(const char * szDeviceName, char *szListBuffer,int iBufferSize)
{
	int any;
	char szRep[MAXSIZE];
	char szQuery[100];
	char szDeviceAlias[ITEMSIZE],szAreaName[ITEMSIZE];
	char szAreaList[LISTSIZE],szProtectionList[LISTSIZE];

	// Get the internal alias corresponding to the 'customer' device name  
	GetAliasDevice(szDeviceName,szDeviceAlias,sizeof(szDeviceAlias));

	// get the area list
	GetAreaList(szDeviceName,szAreaList,sizeof(szAreaList));
	GetNextListItem(szAreaList,szAreaName);
	szListBuffer[0] = '\0'; // Empty string
	// For each area search for list of protections
	while (strlen(szAreaName) != 0) {
		// Get the protection list attached to this area
		sprintf(szQuery,"DEVICE.%s.Eprcore.Tab.%s.Protection",szDeviceAlias,szAreaName);
		any = CGetList("DBCFILE.CNF",szQuery,szRep);
		TrimList(szRep,szProtectionList,sizeof(szProtectionList),c_szListDelimiters);

		// Add the protection to the user list if enought space
		if ((strlen(szListBuffer) + strlen(szProtectionList)) < (unsigned int) iBufferSize) {
			strcat(szListBuffer,szProtectionList);
		}

		// Next memory area
		GetNextListItem(szAreaList,szAreaName);
	}
	return (any == 1);
}

int GetNextMapSector(const char *szDeviceName,const char *szAreaName,
			     unsigned long *lFirstAreaAddr,unsigned long *lLastAreaAddr)
{
	char szQuery[100];
	char szRep[MAXSIZE];
	static char szMap[MAXSIZE];
	char szMapItem[ITEMSIZE];

	char szDeviceAlias[ITEMSIZE];


	if (szDeviceName != NULL && szAreaName != NULL) {

		// Get the internal alias corresponding to the 'customer' device name  
		GetAliasDevice(szDeviceName,szDeviceAlias,sizeof(szDeviceAlias));

		// Get the Mapping for the area
		szMap[0] = 0;
		sprintf(szQuery,"DEVICE.%s.Eprcore.Tab.%s.Map",szDeviceAlias,szAreaName);
		if (CGetSection("DBCFILE.CNF",szQuery,szRep) == 0) {
			return 0;
		}
		TrimList(szRep,szMap,MAXSIZE,c_szListDelimiters);

	}
	GetNextListItem(szMap, szMapItem);
	if (sscanf(szMapItem,"[0x%lx-0x%lx]",lFirstAreaAddr,lLastAreaAddr) == 2) {
		// Convert successfully 
		if (strlen(szMap) == 0)  {
			// ... but the end of the list has been reached, notify
			return -1;
		}
		else {
			// let's go
			return 1;
		}
	}
	else {
		// Cannot convert 
		*lFirstAreaAddr=*lLastAreaAddr=-1;
		return 0;
	}
}

int GetAreaMapping(const char *szDeviceName,const char *szAreaName,
			   unsigned long *lFirstAreaAddr,unsigned long *lLastAreaAddr)
{
	int any;
	unsigned long lDummy;
	char szDeviceAlias[ITEMSIZE];

	// Get the internal alias corresponding to the 'customer' device name  
	GetAliasDevice(szDeviceName,szDeviceAlias,sizeof(szDeviceAlias));

	any = GetNextMapSector(szDeviceAlias,szAreaName,lFirstAreaAddr,lLastAreaAddr);
	while (any == 1) {
		any = GetNextMapSector(NULL,NULL,&lDummy,lLastAreaAddr);
	}
	// Reach end of map
	return (any == -1);
}


int FillImageMemory(const char *szDeviceName, const char *szAreaName)
{
	unsigned long lFirstAreaAddr,lLastAreaAddr,dwId;
	long lMemSize;
	unsigned char cBlankValue;
	char szQuery[100],szRep[MAXSIZE];
	char *endptr;
	char *ptrArea;
	unsigned long i;

	char szDeviceAlias[ITEMSIZE];

	// Get the internal alias corresponding to the 'customer' device name  
	GetAliasDevice(szDeviceName,szDeviceAlias,sizeof(szDeviceAlias));

	sprintf(szQuery,"DEVICE.%s.Eprcore.Tab.%s",szDeviceAlias,szAreaName);
	if (CGetKeyValue("DBCFILE.CNF",szQuery,"BlankValue",szRep) == 0) {
			return 0;
	}
	cBlankValue = (unsigned char) strtol( szRep, &endptr, 0);
	if (*endptr != 0) {
		return 0;
	}

	GetAreaMapping(szDeviceName,szAreaName,&lFirstAreaAddr,&lLastAreaAddr);

	if (EGetId(szAreaName,&dwId)==1) {
		ptrArea = EGetImagePtr(dwId, &lMemSize);
		if (ptrArea == NULL) {
			// invalid memory image pointer 
			return 0;
		}
	}
	else {
		// invalid area
		return 0;
	}

	for (i = lFirstAreaAddr; i<=lLastAreaAddr; i++) {
		ptrArea[i] =  cBlankValue;
	}
	return 1;
}

int SetByteInImageMemory(const char *szDeviceName, const char *szAreaName, 
					 unsigned long dwAddr, unsigned char uValue)
{
	long lMemSize;
	char *ptrArea=NULL;
	unsigned long dwId,FirstAreaAddr,LastAreaAddr;

	// Check that address is in the memory mapping range
	GetAreaMapping(szDeviceName,szAreaName,&FirstAreaAddr,&LastAreaAddr);
	if (FirstAreaAddr > dwAddr || LastAreaAddr < dwAddr) {
		return 0;
	}
	// Get the Area ID 
	if (EGetId(szAreaName,&dwId) == 1) {;
	    // then the pointer on memory image
		ptrArea = EGetImagePtr(dwId, &lMemSize);
		if (ptrArea==NULL) {
			// Invalid memory area pointer
			return 0;
		}
	}
	else {
		// invalid area
		return 0;
	}
	ptrArea[dwAddr] = uValue;
	return 1;
}

int GetByteFromImageMemory(const char *szDeviceName, const char *szAreaName, 
					   unsigned long dwAddr, unsigned char *uValue)
{
	long lMemSize;
	char *ptrArea=NULL;
	unsigned long dwId,FirstAreaAddr,LastAreaAddr;

	GetAreaMapping(szDeviceName,szAreaName,&FirstAreaAddr,&LastAreaAddr);
	if (FirstAreaAddr > dwAddr || LastAreaAddr < dwAddr) {
		return 0;
	}
	// Get the Area ID 
	if (EGetId(szAreaName,&dwId) == 1) {
	    // then the pointer on memory image
		ptrArea = EGetImagePtr(dwId, &lMemSize);
		if (ptrArea==NULL) {
			// Invalid memory area pointer
			return 0;
		}
	}
	else {
		// invalid area
		return 0;
	}
	*uValue = ptrArea[dwAddr];
	return 1;
}

int IsErasableArea(const char *szDeviceName, const char *szAreaName)
{
	char szQuery[100];
	char szRep[MAXSIZE];
	int bEraseAble = 1;

	if (szDeviceName==NULL || szAreaName==NULL) {
		return 0;
	}

	if (strcmp(szAreaName,"OPTION BYTE") == 0) {
		// On option byte blank check is definitively not allowed
		bEraseAble = 0;
	}
	else {
		char szDeviceAlias[ITEMSIZE];

		// Get the internal alias corresponding to the 'customer' device name  
		GetAliasDevice(szDeviceName,szDeviceAlias,sizeof(szDeviceAlias));

		// What is the memory physical type ?
		sprintf(szQuery,"DEVICE.%s.Eprcore.Tab.%s",szDeviceAlias,szAreaName);
		if (CGetKeyValue("DBCFILE.CNF",szQuery,"MemType",szRep) == 0) {
			bEraseAble = 0;
		}
		else {
			// Erase is available only on FLASH memory
			bEraseAble = (strcmp(szRep,"FLASH")==0);
		}
	}
	return bEraseAble;
}

int IsBlankCheckAvailable(const char *szDeviceName, const char *szAreaName)
{
	char szQuery[100];
	char szRep[MAXSIZE];

	if (szDeviceName==NULL || szAreaName==NULL) {
		return 0;
	}

	int bAvailable = 1;
	if (strcmp(szAreaName,"OPTION BYTE") == 0) {
		// On option byte blank check is definitively not allowed
		bAvailable = 0;
	}
	else {
		char szDeviceAlias[ITEMSIZE];

		// Get the internal alias corresponding to the 'customer' device name  
		GetAliasDevice(szDeviceName,szDeviceAlias,sizeof(szDeviceAlias));

		// What is the memory physical type ?
		sprintf(szQuery,"DEVICE.%s.Eprcore.Tab.%s",szDeviceAlias,szAreaName);
		if (CGetKeyValue("DBCFILE.CNF",szQuery,"MemType",szRep) == 0) {
			bAvailable = 0;
		}
		else {
			// Blank check available only on FLASH and EPROM memories
			bAvailable = (strcmp(szRep,"EPROM")==0) || (strcmp(szRep,"FLASH")==0);
		}
	}
	return bAvailable;
}