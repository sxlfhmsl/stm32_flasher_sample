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

#include "stdafx.h"
#include "EditWnd.h"

/////////////////////////////////////////////////////////////////////////////
//	COutputEditWnd class implementation
//	Standard class for CEdit control mapped inside the GDB controlbar
//
IMPLEMENT_DYNAMIC(COutputEditWnd, CRichEditCtrl);


COutputEditWnd::COutputEditWnd()
: m_bAutoScroll( TRUE)
, m_nFontSize( 1)
, m_nNbLine( 1)
{
	// Allocate resources
	m_brushBackground = new CBrush (GetSysColor (COLOR_WINDOW));
}


COutputEditWnd::~COutputEditWnd()
{
	// free resources
	delete m_brushBackground;
}


BEGIN_MESSAGE_MAP(COutputEditWnd, CRichEditCtrl)
	//{{AFX_MSG_MAP(COutputEditWnd)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//	FUNCTION		OnSize
//	DESCRIPTION		Update number of displayed lines in output window
/////////////////////////////////////////////////////////////////////////////
void COutputEditWnd::OnSize(UINT nType, int cx, int cy) 
{
	m_nNbLine = cy / max(1, m_nFontSize+1);	
	CRichEditCtrl::OnSize(nType, cx, cy);
}

/////////////////////////////////////////////////////////////////////////////
//	FUNCTION		OnSetFont
//	DESCRIPTION		Change font for all lines (keeping paragraph format)
/////////////////////////////////////////////////////////////////////////////
LONG COutputEditWnd::OnSetFont(UINT wParam, LONG lParam)
{
	//get font height
	LOGFONT fontProperty;
	GetObject( (HFONT)wParam, sizeof(LOGFONT), (LPVOID)&fontProperty);
	m_nFontSize = max(1, abs(fontProperty.lfHeight));
	CRect rect;
	GetClientRect( rect);
	m_nNbLine = rect.Height()/m_nFontSize;

	//set font to window
	DefWindowProc( WM_SETFONT, wParam, lParam);
	
	//get window content
	CString strWindowText;
	GetWindowText ( strWindowText);
	//remove its content
	SetWindowText( NULL);
	//and redraw all (with eventually formatting for derived members)
	AppendText( strWindowText, NULL);

	return( FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//	FUNCTION		AppendText
//	DESCRIPTION		Add text to end of this window
/////////////////////////////////////////////////////////////////////////////
void COutputEditWnd::AppendText( const CString& strBuffer, CHARFORMAT* pFormat)
{	
	// a. Store focus & caret window 
		
	HWND hOldFocus;
	if (m_bAutoScroll) {
		hOldFocus = ::SetFocus( m_hWnd);
	}

	LimitText(0);
	// b. Add text at end of current buffer:
	// Check for new text size

	int iMax = GetLimitText ();
	int iNewSize = GetTextLength() + strBuffer.GetLength()+1;
	// Check for size of data in buffer
	if (iNewSize >= iMax) {
		// Too large: remove beginning text
//		CString strWindowText;
//		GetWindowText (strWindowText);
//		SetWindowText (strWindowText.Right (iMax));
		int iOverFlow = iNewSize - iMax;
		int iBegin,iEnd;
		if ((iBegin=LineIndex(0)) != -1)
		{
			iEnd = iBegin;
			BOOL bSaveReadOnly = (GetStyle() & ES_READONLY) == ES_READONLY;
			SetReadOnly(FALSE);
			// At the beginning of the control, remove  as many lines as necessary
			// to insert the new line
			while (iEnd <= iOverFlow) {
				iEnd = iEnd + LineLength(0)+1; // +1 because \n is not included
				// Select the first line
				SetSel(LineIndex(0), LineIndex(0) + LineLength(0)+1);
				// And remove it
				Clear();
			}
			SetReadOnly(bSaveReadOnly);
		}
	} 
	
	// move cursor at end of buffer
	SetSel (-2, -1);
	// Add new buffer at the end of buffer
	if (pFormat)
		SetSelectionCharFormat( *pFormat);
	ReplaceSel (strBuffer);
	
	// Set caret at end of new buffer (set empty selection)
	SetSel (-2, -1);

	// c. Restore focus & caret
	if (m_bAutoScroll) {
		int nLineCount = GetLineCount();
		int nFirstLine = GetFirstVisibleLine();
		int nNbScroll = nLineCount-m_nNbLine-nFirstLine;
		//is scroll needed ?
		if (nNbScroll>0)
			LineScroll( nNbScroll);
		::SetFocus( hOldFocus);
	}
}




/////////////////////////////////////////////////////////////////////////////
// COutputEditWnd message handlers
//


/////////////////////////////////////////////////////////////////////////////
//	FUNCTION		CtlColor 
//	DESCRIPTION		set color attributes
/////////////////////////////////////////////////////////////////////////////
HBRUSH COutputEditWnd::CtlColor (CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	pDC->SetTextColor (GetSysColor (COLOR_WINDOWTEXT));
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return HBRUSH (*m_brushBackground);
}


/////////////////////////////////////////////////////////////////////////////
//	FUNCTION		GetSelLine / GetSelText
//	DESCRIPTION		get the current selection line / text
/////////////////////////////////////////////////////////////////////////////
CString COutputEditWnd::GetSelLine (void) 
{
	long	nStartChar, nEndChar;
	int		iNbChar, iLineNo, iNbLines;
	char	szBuffer [256] = "";

	// Get full string of current selected line
	GetSel (nStartChar, nEndChar);
	iLineNo = LineFromChar (nStartChar);
	iNbLines = GetLineCount ();
	iNbChar = GetLine (iLineNo, szBuffer, sizeof (szBuffer) - 1);
	if (iNbChar) {
		// Mark end of string
		szBuffer [iNbChar] = 0;
	}
	return CString (szBuffer);
}

CString COutputEditWnd::GetSelText (void) 
{
	long	nStartChar, nEndChar;
	CString strBuffer, strSelection;

	// Get selected string only
	GetSel (nStartChar, nEndChar);
	GetWindowText (strBuffer);
	if (nStartChar < strBuffer.GetLength () && nEndChar >= nStartChar) {
		strSelection = strBuffer.Mid (nStartChar, nEndChar - nStartChar);
	}
	return strSelection;
}


void COutputEditWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CRichEditCtrl::OnRButtonUp(nFlags, point);
	//sent WM_CONTEXTMENU to parent
	CWnd* pParent = GetParent();
	if (pParent)
	{
		long lParam = point.y;
		lParam <<= 16;
		lParam |= point.x;
		pParent->SendMessage( WM_CONTEXTMENU, (WPARAM)m_hWnd, (LPARAM)lParam);
	}
}
