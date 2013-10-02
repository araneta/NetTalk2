#if !defined(AFX_PENGECUALIANDLG_H__B93925AC_A6A1_4B9A_9127_4D6844537BB6__INCLUDED_)
#define AFX_PENGECUALIANDLG_H__B93925AC_A6A1_4B9A_9127_4D6844537BB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PengecualianDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPengecualianDlg dialog
#include "library/ListCtrlSort.h"
#include "library/ButtonStyle.h"
#include "library/TitleStyle.h"
#include "library/StaticStyle.h"

class CPengecualianDlg : public CDialog
{
// Construction
public:
	void SetDB(CADODatabase * pDb);
	CPengecualianDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPengecualianDlg)
	enum { IDD = IDD_DATA_PENGECUALIAN };
	CStaticStyle	m_cstPengucapan;
	CStaticStyle	m_cstList;
	CStaticStyle	m_cstKata;
	CButtonStyle	m_cDelete;
	CButtonStyle	m_cTambah;
	CButtonStyle	m_cSimbol3;
	CButtonStyle	m_cSimbol2;
	CButtonStyle	m_cSimbol1;
	ListCtrlSort	m_cListPengecualian;
	//CListCtrl	m_cListPengecualian;
	CString	m_sPengucapan;
	CString	m_sKata;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPengecualianDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadPengecualian();
	// Generated message map functions
	//{{AFX_MSG(CPengecualianDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnTambah();
	afx_msg void OnSimbol1();
	afx_msg void OnSimbol2();
	afx_msg void OnSimbol3();
	afx_msg void OnItemchangedDaftarpengecualian(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetStyle();
	CADODatabase *m_pDb;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PENGECUALIANDLG_H__B93925AC_A6A1_4B9A_9127_4D6844537BB6__INCLUDED_)
