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
 *			  STMicroelectronics : Programming ToolKit
 *
 * 			         Source File : PtkAPI.cpp
 *
 *	 
 *  This file provides a set of functions that may be used to access the DLLs API
 *  without linking the application with the .lib files generated by Microsoft
 *  Visual Studio.
 *
 *	This functions should be reused in other applications. 
 *  TAKE CARE only to the ProgressCB and ErrorCB typedef that are based on the 
 *  CALLBACK keyword specific to Microsoft which is used in place of FAR PASCAL in 
 *  application callback routines such as window procedures and dialog procedures. 
 *  WINAPI is also used in place of FAR PASCAL.
 *  if you are not using Microsoft Visual C++ 6.0. CALLBACK and WINAPI would 
 *  probably have to be replaced with the compiler appropriated calling conventions
 *  keywords. 
 *  
 *	Date : Oct 24 2001
 *
 *  Compiler : Microsoft Visual C++ 6.00
 *
  ________________________________________________________________________________*/


#include <windows.h>

#include <assert.h>
#include <stdio.h>

#include "PtkAPI.h"


// List of exported functions that may be called in EPRCORE.DLL
static char* m_dll_eprcore[] = 
{
	"ESelectHard",
	"ESelectDevice",
	"ESelectPort",
	"ESetPreferences",
	"EGetId",
	"ELoadFile",
	"ESaveFile",
	"EGetImagePtr",
	"ECheckSum",
	"EProgAll",
	"EProgArea",
	"EAllRead",
	"EReadArea",
	"EVerifyAll",
	"EVerifyArea",
	"EBlankAll",
	"EBlankArea",
	"EEraseAll",
	"EEraseArea",
	"ESetProtection",
	"ECloseComm",
};
#define NB_FUNC_EPRCORE (sizeof(m_dll_eprcore)/sizeof(char*))

// List of exported functions that may be called in DBCA.DLL
static char* m_dll_dbca[] = 
{
	"CGetList",
	"CGetSection",
	"CGetKeyValue",
	"CGetLastError",
	"CSetWorkingDir",
};
#define NB_FUNC_DBCA (sizeof(m_dll_dbca)/sizeof(char*))

// List of exported functions that may be called in LEF.DLL
static char* m_dll_lef[] = 
{
	"LSetErrorCallBack",
	"LSetWarningCallBack",
	"LSetMessageCallBack",
	"LSetProgressCallBack",
	"LDisplayError",
	"LDisplayWarning",
	"LDisplayMessage",
	"LDisplayProgress",
	"LOpenLog",
	"LTraceLog",
	"LCloseLog",
	"LSetLogOptions"
};
#define NB_FUNC_LEF (sizeof(m_dll_lef)/sizeof(char*))


#define NB_FUNC_DLL NB_FUNC_EPRCORE+NB_FUNC_DBCA+NB_FUNC_LEF

// Array that contains pointer to DLLs exported functions after calling LoadDlls()
NoParamProc m_fp_func[NB_FUNC_DLL];

HINSTANCE m_hEprcore; //EPRCORE Instance handle
HINSTANCE m_hDbca;	  //DBCA Instance handle
HINSTANCE m_hLef;	  //LEF Instance handle


void DisplaySysError()
{
	DWORD dwId = GetLastError();

	LPVOID lpMsgBuf;
	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						dwId,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL 
					);
	// Free the buffer.
	MessageBox(NULL,(char *) lpMsgBuf,NULL,MB_OK|MB_ICONERROR);
	LocalFree( lpMsgBuf );
}

