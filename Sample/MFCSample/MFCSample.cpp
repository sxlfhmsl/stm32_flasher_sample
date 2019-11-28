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

// MFCSample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MFCSample.h"
#include "MFCSampleDlg.h"
#include "IOMessage.h"
#include "PtkAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleApp

BEGIN_MESSAGE_MAP(CMFCSampleApp, CWinApp)
	//{{AFX_MSG_MAP(CMFCSampleApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleApp construction

CMFCSampleApp::CMFCSampleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMFCSampleApp object

CMFCSampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleApp initialization

BOOL CMFCSampleApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	AfxEnableControlContainer( );
	AfxInitRichEdit( );
    ///////////////////////////////////////////////////////
	char szBuffer[1000];
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule,szBuffer,sizeof(szBuffer));

	// Extract the path from the full module file name
	char drive [_MAX_DRIVE], dir [_MAX_DIR];
	char fname [_MAX_FNAME], ext [_MAX_EXT];

	// Get components
	_splitpath (szBuffer, drive, dir, fname, ext);

	// Compute the APISample path
	sprintf(szBuffer,"%s%s",drive,dir);

	LoadDlls(szBuffer);

	int nReturn = LSetErrorCallBack (PrintError);
	nReturn = LSetWarningCallBack (PrintWarning);
	nReturn = LSetMessageCallBack (PrintMessage);
	nReturn = LSetProgressCallBack(DisplayProgress);

	nReturn = CSetWorkingDir (szBuffer);

    ////////////////////////////////////////////////////////
	CMFCSampleDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	Clean();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
