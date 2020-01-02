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

// MFCSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "MFCSampleDlg.h"
#include "PtkToolBox.h"
#include "PtkAPI.h"
#include "IOMessage.h"
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleDlg dialog

static CHARFORMAT c_normalCharFormat =
{
    sizeof(CHARFORMAT)	// size; 
    , CFM_COLOR|CFM_UNDERLINE|CFM_BOLD // Bold bitfield are valid
    , 0			// Bold character set 
    , 0			// Character height, in twips. A twip is 1/1440 of an inch, or 1/20 of a printer's point.  (ignored)
    , 0			// Character offset, in twips, from the baseline (ignored)
    , RGB( 0, 0, 0)// Text color. This member is ignored if the CFE_AUTOCOLOR character effect is specified (ignored) 
    , 0			// Character set value. Can be one of the values specified for the lfCharSet member of the LOGFONT structure  (ignored) 
    , 0			// Font family and pitch. This member is the same as the lfPitchAndFamily member of the LOGFONT structure (ignored)  
    , NULL		// Null-terminated character array specifying the font face name
};

static CHARFORMAT c_warningCharFormat =
{
    sizeof(CHARFORMAT)	// size; 
    , CFM_COLOR // Bold bitfield are valid
	, 0 // Bold character set 
    , 0			// Character height, in twips. A twip is 1/1440 of an inch, or 1/20 of a printer's point.  (ignored)
    , 0			// Character offset, in twips, from the baseline (ignored)
    , RGB(255,128,64)// Text color. This member is ignored if the CFE_AUTOCOLOR character effect is specified (ignored) 
    , 0			// Character set value. Can be one of the values specified for the lfCharSet member of the LOGFONT structure  (ignored) 
    , 0			// Font family and pitch. This member is the same as the lfPitchAndFamily member of the LOGFONT structure (ignored)  
    , NULL		// Null-terminated character array specifying the font face name
};

static CHARFORMAT c_errorCharFormat =
{
    sizeof(CHARFORMAT)	// size; 
    , CFM_COLOR // Bold bitfield are valid
	, 0 // Bold character set 
    , 0			// Character height, in twips. A twip is 1/1440 of an inch, or 1/20 of a printer's point.  (ignored)
    , 0			// Character offset, in twips, from the baseline (ignored)
    , RGB(255,0,0)// Text color. This member is ignored if the CFE_AUTOCOLOR character effect is specified (ignored) 
    , 0			// Character set value. Can be one of the values specified for the lfCharSet member of the LOGFONT structure  (ignored) 
    , 0			// Font family and pitch. This member is the same as the lfPitchAndFamily member of the LOGFONT structure (ignored)  
    , NULL		// Null-terminated character array specifying the font face name
};

CMFCSampleDlg::CMFCSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCSampleDlg::IDD, pParent)
	, m_strFileName_app(_T(""))
{
	//{{AFX_DATA_INIT(CMFCSampleDlg)
	m_strFileName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMFCSampleDlg)
	DDX_Control(pDX, IDC_COMBO5, m_comboMemoryArea);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	DDX_Control(pDX, IDC_RICHEDIT1, m_outputEdit);
	DDX_Control(pDX, IDC_COMBO4, m_comboPort);
	DDX_Control(pDX, IDC_COMBO2, m_comboProtocol);
	DDX_Control(pDX, IDC_COMBO1, m_comboHardware);
	DDX_Control(pDX, IDC_COMBO3, m_comboDevice);
	DDX_Control(pDX, IDC_CHECK1, m_btFirst);
	DDX_Control(pDX, IDC_CHECK2, m_btSecond);

	DDX_Control(pDX, IDC_EDIT_MASK, m_edMask);
	DDX_Control(pDX, IDC_EDIT_TEMP, m_edTemp);
	DDX_Control(pDX, IDC_EDIT_LPT, m_edLpt);

	DDX_Text(pDX, IDC_EDIT1, m_strFileName);

	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT3, m_strFileName_app);
	DDX_Control(pDX, IDC_EDIT5, m_EdAPPVersion);
	DDX_Control(pDX, IDC_CHECK4, m_checkAllowUp);
	DDX_Control(pDX, IDC_CHECK5, m_btnUpAddr);
	DDX_Control(pDX, IDC_EDIT6, m_edBootAddr);
}

