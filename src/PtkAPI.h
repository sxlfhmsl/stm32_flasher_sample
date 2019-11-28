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
 * 			         Header File : PtkAPI.h
 *
 *	Header file associated with PtkAPI.cpp source file. It provides:
 *    . Prototypes of each API functions
 *	  . typedef used to cast pointers of API's functions
 *    . defines giving the position of each API's pointer in the array containing all
 *  the pointers on all API functions.
 *  
 *	Date : Oct 24 2001
 *
 *  Compiler : Microsoft Visual C++ 6.00
 *
  ________________________________________________________________________________*/


typedef long (CALLBACK *ErrorCB)(const char *);
typedef long (CALLBACK *ProgressCB)(int);

//available options
#define PROTECTION_WARNING	0x01 // For ESetPreferences ftn
#define LOG_MSG				1 
#define LOG_WARN			2 
#define LOG_ERR				4
#define LOG_ALL				LOG_MSG|LOG_WARN|LOG_ERR
typedef enum {OR, AND}		OPERATOR;
#define TestPFunc( x)		if (!x) {assert(FALSE); return( FALSE);}

// Exported functions coming from EPRCORE
// for casting function pointers
typedef BOOL  (WINAPI *NoParamProc) (); 
typedef BOOL  (*SetHardProc) (const char*,const char*, BOOL);
typedef BOOL  (*SetDeviceProc) (const char*);
typedef BOOL  (*SetPortProc) (const char* );
typedef BOOL  (*GetIdProc) (const char *, unsigned long*);
typedef BOOL  (*FileIOProc) (const char *, unsigned long);
typedef char *(*GetImgPtrProc) (DWORD, long *);
typedef BOOL  (*ChecksumProc) (unsigned long, long *, long *);
typedef BOOL  (*ActionAllProc) (unsigned long);
typedef BOOL  (*ActionAreaProc) (unsigned long, long , long);
typedef BOOL  (*SetProtectProc) (const char *, BOOL);
typedef BOOL  (*SetPreferencesProc) (int, BOOL);

// Position of functions in the array of pointers on API function. 
// Order should be the same that in the array providing the function name 
#define  ISelectHard     m_fp_func[0]
#define  ISelectDevice   m_fp_func[1]
#define  ISelectPort     m_fp_func[2]
#define  ISetPreferences m_fp_func[3]
#define  IGetId          m_fp_func[4]
#define  ILoadFile       m_fp_func[5]
#define  ISaveFile       m_fp_func[6]
#define  IGetImagePtr    m_fp_func[7]
#define  ICheckSum       m_fp_func[8]
#define  IProgAll        m_fp_func[9]
#define  IProgArea       m_fp_func[10]
#define  IAllRead        m_fp_func[11]
#define  IReadArea       m_fp_func[12]
#define  IVerifyAll      m_fp_func[13]
#define  IVerifyArea     m_fp_func[14]
#define  IBlankAll       m_fp_func[15]
#define  IBlankArea      m_fp_func[16]
#define  IEraseAll       m_fp_func[17]
#define  IEraseArea      m_fp_func[18]
#define  ISetProtection  m_fp_func[19]
#define  ICloseComm      m_fp_func[20]

// Exported functions coming from DBCA
// for casting function pointers
typedef BOOL  (*SetWorkingDirProc)(const char* );
typedef BOOL  (*GetSectionProc)   (const char *, const char *, char *);
typedef BOOL  (*GetKeyValueProc)  (const char *, const char *, const char * , char *);
typedef char *(*GetLastErrorProc) (void);
typedef BOOL  (*GetListProc)      (const char *, const char *, char *);

// Position of functions in the array of pointers on API function. 
// Order should be the same that in the array providing the function name 
#define	 IGetList		m_fp_func[21]
#define	 IGetSection	m_fp_func[22]
#define	 IGetKeyValue	m_fp_func[23]
#define	 IGetLastError	m_fp_func[24]
#define	 ISetWorkingDir	m_fp_func[25]

