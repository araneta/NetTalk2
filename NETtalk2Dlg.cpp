// NETtalk2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "NETtalk2.h"
#include "NETtalk2Dlg.h"

#include "Mmsystem.h"
#pragma comment( lib, "Winmm.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
const UINT WM_LOAD_THREAD_FINISHED = WM_APP + 3;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNETtalk2Dlg dialog

CNETtalk2Dlg::CNETtalk2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNETtalk2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNETtalk2Dlg)
	m_sText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNETtalk2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNETtalk2Dlg)
	DDX_Control(pDX, IDC_SHOWLOG, m_cShowLog);
	DDX_Control(pDX, IDC_RELOG, m_cLog);
	DDX_Control(pDX, IDC_LOAD, m_cLoad);
	DDX_Control(pDX, IDC_RICHEDIT1, m_cText);
	DDX_Control(pDX, IDC_CLEAR, m_cClear);
	DDX_Control(pDX, IDC_SPEAK, m_cSpeak);
	DDX_Control(pDX, IDC_STOP, m_cStop);
	DDX_Text(pDX, IDC_RICHEDIT1, m_sText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNETtalk2Dlg, CDialog)
	//{{AFX_MSG_MAP(CNETtalk2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SPEAK, OnSpeak)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SHOWLOG, OnShowlog)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LOAD_THREAD_FINISHED,OnThreadLoadFinisihed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNETtalk2Dlg message handlers

BOOL CNETtalk2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_bSpeaking = FALSE;
	
	SetStyle();

	m_playkata = new PLAYKATA;
	InitializeCriticalSection (&m_playkata->guard);
	m_NatLang->SetSharedVar(m_playkata);
	m_NatLang->SetStatusCtrl(&m_cText,&m_cLog);

	m_pLoadThread = AfxBeginThread(LoadBobot,this,THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
	if (m_pLoadThread == NULL)
	{		
		MessageBox("Gagal dalam menjalankan thread load bobot",MB_OK);
		return TRUE;
	}
	m_pLoadThread->m_bAutoDelete = TRUE;
	m_pLoadThread->ResumeThread();
	m_bLoading = TRUE;

	GetDlgItem(IDC_SPEAK)->EnableWindow(FALSE);		
	m_cLog.ShowWindow(SW_HIDE);

	//m_pRichEdit = &m_cText;
	//initialize direct music 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNETtalk2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNETtalk2Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
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
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNETtalk2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNETtalk2Dlg::OnOK() 
{
	
	
}
LRESULT CNETtalk2Dlg::OnThreadLoadFinisihed(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_SPEAK)->EnableWindow(TRUE);
	m_bLoading = FALSE;
	return 0L;
}
void CNETtalk2Dlg::SetConnDB(CADODatabase *db)
{
	m_pDb = db;	
}