BEGIN_MESSAGE_MAP(CMFCSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CMFCSampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeHardware)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeProtocol)
	ON_BN_CLICKED(IDC_BUTTON2, OnProgram)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeDevice)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangePort)
	ON_BN_CLICKED(IDC_BUTTON3, OnVerify)
	ON_CBN_SELCHANGE(IDC_COMBO5, OnSelchangeMemoryArea)
	ON_BN_CLICKED(IDC_BUTTON5, OnBlankCheck)
	ON_BN_CLICKED(IDC_BUTTON4, OnErase)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeFileEdit)
	ON_BN_CLICKED(IDC_CHECK3, OnCheckDisableWarning)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, &CMFCSampleDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCSampleDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCSampleDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_CHECK5, &CMFCSampleDlg::OnBnClickedCheck5)
END_MESSAGE_MAP()

long  CMFCSampleDlg::DisplayMessage(const char *szMessage)
{
	// Add to the CRichEdit control
	m_outputEdit.AppendText( szMessage, &c_normalCharFormat);
	return TRUE;
}

long  CMFCSampleDlg::DisplayError(const char *szErrorMsg)
{
	// Add to the CRichEdit control
	m_outputEdit.AppendText( szErrorMsg, &c_errorCharFormat);
	// And open a dialog box
	AfxMessageBox(szErrorMsg);
	return TRUE;
}

long CMFCSampleDlg::DisplayWarning(const char *szWarningMsg)
{
	// Add to the CRichEdit control
	m_outputEdit.AppendText( szWarningMsg, &c_warningCharFormat);
	return TRUE;
}

