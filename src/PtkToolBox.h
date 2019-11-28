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
 *		STMicroelectronics : Programming ToolKit
 *
 * 		       Header File : PtkToolBox.h
 *
 *	Header file associated with PtkToolBox.cpp source file. 
 *  It provides function's prototypes.
 *  
 *	Date : Oct 24 2001
 *
 *  Compiler : Microsoft Visual C++ 6.00
 *
  ________________________________________________________________________________*/

#define LISTSIZE	5000
#define ITEMSIZE	50

void GetNextListItem(char *szItemList, char *szFirstItem);

// Access to configuration files information
int GetHardwareList(char *szListBuffer,int iBufferSise);
int GetProtocolList(const char * szHardName, char *szListBuffer,int iBufferSise);
int GetDeviceList(const char * szHardName,const char *szProtocolName, char *szListBuffer,int iBufferSise);
int GetPortList(const char * szHardName, char *szListBuffer,int iBufferSize);

int GetAreaList(const char * szDeviceName, char *szListBuffer,int iBufferSise);
int GetProtectionList(const char * szDeviceName, char *szListBuffer,int iBufferSize);
int GetNextMapSector(const char *szDeviceName,const char *szAreaName,
			     unsigned long *lFirstAreaAddr,unsigned long *lLastAreaAddr);
int GetAreaMapping(const char *szDeviceName,const char *szAreaName,
			   unsigned long *lFirstAreaAddr,unsigned long *lLastAreaAddr);

int IsBlankCheckAvailable(const char *szDeviceName, const char *szAreaName);
int IsErasableArea(const char *szDeviceName, const char *szAreaName);

// Access to image memories
int FillImageMemory(const char *szDeviceName,const char *szAreaName);
int SetByteInImageMemory(const char *szDeviceName,const char *szAreaName, 
					 unsigned long dwAddr, unsigned char uValue);
int GetByteFromImageMemory(const char *szDeviceName,const char *szAreaName, 
					   unsigned long dwAddr, unsigned char *uValue);
