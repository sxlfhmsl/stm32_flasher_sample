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

//////////////////////////////////////////////////////////////////////////
//	Source File : main.cpp
//
//  This sample intends to illustrate the use of the STVP programming
//  DDLs API and the use of a programming toolkit.
//  It is configured to connect with a STM8L15xC6 using the SWIM protocol
//  on the ST-LINK board connected on a USB port. If you want
//  to change any of these programming parameters, you should change the
//  appropriated global variables in the Programming parameters section below.
//
//	This program will:
//
//    . first make some initializations (DLLs loading, Callbacks settings, ...)
//    . configure programming parameters
//    . load binary files which are supposed to be binary code to be programmed
//      in both PROGRAM MEMORY and OPTION BYTE areas
//    . Then the sample will loop doing programming and verifying
//      both memory areas until <ESC> Key is struck
//
//	Of course the binary files (progmem.hex and option.hex) which are used as
//  application codes are dummy files that should be replaced by the real
//	application files (note that for option byte the file may be generated with
//	STVP)
//
//
//  This sample illustrates the following PTK API usage:
//		ESelectHard     : To select the programming board
//      ESetPreferences : To disable the warning Message Box if Option Byte protection
//		ESelectDevice   : To select the device to be programmed
//		ESelectPort     : To define the communication port
//		EGetId          : To get the internal identifier associated with a memory area
//		ELoadFile       : To load a binary code in a given memory area
//      EProgAll	    : To program the content of a memory area
//      EReadAll	    : To read the content of a memory area
//		EVerifyAll	    : To check that what is programmed in the device is what
//						  is loaded in the memory area
//		EBlankAll	    : To Check whether the device is blank
//		EEraseAll	    : To Erase flash memory areas (only)
//      ECloseComm      : To Close communication with the tool
//
//      CSetWorkingDir  : To define the directory where the cnf file must be searched
//
//		LOpenLog	    : Open a log file
//		LSetLogOptions  : To filter which kind of information (Warning,error,messages)
//					  	  must be logged
//		LCloseLog	    : Close the log file
//
//		LSetErrorCallBack	: To give to the API a function that will handle the
//							  error messages
//		LSetWarningCallBack : To give to the API a function that will handle the
//							  warning messages
//		LSetMessageCallBack : To give to the API a function that will handle the
//							  messages
//		LDisplayError		: To call the error display handler through the API DLL
//							  It will also write the error in the log file
//		LDisplayMessage		: To call the error display handler through the API DLL
//							  It will also write the error in the log file
//		LDisplayWarning		: To call the error display handler through the API DLL
//							  It will also write the error in the log file
//
//	You should rather used the LDisplayMessage, LDisplayError, LDisplayWarning
//	API functions instead of using directly the callbacks functions given to the
//	API. Because doing that, you will take benefit of the API file logging system
//
//	This sample also demonstrates some PtkToolBox functions usage:
//		IsBlankCheckAvailable : To know whether blank check is available for
//							    a device memory area.
//		IsErasableArea		  : To know whether it is possible to erase a device
//								memory area
//

#include "windows.h"
#include "conio.h"
#include "stdio.h"

#include "PtkApi.h"
#include "PtkToolbox.h"

#include "callbacks.h"

// To display version of this software
char g_szVersion[4] = "1.4";

//////////////////////////////////////////////////////////////////////////////
// Programming parameters : Should be updated accordingly to the configuration
//////////////////////////////////////////////////////////////////////////////

char g_szPrgBoard[50]    = "ST-LINK";
char g_szPrgProtocol[50] = "SWIM";
char g_szPrgDevice[50]   = "STM8L15xC6";
char g_szPort[50]	     = "USB";

BOOL g_b_warn_prot = TRUE;