long CMFCSampleDlg::DisplayProgress(int Percentage)
{
	m_ProgressCtrl.SetPos(Percentage);
//	RestoreWaitCursor();
	AfxGetApp()->DoWaitCursor(0);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMFCSampleDlg message handlers

BOOL CMFCSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bDisableWarning=FALSE;
	m_bConfigurationDone = FALSE;
	m_bFileLoaded = FALSE;
	m_iCurrentAreaMemoryID = -1;

	char szListBuffer[LISTSIZE];
	char szListItem[ITEMSIZE];

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Fill the hardware dialog
	int nReturn = GetHardwareList(szListBuffer,sizeof(szListBuffer));
	GetNextListItem(szListBuffer,szListItem);
	while (strlen(szListItem) != 0) {
		m_comboHardware.AddString(szListItem);
		// Next Item
		GetNextListItem(szListBuffer,szListItem);
	}

	m_btFirst.SetCheck(true);

	m_comboHardware.SetCurSel(36);
	OnSelchangeHardware();

	m_comboProtocol.SetCurSel(1);
	OnSelchangeProtocol();

	m_comboDevice.SetCurSel(3);
	OnSelchangeDevice();

	m_comboPort.SetCurSel(0);
	OnSelchangePort();

	m_comboMemoryArea.SetCurSel(1);
	OnSelchangeMemoryArea();


	m_comboHardware.EnableWindow(FALSE);
	m_comboProtocol.EnableWindow(FALSE);
	m_comboDevice.EnableWindow(FALSE);
	m_comboPort.EnableWindow(FALSE);
	m_comboMemoryArea.EnableWindow(FALSE);
	m_ProgressCtrl.SetRange(0,100);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_edMask.SetWindowText(_T("45:58:0A:0A"));
	m_edTemp.SetWindowText(_T("30"));
	m_edLpt.SetWindowText(_T("40"));
	m_EdAPPVersion.SetWindowText(_T("0.0.1"));
	m_edBootAddr.SetWindowText(_T("255.255.255.255,12345"));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMFCSampleDlg::OnSelchangeHardware() 
{
	// TODO: Add your control notification handler code here
	// Fill the device combobox 
	char szListBuffer[LISTSIZE];
	char szListItem[ITEMSIZE];

	// Get the selected hardware 
	int iIndex ;
	if ( (iIndex = m_comboHardware.GetCurSel()) != CB_ERR) {
		// Get the currently selected hardware 
		m_comboHardware.GetLBText(iIndex,m_strCurrentHardware);
		m_bConfigurationDone = FALSE;

		// Since the programming tool is known, it is 
		// possible to fill the protocol combo box ...
		m_comboProtocol.ResetContent();
		int nReturn = GetProtocolList(m_strCurrentHardware,szListBuffer,
									  sizeof(szListBuffer));
		GetNextListItem(szListBuffer,szListItem);
		while (strlen(szListItem) != 0) {
			m_comboProtocol.AddString(szListItem);
			// Next Item
			GetNextListItem(szListBuffer,szListItem);
		}
		// Enable the protocol combo
		m_comboProtocol.EnableWindow();

		// ... and to fill the port selection combo
		m_comboPort.ResetContent();
		nReturn = GetPortList(m_strCurrentHardware,szListBuffer,sizeof(szListBuffer));
		GetNextListItem(szListBuffer,szListItem);
		while (strlen(szListItem) != 0) {
			m_comboPort.AddString(szListItem);
			// Next Item
			GetNextListItem(szListBuffer,szListItem);
		}
		// Enable the port combo
		m_comboPort.EnableWindow();

		// Since the protocol has changed, the device selection and the
		// memory area are no more valid ...
		// Empty and disable the device combo
		m_comboDevice.ResetContent();
		m_comboDevice.EnableWindow(FALSE);

		// Empty and disable the memory area combo
		m_comboMemoryArea.ResetContent();
		m_comboMemoryArea.EnableWindow(FALSE);
		m_strCurrentPort.Empty();
		m_strCurrentProtocol.Empty();
		m_strCurrentDevice.Empty();
		m_strCurrentAreaMemory.Empty();
	}
}


void CMFCSampleDlg::OnSelchangeProtocol() 
{
	// TODO: Add your control notification handler code here
	char szListBuffer[LISTSIZE];
	char szListItem[ITEMSIZE];

	// Get the selected protocol 
	int iIndex ;
	if ( (iIndex = m_comboProtocol.GetCurSel()) != CB_ERR) {
		// Get the currently selected hardware 
		char szProtocol[ITEMSIZE];
		m_comboProtocol.GetLBText(iIndex,szProtocol);
		m_strCurrentProtocol = szProtocol;
		m_bConfigurationDone = FALSE;

		// Since the protocol is defined, it is possible 
		// to Fill the Device combo box
		m_comboDevice.ResetContent();
		int nReturn = GetDeviceList( (LPCTSTR) m_strCurrentHardware,szProtocol,
									szListBuffer,sizeof(szListBuffer));
		GetNextListItem(szListBuffer,szListItem);
		while (strlen(szListItem) != 0) {
			m_comboDevice.AddString(szListItem);
			// Next Item
			GetNextListItem(szListBuffer,szListItem);
		}
		m_comboDevice.EnableWindow();
		m_strCurrentDevice.Empty();
		m_strCurrentAreaMemory.Empty();
		m_iCurrentAreaMemoryID = -1;
	}
}

void CMFCSampleDlg::OnSelchangeDevice() 
{
	// TODO: Add your control notification handler code here
	int iIndex;
	char szListBuffer[LISTSIZE];
	char szListItem[ITEMSIZE];
	if ( (iIndex = m_comboDevice.GetCurSel()) != CB_ERR) {
		// Get the currently selected device 
		m_comboDevice.GetLBText(iIndex,m_strCurrentDevice);
		m_bConfigurationDone = FALSE;

		// Since the device is defined, it is possible to fill the
		// the Memory Area combo box
		m_comboMemoryArea.ResetContent();
		GetAreaList(m_strCurrentDevice,szListBuffer,sizeof(szListBuffer));

		GetNextListItem(szListBuffer,szListItem);
		while (strlen(szListItem) != 0) {
			m_comboMemoryArea.AddString(szListItem);
			// Next Item
			GetNextListItem(szListBuffer,szListItem);
		}
		m_comboMemoryArea.EnableWindow();
		m_strCurrentAreaMemory.Empty();
		m_iCurrentAreaMemoryID = -1;
	}
}

void CMFCSampleDlg::OnSelchangePort() 
{
	// TODO: Add your control notification handler code here
	int iIndex;
	if ( (iIndex = m_comboPort.GetCurSel()) != CB_ERR) {
		// Get the currently selected communication port 
		m_comboPort.GetLBText(iIndex,m_strCurrentPort);
		m_bConfigurationDone = FALSE;
	}
}

void CMFCSampleDlg::OnSelchangeMemoryArea() 
{
	// TODO: Add your control notification handler code here
	// Enable or Disable the Erase and Blank check buttons accordingly with
	// the selected device and area
	int iIndex;
	if ( (iIndex = m_comboMemoryArea.GetCurSel()) != CB_ERR) {
		if (ConfigurationSetup() == TRUE) {
			// Get the currently selected Area memory name 
			m_comboMemoryArea.GetLBText(iIndex,m_strCurrentAreaMemory);
			EGetId(m_strCurrentAreaMemory, &m_iCurrentAreaMemoryID);
			CButton *pButton = (CButton *) GetDlgItem(IDC_BUTTON4);
			pButton->EnableWindow(IsErasableArea((LPCTSTR) m_strCurrentDevice,
														   m_strCurrentAreaMemory));
			pButton = (CButton *) GetDlgItem(IDC_BUTTON5);
			pButton->EnableWindow(IsBlankCheckAvailable((LPCTSTR) m_strCurrentDevice,
													     m_strCurrentAreaMemory));
			// force the file loading
			m_bFileLoaded = FALSE;

		}
	}
}


void CMFCSampleDlg::OnButton1() 
{
	CString strFilter("Intel Hex (*.hex)|*.hex|Motorola S19 (*.s19)|*.s19|All files (*.*)|*.*||");
	// TODO: Add your control notification handler code here
	CFileDialog fileDialog(TRUE,NULL,NULL,OFN_EXPLORER|OFN_FILEMUSTEXIST,strFilter);
	fileDialog.m_ofn.lpstrTitle = "Enter your application file";
	if (fileDialog.DoModal() == IDOK) {
		m_strFileName = fileDialog.GetPathName();
		m_bFileLoaded = FALSE;
		UpdateData(FALSE);
	}
}


void CMFCSampleDlg::OnChangeFileEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bFileLoaded = FALSE;
	UpdateData(TRUE);
}

