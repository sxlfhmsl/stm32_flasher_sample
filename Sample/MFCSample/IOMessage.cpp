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
 *			  APISample STMicroelectronics
 *
 *
 * 			Source File : IOMessage.cpp
 *
 *	This files contains the Call Back functions that may be passed to the
 *  DLLs API. Initializing Call backs allows the application to display the
 *  messages, the warnings ot the errors that are coming from the DLLs.
 *  It also illustrates the way to use the API logging file mechanism.
 *
 *  This file cannot be used as it :
 *		. It is the responsability of the application to provide a PrintMessage
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


#include "stdafx.h"

#include "stdio.h"
#include "MFCSample.h"
#include "MFCSampleDlg.h"
#include "PtkAPI.h"


static BOOL m_bLogFileOpened;

static const TCHAR c_szCrLf[]= _T("\015\012");

void OpenLog(char *szFileName)
{
	if (LOpenLog(szFileName)) {
		m_bLogFileOpened = TRUE;
	}
	else {
		m_bLogFileOpened = FALSE;
	}
}

void CloseLog(char *szFileName)
{
	LCloseLog();
}

long CALLBACK PrintMessage(const char *szMessage)
{
	((CMFCSampleDlg*)AfxGetApp()->m_pMainWnd)->DisplayMessage(szMessage);
	((CMFCSampleDlg*)AfxGetApp()->m_pMainWnd)->DisplayMessage(c_szCrLf);
	return 1;
}

long CALLBACK PrintError(const char *szErrorMsg)
{
	((CMFCSampleDlg*)AfxGetApp()->m_pMainWnd)->DisplayError(szErrorMsg);
	((CMFCSampleDlg*)AfxGetApp()->m_pMainWnd)->DisplayMessage(c_szCrLf);
	return 1;
}

long CALLBACK PrintWarning(const char *szWarningMsg)
{
	((CMFCSampleDlg*)AfxGetApp()->m_pMainWnd)->DisplayWarning(szWarningMsg);
	((CMFCSampleDlg*)AfxGetApp()->m_pMainWnd)->DisplayMessage(c_szCrLf);
	return 1;
}

long CALLBACK DisplayProgress(int Percentage)
{
	((CMFCSampleDlg*)AfxGetApp()->m_pMainWnd)->DisplayProgress(Percentage);
	return 1;
}
