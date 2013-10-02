// NETtalk2Dlg.h : header file
//

#if !defined(AFX_NETTALK2DLG_H__3A7BFCCF_3475_4BB7_BB53_6D8327882718__INCLUDED_)
#define AFX_NETTALK2DLG_H__3A7BFCCF_3475_4BB7_BB53_6D8327882718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NaturalLanguage.h"
#include "library/ButtonStyle.h"
#include "library/StaticStyle.h"
#include "library/TitleStyle.h"

/////////////////////////////////////////////////////////////////////////////
// CNETtalk2Dlg dialog

class CNETtalk2Dlg : public CDialog
{
// Construction
public:
	CNaturalLanguage * m_NatLang;
	BOOL m_bSpeaking;
	BOOL m_bLoading;
	HWND m_hLearning;
	CString m_sFolderpath;
	void LoadPembelajaran();
	void SetNatLang(CNaturalLanguage * pNatLang);
	CNETtalk2Dlg(CWnd* pParent = NULL);	// standard constructor
	void SetConnDB(CADODatabase * db);
	PLAYKATA* m_playkata;
// Dialog Data
	//{{AFX_DATA(CNETtalk2Dlg)
	enum { IDD = IDD_NETTALK2_DIALOG };
	CButtonStyle	m_cShowLog;
	CRichEditCtrl	m_cLog;
	CButtonStyle	m_cLoad;
	CRichEditCtrl	m_cText;
	CButtonStyle	m_cClear;
	CButtonStyle	m_cSpeak;
	CButtonStyle	m_cStop;
	CString	m_sText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNETtalk2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNETtalk2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSpeak();
	afx_msg void OnClear();
	afx_msg void OnStop();
	afx_msg void OnLoad();
	afx_msg void OnClose();
	afx_msg void OnShowlog();
	//}}AFX_MSG
	afx_msg LRESULT OnThreadLoadFinisihed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void SetStyle();
	
	CADODatabase *m_pDb;
	CWinThread *m_pLoadThread;
	CWinThread *m_pParsingThread;
	CWinThread *m_pSpeakingThread;
};
UINT LoadBobot(LPVOID param);
UINT Parsing(LPVOID param);
UINT Speaking(LPVOID param);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETTALK2DLG_H__3A7BFCCF_3475_4BB7_BB53_6D8327882718__INCLUDED_)