void CMFCSampleDlg::OnProgram() 
{
	// TODO: Add your control notification handler code here
	//hex烧写

	CString strMask;
	CString strTemp;
	CString strLpt;
	CString strVersionApp;
	m_edMask.GetWindowText(strMask);
	m_edTemp.GetWindowText(strTemp);
	m_edLpt.GetWindowText(strLpt);
	m_EdAPPVersion.GetWindowText(strVersionApp);

	if (strMask.GetLength() != 11)
	{
		AfxMessageBox(_T("请正确填写MAC"));
		return;
	}
	if (strMask == _T(""))
	{
		AfxMessageBox(_T("MAC不能为空"));
		return;
	}
	if (strTemp == _T(""))
	{
		AfxMessageBox(_T("请填写温度限制值"));
		return;
	}

	if (m_btFirst.GetCheck())
	{
		if (strLpt == _T(""))
		{
			AfxMessageBox(_T("请填写剩余电流门限"));
			return;
		}
	}
	if (m_strFileName.IsEmpty())
	{
		AfxMessageBox(_T("boot程序不能为空"));
		return;
	}

	if (m_strFileName_app.IsEmpty())
	{
		AfxMessageBox(_T("app程序不能为空"));
		return;
	}

	if (strVersionApp.IsEmpty())
	{
		AfxMessageBox(_T("app版本号不能为空"));
		return;
	}

	if (m_btFirst.GetCheck())
	{
		CString str = DataConvertStm32(strMask, atoi(strTemp.GetString()), atoi(strLpt));
		WriteDataToStm32Hex(m_strFileName, str.GetBuffer(0), str.GetLength());
	}
	else
	{
		CString str = DataConvertStm8(strMask, atoi(strTemp.GetString()));
		WriteDataToStm8Hex(m_strFileName, str.GetBuffer(0), str.GetLength());
	}

	if (ConfigurationSetup() == TRUE) {
		// hardware configuration succeeds
		// Load the file 
		if (!m_strFileName.IsEmpty()) {
			StartLengthyProcess();
			if (m_bFileLoaded == FALSE) {
				FillImageMemory(m_strCurrentDevice,m_strCurrentAreaMemory);
				
				InitProgressBar("Load File");
				LDisplayMessage("Loading file : " + m_strFileName + 
					            " in " + m_strCurrentAreaMemory); 
				int nReturn = ELoadFile(m_strFileName, m_iCurrentAreaMemoryID);
				m_bFileLoaded = (nReturn == 1);

				nReturn = ELoadFile(m_strFileName_app, m_iCurrentAreaMemoryID);
				m_bFileLoaded = (nReturn == 1);

				nReturn = ELoadFile(m_strFileName + _T("config"), m_iCurrentAreaMemoryID);
				m_bFileLoaded = (nReturn == 1);
				ResetProgressBar();
			}
			if (m_bFileLoaded == TRUE) {
				// File is loaded, try to program
				if (m_iCurrentAreaMemoryID != -1) {
					InitProgressBar("Program");
					LDisplayMessage("Programming " + m_strCurrentAreaMemory);
					EProgAll(m_iCurrentAreaMemoryID);
					ResetProgressBar();
				}
				else {
					AfxMessageBox("No memory area is selected");
				}
			}
			EndLengthyProcess();
		}
		else {
			// No file
			AfxMessageBox("A file must be specified");
		}
	}
}

