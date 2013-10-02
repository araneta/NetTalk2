// LearningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NETtalk2.h"
#include "LearningDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLearningDlg dialog
const UINT WM_LEARNING_THREAD_FINISHED = WM_APP + 1;
const UINT WM_SAVE_THREAD_FINISHED = WM_APP + 2;
const UINT WM_LOAD_THREAD_FINISHED = WM_APP + 3;


CLearningDlg::CLearningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLearningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLearningDlg)
	m_sStatus = _T("");
	m_sTeksFonem = _T("");
	m_sTeksInput = _T("");
	m_uiMaxEpoch = 0;
	m_dLearningRate = 0.0;
	m_uiHiddenUnit = 0;
	m_dTargetError = 0.0;
	//}}AFX_DATA_INIT
}


void CLearningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLearningDlg)
	DDX_Control(pDX, IDC_TEST, m_cTest);
	DDX_Control(pDX, IDC_STTITLE2, m_cstTitle2);
	DDX_Control(pDX, IDC_STTITLE1, m_cstTitle1);
	DDX_Control(pDX, IDC_STTARGET, m_cstTarget);
	DDX_Control(pDX, IDC_STSTATUS, m_cstStatus);
	DDX_Control(pDX, IDC_STPROG, m_cstProg);
	DDX_Control(pDX, IDC_STMAXEPOCH, m_cstMaxEpoch);
	DDX_Control(pDX, IDC_STLEARNRATE, m_cstLearn);
	DDX_Control(pDX, IDC_STINPUT, m_cstInput);
	DDX_Control(pDX, IDC_STHIDDEN, m_cstHidden);
	DDX_Control(pDX, IDC_STFONEM, m_cstFonem);
	DDX_Control(pDX, IDC_PROSES, m_cProcess);
	DDX_Control(pDX, IDC_SAVE, m_cSave);
	DDX_Control(pDX, IDC_LEARNING, m_cLearn);
	DDX_Control(pDX, IDC_STATUS, m_cStatus);
	DDX_Control(pDX, IDC_PROGLEARNING, m_cProgLearning);	
	DDX_Text(pDX, IDC_STATUS, m_sStatus);
	DDX_Text(pDX, IDC_TEKSFONEM, m_sTeksFonem);
	DDX_Text(pDX, IDC_TEKSINPUT, m_sTeksInput);
	DDX_Text(pDX, IDC_MAXEPOCH, m_uiMaxEpoch);
	DDX_Text(pDX, IDC_LEARNINGRATE, m_dLearningRate);
	DDX_Text(pDX, IDC_HIDDENUNIT, m_uiHiddenUnit);
	DDX_Text(pDX, IDC_TARGETERROR, m_dTargetError);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLearningDlg, CDialog)
	//{{AFX_MSG_MAP(CLearningDlg)
	ON_BN_CLICKED(IDC_LEARNING, OnLearning)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PROSES, OnProses)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LEARNING_THREAD_FINISHED, OnThreadLearningFinished)
	ON_MESSAGE(WM_SAVE_THREAD_FINISHED, OnThreadSaveFinished)
	ON_MESSAGE(WM_LOAD_THREAD_FINISHED,OnThreadLoadFinisihed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLearningDlg message handlers
LRESULT CLearningDlg::OnThreadLearningFinished(WPARAM wParam, LPARAM /*lParam*/)
{
	m_bTraining = FALSE;
	return 0L;
}
LRESULT CLearningDlg::OnThreadSaveFinished(WPARAM wParam, LPARAM /*lParam*/)
{
	m_bSaving = FALSE;
	return 0L;
}
LRESULT CLearningDlg::OnThreadLoadFinisihed(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_LEARNING)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_PROSES)->EnableWindow(TRUE);
	m_bLoading = FALSE;
	return 0L;
}
void CLearningDlg::OnLearning() 
{
	UpdateData();
	m_pSPAW->m_BpFonem->SetParam(m_uiHiddenUnit,m_dLearningRate,m_dTargetError,m_uiMaxEpoch);	
	if(!m_bSaving && !m_bLoading && !m_bTraining)
	{		
		m_pTrainingThread = AfxBeginThread(TrainingThread,this,THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
		if (m_pTrainingThread == NULL)
		{		
			MessageBox("Gagal dalam menjalankan thread pembelajaran",MB_OK);
			return;
		}
		m_pTrainingThread->m_bAutoDelete = TRUE;
		m_pTrainingThread->ResumeThread();
		GetDlgItem(IDC_LEARNING)->SetWindowText("STOP");
		m_bTraining = TRUE;
	}else
	{
		m_pSPAW->Stop(TRUE);		
	}
	
}
UINT TrainingThread(LPVOID param)
{
	CLearningDlg * dlg = (CLearningDlg*)param;
	dlg->m_cStatus.SetWindowText("");
	dlg->m_pSPAW->Stop(FALSE);
	dlg->m_pSPAW->Training(&dlg->m_cProgLearning,&dlg->m_cStatus);
	dlg->m_pSPAW->Test(&dlg->m_cStatus);
	dlg->GetDlgItem(IDC_LEARNING)->SetWindowText("Pembelajaran");
	PostMessage(dlg->m_hWnd,WM_LEARNING_THREAD_FINISHED,0,0);	
	AfxEndThread(0);	
	return 0;
}


BOOL CLearningDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bTraining = FALSE;
	m_bSaving = FALSE;	
	m_bLoading = TRUE;
	m_dLearningRate = 0.001;
	m_dTargetError = 0.01;
	m_uiHiddenUnit = 20;
	m_uiMaxEpoch = 2000;
	SetStyle();
	GetDlgItem(IDC_LEARNING)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_PROSES)->EnableWindow(FALSE);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLearningDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData();
	if(!m_bSaving && !m_bTraining)
	{
		CDialog::OnClose();
	}else
	{
		m_pSPAW->Stop(TRUE);					
	}		
}

