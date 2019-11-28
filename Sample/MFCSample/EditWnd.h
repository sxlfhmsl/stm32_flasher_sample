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

#ifndef __EDITWND_H_
#define __EDITWND_H_

/////////////////////////////////////////////////////////////////////////////
// COutputEditWnd window
//
class COutputEditWnd : public CRichEditCtrl
{
// Construction
public:
	DECLARE_DYNAMIC(COutputEditWnd);

	COutputEditWnd();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputEditWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputEditWnd();
	virtual void AppendText( const CString& strText, CHARFORMAT* pFormat);

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputEditWnd)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnSetFont(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()

protected:
	// Tools
	CString GetSelLine (void);
	CString GetSelText (void);

	// Data
	CBrush *m_brushBackground;
	BOOL	m_bAutoScroll;
	int		m_nNbLine;
	int		m_nFontSize;
};


#endif