void CMFCSampleDlg::OnVerify() 
{
	// TODO: Add your control notification handler code here
	if (ConfigurationSetup() == TRUE) {
		// hardware configuration succeeds
		StartLengthyProcess();
		if (m_iCurrentAreaMemoryID != -1) {
			InitProgressBar("Verify");
			LDisplayMessage("Verifying " + m_strCurrentAreaMemory);
			EVerifyAll(m_iCurrentAreaMemoryID);
			ResetProgressBar();
		}
		else {
			AfxMessageBox("No memory area is selected");
		}
		EndLengthyProcess();
	}
}


void CMFCSampleDlg::OnBlankCheck() 
{
	// TODO: Add your control notification handler code here
	if (ConfigurationSetup() == TRUE) {
		// hardware configuration succeeds
		StartLengthyProcess();
		if (m_iCurrentAreaMemoryID != -1) {
			InitProgressBar("Blank Check");
			LDisplayMessage("Blank checking " + m_strCurrentAreaMemory);
			EBlankAll(m_iCurrentAreaMemoryID);
			ResetProgressBar();
		}
		else {
			AfxMessageBox("No memory area is selected");
		}
		EndLengthyProcess();
	}
}


void CMFCSampleDlg::OnErase() 
{
	// TODO: Add your control notification handler code here
unsigned long FirstAreaAddr,LastAreaAddr;

	if (ConfigurationSetup() == TRUE) {
		// hardware configuration succeeds
		StartLengthyProcess();
		if (m_iCurrentAreaMemoryID != -1) {
			InitProgressBar("Erase");
			LDisplayMessage("Erasing " + m_strCurrentAreaMemory);
			if (_strnicmp (m_strCurrentDevice,"ST7",3) == 0) {
				EEraseAll(m_iCurrentAreaMemoryID);
			}
			else {
				if (GetAreaMapping(m_strCurrentDevice,m_strCurrentAreaMemory,&FirstAreaAddr,&LastAreaAddr) == 1) {
					EEraseArea(m_iCurrentAreaMemoryID, FirstAreaAddr, LastAreaAddr);
				}
				else {
					AfxMessageBox("Error getting memory mapping");
				}
			}
			ResetProgressBar();
		}
		else {
			AfxMessageBox("No memory area is selected");
		}
		EndLengthyProcess();
	}
}

// OnOK must be overridden because even if there is no button with the IDOK ID,
// this virtual callback will still be called when the user hits return and the
// default  CDialog::OnOK will close the application
void CMFCSampleDlg::OnOK()
{
}


void CMFCSampleDlg::OnCheckDisableWarning()
{
BOOL nReturn=FALSE;

	if (static_cast<CButton*>(GetDlgItem( IDC_CHECK3))->GetCheck()) {
		m_bDisableWarning=TRUE; // Box Checked => ReConfigure with warning disabled
		m_bConfigurationDone = FALSE;
		nReturn=ConfigurationSetup();
	}
	else {
		m_bDisableWarning=FALSE; // Box UnChecked => ReConfigure with warning enabled
		m_bConfigurationDone = FALSE;
		nReturn=ConfigurationSetup();
	}
}


void CMFCSampleDlg::InitProgressBar(CString Process)
{
	CStatic *pStatic = (CStatic *) GetDlgItem(IDC_STATIC_PROCESS);
	pStatic->SetWindowText(Process);
	// Reset the progress bar
	m_ProgressCtrl.SetPos(0);
}

void CMFCSampleDlg::ResetProgressBar()
{
	CStatic *pStatic = (CStatic *) GetDlgItem(IDC_STATIC_PROCESS);
	pStatic->SetWindowText("Progress bar");
	// Reset the progress bar
	m_ProgressCtrl.SetPos(0);
}