void CNETtalk2Dlg::OnSpeak() 
{
	UpdateData();
	m_NatLang->ReloadData();
	if(!m_bSpeaking && !m_bLoading)
	{		
		m_playkata->m_vKata.clear();
		
		m_playkata->ready = 0;	m_playkata->processed = 0;
		m_playkata->finish = 0;	m_playkata->unprocessed =0;	
		m_playkata->sFolderPath = m_NatLang->m_sFolderPath;
		m_playkata->stop = 0;
		m_pParsingThread = AfxBeginThread(Parsing,this,THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
		if (m_pParsingThread == NULL)
		{		
			MessageBox("Gagal dalam menjalankan thread parsing",MB_OK);
			return;
		}
		m_pParsingThread->m_bAutoDelete = TRUE;
		
		m_pSpeakingThread = AfxBeginThread(Speaking,this,THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
		if (m_pSpeakingThread == NULL)
		{		
			MessageBox("Gagal dalam menjalankan thread speaking",MB_OK);
			return;
		}
		m_pSpeakingThread->m_bAutoDelete = TRUE;		
		
		m_pParsingThread->ResumeThread();		
		m_pSpeakingThread->ResumeThread();
		
		m_bSpeaking = TRUE;
	}	
}
UINT Parsing(LPVOID param)
{
	CNETtalk2Dlg * dlg = (CNETtalk2Dlg *)param;
	dlg->m_NatLang->SetText(dlg->m_sText);	
	dlg->m_NatLang->Run();
	
	AfxEndThread(0);
	return 0;
}
UINT Speaking(LPVOID param)
{	
	CNETtalk2Dlg * dlg = (CNETtalk2Dlg *)param;
	int finish=0,unprocessed=-1;	
	KATA kata;
	Sleep(2000);
	dlg->m_cSpeak.EnableWindow(FALSE);
	dlg->m_cLoad.EnableWindow(FALSE);
	dlg->m_cClear.EnableWindow(FALSE);
	while(finish==0)
	{
		unprocessed = -1;
		
		EnterCriticalSection(&dlg->m_playkata->guard);			
		if((dlg->m_playkata->finish==1)&&(dlg->m_playkata->processed==dlg->m_playkata->unprocessed)||(dlg->m_playkata->stop==1))
		{
			finish = 1;
		}else
		{
			if(dlg->m_playkata->ready==1)
			{
				unprocessed = dlg->m_playkata->processed;				
				kata = dlg->m_playkata->m_vKata[unprocessed];				
			}			
		}
		LeaveCriticalSection(&dlg->m_playkata->guard);	
		if((unprocessed!=-1)&&(finish==0))
		{
			CString sFile;
			sFile.Format("%s\\hasil%d.wav",dlg->m_playkata->sFolderPath,unprocessed);								
			//m_pRichEdit->SetSel(kata.start,kata.end);
			dlg->m_cText.SetSel(kata.start,kata.end);
			HANDLE hFile = CreateFile(sFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile!=INVALID_HANDLE_VALUE)
			{
				CloseHandle(hFile);
				sndPlaySound(sFile,SND_FILENAME|SND_SYNC);				
			}
			
			EnterCriticalSection(&dlg->m_playkata->guard);	
			dlg->m_playkata->processed++;
			LeaveCriticalSection(&dlg->m_playkata->guard);	
		}
	}
	dlg->m_bSpeaking = FALSE;
	dlg->m_cSpeak.EnableWindow(TRUE);
	dlg->m_cClear.EnableWindow(TRUE);
	dlg->m_cLoad.EnableWindow(TRUE);
	AfxEndThread(0);	
	return 0;
	
}
void CNETtalk2Dlg::OnClear() 
{
	m_playkata->stop = 1;
	m_sText = "";
	UpdateData(FALSE);
}

void CNETtalk2Dlg::OnStop() 
{
	m_playkata->stop = 1;
}
UINT LoadBobot(LPVOID param)
{
	CNETtalk2Dlg * dlg = (CNETtalk2Dlg*)param;
	dlg->LoadPembelajaran();	
	dlg->UpdateData(FALSE);
	PostMessage(dlg->m_hWnd,WM_LOAD_THREAD_FINISHED,0,0);
	PostMessage(dlg->m_hLearning,WM_LOAD_THREAD_FINISHED,0,0);

	AfxEndThread(0);
	return 0;
}

void CNETtalk2Dlg::SetNatLang(CNaturalLanguage *pNatLang)
{
	m_NatLang = pNatLang;
}

void CNETtalk2Dlg::LoadPembelajaran()
{
	m_NatLang->LoadSPAW();
}

void CNETtalk2Dlg::SetStyle()
{
	m_cClear.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cClear.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cClear.FontStyle(_T("MS Sans Serif"));
	m_cSpeak.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cSpeak.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cSpeak.FontStyle(_T("MS Sans Serif"));
	m_cStop.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cStop.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cStop.FontStyle(_T("MS Sans Serif"));
	m_cLoad.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
	m_cLoad.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cLoad.FontStyle(_T("MS Sans Serif"));
	m_cShowLog.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
	m_cShowLog.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cShowLog.FontStyle(_T("MS Sans Serif"));
}

static DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CFile* pFile = (CFile*) dwCookie;

   *pcb = pFile->Read(pbBuff, cb);

   return 0;
}

void CNETtalk2Dlg::OnLoad() 
{
	char szFilters[]= "Rich Text Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";

	CFileDialog cf (TRUE, "rtf", "*.rtf", OFN_FILEMUSTEXIST, szFilters, this);
	if(cf.DoModal() == IDOK){				
		CString sFilePath = cf.m_ofn.lpstrFile;		
		CFile cFile(sFilePath, CFile::modeRead);
		EDITSTREAM es;		

		es.dwCookie = (DWORD) &cFile;		
		es.pfnCallback = MyStreamInCallback; 
		if(cf.GetFileExt()=="rtf")
			m_cText.StreamIn(SF_RTF, es);
		else
			m_cText.StreamIn(SF_TEXT, es);

	}	
}

void CNETtalk2Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DeleteCriticalSection (&m_playkata->guard);
	delete m_playkata;
	CDialog::OnClose();
}

void CNETtalk2Dlg::OnShowlog() 
{
	if(m_cLog.ShowWindow(SW_SHOW)!=0)
	{
		m_cText.ShowWindow(SW_SHOW);
		m_cLog.ShowWindow(SW_HIDE);
	}else
	{
		m_cText.ShowWindow(SW_HIDE);
		m_cLog.ShowWindow(SW_SHOW);
	}		
}