////////////////////////////////////////////////////////////////////////////
// Must be called before any programming actions to choose the programming
// board, the protocol, the device and the communication port.
// Names that should be passed are those used in STVP configuration dialog
// An other way to get at run time the list of allowed names is to use the
// appropriated functions in the PtkToolBox.cpp file
//
BOOL SetProgrammingConfiguration(const char *szPrgBoard,const char *szPrgProtocol,
								 const char *szPrgDevice, const char *szPort)
{
	int nReturn = 1;
	// Select the hardware and the programming protocol
	// 3rd parameter should be FALSE otherwise the demonstration mode will be activated
	nReturn = ESelectHard(szPrgBoard,szPrgProtocol,FALSE);

	// Set Preferences: By default, the Protection warning is TRUE (message box)
	if (!g_b_warn_prot)
	{
		nReturn &= ESetPreferences(PROTECTION_WARNING,FALSE);
	}

	// Select the device
	nReturn &= ESelectDevice(szPrgDevice);

	// Select the communication port
	nReturn &= ESelectPort(szPort);

	return (nReturn==1);
}

////////////////////////////////////////////////////////////////////////////
//  Load an Intel (Hex) or a Motorola (S19) file for the specified area.
//  in the PC memory image.
//  The recognized areas in this predefined programming configuration
//  should be "PROGRAM MEMORY" or "DATA MEMORY" or "OPTION BYTE".
//  Note you can use the save file menu in STVP to generate the binary files
//  specially the file for Option byte.
//
BOOL LoadFile(const char *szFileName, const char *szAreaName)
{
	int iReturn;
	unsigned long iAreaId;
	if ((iReturn = EGetId(szAreaName, &iAreaId)) == 1)
	{
		iReturn = ELoadFile(szFileName, iAreaId);
	}
	else
	{
		LDisplayError("Unknown memory Area name");
	}
	return (iReturn==1);
}

///////////////////////////////////////////////////////////////////////
// Read the specified memory area from the device and store the
// data in the PC memory image.
//
BOOL Read(const char *szAreaName) 
{
	int iReturn;
	unsigned long iAreaId;
	if ((iReturn = EGetId(szAreaName, &iAreaId)) == 1)
	{
		iReturn = EAllRead(iAreaId);
	}
	else
	{
		LDisplayError("Unknown memory Area name");
	}
	return (iReturn==1);
}

///////////////////////////////////////////////////////////////////////
// Program the specified memory area with the binary file that was
// previously loaded in the PC memory image
// Note that if no file was previously loaded before programming
// the programming result is unpredictable
//
BOOL Program(const char *szAreaName) 
{
	int iReturn;
	unsigned long iAreaId;
	if ((iReturn = EGetId(szAreaName, &iAreaId)) == 1)
	{
		iReturn = EProgAll(iAreaId);
	}
	else
	{
		LDisplayError("Unknown memory Area name");
	}
	return (iReturn==1);
}

//////////////////////////////////////////////////////////////////////////
// Check that for the specified memory area the data read from the device
// matches with the data stored in the PC memory image.
//
BOOL Verify(const char *szAreaName) 
{
	int iReturn;
	unsigned long iAreaId;
	if ((iReturn = EGetId(szAreaName, &iAreaId)) == 1)
	{
		iReturn = EVerifyAll(iAreaId);
	}
	else
	{
		LDisplayError("Unknown memory Area name");
	}
	return (iReturn==1);
}


/////////////////////////////////////////////////////////////////
// Check whether the device memory area is blank.
// be careful depending on the device and on the memory area to
// process blank checking may be unavailable
//
BOOL BlankCheck(const char *szAreaName)
{
	int iReturn=0;
	unsigned long iAreaId;
	if (IsBlankCheckAvailable(g_szPrgDevice, "PROGRAM MEMORY"))
	{
		if ((iReturn = EGetId(szAreaName, &iAreaId)) == 1)
		{
			iReturn = EBlankAll(iAreaId);
		}
		else
		{
			LDisplayError("Unknown memory Area name");
		}
	}
	else
	{
		char szErrorMsg[200];
		sprintf(szErrorMsg,"Blank Check is not available on %s for %s",
							szAreaName,g_szPrgDevice);
		LDisplayError(szErrorMsg);
	}
	return (iReturn==1);
}