BOOL CMFCSampleDlg::ConfigurationSetup()
{
	int nReturn = 1;
	if (!m_strCurrentDevice.IsEmpty() && !m_strCurrentPort.IsEmpty() &&
		!m_strCurrentHardware.IsEmpty() && !m_strCurrentProtocol.IsEmpty() ) {

		if (m_bConfigurationDone == FALSE) {

			// close communication on a previous session (if needed)
			ECloseComm();

			// programming parameters initialization
			nReturn = ESelectHard(m_strCurrentHardware,m_strCurrentProtocol,FALSE);

			// Set Preferences: By default, the Protection warning is TRUE (message box)
			if (m_bDisableWarning==TRUE)
			{
				nReturn &= ESetPreferences(PROTECTION_WARNING,FALSE); // No warning message box
			}
			else {
				nReturn &= ESetPreferences(PROTECTION_WARNING,TRUE); // warning message box
			}

			nReturn &= ESelectDevice(m_strCurrentDevice);
			
			nReturn &= ESelectPort(m_strCurrentPort);
			
			m_bConfigurationDone = TRUE;
			// The memory image buffers has been cleared, the file is no more loaded
			m_bFileLoaded = FALSE;
		}
	}
	else {
		//at least one configuration parameter is not defined
		AfxMessageBox("All configuration parameters must be initialized");
		nReturn = 0;
	}
	return (nReturn==1);
}

void CMFCSampleDlg::StartLengthyProcess() 
{	
	//Show wait cursor
	AfxGetApp()->DoWaitCursor(1);

	// Disable all controls excepted progress bar and output edit
	m_comboMemoryArea.EnableWindow(FALSE);
	m_comboPort.EnableWindow(FALSE);
	m_comboProtocol.EnableWindow(FALSE);
	m_comboHardware.EnableWindow(FALSE);
	m_comboDevice.EnableWindow(FALSE);

	// Disable the action buttons
	CButton *pButton = (CButton *) GetDlgItem(IDC_BUTTON2);
	pButton->EnableWindow(FALSE);
	pButton = (CButton *) GetDlgItem(IDC_BUTTON3);
	pButton->EnableWindow(FALSE);
	pButton = (CButton *) GetDlgItem(IDC_BUTTON4);
	pButton->EnableWindow(FALSE);
	pButton = (CButton *) GetDlgItem(IDC_BUTTON5);
	pButton->EnableWindow(FALSE);
}

void CMFCSampleDlg::EndLengthyProcess() 
{
	//suppress the wait cursor
	AfxGetApp()->DoWaitCursor(-1);
	m_comboMemoryArea.EnableWindow(FALSE);
	m_comboPort.EnableWindow(FALSE);
	m_comboProtocol.EnableWindow(FALSE);
	m_comboHardware.EnableWindow(FALSE);
	m_comboDevice.EnableWindow(FALSE);

	// Enable the action buttons again (remember that erase button and
	// blank check button states depend on the device and the memory area 
	// currently selected)
	CButton *pButton = (CButton *) GetDlgItem(IDC_BUTTON2);
	pButton->EnableWindow(TRUE);
	pButton = (CButton *) GetDlgItem(IDC_BUTTON3);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *) GetDlgItem(IDC_BUTTON4);
	pButton->EnableWindow(IsErasableArea((LPCTSTR) m_strCurrentDevice,
										  m_strCurrentAreaMemory));
	
	pButton = (CButton *) GetDlgItem(IDC_BUTTON5);
	pButton->EnableWindow(IsBlankCheckAvailable((LPCTSTR) m_strCurrentDevice,
											    m_strCurrentAreaMemory));
}


void CMFCSampleDlg::OnBnClickedCheck1()
{
	// TODO: ????????????????????
	m_btFirst.SetCheck(true);
	m_btSecond.SetCheck(false);
	m_comboHardware.SetCurSel(36);
	OnSelchangeHardware();

	m_comboProtocol.SetCurSel(1);
	OnSelchangeProtocol();

	m_comboDevice.SetCurSel(3);
	OnSelchangeDevice();

	m_comboPort.SetCurSel(0);
	OnSelchangePort();

	m_comboMemoryArea.SetCurSel(1);
	OnSelchangeMemoryArea();

	m_comboHardware.EnableWindow(FALSE);
	m_comboProtocol.EnableWindow(FALSE);
	m_comboDevice.EnableWindow(FALSE);
	m_comboPort.EnableWindow(FALSE);
	m_comboMemoryArea.EnableWindow(FALSE);
	m_edLpt.EnableWindow(TRUE);
}


