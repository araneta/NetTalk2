// ButtonStyle.cpp : implementation file
//

#include "stdafx.h"
#include "ButtonStyle.h"

//#include "Buttons.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonStyle

CButtonStyle::CButtonStyle()
{
	nState=bMouse=TRUE;
	m_TextColorDark=m_TextColorLight=RGB(255,255,255);
	m_hIcon1=m_hIcon2=NULL;
	m_Selected = FALSE;
	m_bTab = FALSE;
}

CButtonStyle::~CButtonStyle()
{
}

BEGIN_MESSAGE_MAP(CButtonStyle, CButton)
	//{{AFX_MSG_MAP(CButtonStyle)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonStyle message handlers
void CButtonStyle::PreSubclassWindow()
{
	ModifyStyle(0,BS_OWNERDRAW | BS_DEFPUSHBUTTON);
	CButton::PreSubclassWindow();
}
void CButtonStyle::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	
	CString sCaption;
	CRect rect=lpDIS->rcItem;

	GetWindowText(sCaption);
	pDC->SetBkMode(TRANSPARENT);

	CFont *oldFont = pDC->SelectObject(&m_Font);
	
	m_Selected = (lpDIS->itemState & ODS_SELECTED);
	
	if((!m_bTab) || (GetDlgCtrlID()!=m_IDSelected))
	{
		if(nState==1){
			if(m_hHdc!=NULL)
				PaintBG(pDC,1);
			pDC->DrawIcon(0,3,m_hIcon1);
			pDC->SetTextColor(m_TextColorDark);
			pDC->DrawText(sCaption, rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		}				
		else if(nState==3){
			if(m_hHdc!=NULL)
				PaintBG(pDC,3);
			if(m_Selected)
			{
				pDC->DrawIcon(1,4,m_hIcon2);
				pDC->SetTextColor(m_TextColorLight);
				pDC->DrawText(sCaption, rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			}
			else
			{
				pDC->DrawIcon(0,3,m_hIcon2);
				pDC->SetTextColor(m_TextColorLight);
				pDC->DrawText(sCaption, rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			}				
		}else if(nState==2){
			if(m_hHdc!=NULL)
				PaintBG(pDC,2);
			pDC->DrawIcon(0,3,m_hIcon2);
			pDC->SetTextColor(m_TextColorLight);
			pDC->DrawText(sCaption, rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);		
		}		
	}else
	{		
		if(m_hHdc!=NULL)
			PaintBG(pDC,1);
		pDC->DrawIcon(0,3,m_hIcon2);
		pDC->SetTextColor(m_TextColorLight);	
		pDC->DrawText(sCaption, rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);					
	}
	pDC->SelectObject(oldFont);					
	

}

void CButtonStyle::OnMouseMove(UINT nFlags, CPoint point)
{
	if(bMouse)
	{
		nState=2;
		Invalidate();
		bMouse=FALSE;
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = m_hWnd;
	::_TrackMouseEvent(&tme);
	
	CButton::OnMouseMove(nFlags, point);
}

void CButtonStyle::OnLButtonDown(UINT nFlags, CPoint point) 
{
	nState=3;
	Invalidate();

	CButton::OnLButtonDown(nFlags, point);
}

void CButtonStyle::OnLButtonUp(UINT nFlags, CPoint point) 
{
	nState=2;
	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}

void CButtonStyle::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SendMessage(WM_LBUTTONDOWN);	
	Invalidate();
	//CButton::OnLButtonDblClk(nFlags, point);
}

LRESULT CButtonStyle::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	nState=1;
	bMouse=TRUE;
	Invalidate();	
	return(0);
}

COLORREF CButtonStyle::SetTextColor(COLORREF DarkColor, COLORREF LightColor)
{
	m_TextColorDark=DarkColor;
	m_TextColorLight=LightColor;
		
	return (DarkColor);
}

void CButtonStyle::PaintBG(CDC *pDC,int iState)
{
	CRect cltRect;

	GetClientRect(&cltRect);
	switch(iState){
		case 1:
			TransparentBlt(pDC->m_hDC,0,0,cltRect.Width(), cltRect.Height(),m_hHdc,iX,iY1,iWidth,iHeight,RGB(255,0,255));
		break;
		case 2:
			TransparentBlt(pDC->m_hDC,0,0,cltRect.Width(), cltRect.Height(),m_hHdc,iX,iY2,iWidth,iHeight,RGB(255,0,255));
		break;
		case 3:
			TransparentBlt(pDC->m_hDC,0,0,cltRect.Width(), cltRect.Height(),m_hHdc,iX,iY3,iWidth,iHeight,RGB(255,0,255));
		break;
	}		

}

void CButtonStyle::SetTransparent(UINT uiBitmap, int _x, int _y1, int _y2, int _y3, int Width, int Height)
{
		HBITMAP hBmp = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(uiBitmap));
		m_hHdc = ::CreateCompatibleDC(NULL);
		SelectObject(m_hHdc, hBmp);
		iX = _x; iY1 = _y1; iY2 = _y2; iY3 = _y3;
		iWidth = Width; iHeight = Height;
}

void CButtonStyle::SetIcons(UINT nIcon1, UINT nIcon2)
{
	m_hIcon1 = ::LoadIcon(::AfxGetInstanceHandle(), MAKEINTRESOURCE(nIcon1));
	m_hIcon2 = ::LoadIcon(::AfxGetInstanceHandle(), MAKEINTRESOURCE(nIcon2));
}

void CButtonStyle::FontStyle(CString sFont)
{
	m_Font.CreateFont(7, 2, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0,0, sFont);	
	
}

BOOL CButtonStyle::OnEraseBkgnd(CDC* pDC) 
{
	//dont erase the background button
	return 1;//CButton::OnEraseBkgnd(pDC);
}

void CButtonStyle::SetSelected(int ctrlid)
{
	m_IDSelected = ctrlid;
	Invalidate();
}

void CButtonStyle::SetTab(BOOL bTrue)
{
	m_bTab = bTrue;	
}