// Exported functions coming from LEF
// for casting function pointers
typedef BOOL  (*LSetMessageCallBackProc) (ErrorCB);
typedef BOOL  (*LSetProgressCallBackProc)(ProgressCB);
typedef BOOL  (*LDisplayMessageProc) (const char *);
typedef BOOL  (*LDisplayProgressProc) (int);
typedef BOOL  (*LOpenLogProc) (const char *);
typedef BOOL  (*LSetLogOptionsProc) (unsigned short, int);

// Position of functions in the array of pointers on API function. 
// Order should be the same that in the array providing the function name 
#define	 ISetErrorCallBack		m_fp_func[26]
#define	 ISetWarningCallBack	m_fp_func[27]
#define	 ISetMessageCallBack	m_fp_func[28]
#define	 ISetProgressCallBack	m_fp_func[29]
#define	 IDisplayError			m_fp_func[30]
#define	 IDisplayWarning		m_fp_func[31]
#define	 IDisplayMessage		m_fp_func[32]
#define	 IDisplayProgress		m_fp_func[33]
#define	 IOpenLog				m_fp_func[34]
#define	 ITraceLog				m_fp_func[35]
#define  ICloseLog 				m_fp_func[36]
#define  ISetLogOptions			m_fp_func[37]

BOOL LoadDlls(char *szDirectoryPathForDlls="");
void Clean();

// API functions declaration
BOOL ESelectHard(const char *szHardName,const char* szProtocol, BOOL bDemo);
BOOL ESelectDevice(const char *szDeviceName);
int  ESelectPort(const char *szPortNumber);
BOOL ESetProtection(const char *szProtectMode, BOOL bState);
BOOL ESetPreferences(int iPreference, BOOL bState);
int  EGetId(const char *szAreaName, unsigned long* pdwId);
int  ELoadFile(const char *FileName, unsigned long dwMemType);
int  ESaveFile(const char *FileName, unsigned long dwMemType);
char *EGetImagePtr(DWORD dwMemType, long *MemSize);
int  ECheckSum(unsigned long dwMemType, long *nFileCheckSum, long *nMemoryCheckSum);
int  EProgAll(unsigned long dwMemType);
int  EProgArea(unsigned long dwMemType, long FirstAddr, long LastAddr);
int  EAllRead(unsigned long dwMemType);
int  EReadArea(unsigned long dwMemType, long FirstAddr, long LastAddr);
int  EVerifyAll(unsigned long dwMemType);
int  EVerifyArea(unsigned long dwMemType, long FirstAddr, long LastAddr);
int  EBlankAll(unsigned long dwMemType);
int  EBlankArea(unsigned long dwMemType, long FirstAddr, long LastAddr);
int  EEraseAll(unsigned long dwMemType);
int  EEraseArea(unsigned long dwMemType, long FirstAddr, long LastAddr);
int  ECloseComm();

int  CSetWorkingDir(const char *WorkingDir);
int  CGetList (const char *FileName, const char *PathSection, char *Buffer);
int  CGetSection(const char *FileName, const char *PathSection, char *Buffer);
int  CGetKeyValue(const char *FileName, const char *PathSection, 
				 const char * KeyName, char *Buffer);
char *CGetLastError(void);

int  LSetErrorCallBack ( ErrorCB fp_funct);
int  LSetWarningCallBack ( ErrorCB fp_funct);
int  LSetMessageCallBack ( ErrorCB fp_funct);
int  LSetProgressCallBack(ProgressCB fp_funct);
int  LDisplayError ( const char *szMessage);
int  LDisplayWarning ( const char *szMessage);
int  LDisplayMessage ( const char *szMessage);
int  LDisplayProgress (int nAverage);
int  LOpenLog (const char *szFileName);
int  LTraceLog ( const char *szMessage);
int  LCloseLog ();
int  LSetLogOptions (unsigned short uOptions, OPERATOR ope);
