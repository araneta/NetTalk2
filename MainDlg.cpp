// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NETtalk2.h"
#include "MainDlg.h"
#include "SHLWAPI.H"	// for the path functions
#pragma comment( lib, "shlwapi.lib" )
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_pPrevWnd = NULL;
	//}}AFX_DATA_INIT
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_PLAY, m_cPlay);
	DDX_Control(pDX, IDC_CLOSE, m_cClose);
	DDX_Control(pDX, IDC_MIN, m_cMin);
	DDX_Control(pDX, IDC_STCAPTION, m_cstCaption);
	DDX_Control(pDX, IDC_KATA, m_cKata);
	DDX_Control(pDX, IDC_SINGKATAN, m_cSingkatan);
	DDX_Control(pDX, IDC_PENGECUALIAN, m_cPengecualian);
	DDX_Control(pDX, IDC_PENGUCAPAN, m_cPengucapan);
	DDX_Control(pDX, IDC_PEMBELAJARAN, m_cPembelajaran);
	DDX_Control(pDX, IDC_MAINPANEL, m_cMainPanel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PEMBELAJARAN, OnPembelajaran)
	ON_BN_CLICKED(IDC_PENGUCAPAN, OnPengucapan)
	ON_BN_CLICKED(IDC_KATA, OnKata)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PENGECUALIAN, OnPengecualian)
	ON_BN_CLICKED(IDC_SINGKATAN, OnSingkatan)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_MIN, OnMin)
	ON_BN_CLICKED(IDC_CLOSE, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers
void CMainDlg::SetFolderPath()
{
	m_sFolderPath.Empty();	
	::GetModuleFileName( NULL, m_sFolderPath.GetBuffer(255), 255 );
	::PathMakePretty( m_sFolderPath.GetBuffer(255) );
	::PathRemoveFileSpec( m_sFolderPath.GetBuffer(255) );
	m_sFolderPath.ReleaseBuffer();
}

void CMainDlg::SetActiveDialog(CWnd *pWnd)
{
	if(m_pPrevWnd!=NULL)
	{
		m_pPrevWnd->ShowWindow( SW_HIDE );
	}
	CRect rect;
	m_cMainPanel.GetWindowRect(rect);
	pWnd->SetWindowPos(NULL, 0,0,rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW );
	m_pPrevWnd = pWnd;
}


BOOL CMainDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();		
	SetFolderPath();
	//skin
	CString sSkinPath;
	sSkinPath.Format("%s\\skin.bmp",m_sFolderPath);
	m_hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(),sSkinPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_bFirst = TRUE;
	//inisialisasi database
	CString sDBPath;
	
	m_pDb = new CADODatabase();
	sDBPath.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\\db\\tts.mdb;Persist Security Info=False",m_sFolderPath);		

	if(m_pDb->Open(sDBPath))
	{			
		//inisialiasi backpropagation
		m_backpro.SetConnDB(m_pDb);		
		//inisialiasi second positioning asymetric windowing
		m_spaw.SetConnDB(m_pDb);
		m_spaw.SetBackPro(&m_backpro);
		
		m_spaw.Initialize(WINDOWSIZE,CENTERWINDOWPOS,4000,0.01, 0.01,4000);
		
		//inisialisasi speech
	
		//inisialisai natural language
		m_NatLang.SetConnDB(m_pDb);
		m_NatLang.m_sFolderPath = m_sFolderPath;
		m_NatLang.SetSPAW(&m_spaw);	

		//inisialisasi tiap dialog di memory		
		m_pLearningDlg = new CLearningDlg;	
		m_pLearningDlg->SetSPAW(&m_spaw);
		m_pLearningDlg->Create( IDD_LEARNING_DIALOG, &m_cMainPanel);
		m_pLearningDlg->ShowWindow(SW_HIDE);
					
		m_pKataDlg = new CKataDlg;
		m_pKataDlg->SetSPAW(&m_spaw);
		m_pKataDlg->SetDB(m_pDb);
		m_pKataDlg->Create( IDD_DATA_KATA, &m_cMainPanel);
		m_pKataDlg->ShowWindow(SW_HIDE);
		
		m_pPengecualianDlg = new CPengecualianDlg;
		m_pPengecualianDlg->SetDB(m_pDb);
		m_pPengecualianDlg->Create(IDD_DATA_PENGECUALIAN, &m_cMainPanel);
		m_pPengecualianDlg->ShowWindow(SW_HIDE);

		m_pSingkatanDlg = new CSingkatanDlg;
		m_pSingkatanDlg->SetDB(m_pDb);
		m_pSingkatanDlg->Create(IDD_DATA_SINGKATAN,&m_cMainPanel);
		m_pSingkatanDlg->ShowWindow(SW_HIDE);

		m_pPengucapanDlg = new CNETtalk2Dlg;
		m_pPengucapanDlg->SetNatLang(&m_NatLang);
		m_pPengucapanDlg->SetConnDB(m_pDb);
		m_pPengucapanDlg->Create(IDD_NETTALK2_DIALOG,&m_cMainPanel);
		m_pPengucapanDlg->ShowWindow(SW_SHOW);
		m_pPengucapanDlg->m_hLearning = m_pLearningDlg->m_hWnd;
		m_pPengucapanDlg->m_sFolderpath = m_sFolderPath;
		SetActiveDialog(m_pPengucapanDlg);
	
		SetStyle();
	}
	else
	{
		MessageBox("Error dalam membuka database","NETtalk2",MB_OK);	
		EndDialog(0);
	}
	
	SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{		

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		
		//CDialog::OnPaint();
					
		RECT rect;
		GetClientRect(&rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;				
		COLORREF colortrans = RGB(0,255,0);
		HBRUSH hBrush = CreateSolidBrush(colortrans);
		FillRect(dc,&rect,hBrush);
		DeleteObject(hBrush);		
				
		BITMAP bmp;
		GetObject(m_hBmp,sizeof(bmp),&bmp);
		HDC hBmpDC = CreateCompatibleDC(dc);
		SelectObject(hBmpDC,m_hBmp);
		
		TransparentBlt(dc,0,0,bmp.bmWidth, bmp.bmHeight,hBmpDC,0,0,bmp.bmWidth,bmp.bmHeight,RGB(0,255,0));
		DeleteDC(hBmpDC);			
							
		if(m_bFirst){						
			HRGN rgn = GetTrans(dc,GetPixel(dc,0,0),0,0,bmp.bmWidth, bmp.bmHeight);	
			::SetWindowRgn(m_hWnd,rgn,TRUE);						
			m_bFirst = FALSE;								
		}
		
	}
}
HRGN CMainDlg::GetTrans(HDC _hDC, COLORREF color, int _x, int _y, int iWidth, int iHeight)
{
	HRGN TempRGN;
	HRGN TransRGN;
	BOOL bFirstCheck=FALSE;
	BOOL bInFirstRGN=TRUE;
		
	int x,y,iXNow=0;
	for(y=_y;y<=iHeight;y++){
		for(x=_x;x<=iWidth;x++){
			if(GetPixel(_hDC,x,y)==color||x==iWidth){ // check whether it has the same color
				if(bFirstCheck){
					bFirstCheck=FALSE;
					TempRGN=CreateRectRgn(iXNow, y, x, y+1); //Create RectRGN from the previous x (iXNow) if it has the same color
					if (bInFirstRGN){  //Get First RGN and we combine it with other RGN which have the same color
						TransRGN = TempRGN;
						bInFirstRGN = FALSE;
						
					}
					else{
						CombineRgn(TransRGN, TransRGN, TempRGN, RGN_OR);
						DeleteObject(TempRGN);
					}
				}
			}else{
				if(bFirstCheck==FALSE){
                   bFirstCheck = TRUE;
                   iXNow = x; //get previous x
				}
			}
		}
	}
	return TransRGN;
}
void CMainDlg::OnPengecualian() 
{
	SetActiveDialog(m_pPengecualianDlg);
	SetSelected(m_cPengecualian.GetDlgCtrlID());	
}

void CMainDlg::OnSingkatan() 
{
	SetActiveDialog(m_pSingkatanDlg);
	SetSelected(m_cSingkatan.GetDlgCtrlID());	
}
void CMainDlg::OnPembelajaran() 
{
	SetActiveDialog(m_pLearningDlg);
	SetSelected(m_cPembelajaran.GetDlgCtrlID());
}

void CMainDlg::OnPengucapan() 
{
	SetActiveDialog(m_pPengucapanDlg);
	SetSelected(m_cPengucapan.GetDlgCtrlID());
}

void CMainDlg::OnKata() 
{
	SetActiveDialog(m_pKataDlg)	;
	SetSelected(m_cKata.GetDlgCtrlID());
}

void CMainDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pDb->IsOpen())
	{
		m_pDb->Close();
	}
	delete m_pDb;
	CDialog::OnClose();
}


