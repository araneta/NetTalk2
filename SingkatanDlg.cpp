// SingkatanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nettalk2.h"
#include "SingkatanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSingkatanDlg dialog


CSingkatanDlg::CSingkatanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSingkatanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSingkatanDlg)
	m_sSingkatan = _T("");
	//}}AFX_DATA_INIT
}


void CSingkatanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSingkatanDlg)
	DDX_Control(pDX, IDC_STSINGKATAN, m_cstSingkatan);
	DDX_Control(pDX, IDC_STLIST, m_cstList);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_TAMBAH, m_cTambah);
	DDX_Control(pDX, IDC_DAFTARSINGKATAN, m_cListSingkatan);
	DDX_Text(pDX, IDC_SINGKATAN, m_sSingkatan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSingkatanDlg, CDialog)
	//{{AFX_MSG_MAP(CSingkatanDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_TAMBAH, OnTambah)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSingkatanDlg message handlers

void CSingkatanDlg::LoadSingkatan()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_cListSingkatan.DeleteAllItems();
	CString sCmd;
	sCmd.Format("SELECT singkatan FROM TSingkatan");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{												
			CString sSingkatan;
			pRs->GetFieldValue("Singkatan",sSingkatan);									
			int i = m_cListSingkatan.InsertItem(0,sSingkatan);			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox("Error on opening table TSingkatan","NETtalk2",MB_OK);
	}
	delete pRs;
}

void CSingkatanDlg::SetDB(CADODatabase *pDb)
{
	m_pDb = pDb;
}

BOOL CSingkatanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetStyle();			
	m_cListSingkatan.InsertColumn(0,"Singkatan",LVCFMT_LEFT,130);
	LoadSingkatan();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSingkatanDlg::OnDelete() 
{
	int i = m_cListSingkatan.GetSelectionMark();
	if(i==-1)
		return;

	CString sKata;
	sKata = m_cListSingkatan.GetItemText(i,0);
	CString sCmd;
	sCmd.Format("DELETE FROM TSingkatan WHERE Singkatan = '%s'",sKata);
	m_pDb->Execute(sCmd);	
	LoadSingkatan();
	
}

void CSingkatanDlg::OnTambah() 
{
	UpdateData();
	if(m_sSingkatan.GetLength()==0)
		return;
	CADORecordset * pRs = new CADORecordset(m_pDb);
	CString sCmd;	
	sCmd.Format("SELECT * FROM TSingkatan WHERE Singkatan = '%s'",m_sSingkatan);		
	BOOL bExist = FALSE;
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{
		if(pRs->GetRecordCount()>0)
			bExist = TRUE;
		pRs->Close();
	}
	
	if(bExist)
	{
		sCmd.Format("UPDATE TSingkatan SET Singkatan = '%s' WHERE Singkatan = '%s'",m_sSingkatan,m_sSingkatan);
		m_pDb->Execute(sCmd.GetBuffer(0));
	}else
	{							
		if(pRs->Open("TPengecualian", CADORecordset::openTable))		
		{							
			pRs->AddNew();					
			pRs->SetFieldValue(0, m_sSingkatan);						
			pRs->Update();
			pRs->Close();					
		}	
	}	
	delete pRs;
	m_cListSingkatan.DeleteAllItems();	
	LoadSingkatan();
	
}

void CSingkatanDlg::OnPaint() 
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

void CSingkatanDlg::SetStyle()
{
	m_cDelete.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cDelete.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cDelete.FontStyle(_T("MS Sans Serif"));
	m_cTambah.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cTambah.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cTambah.FontStyle(_T("MS Sans Serif"));
	
}
