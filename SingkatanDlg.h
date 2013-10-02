#if !defined(AFX_SINGKATANDLG_H__6F1FC885_D08A_4596_8AC8_F8958D431EE8__INCLUDED_)
#define AFX_SINGKATANDLG_H__6F1FC885_D08A_4596_8AC8_F8958D431EE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SingkatanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSingkatanDlg dialog
#include "library/ListCtrlSort.h"
#include "library/ButtonStyle.h"
#include "library/StaticStyle.h"
#include "library/TitleStyle.h"

class CSingkatanDlg : public CDialog
{
// Construction
public:
	void SetDB(CADODatabase *pDb);
	CSingkatanDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSingkatanDlg)
	enum { IDD = IDD_DATA_SINGKATAN };
	CStaticStyle	m_cstSingkatan;
	CStaticStyle	m_cstList;
	CButtonStyle	m_cDelete;
	CButtonStyle	m_cTambah;
	ListCtrlSort	m_cListSingkatan;
	//CListCtrl	m_cListSingkatan;
	CString	m_sSingkatan;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSingkatanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadSingkatan();

	// Generated message map functions
	//{{AFX_MSG(CSingkatanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnTambah();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetStyle();
	CADODatabase *m_pDb;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGKATANDLG_H__6F1FC885_D08A_4596_8AC8_F8958D431EE8__INCLUDED_)
