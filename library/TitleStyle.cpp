// TitleStyle.cpp : implementation file
//

#include "stdafx.h"
#include "TitleStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleStyle

CTitleStyle::CTitleStyle()
{
	m_cColor = RGB(0,0,255);
}

CTitleStyle::~CTitleStyle()
{
}


BEGIN_MESSAGE_MAP(CTitleStyle, CStatic)
	//{{AFX_MSG_MAP(CTitleStyle)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleStyle message handlers

void CTitleStyle::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CString sCaption;
	CRect rect;
	GetClientRect(&rect);

	GetWindowText(sCaption);
	dc.SetBkMode(TRANSPARENT);
	CFont Font;
	Font.CreateFont(7, 2, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0,0, _T("MS Sans Serif"));
	CFont *oldFont = dc.SelectObject(&Font);
	
	dc.SetTextColor(m_cColor);
	dc.DrawText(sCaption, rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
}

void CTitleStyle::SetColor(COLORREF cr)
{
	m_cColor = cr;
}
