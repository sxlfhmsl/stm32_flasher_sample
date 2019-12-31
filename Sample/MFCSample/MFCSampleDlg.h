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

// MFCSampleDlg.h : header file
//

#if !defined(AFX_MFCSampleDLG_H__A457847B_31CE_44DE_AFBC_C6803BEE3571__INCLUDED_)
#define AFX_MFCSampleDLG_H__A457847B_31CE_44DE_AFBC_C6803BEE3571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EditWnd.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleDlg dialog

class CMFCSampleDlg : public CDialog
{
// Construction
public:
	CMFCSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMFCSampleDlg)
	enum { IDD = IDD_MFCSAMPLE_DIALOG };
	CComboBox	m_comboMemoryArea;
	CProgressCtrl	m_ProgressCtrl;
	COutputEditWnd	m_outputEdit;
	CComboBox	m_comboPort;
	CComboBox	m_comboProtocol;
	CComboBox	m_comboHardware;
	CComboBox	m_comboDevice;
	CString	m_strFileName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMFCSampleDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnSelchangeHardware();
	afx_msg void OnSelchangeProtocol();
	afx_msg void OnProgram();
	afx_msg void OnSelchangeDevice();
	afx_msg void OnSelchangePort();
	afx_msg void OnVerify();
	afx_msg void OnSelchangeMemoryArea();
	afx_msg void OnBlankCheck();
	afx_msg void OnErase();
	afx_msg void OnChangeFileEdit();
	afx_msg void OnCheckDisableWarning();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public :
	long /*CALLBACK*/ DisplayMessage(const char *szMessage);
	long /*CALLBACK*/ DisplayError(const char *szErrorMsg);
	long /*CALLBACK*/ DisplayWarning(const char *szErrorMsg);
	long DisplayProgress(int Percentage);

protected:	// members
	BOOL	m_bDisableWarning;
	BOOL	m_bConfigurationDone;
	CString m_strCurrentHardware;
	CString m_strCurrentProtocol;
	CString m_strCurrentDevice;
	CString m_strCurrentPort;
	CString m_strCurrentAreaMemory;
	unsigned long	m_iCurrentAreaMemoryID;
	BOOL	m_bFileLoaded;
	CButton m_btFirst;
	CButton m_btSecond;

	CEdit  m_edMask;
	CEdit  m_edTemp;
	CEdit  m_edLpt;

protected:  // methods
	void InitProgressBar(CString Process);
	void ResetProgressBar();
	BOOL ConfigurationSetup();
	void StartLengthyProcess();
	void EndLengthyProcess();
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();

private:

	//写入阈值到stm8hex文件
	void WriteDataToStm8Hex(CString strFileName,char* pData,int nLength);

	//写入阈值到stm32hex文件
	void WriteDataToStm32Hex(CString strFileName, char* pData, int nLength);

	//数据转换stm8
	CString DataConvertStm8(CString strMaskd,int nTempe);

	//数据转换stm32
	CString DataConvertStm32(CString strMaskd, int nTempe, int nlpt);

public:
	afx_msg void OnBnClickedButton6();
	CString m_strFileName_app;
	CEdit m_EdAPPVersion;
	CButton m_checkAllowUp;
	CButton m_btnUpAddr;
	CEdit m_edBootAddr;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCSampleDLG_H__A457847B_31CE_44DE_AFBC_C6803BEE3571__INCLUDED_)
