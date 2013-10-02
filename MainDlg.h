#if !defined(AFX_MAINDLG_H__9657EB56_38C5_4413_A93C_F8EF12F7FE66__INCLUDED_)
#define AFX_MAINDLG_H__9657EB56_38C5_4413_A93C_F8EF12F7FE66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog
#include "LIBRARY\ado2.h"	// Added by ClassView
#include "1_5SPAW.h"
#include "LearningDlg.h"
#include "NETtalk2Dlg.h"
#include "NaturalLanguage.h"
#include "KataDlg.h"
#include "PengecualianDlg.h"
#include "SingkatanDlg.h"
/////////////
#include "library/ButtonStyle.h"

class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
	CButtonStyle	m_cPlay;
	CButtonStyle	m_cClose;
	CButtonStyle	m_cMin;
	CTitleStyle	m_cstCaption;
	CButtonStyle	m_cKata;
	CButtonStyle	m_cSingkatan;
	CButtonStyle	m_cPengecualian;
	CButtonStyle	m_cPengucapan;
	CButtonStyle	m_cPembelajaran;
	CStatic	m_cMainPanel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
private:
	void SetSelected(int ctrlid);
	HBITMAP m_hBmp;
	void SetStyle();
	BOOL m_bFirst;
	HRGN GetTrans(HDC _hDC, COLORREF color, int _x, int _y, int iWidth, int iHeight);
	CWnd * m_pPrevWnd;
	CLearningDlg * m_pLearningDlg;
	CNETtalk2Dlg * m_pPengucapanDlg;
	CPengecualianDlg * m_pPengecualianDlg;
	CSingkatanDlg * m_pSingkatanDlg;
	CKataDlg * m_pKataDlg;
	void SetFolderPath();
	CString m_sFolderPath;
	void SetActiveDialog(CWnd *pWnd);
	HBITMAP m_hMainSkin;
	CADODatabase * m_pDb;
	CSPAW m_spaw;
	CBackPro m_backpro;
	CNaturalLanguage m_NatLang;
	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnPembelajaran();
	afx_msg void OnPengucapan();
	afx_msg void OnKata();
	afx_msg void OnClose();
	afx_msg void OnPengecualian();
	afx_msg void OnSingkatan();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMin();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__9657EB56_38C5_4413_A93C_F8EF12F7FE66__INCLUDED_)