///////////////////////////////////////
//
HMODULE PtkLoadLibrary(char *szPath,char *szFileName)
{

	char szErrorMsg[1024];
	char szDllPathAndName[_MAX_PATH];  // _MAX_PATH in stdlib.h = 260
	HMODULE hDll;

	if (strlen(szPath) != 0) {
		// Add the given path name to the DLL name
		if (szPath[strlen(szPath)-1] == '\\') {
			sprintf(szDllPathAndName,"%s%s",szPath,szFileName);
		}
		else {
			sprintf(szDllPathAndName,"%s\\%s",szPath,szFileName);
		}
	}
	else {
		strcpy(szDllPathAndName,szFileName);
	}

	hDll = LoadLibraryEx(szDllPathAndName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if ( ((int) hDll) <= 32) {
		DisplaySysError();
		wsprintf (szErrorMsg, "Cannot load %s",szDllPathAndName);
		MessageBox(NULL,szErrorMsg,NULL,MB_OK|MB_ICONERROR);
		hDll = 0;
	}
	return hDll;
}

BOOL LoadDlls(char *szDirectoryPathForDlls)
{
	int iFctNb=0;
	int i;
	char pbuf[1024];

	Clean();

	// First load eprcore DLL
	m_hEprcore = PtkLoadLibrary(szDirectoryPathForDlls,"eprcore60.dll");
	if (m_hEprcore == 0) {
		return FALSE;
	}
	// Initialize handles on exported eprcore functions
	for (i=0; i < NB_FUNC_EPRCORE && iFctNb < NB_FUNC_DLL; i++,iFctNb++) {
		m_fp_func[iFctNb] = GetProcAddress ( m_hEprcore, m_dll_eprcore[i]);
		if ( m_fp_func[iFctNb] == NULL) {
			wsprintf( pbuf, "Bad eprcore60.dll DLL : cannot find \"%s\".",
				      m_dll_eprcore[i]);
			MessageBox(NULL,pbuf,NULL,MB_OK|MB_ICONERROR);
			FreeLibrary( m_hEprcore);
			m_hEprcore = NULL;
			return FALSE;
		}
	}
    
	// Then load dbca DLL
	m_hDbca = PtkLoadLibrary(szDirectoryPathForDlls,"dbca60.dll");;
	if (m_hDbca == 0) {
		return FALSE;
	}
	// Initialize handles on exported dbca functions
	for (i=0; i< NB_FUNC_DBCA && iFctNb < NB_FUNC_DLL; i++,iFctNb++) {
		m_fp_func[iFctNb] = GetProcAddress ( m_hDbca, m_dll_dbca[i]);
		if ( m_fp_func[iFctNb] == NULL) {
			wsprintf( pbuf, "Bad dbca60.dll DLL : cannot find \"%s\".",
				      m_dll_dbca[i]);
			MessageBox(NULL,pbuf,NULL,MB_OK|MB_ICONERROR);
			FreeLibrary( m_hDbca);
			m_hDbca = NULL;
			return FALSE;
		}
	}


	// and Finally load lef DLL
	m_hLef = PtkLoadLibrary(szDirectoryPathForDlls,"lef60.dll");;
	if (m_hLef == 0) {
		return FALSE;
	}

	// Initialize handles on exported lef functions
	for (i=0; i< NB_FUNC_LEF && iFctNb < NB_FUNC_DLL; i++,iFctNb++) {
		m_fp_func[iFctNb] = GetProcAddress ( m_hLef, m_dll_lef[i]);
		if ( m_fp_func[iFctNb] == NULL) {
			wsprintf( pbuf, "Bad lef60.dll DLL : cannot find \"%s\".",
				      m_dll_lef[i]);
			MessageBox(NULL,pbuf,NULL,MB_OK|MB_ICONERROR);
			FreeLibrary( m_hLef);
			m_hLef = NULL;
			return FALSE;
		}
	}

	return( TRUE);
}

void Clean()
{
	int nIndex;
	if (m_hEprcore!=NULL)
	{
		FreeLibrary( m_hEprcore);
		m_hEprcore = NULL;
	}

	if (m_hDbca!=NULL)
	{
		FreeLibrary( m_hDbca);
		m_hDbca = NULL;
	}
	
	if (m_hLef!=NULL)
	{
		FreeLibrary( m_hLef);
		m_hLef = NULL;
	}

	for(nIndex=0; nIndex<NB_FUNC_DLL; nIndex++)
	{
		m_fp_func[ nIndex] = NULL;
	}
}

//-----------------------------------
// API functions coming from EPRCORE
//-----------------------------------


BOOL ESelectHard(const char *szHardName,const char* szProtocol, BOOL bDemo)
{ 
	SetHardProc pFunc;
	TestPFunc(ISelectHard); 
	pFunc = (SetHardProc) ISelectHard;
	return (pFunc)(szHardName, szProtocol, bDemo);
	 
}

BOOL ESelectDevice(const char *szDeviceName)
{ 
	SetDeviceProc pFunc;
	TestPFunc(ISelectDevice); 
	pFunc = (SetDeviceProc) ISelectDevice;
	return (pFunc)(szDeviceName);
}

int ESelectPort(const char *szPortNumber)
{
	SetPortProc pFunc;
	TestPFunc(ISelectPort); 
	pFunc = (SetPortProc) ISelectPort;
	return (pFunc)(szPortNumber);
}

BOOL ESetProtection(const char *szProtectMode, BOOL bState)
{
	SetProtectProc pFunc;
	TestPFunc(ISetProtection); 
	pFunc = (SetProtectProc) ISetProtection;
	return (pFunc)(szProtectMode,bState);
}

BOOL ESetPreferences(int iPreference, BOOL bState)
{
	SetPreferencesProc pFunc;
	TestPFunc(ISetPreferences); 
	pFunc = (SetPreferencesProc) ISetPreferences;
	BOOL bRet = (pFunc)(iPreference, bState);
	return (bRet);
}

int EGetId(const char *szAreaName, unsigned long* pdwId)
{
	GetIdProc pFunc;
	TestPFunc(IGetId); 
	pFunc = (GetIdProc) IGetId;
	return (pFunc)(szAreaName,pdwId);
}

int ELoadFile(const char *FileName, unsigned long dwMemType)
{
	FileIOProc pFunc;
	TestPFunc(ILoadFile); 
	pFunc = (FileIOProc) ILoadFile;
	return (pFunc)(FileName,dwMemType);
}

int ESaveFile(const char *FileName, unsigned long dwMemType)
{
	FileIOProc pFunc;
	TestPFunc(ISaveFile); 
	pFunc = (FileIOProc) ISaveFile;
	return (pFunc)(FileName,dwMemType);
}

char *EGetImagePtr(DWORD dwMemType, long *MemSize)
{
	GetImgPtrProc pFunc;
	TestPFunc(IGetImagePtr); 
	pFunc = (GetImgPtrProc) IGetImagePtr;
	return (pFunc)(dwMemType,MemSize);
}
 
int ECheckSum(unsigned long dwMemType, long *nFileCheckSum, long *nMemoryCheckSum)	
{
	ChecksumProc pFunc;
	TestPFunc(ICheckSum); 
	pFunc = (ChecksumProc) ICheckSum;
	return (pFunc)(dwMemType,nFileCheckSum,nMemoryCheckSum);
}

/* Exported functions accessing the Device Memory */

int EProgAll(unsigned long dwMemType)
{
	ActionAllProc pFunc;
	TestPFunc(IProgAll); 
	pFunc = (ActionAllProc) IProgAll;
	return (pFunc)(dwMemType);
}


int EProgArea(unsigned long dwMemType, long FirstAddr, long LastAddr)
{
	ActionAreaProc pFunc;
	TestPFunc(IProgArea); 
	pFunc = (ActionAreaProc) IProgArea;
	return (pFunc)(dwMemType,FirstAddr,LastAddr);
}


int EAllRead(unsigned long dwMemType)
{
	ActionAllProc pFunc;
	TestPFunc(IAllRead); 
	pFunc = (ActionAllProc) IAllRead;
	return (pFunc)(dwMemType);
}


int EReadArea(unsigned long dwMemType, long FirstAddr, long LastAddr)
{
	ActionAreaProc pFunc;
	TestPFunc(IReadArea); 
	pFunc = (ActionAreaProc) IReadArea;
	return (pFunc)(dwMemType,FirstAddr,LastAddr);
}


int EVerifyAll(unsigned long dwMemType)
{
	ActionAllProc pFunc;
	TestPFunc(IVerifyAll); 
	pFunc = (ActionAllProc) IVerifyAll;
	return (pFunc)(dwMemType);
}

int EVerifyArea(unsigned long dwMemType, long FirstAddr, long LastAddr)
{
	ActionAreaProc pFunc;
	TestPFunc(IVerifyArea); 
	pFunc = (ActionAreaProc) IVerifyArea;
	return (pFunc)(dwMemType,FirstAddr,LastAddr);
}



int EBlankAll(unsigned long dwMemType)
{
	ActionAllProc pFunc;
	TestPFunc(IBlankAll); 
	pFunc = (ActionAllProc) IBlankAll;
	return (pFunc)(dwMemType);
}

int EBlankArea(unsigned long dwMemType, long FirstAddr, long LastAddr)
{
	ActionAreaProc pFunc;
	TestPFunc(IBlankArea); 
	pFunc = (ActionAreaProc) IBlankArea;
	return (pFunc)(dwMemType,FirstAddr,LastAddr);
}

int EEraseAll(unsigned long dwMemType)
{
	ActionAllProc pFunc;
	TestPFunc(IEraseAll); 
	pFunc = (ActionAllProc) IEraseAll;
	return (pFunc)(dwMemType);
}


int EEraseArea(unsigned long dwMemType, long FirstAddr, long LastAddr)
{
	ActionAreaProc pFunc;
	TestPFunc(IEraseArea); 
	pFunc = (ActionAreaProc) IEraseArea;
	return (pFunc)(dwMemType,FirstAddr,LastAddr);
}

int ECloseComm()
{
	NoParamProc pFunc;
	TestPFunc(ICloseComm); 
	pFunc = (NoParamProc) ICloseComm;
	return (pFunc)();
}

//-----------------------------------
// API functions coming from DBCA
//-----------------------------------

int CSetWorkingDir(const char *WorkingDir)
{
	SetWorkingDirProc pFunc;
	TestPFunc(ISetWorkingDir); 
	pFunc = (SetWorkingDirProc) ISetWorkingDir;
	return (pFunc)(WorkingDir);
}

int CGetList (const char *FileName, const char *PathSection, char *Buffer)
{
	GetListProc pFunc;
	TestPFunc(IGetList); 
	pFunc = (GetListProc) IGetList;
	return (pFunc)(FileName, PathSection, Buffer);
}

int CGetSection(const char *FileName, const char *PathSection, char *Buffer)
{
	GetSectionProc pFunc;
	TestPFunc(IGetSection); 
	pFunc = (GetSectionProc) IGetSection;
	return (pFunc)(FileName, PathSection, Buffer);
}

int CGetKeyValue(const char *FileName, const char *PathSection, 
				 const char * KeyName, char *Buffer)
{
	GetKeyValueProc pFunc;
	TestPFunc(IGetKeyValue); 
	pFunc = (GetKeyValueProc) IGetKeyValue;
	return (pFunc)(FileName, PathSection, KeyName, Buffer);
}

char *CGetLastError(void)
{
	GetLastErrorProc pFunc;
	TestPFunc(IGetLastError); 
	pFunc = (GetLastErrorProc) IGetLastError;
	return (pFunc)();
}


//-----------------------------------
// Exported functions coming from LEF
//-----------------------------------

int LSetErrorCallBack ( ErrorCB fp_funct)
{
	LSetMessageCallBackProc pFunc;
	TestPFunc(ISetErrorCallBack); 
	pFunc = (LSetMessageCallBackProc) ISetErrorCallBack;
	return (pFunc)(fp_funct);
}

int LSetWarningCallBack ( ErrorCB fp_funct)
{
	LSetMessageCallBackProc pFunc;
	TestPFunc(ISetWarningCallBack); 
	pFunc = (LSetMessageCallBackProc) ISetWarningCallBack;
	return (pFunc)(fp_funct);
}

int LSetMessageCallBack ( ErrorCB fp_funct)
{
	LSetMessageCallBackProc pFunc;
	TestPFunc(ISetMessageCallBack); 
	pFunc = (LSetMessageCallBackProc) ISetMessageCallBack;
	return (pFunc)(fp_funct);
}

int LSetProgressCallBack(ProgressCB fp_funct)
{
	LSetProgressCallBackProc pFunc;
	TestPFunc(ISetProgressCallBack); 
	pFunc = (LSetProgressCallBackProc) ISetProgressCallBack;
	return (pFunc)(fp_funct);
}

int LDisplayError ( const char *szMessage)
{
	LDisplayMessageProc pFunc;
	TestPFunc(IDisplayError); 
	pFunc = (LDisplayMessageProc) IDisplayError;
	return (pFunc)(szMessage);
}

int LDisplayWarning ( const char *szMessage)
{
	LDisplayMessageProc pFunc;
	TestPFunc(IDisplayWarning); 
	pFunc = (LDisplayMessageProc) IDisplayWarning;
	return (pFunc)(szMessage);
}

int LDisplayMessage ( const char *szMessage)
{
	LDisplayMessageProc pFunc;
	TestPFunc(IDisplayMessage); 
	pFunc = (LDisplayMessageProc) IDisplayMessage;
	return (pFunc)(szMessage);
}

int LDisplayProgress (int nAverage)
{
	LDisplayProgressProc pFunc;
	TestPFunc(IDisplayProgress); 
	pFunc = (LDisplayProgressProc) IDisplayProgress;
	return (pFunc)(nAverage);
}


int LOpenLog (const char *szFileName)
{
	LOpenLogProc pFunc;
	TestPFunc(IOpenLog); 
	pFunc = (LOpenLogProc) IOpenLog;
	return (pFunc)(szFileName);
}

int LTraceLog ( const char *szMessage)
{
	LDisplayMessageProc pFunc;
	TestPFunc(ITraceLog); 
	pFunc = (LDisplayMessageProc) ITraceLog;
	return (pFunc)(szMessage);
}

int LCloseLog ()
{
	NoParamProc pFunc;
	TestPFunc(ICloseLog); 
	pFunc = (NoParamProc) ICloseLog;
	return (pFunc)();
}

int LSetLogOptions (unsigned short uOptions, OPERATOR ope)
{
	LSetLogOptionsProc pFunc;
	TestPFunc(ISetLogOptions); 
	pFunc = (LSetLogOptionsProc) ISetLogOptions;
	return (pFunc)(uOptions,ope);
}
