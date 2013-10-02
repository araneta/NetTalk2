#if !defined(AFX_LEARNINGDLG_H__134FA330_E881_474E_817E_EBEA06618176__INCLUDED_)
#define AFX_LEARNINGDLG_H__134FA330_E881_474E_817E_EBEA06618176__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LearningDlg.h : header file
//

#include "1_5SPAW.h"
#include "library/ButtonStyle.h"
#include "library/StaticStyle.h"
#include "library/TitleStyle.h"
/////////////////////////////////////////////////////////////////////////////
// CLearningDlg dialog

class CLearningDlg : public CDialog
{

// Construction
public:
private:
	void SetStyle();
	BOOL m_bTraining,m_bSaving,m_bLoading;
	CWinThread* m_pTrainingThread;
	CWinThread* m_pSaveThread;
public:	
	void SetSPAW(CSPAW *pSPAW);
	CSPAW *m_pSPAW;
	CLearningDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CLearningDlg)
	enum { IDD = IDD_LEARNING_DIALOG };
	CButtonStyle	m_cTest;
	CTitleStyle	m_cstTitle2;
	CTitleStyle	m_cstTitle1;
	CStaticStyle	m_cstTarget;
	CStaticStyle	m_cstStatus;
	CStaticStyle	m_cstProg;
	CStaticStyle	m_cstMaxEpoch;
	CStaticStyle	m_cstLearn;
	CStaticStyle	m_cstInput;
	CStaticStyle	m_cstHidden;
	CStaticStyle	m_cstFonem;
	CButtonStyle	m_cProcess;
	CButtonStyle	m_cSave;
	CButtonStyle	m_cLearn;
	CEdit	m_cStatus;
	CProgressCtrl	m_cProgLearning;
	CString	m_sStatus;
	CString	m_sTeksFonem;
	CString	m_sTeksInput;
	UINT	m_uiMaxEpoch;
	double	m_dLearningRate;
	UINT	m_uiHiddenUnit;	
	double	m_dTargetError;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLearningDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLearningDlg)
	afx_msg void OnLearning();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnProses();
	afx_msg void OnSave();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTest();
	//}}AFX_MSG
	afx_msg LRESULT OnThreadLearningFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadSaveFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadLoadFinisihed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
};
UINT TrainingThread(LPVOID param);
UINT SaveBobotThread(LPVOID param);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEARNINGDLG_H__134FA330_E881_474E_817E_EBEA06618176__INCLUDED_)

