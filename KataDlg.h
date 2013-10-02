#if !defined(AFX_KATADLG_H__33DF515E_7BDC_4390_915D_35419CFEF09C__INCLUDED_)
#define AFX_KATADLG_H__33DF515E_7BDC_4390_915D_35419CFEF09C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKataDlg dialog

#include "library/ListCtrlSort.h"
#include "library/ButtonStyle.h"
#include "library/StaticStyle.h"
#include "library/TitleStyle.h"
#include "1_5SPAW.h"

class CKataDlg : public CDialog
{
// Construction
public:
	void SetDB(CADODatabase *pDb);
	void SetSPAW(CSPAW *pSPAW);
	CKataDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CKataDlg)
	enum { IDD = IDD_DATA_KATA };
	CStaticStyle	m_cstKata;
	CStaticStyle	m_cstList;
	CStaticStyle	m_cstFonem;
	CButtonStyle	m_cTambah;
	CButtonStyle	m_cDelete;
	CButtonStyle	m_cSimbol3;
	CButtonStyle	m_cSimbol2;
	CButtonStyle	m_cSimbol1;
	ListCtrlSort	m_cListKata;
	//CListCtrl	m_cListKata;
	
	CString	m_sKata;
	CString	m_sFonem;
	//}}AFX_DATA	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKataDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadKata();

	// Generated message map functions
	//{{AFX_MSG(CKataDlg)
	afx_msg void OnTambah();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedDaftarkata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSimbol1();
	afx_msg void OnSimbol2();
	afx_msg void OnSimbol3();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetStyle();
	CSPAW* m_pSPAW;
	CADODatabase *m_pDb;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KATADLG_H__33DF515E_7BDC_4390_915D_35419CFEF09C__INCLUDED_)