//////////////////////////////////////////////////////////////////
// Erase a memory area.
// be careful depending on the device and on the memory area to
// process Erase may be unavailable
//
BOOL Erase(const char *szAreaName)
{
	int iReturn=0;
	unsigned long iAreaId,FirstAreaAddr,LastAreaAddr;
	if (IsErasableArea(g_szPrgDevice, "PROGRAM MEMORY"))
	{
		if ((iReturn = EGetId(szAreaName, &iAreaId)) == 1)
		{
			if (_strnicmp (g_szPrgDevice,"ST7",3) == 0)
			{
				iReturn = EEraseAll(iAreaId);
			}
			else
			{
				if ((iReturn = GetAreaMapping(g_szPrgDevice,szAreaName,&FirstAreaAddr,&LastAreaAddr)) == 1)
				{
					iReturn = EEraseArea(iAreaId, FirstAreaAddr, LastAreaAddr);
				}
				else
				{
					LDisplayError("Error getting memory mapping");
				}
			}
		}
		else
		{
			LDisplayError("Unknown memory Area name");
		}
	}
	else
	{
		char szErrorMsg[200];
		sprintf(szErrorMsg,"Erase is not available on %s for %s",
							szAreaName,g_szPrgDevice);
		LDisplayError(szErrorMsg);
	}
	return (iReturn==1);
}

////////////////////////////////////////////////////////////////////////
// Close Communication, Close log file if opened & Unload the API DLLs
//
void CloseComm_CloseLog_Clean(BOOL b_logging)
{
	ECloseComm(); // Close communication

	if (b_logging)
	{
		CloseLog(); // Close log file
	}
	Clean(); // Unload the API DLLs
}