void CLearningDlg::OnProses() 
{
	UpdateData();
	if(!m_bSaving && !m_bLoading && !m_bTraining){							
		m_sTeksFonem = m_pSPAW->Text2Fonem(m_sTeksInput);			
		UpdateData(FALSE);
	}
}

void CLearningDlg::OnSave() 
{
	UpdateData();
	if(!m_bSaving && !m_bLoading && !m_bTraining)
	{		
		m_pSaveThread=AfxBeginThread(SaveBobotThread,this,THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
		if (m_pSaveThread == NULL)
		{		
			MessageBox("Gagal dalam menjalankan thread penyimpanan bobot",MB_OK);
			return;
		}
		m_pSaveThread->m_bAutoDelete = TRUE;
		m_pSaveThread->ResumeThread();
		GetDlgItem(IDC_SAVE)->SetWindowText("STOP");
		m_bSaving = TRUE;
	}else
	{		
		m_pSPAW->Stop(TRUE);	
	}
}
UINT SaveBobotThread(LPVOID param)
{
	CLearningDlg * dlg = (CLearningDlg*)param;
	dlg->m_pSPAW->Stop(FALSE);
	dlg->m_pSPAW->SaveBackPro(&dlg->m_cProgLearning);
	dlg->GetDlgItem(IDC_SAVE)->SetWindowText("SAVE");
	PostMessage(dlg->m_hWnd,WM_SAVE_THREAD_FINISHED,0,0);
	AfxEndThread(0);
	return 0;
	
}



void CLearningDlg::OnDestroy() 
{
	/*
	if (m_pTrainingThread)
	{
		WaitForSingleObject(m_pTrainingThread->m_hThread, INFINITE);
		delete m_pTrainingThread;
		m_pTrainingThread = NULL;
	}
	if (m_pSaveThread)
	{
		WaitForSingleObject(m_pSaveThread->m_hThread, INFINITE);
		delete m_pSaveThread;
		m_pSaveThread = NULL;
	}
	*/
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CLearningDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT rect;
	GetClientRect(&rect);
	//set bk color
	HBITMAP hBack = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BACK));
	HDC hBackDC = CreateCompatibleDC(dc);
	SelectObject(hBackDC,hBack);
	HBRUSH hBrush = CreateSolidBrush(GetPixel(hBackDC,0,0));
	FillRect(dc,&rect,hBrush);
	DeleteObject(hBrush);
	DeleteDC(hBackDC);
}


void CLearningDlg::SetSPAW(CSPAW *pSPAW)
{
	m_pSPAW = pSPAW;
}

void CLearningDlg::SetStyle()
{
	m_cSave.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cSave.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cSave.FontStyle(_T("MS Sans Serif"));
	m_cLearn.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cLearn.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cLearn.FontStyle(_T("MS Sans Serif"));
	m_cProcess.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cProcess.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cProcess.FontStyle(_T("MS Sans Serif"));
	m_cTest.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cTest.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cTest.FontStyle(_T("MS Sans Serif"));
}

void CLearningDlg::OnTest() 
{
	m_pSPAW->Stop(FALSE);
	m_pSPAW->Test(&m_cStatus);
	
}