void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION);	
	CDialog::OnLButtonDown(nFlags, point);
}

void CMainDlg::SetStyle()
{
	m_cPembelajaran.SetTransparent(IDB_TAB, 0, 0, 100, 69, 100, 33);
    m_cPembelajaran.SetTextColor(RGB(0,220,0), RGB(255,255,255));
	m_cPembelajaran.FontStyle(_T("MS Sans Serif"));
	m_cPembelajaran.SetTab(TRUE);
	m_cKata.SetTransparent(IDB_TAB, 0, 0, 100, 69, 100, 33);
    m_cKata.SetTextColor(RGB(0,220,0), RGB(255,255,255));
	m_cKata.FontStyle(_T("MS Sans Serif"));
	m_cKata.SetTab(TRUE);
	m_cPengecualian.SetTransparent(IDB_TAB, 0, 0, 100, 69, 100, 33);
    m_cPengecualian.SetTextColor(RGB(0,220,0), RGB(255,255,255));
	m_cPengecualian.FontStyle(_T("MS Sans Serif"));
	m_cPengecualian.SetTab(TRUE);
	m_cSingkatan.SetTransparent(IDB_TAB, 0, 0, 100, 69, 100, 33);
    m_cSingkatan.SetTextColor(RGB(0,220,0), RGB(255,255,255));
	m_cSingkatan.FontStyle(_T("MS Sans Serif"));
	m_cSingkatan.SetTab(TRUE);
	m_cPengucapan.SetTransparent(IDB_TAB, 0, 0, 100, 69, 100, 33);
    m_cPengucapan.SetTextColor(RGB(0,220,0), RGB(255,255,255));
	m_cPengucapan.FontStyle(_T("MS Sans Serif"));
	m_cPengucapan.SetTab(TRUE);
	m_cstCaption.SetColor(RGB(255,255,255));

	m_cMin.SetTransparent(IDB_MIN, 0, 0, 0, 0, 30, 23);
	m_cClose.SetTransparent(IDB_CLOSE, 0, 0, 0, 0, 47, 23);	
	m_cPlay.SetTransparent(IDB_PLAY, 0, 0, 80, 98, 47, 48);
//	m_cstIcon.SetWindowPos(&CWnd::wndTop,0,0,16,16,SWP_NOMOVE);
}

void CMainDlg::SetSelected(int ctrlid)
{
	m_cPembelajaran.SetSelected(ctrlid);
	m_cKata.SetSelected(ctrlid);
    m_cPengecualian.SetSelected(ctrlid);
    m_cSingkatan.SetSelected(ctrlid);
    m_cPengucapan.SetSelected(ctrlid);    
}

void CMainDlg::OnMin() 
{	
	CloseWindow();	
}

void CMainDlg::OnExit() 
{
	EndDialog(0);
}