//////////////////
// Main program
//
 int main (int argc, char *argv[])
{
BOOL b_Arg_Ok =       FALSE;
BOOL b_NoArg =        TRUE;
BOOL b_Msg =          TRUE;
BOOL b_Progress =     FALSE;
BOOL b_Erase =        FALSE;
BOOL b_BlankCheck =   FALSE;
BOOL b_Verif =        TRUE;
BOOL b_Log =          TRUE;
BOOL b_Loop =         TRUE;
BOOL b_MemProg =      FALSE;
BOOL b_MemRead =      FALSE;
BOOL b_DataProg =     FALSE;
BOOL b_DataRead =     FALSE;
BOOL b_OptionProg =   FALSE;
BOOL b_OptionRead =   FALSE;
BOOL b_NoProgMem =    FALSE;
BOOL b_NoProgData =   FALSE;
BOOL b_NoProgOption = FALSE;
BOOL bFileLoaded =    FALSE;
char ProgMemFileName[_MAX_PATH]   = "ProgMem.hex";
char DataMemFileName[_MAX_PATH]   = "DataMem.hex";
char OptionMemFileName[_MAX_PATH] = "Option.hex";
char szBuffer[1000], szMsg[300], ch;
char drive [_MAX_DRIVE], dir [_MAX_DIR];
char fname [_MAX_FNAME], ext [_MAX_EXT];
HMODULE hModule;
int nReturn,i;

	i = argc - 1;
	while (i)
	{
		b_NoArg = FALSE;
		b_Arg_Ok = FALSE;
		if (_stricmp (argv[i],"-help") == 0)
		{
			printf("\nSTVP_CmdLine version %s\n", g_szVersion);
			printf("Usage:STVP_CmdLine [-BoardName=STxxx] [-Port=xxx] [-ProgMode=xxx] [-Device=STxxx]\n");
			printf("[-help] [-version] [-verbose]|[-no_verbose] [-log]|[-no_log] [-loop]|[-no_loop]\n");
			printf("[-no_progProg] [-no_progData] [-no_progOption] [-progress]|[-no_progress]\n");
			printf("[-warn_protect]|[-no_warn_protect] [-readProg] [-readData] [-readOption]\n");
			printf("[-erase]|[-no_erase] [-blank]|[-no_blank] [-verif]|[-no_verif]\n");
			printf("[-FileProg=fname.hex/s19] [-FileData=fname.hex/s19] [-FileOption=fname.hex/s19]\n");
			printf("With:\n");
			printf("[-BoardName=STxxx] ==========> Programming Tool name\n");
			printf("[-Port=xxx] =================> Communication Port (USB, LPT1)\n");
			printf("[-ProgMode=xxx] =============> Programming mode (protocol)\n");
			printf("[-Device=STxxx] =============> Device name\n");
			printf("[-version] ==================> Display version of this application\n");
			printf("[-verbose] ==================> Display messages, warnings, errors\n");
			printf("[-log] ======================> Generate or append Result.log log file\n");
			printf("[-loop] =====================> Loop on actions until ESC key hit\n");
			printf("[-progress] =================> Display progress of each action\n");
			printf("[-warn_protect] =============> Message Box if programming Option Byte protection\n");
			printf("[-no_progProg] ==============> Do not program PROGRAM MEMORY (used to verify device from a file)\n");
			printf("[-no_progData] ==============> Do not program DATA MEMORY (used to verify device from a file)\n");
			printf("[-no_progOption] ============> Do not program OPTION BYTE (used to verify device from a file)\n");
			printf("[-readProg] =================> Read PROGRAM MEMORY\n");
			printf("[-readData] =================> Read DATA MEMORY\n");
			printf("[-readOption] ===============> Read OPTION BYTE\n");
			printf("[-erase] ====================> Erase the device (before programming)\n");
			printf("[-blank] ====================> Blank Check the device (before programming)\n");
			printf("[-verif] ====================> Verify the device after programming\n");
			printf("[-FileProg=fname.hex/s19] ===> File name to program PROGRAM MEMORY area (hex or s19)\n");
			printf("[-FileData=fname.hex/s19] ===> File name to program DATA MEMORY area (hex or s19)\n");
			printf("[-FileOption=fname.hex/s19] => File name to program OPTION BYTE area (hex or s19)\n");
			return 0;
		}
		if ((!b_Arg_Ok) && (_strnicmp (argv[i],"-BoardName=",11) == 0))
		{
			strncpy (g_szPrgBoard, (argv[i]+11), (strlen(argv[i])-11+1)); // +1 to copy '\0'
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_strnicmp (argv[i],"-Port=",6) == 0))
		{
			strncpy (g_szPort, (argv[i]+6), (strlen(argv[i])-6+1)); // +1 to copy '\0'
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_strnicmp (argv[i],"-ProgMode=",10) == 0))
		{
			strncpy (g_szPrgProtocol, (argv[i]+10), (strlen(argv[i])-10+1)); // +1 to copy '\0'
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_strnicmp (argv[i],"-Device=",8) == 0))
		{
			strncpy (g_szPrgDevice, (argv[i]+8), (strlen(argv[i])-8+1)); // +1 to copy '\0'
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-version") == 0))
		{
			printf("\nSTVP_CmdLine version %s\n", g_szVersion);
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-verbose") == 0))
		{
			b_Msg = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_verbose") == 0))
		{
			b_Msg = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-progress") == 0))
		{
			b_Progress = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_progress") == 0))
		{
			b_Progress = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-erase") == 0))
		{
			b_Erase = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_erase") == 0))
		{
			b_Erase = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-blank") == 0))
		{
			b_BlankCheck = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_blank") == 0))
		{
			b_BlankCheck = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_progProg") == 0))
		{
			b_NoProgMem = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_progData") == 0))
		{
			b_NoProgData = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_progOption") == 0))
		{
			b_NoProgOption = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-readProg") == 0))
		{
			b_MemRead = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-readData") == 0))
		{
			b_DataRead = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-readOption") == 0))
		{
			b_OptionRead = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-verif") == 0))
		{
			b_Verif = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_verif") == 0))
		{
			b_Verif = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-log") == 0))
		{
			b_Log = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_log") == 0))
		{
			b_Log = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-loop") == 0))
		{
			b_Loop = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_loop") == 0))
		{
			b_Loop = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-warn_protect") == 0))
		{
			g_b_warn_prot = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_stricmp (argv[i],"-no_warn_protect") == 0))
		{
			g_b_warn_prot = FALSE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_strnicmp (argv[i],"-FileProg=",10) == 0))
		{
			strncpy (ProgMemFileName, (argv[i]+10), (strlen(argv[i])-10+1)); // +1 to copy '\0'
			b_MemProg = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_strnicmp (argv[i],"-FileData=",10) == 0))
		{
			strncpy (DataMemFileName, (argv[i]+10), (strlen(argv[i])-10+1)); // +1 to copy '\0'
			b_DataProg = TRUE;
			b_Arg_Ok = TRUE;
		}
		if ((!b_Arg_Ok) && (_strnicmp (argv[i],"-FileOption=",12) == 0))
		{
			strncpy (OptionMemFileName, (argv[i]+12), (strlen(argv[i])-12+1)); // +1 to copy '\0'
			b_OptionProg = TRUE;
			b_Arg_Ok = TRUE;
		}
		if (!b_Arg_Ok)
		{
			printf("\nSTVP_CmdLine version %s\n", g_szVersion);
			printf("Argument ERROR or unknown argument: %s\n\n",argv[i]);
			printf("Usage:STVP_CmdLine [-BoardName=STxxx] [-Port=xxx] [-ProgMode=xxx] [-Device=STxxx]\n");
			printf("[-help] [-version] [-verbose]|[-no_verbose] [-log]|[-no_log] [-loop]|[-no_loop]\n");
			printf("[-no_progProg] [-no_progData] [-no_progOption] [-progress]|[-no_progress]\n");
			printf("[-warn_protect]|[-no_warn_protect] [-readProg] [-readData] [-readOption]\n");
			printf("[-erase]|[-no_erase] [-blank]|[-no_blank] [-verif]|[-no_verif]\n");
			printf("[-FileProg=fname.hex/s19] [-FileData=fname.hex/s19] [-FileOption=fname.hex/s19]\n");
			printf("With:\n");
			printf("[-BoardName=STxxx] ==========> Programming Tool name\n");
			printf("[-Port=xxx] =================> Communication Port (USB, LPT1)\n");
			printf("[-ProgMode=xxx] =============> Programming mode (protocol)\n");
			printf("[-Device=STxxx] =============> Device name\n");
			printf("[-version] ==================> Display version of this application\n");
			printf("[-verbose] ==================> Display messages, warnings, errors\n");
			printf("[-log] ======================> Generate or append Result.log log file\n");
			printf("[-loop] =====================> Loop on actions until ESC key hit\n");
			printf("[-progress] =================> Display progress of each action\n");
			printf("[-warn_protect] =============> Message Box if programming Option Byte protection\n");
			printf("[-no_progProg] ==============> Do not program PROGRAM MEMORY (used to verify device from a file)\n");
			printf("[-no_progData] ==============> Do not program DATA MEMORY (used to verify device from a file)\n");
			printf("[-no_progOption] ============> Do not program OPTION BYTE (used to verify device from a file)\n");
			printf("[-readProg] =================> Read PROGRAM MEMORY\n");
			printf("[-readData] =================> Read DATA MEMORY\n");
			printf("[-readOption] ===============> Read OPTION BYTE\n");
			printf("[-erase] ====================> Erase the device (before programming)\n");
			printf("[-blank] ====================> Blank Check the device (before programming)\n");
			printf("[-verif] ====================> Verify the device after programming\n");
			printf("[-FileProg=fname.hex/s19] ===> File name to program PROGRAM MEMORY area (hex or s19)\n");
			printf("[-FileData=fname.hex/s19] ===> File name to program DATA MEMORY area (hex or s19)\n");
			printf("[-FileOption=fname.hex/s19] => File name to program OPTION BYTE area (hex or s19)\n");
			return 1;
		}
		i--;
	}

	// Extract the path from the full module file name
	hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule,szBuffer,sizeof(szBuffer));

	// Get components
	_splitpath (szBuffer, drive, dir, fname, ext);

	// Compute the STVP_CmdLine path
	sprintf(szBuffer,"%s%s",drive,dir);

	// Before calling any API function, the API DLLs must be loaded: Assume that
	// the DLLs are located in the same directory as STVP_CmdLine.exe file
	LoadDlls(szBuffer);

	// It is possible to define the path location for the configuration files 
	// (dbcfile.cnf and tool.cnf), by default the Working directory will be 
	// the directory in which the application is running
	nReturn = CSetWorkingDir (szBuffer);

	// Messages, warnings or Errors may be issued at any level in the bunch
	// of API DLLs to be informed some callbacks must be provided to the DLLs
	if (b_Msg)
	{
		nReturn = LSetErrorCallBack (PrintError);
		nReturn = LSetWarningCallBack (PrintWarning);
		nReturn = LSetMessageCallBack (PrintMessage);
	}

	// A call back may also be defined to be informed of the action progress
	if (b_Progress)
	{
		nReturn = LSetProgressCallBack(DisplayProgress);
	}

	// It is possible to log all messages, warnings or errors issued by the DLLs
	if (b_Log)
	{
		OpenLog("Result.log");
		LSetLogOptions(LOG_ALL,OR);
	}
	// Note that it is possible to make a selective logging : 
	// LSetLogOptions(LOG_WARN,OR) will log only the warnings
	// LSetLogOptions(LOG_WARN|LOG_MSG,OR) will log warnings and messages
	// LSetLogOptions(~LOG_ALL,AND) nothing will be logged 

	// Print and Log version of this software
	sprintf(szMsg,"STVP_CmdLine version %s\n", g_szVersion);
	LDisplayMessage(szMsg);
	if (!b_Msg)
		printf(szMsg);

	if (b_NoArg)
	{
		LDisplayMessage("NO ARGUMENTS GIVEN: will use default config. Type STVP_CmdLine -help for usage\n");
		sprintf(szMsg,"Configuration:\nBoardName=%s Device=%s Port=%s ProgMode=%s", 
		        g_szPrgBoard, g_szPrgDevice, g_szPort, g_szPrgProtocol);
		LDisplayMessage(szMsg);
		sprintf(szMsg,"ProgMemFileName=%s", ProgMemFileName);
		LDisplayMessage(szMsg);
		sprintf(szMsg,"OptionFileName=%s\n", OptionMemFileName);
		LDisplayMessage(szMsg);
	}

	// Print and log options
	if (b_Msg)
		LDisplayMessage("Verbose ON");
	else
	{
		LDisplayMessage("Verbose OFF"); // To log
		printf("Verbose OFF\n");
	}

	if (b_Log)
		LDisplayMessage("Log activity ON");
	else
		LDisplayMessage("Log activity OFF");

	if (b_Progress)
		LDisplayMessage("Display Progress ON");
	else
		LDisplayMessage("Display Progress OFF");

	if (g_b_warn_prot)
		LDisplayMessage("Message box warning if protect option byte ON");
	else
		LDisplayMessage("Message box warning if protect option byte OFF");

	if (b_Loop)
		LDisplayMessage("Loop mode ON");
	else
		LDisplayMessage("Loop mode OFF");

	if (b_Erase)
		LDisplayMessage("Erase device ON");
	else
		LDisplayMessage("Erase device OFF");

	if (b_BlankCheck)
		LDisplayMessage("Blank check device ON");
	else
		LDisplayMessage("Blank check device OFF");

	if (b_Verif)
		LDisplayMessage("Verify device ON\n");
	else
		LDisplayMessage("Verify device OFF\n");

	// Set the programming configuration in this case :
	// Here a STM8L15xC6 device will be programmed using a ST-LINK dongle with 
	// SWIM protocol connected on a USB port
	if (SetProgrammingConfiguration(g_szPrgBoard,g_szPrgProtocol,g_szPrgDevice,g_szPort) == TRUE)
	{
		if (!b_NoArg) // Configuration already printed and loggged
		{
			sprintf(szMsg,"Configuration:\nBoardName=%s Device=%s Port=%s ProgMode=%s", 
					g_szPrgBoard, g_szPrgDevice, g_szPort, g_szPrgProtocol);
			LDisplayMessage(szMsg);
		}

		// Try to load application files...
		bFileLoaded = FALSE;
		if (b_MemProg || b_NoArg)
		{
			LDisplayMessage("\n>>> Filling PROGRAM MEMORY image in computer with Blank Value");
			if (FillImageMemory(g_szPrgDevice,"PROGRAM MEMORY") != 1)
			{
				// Close Communication, Close log file if opened & Unload the API DLLs
				CloseComm_CloseLog_Clean(b_Log);
				return 1; // Error
			}
			LDisplayMessage("<<< Filling PROGRAM MEMORY image in computer succeeds");

			sprintf(szMsg,"\n>>> Loading file %s in PROGRAM MEMORY image in computer",ProgMemFileName);
			LDisplayMessage(szMsg);
			bFileLoaded = LoadFile(ProgMemFileName,"PROGRAM MEMORY");
			if (!bFileLoaded)
			{
				// Close Communication, Close log file if opened & Unload the API DLLs
				CloseComm_CloseLog_Clean(b_Log);
				return 1; // Error
			}
			LDisplayMessage("<<< Loading file succeeds");
		}

		if (b_DataProg) // Only if a file was given for Data memory
		{
			LDisplayMessage("\n>>> Filling DATA MEMORY image in computer with Blank Value");
			if (FillImageMemory(g_szPrgDevice,"DATA MEMORY") != 1)
			{
				// Close Communication, Close log file if opened & Unload the API DLLs
				CloseComm_CloseLog_Clean(b_Log);
				return 1; // Error
			}
			LDisplayMessage("<<< Filling DATA MEMORY image in computer succeeds");

			sprintf(szMsg,"\n>>> Loading file %s in DATA MEMORY image in computer",DataMemFileName);
			LDisplayMessage(szMsg);
			bFileLoaded = LoadFile(DataMemFileName,"DATA MEMORY");
			if (!bFileLoaded)
			{
				// Close Communication, Close log file if opened & Unload the API DLLs
				CloseComm_CloseLog_Clean(b_Log);
				return 1; // Error
			}
			LDisplayMessage("<<< Loading file succeeds");
		}

		if (b_OptionProg || b_NoArg)
		{
			sprintf(szMsg,"\n>>> Loading file %s in OPTION BYTE image in computer",OptionMemFileName);
			LDisplayMessage(szMsg);
			bFileLoaded = LoadFile(OptionMemFileName,"OPTION BYTE");
			if (!bFileLoaded)
			{
				// Close Communication, Close log file if opened & Unload the API DLLs
				CloseComm_CloseLog_Clean(b_Log);
				return 1; // Error
			}
			LDisplayMessage("<<< Loading file succeeds");
		}

		if (b_NoProgMem)
			b_MemProg = FALSE;

		if (b_NoProgData)
			b_DataProg = FALSE;

		if (b_NoProgOption)
			b_OptionProg = FALSE;

		// At least 1 file loaded or Erase or Blank Check or Read
		if (bFileLoaded || b_Erase || b_BlankCheck || b_MemRead || b_DataRead || b_OptionRead)
		{
			ch = 0;
			while (ch != 0x1b) // Loop until <Esc> key is struck or b_Loop=False
			{
				if (b_Erase)
				{
					LDisplayMessage("\n>>> Erasing PROGRAM MEMORY");
					if (Erase("PROGRAM MEMORY") != TRUE)
					{
						LDisplayMessage("\n<<< Erasing PROGRAM MEMORY fails");
						// Close Communication, Close log file if opened & Unload the API DLLs
						CloseComm_CloseLog_Clean(b_Log);
						return 1; // Erase Error
					}
					else
					{
						LDisplayMessage("\n<<< Erasing PROGRAM MEMORY succeeds");
					}
				}

				if (b_BlankCheck)
				{
					LDisplayMessage("\n>>> Blank checking PROGRAM MEMORY");
					if (BlankCheck("PROGRAM MEMORY") != TRUE)
					{
						LDisplayMessage("\n<<< Blank checking PROGRAM MEMORY fails");
						// Close Communication, Close log file if opened & Unload the API DLLs
						CloseComm_CloseLog_Clean(b_Log);
						return 1; // Blank check Error
					}
					else
					{
						LDisplayMessage("\n<<< Blank checking PROGRAM MEMORY succeeds");
					}
				}

				if (b_MemProg || b_NoArg || b_MemRead || b_NoProgMem) // If a file was given for Program memory or No arguments Or Read Or No Prog
				{
					if (b_MemProg || b_NoArg)
					{
						LDisplayMessage("\n>>> Programming PROGRAM MEMORY");
						if (Program("PROGRAM MEMORY") != TRUE)
						{
							LDisplayMessage("<<< Programming PROGRAM MEMORY fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Prog Error
						}
						else
						{
							LDisplayMessage("<<< Programming PROGRAM MEMORY succeeds");
						}
					}
					else if (b_MemRead)
					{
						LDisplayMessage("\n>>> Reading PROGRAM MEMORY");
						if (Read("PROGRAM MEMORY") != TRUE)
						{
							LDisplayMessage("<<< Reading PROGRAM MEMORY fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Read Error
						}
						else
						{
							LDisplayMessage("<<< Reading PROGRAM MEMORY succeeds");
						}
					}

					if (b_Verif)
					{
						LDisplayMessage("\n>>> Verifying PROGRAM MEMORY");
						if (Verify("PROGRAM MEMORY") != TRUE)
						{
							LDisplayMessage("<<< Verifying PROGRAM MEMORY fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Verif Error
						}
						else
						{
							LDisplayMessage("<<< Verifying PROGRAM MEMORY succeeds");
						}
					}
				}

				if (b_DataProg || b_DataRead || b_NoProgData) // Only if a file was given for Data memory Or Read Or No Prog
				{
					if (b_DataProg)
					{
						LDisplayMessage("\n>>> Programming DATA MEMORY");
						if (Program("DATA MEMORY") != TRUE)
						{
							LDisplayMessage("<<< Programming DATA MEMORY fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Prog Error
						}
						else
						{
							LDisplayMessage("<<< Programming DATA MEMORY succeeds");
						}
					}
					else if (b_DataRead)
					{
						LDisplayMessage("\n>>> Reading DATA MEMORY");
						if (Read("DATA MEMORY") != TRUE)
						{
							LDisplayMessage("<<< Reading DATA MEMORY fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Read Error
						}
						else
						{
							LDisplayMessage("<<< Reading DATA MEMORY succeeds");
						}
					}

					if (b_Verif)
					{
						LDisplayMessage("\n>>> Verifying DATA MEMORY");
						if (Verify("DATA MEMORY") != TRUE)
						{
							LDisplayMessage("<<< Verifying DATA MEMORY fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Verif Error
						}
						else
						{
							LDisplayMessage("<<< Verifying DATA MEMORY succeeds");
						}
					}
				}

				if (b_OptionProg || b_NoArg || b_OptionRead || b_NoProgOption) // If a file was given for Option bytes or No arguments or Read or No Prog
				{
					if (b_OptionProg || b_NoArg)
					{
						LDisplayMessage("\n>>> Programming OPTION BYTE");
						if (Program("OPTION BYTE") != TRUE)
						{
							LDisplayMessage("<<< Programming OPTION BYTE fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Prog Error
						}
						else
						{
							LDisplayMessage("<<< Programming OPTION BYTE succeeds");
						}
					}
					else if (b_OptionRead)
					{
						LDisplayMessage("\n>>> Reading OPTION BYTE");
						if (Read("OPTION BYTE") != TRUE)
						{
							LDisplayMessage("<<< Reading OPTION BYTE fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Read Error
						}
						else
						{
							LDisplayMessage("<<< Reading DATA MEMORY succeeds");
						}
					}

					if (b_Verif)
					{
						LDisplayMessage("\n>>> Verifying OPTION BYTE");
						if (Verify("OPTION BYTE") != TRUE)
						{
							LDisplayMessage("<<< Verify OPTION BYTE fails");
							// Close Communication, Close log file if opened & Unload the API DLLs
							CloseComm_CloseLog_Clean(b_Log);
							return 1; // Verif Error
						}
						else
						{
							LDisplayMessage("<<< Verify OPTION BYTE succeeds");
						}
					}
				}

				ECloseComm(); // Close communication (needed if tool is changed between loops)

				if (b_Loop)
				{
					printf ("\nHit ESC key to stop or any other key to loop\n");
					ch = _getch();
				}
				else
				{
					ch = 0x1b; // To stop the loop
				}
			}
		}
		else
		{
			// Close Communication, Close log file if opened & Unload the API DLLs
			CloseComm_CloseLog_Clean(b_Log);
			return 1; // File Error
		}
	}
	else
	{
		// Close Communication, Close log file if opened & Unload the API DLLs
		CloseComm_CloseLog_Clean(b_Log);
		return 1; // Config Error
	}

	// Close Communication, Close log file if opened & Unload the API DLLs
	CloseComm_CloseLog_Clean(b_Log);
	return 0; // No errors: OK
}