void CMFCSampleDlg::OnBnClickedCheck2()
{
	// TODO: ????????????????????
	m_btFirst.SetCheck(false);
	m_btSecond.SetCheck(true);
	m_comboHardware.SetCurSel(36);
	OnSelchangeHardware();

	m_comboProtocol.SetCurSel(2);
	OnSelchangeProtocol();

	m_comboDevice.SetCurSel(88);
	OnSelchangeDevice();

	m_comboPort.SetCurSel(0);
	OnSelchangePort();

	m_comboMemoryArea.SetCurSel(2);
	OnSelchangeMemoryArea();

	m_comboHardware.EnableWindow(FALSE);
	m_comboProtocol.EnableWindow(FALSE);
	m_comboDevice.EnableWindow(FALSE);
	m_comboPort.EnableWindow(FALSE);
	m_comboMemoryArea.EnableWindow(FALSE);
	m_edLpt.EnableWindow(FALSE);
}

void CMFCSampleDlg::WriteDataToStm8Hex(CString strFileName, char* pData, int nLength)
{
	CFileFind fileFind;
	if (!fileFind.FindFile(strFileName))
	{
		return;
	}
	CFile file;
	if (!file.Open(strFileName, CFile::modeReadWrite))
	{
		return;
	}
	long length = file.GetLength();
	char* pBuffer = new char[length];
	file.Read(pBuffer, length);
	int ncmp = memcmp(pBuffer, _T(":06ff8000"), 9);
	if (ncmp == 0)
	{
		file.Seek(0, CFile::begin);
		file.Write(pData, nLength);
	}
	else
	{
		file.Seek(0, CFile::begin);
		file.Write(pData, nLength);
		file.Seek(nLength, CFile::begin);
		file.Write(pBuffer, length);
	}
	file.Close();
}

void CMFCSampleDlg::WriteDataToStm32Hex(CString strFileName, char* pData, int nLength)
{
	CFileFind fileFind;
	if (!fileFind.FindFile(strFileName))
	{
		return;
	}
	CFile file;
	if (!file.Open(strFileName+"config", CFile::modeCreate | CFile::modeReadWrite))
	{
		return;
	}
	file.Write(pData, nLength);
	file.Write(_T(":00000001FF"), 12);
	file.Close();
}

CString CMFCSampleDlg::DataConvertStm8(CString strMaskd, int nTempe)
{
	CString str = _T("");
	int iPos = 0;
	str += strMaskd.Tokenize(_T(":"), iPos);
	str += strMaskd.Tokenize(_T(":"), iPos);
	str += strMaskd.Tokenize(_T(":"), iPos);
	str += strMaskd.Tokenize(_T(":"), iPos);

	CString strTempe = _T("");
	nTempe = nTempe / 0.0625;

	strTempe.Format("%04x", nTempe);
	CString DataBuffer = "06ff8000" + str + strTempe;

	//0x100 - ((0x02 + 0x00 + 0x00 + 0x04 + 0x00 + 0x03) % 256) = 0xF7
	int n = 0;
	int nCount = DataBuffer.GetLength() / 2;
	int sum = 0;
	for (int i=0;i<nCount;i++)
	{
		CString str = DataBuffer.Mid(i * 2, 2);
		sscanf(str, "%x", &n);
		sum += n;
	}
	sum = 0x100 - sum % 256;

	strTempe.Format("%02x", sum);

	DataBuffer += strTempe;

	return _T(":")+DataBuffer+_T("\n");


}

