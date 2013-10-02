#if !defined(AFX_TITLESTYLE_H__7A3535F0_1E1A_4A6D_AEF4_90CFA8A7A387__INCLUDED_)
#define AFX_TITLESTYLE_H__7A3535F0_1E1A_4A6D_AEF4_90CFA8A7A387__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitleStyle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTitleStyle window

class CTitleStyle : public CStatic
{
// Construction
public:
	CTitleStyle();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleStyle)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF cr);
	virtual ~CTitleStyle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTitleStyle)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_cColor;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLESTYLE_H__7A3535F0_1E1A_4A6D_AEF4_90CFA8A7A387__INCLUDED_)
