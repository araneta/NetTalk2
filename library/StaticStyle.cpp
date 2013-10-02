// StaticStyle.cpp : implementation file
//

#include "stdafx.h"
#include "StaticStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticStyle

CStaticStyle::CStaticStyle()
{
}

CStaticStyle::~CStaticStyle()
{
}


BEGIN_MESSAGE_MAP(CStaticStyle, CStatic)
	//{{AFX_MSG_MAP(CStaticStyle)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticStyle message handlers

void CStaticStyle::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CString sCaption;
	CRect rect;
	GetClientRect(&rect);

	GetWindowText(sCaption);
	dc.SetBkMode(TRANSPARENT);
	CFont Font;
	Font.CreateFont(7, 2, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0,0, _T("MS Sans Serif"));
	CFont *oldFont = dc.SelectObject(&Font);
	
	dc.SetTextColor(RGB(0,0,255));
	dc.DrawText(sCaption, rect, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	
}