CString CMFCSampleDlg::DataConvertStm32(CString strMaskd, int nTempe, int nlpt)
{
	CString str = _T("");
	int iPos = 0;
	str += (strMaskd.Tokenize(_T(":"), iPos));
	str += (strMaskd.Tokenize(_T(":"), iPos));
	str += (strMaskd.Tokenize(_T(":"), iPos));
	str += (strMaskd.Tokenize(_T(":"), iPos));

	CString strTempe = _T("");
	nTempe = nTempe / 0.0625;
	strTempe.Format("%04X", nTempe);
	strTempe = strTempe.Right(2) + strTempe.Left(2);

	CString strLpt = _T("");
	strLpt.Format("%04X", nlpt);
	strLpt = strLpt.Right(2) + strLpt.Left(2);

	// ??д??????-----??????????-----???????????
	CString DataBuffer = _T("04E00000") + strTempe + strLpt;


	//0x100 - ((0x02 + 0x00 + 0x00 + 0x04 + 0x00 + 0x03) % 256) = 0xF7
	int n = 0;
	int nCount = DataBuffer.GetLength() / 2;
	int sum = 0;
	for (int i = 0; i<nCount; i++)
	{
		CString buffer01 = DataBuffer.Mid(i * 2, 2);
		sscanf(buffer01, "%x", &n);
		sum += n;
	}
	sum = 0x100 - sum % 256;

	strTempe.Format("%02x", sum);

	DataBuffer += strTempe;

	// ??????дmac
	CString mac = _T("04D00000") + str;
	n = sum = 0;
	nCount = mac.GetLength() / 2;
	for (int i = 0; i<nCount; i++)
	{
		CString buffer01 = mac.Mid(i * 2, 2);
		sscanf(buffer01, "%x", &n);
		sum += n;
	}
	sum = 0x100 - sum % 256;
	strTempe.Format("%02x", sum);
	mac = _T(":") + mac + strTempe;
	DataBuffer = DataBuffer + _T("\n") + mac;


	// 版本号
	CString str_Version;
	m_EdAPPVersion.GetWindowText(str_Version);
	uint16_t big_v = 0, mid_v = 0, sma_v = 0;
	int v_pos = 0;
	
	CString v_Buffer;
	big_v = atoi(str_Version.Tokenize(_T("."), v_pos));
	mid_v = atoi(str_Version.Tokenize(_T("."), v_pos));
	sma_v = atoi(str_Version.Tokenize(_T("."), v_pos));
	v_Buffer.Format("%04X", big_v);
	v_Buffer = v_Buffer.Right(2) + v_Buffer.Left(2);
	str_Version = v_Buffer;

	v_Buffer.Format("%04X", mid_v);
	v_Buffer = v_Buffer.Right(2) + v_Buffer.Left(2);
	str_Version += v_Buffer;

	v_Buffer.Format("%04X", sma_v);
	v_Buffer = v_Buffer.Right(2) + v_Buffer.Left(2);
	str_Version += v_Buffer;
	str_Version += _T("000000000000");
	str_Version = _T("0ED800000100") + str_Version;
	n = sum = 0;
	nCount = str_Version.GetLength() / 2;
	for (int i = 0; i<nCount; i++)
	{
		CString buffer01 = str_Version.Mid(i * 2, 2);
		sscanf(buffer01, "%x", &n);
		sum += n;
	}
	sum = 0x100 - sum % 256;
	strTempe.Format("%02x", sum);
	str_Version = _T(":") + str_Version + strTempe;
	DataBuffer = DataBuffer + _T("\n") + str_Version;

	if (m_checkAllowUp.GetCheck() != 0)
	{// D104  ioc F800
		CString al_up = _T(":02D10400000029");
		CString la_up = _T(":02F80000000006");
		DataBuffer = DataBuffer + _T("\n") + al_up;
		DataBuffer = DataBuffer + _T("\n") + la_up;
	}

	// 设置网络地址
	if (m_btnUpAddr.GetCheck() != 0)
	{
		CString addr_data = _T("");
		CString addr_src = _T("");
		m_edBootAddr.GetWindowText(addr_src);
		int addr_length = addr_src.GetLength();
		addr_data.Format("%02X", addr_length + 1);
		addr_data += _T("D10600");
		for (int i = 0; i < addr_length; i++)
		{
			CString addr_buf = _T("");
			addr_buf.Format("%02X", addr_src[i]);
			addr_data += addr_buf;
		}
		
		CString addr_buf = _T("");
		addr_buf.Format("%02X", ':');
		addr_data += addr_buf;

		n = sum = 0;
		nCount = addr_data.GetLength() / 2;
		for (int i = 0; i<nCount; i++)
		{
			CString buffer01 = addr_data.Mid(i * 2, 2);
			sscanf(buffer01, "%x", &n);
			sum += n;
		}
		sum = 0x100 - sum % 256;
		strTempe.Format("%02X", sum);
		addr_data = _T(":") + addr_data + strTempe;
		DataBuffer = DataBuffer + _T("\n") + addr_data;
	}

	return _T(":020000040803EF\n:") + DataBuffer + _T("\n");
}


void CMFCSampleDlg::OnBnClickedButton6()
{
	// TODO: ????????????????????
	CString strFilter("Intel Hex (*.hex)|*.hex|Motorola S19 (*.s19)|*.s19|All files (*.*)|*.*||");
	// TODO: Add your control notification handler code here
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, strFilter);
	fileDialog.m_ofn.lpstrTitle = "Enter your application file";
	if (fileDialog.DoModal() == IDOK) {
		m_strFileName_app = fileDialog.GetPathName();
		m_bFileLoaded = FALSE;
		UpdateData(FALSE);
	}
}


void CMFCSampleDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_btnUpAddr.GetCheck() != 0)
	{
		m_edBootAddr.SetReadOnly(FALSE);
	}
	else
	{
		m_edBootAddr.SetReadOnly(TRUE);
	}
}
