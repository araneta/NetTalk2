#if !defined(AFX_STATICSTYLE_H__C9A6A753_2F9E_4E5E_9464_FAA6AECE4995__INCLUDED_)
#define AFX_STATICSTYLE_H__C9A6A753_2F9E_4E5E_9464_FAA6AECE4995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticStyle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticStyle window

class CStaticStyle : public CStatic
{
// Construction
public:
	CStaticStyle();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticStyle)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticStyle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticStyle)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICSTYLE_H__C9A6A753_2F9E_4E5E_9464_FAA6AECE4995__INCLUDED_)
