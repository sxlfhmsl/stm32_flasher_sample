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
 *	STMicroelectronics
 *
 * 	Source File : callbacks.cpp
 *
 *	This files contains the Call Back functions that may be passed to the
 *  DLLs API. Initializing Call backs allows the application to display the
 *  messages, the warnings or the errors that are coming from the DLLs.
 *  It also illustrates the way to use the API logging file mechanism.
 *
 *  This file cannot be used as it :
 *		. It is the responsibility of the application to provide a PrintMessage
 *		  function.in charge of displaying messages.
 *		. CALLBACK is a Microsoft specific alias standing for __stdcall and 
 *        use in place of FAR PASCAL in application callback routines such as 
 *        window procedures and dialog procedures. If you are not using Visual C++
 *		  6.0 you will probably have to modify it.
 *  
 *	Date : Oct 24 2001
 *
 *  Compiler : Microsoft Visual C++ 6.00
 *
  ________________________________________________________________________________*/


#include "windows.h"
#include "stdio.h"

#include "PtkAPI.h"

static BOOL m_bLogFileOpened;

void OpenLog(char *szFileName)
{
	if (LOpenLog(szFileName)) {
		m_bLogFileOpened = TRUE;
	}
	else {
		m_bLogFileOpened = FALSE;
	}
}

void CloseLog()
{
	LCloseLog();
}

long CALLBACK PrintMessage(const char *szMessage)
{
	printf("%s",szMessage);
	if (szMessage[strlen(szMessage)] != '\n') {
		// Append '\n'
		printf("\n");
	}
	return TRUE;
}

long CALLBACK PrintError(const char *szErrorMsg)
{
	char *ErrorPrefix = "(API) ERROR : ";
	// allocate one more byte for 0 terminate character
	char *szMessage = (char *) malloc(strlen(szErrorMsg) + strlen(ErrorPrefix) + 1);
	sprintf(szMessage,"%s%s",ErrorPrefix,szErrorMsg);
	PrintMessage(szMessage);
	free (szMessage);
	return TRUE;
}

long CALLBACK PrintWarning(const char *szErrorMsg)
{
	char *ErrorPrefix = "(API) WARNING : ";
	// allocate one more byte in case that an '\n' should be appended
	char *szMessage = (char *) malloc(strlen(szErrorMsg) + strlen(ErrorPrefix) + 1);
	sprintf(szMessage,"%s%s",ErrorPrefix,szErrorMsg);
	PrintMessage(szMessage);
	free (szMessage);
	return TRUE;
}

long CALLBACK DisplayProgress(int Percentage)
{
	printf("%d ",Percentage);
	return 1;
}

