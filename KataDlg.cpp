// KataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nettalk2.h"
#include "KataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKataDlg dialog


CKataDlg::CKataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKataDlg)
	m_sKata = _T("");
	m_sFonem = _T("");
	
	//}}AFX_DATA_INIT
}


void CKataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKataDlg)
	DDX_Control(pDX, IDC_STKATA, m_cstKata);
	DDX_Control(pDX, IDC_STLIST, m_cstList);
	DDX_Control(pDX, IDC_STFONEM, m_cstFonem);
	DDX_Control(pDX, IDC_TAMBAH, m_cTambah);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_SIMBOL3, m_cSimbol3);
	DDX_Control(pDX, IDC_SIMBOL2, m_cSimbol2);
	DDX_Control(pDX, IDC_SIMBOL1, m_cSimbol1);
	DDX_Control(pDX, IDC_DAFTARKATA, m_cListKata);
	DDX_Text(pDX, IDC_KATA, m_sKata);
	DDX_Text(pDX, IDC_FONEM, m_sFonem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKataDlg, CDialog)
	//{{AFX_MSG_MAP(CKataDlg)
	ON_BN_CLICKED(IDC_TAMBAH, OnTambah)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DAFTARKATA, OnItemchangedDaftarkata)
	ON_BN_CLICKED(IDC_SIMBOL1, OnSimbol1)
	ON_BN_CLICKED(IDC_SIMBOL2, OnSimbol2)
	ON_BN_CLICKED(IDC_SIMBOL3, OnSimbol3)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKataDlg message handlers

void CKataDlg::OnTambah() 
{
	UpdateData();
	if(m_sKata.GetLength()==0 || m_sFonem.GetLength()==0)
		return;
	CADORecordset * pRs = new CADORecordset(m_pDb);
	CString sCmd;	
	sCmd.Format("SELECT * FROM TKata WHERE Kata = '%s'",m_sKata);		
	BOOL bExist = FALSE;
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{
		if(pRs->GetRecordCount()>0)
			bExist = TRUE;
		pRs->Close();
	}
	
	if(bExist)
	{
		sCmd.Format("UPDATE TKata SET Fonem = '%s' WHERE Kata = '%s'",m_sFonem,m_sKata);
		m_pDb->Execute(sCmd.GetBuffer(0));
	}else
	{							
		if(pRs->Open("TKata", CADORecordset::openTable))		
		{							
			pRs->AddNew();					
			pRs->SetFieldValue(0, m_sKata);
			pRs->SetFieldValue(1, m_sFonem);
			
			pRs->Update();
			pRs->Close();					
		}	
	}	
	delete pRs;
	m_cListKata.DeleteAllItems();	
	LoadKata();
	
}

void CKataDlg::LoadKata()
{
	
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_cListKata.DeleteAllItems();
	CString sCmd;
	sCmd.Format("SELECT kata, fonem FROM TKata");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{												
			CString sFonem,sKata;
			pRs->GetFieldValue("kata",sKata);						
			pRs->GetFieldValue("fonem", sFonem);						
			int i = m_cListKata.InsertItem(0,sKata);
			m_cListKata.SetItemText(i,1,sFonem);		
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox("Error on opening table TKata","NETtalk",MB_OK);
	}
	delete pRs;
	
}

void CKataDlg::OnDelete() 
{
	int i = m_cListKata.GetSelectionMark();
	if(i==-1)
		return;

	CString sKata;
	sKata = m_cListKata.GetItemText(i,0);
	CString sCmd;
	sCmd.Format("DELETE FROM TKata WHERE Kata = '%s'",sKata);
	m_pDb->Execute(sCmd);	
	LoadKata();
}

BOOL CKataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: Add extra initialization here
	SetStyle();
	m_cListKata.InsertColumn(0,"Kata",LVCFMT_LEFT,130);
	m_cListKata.InsertColumn(1,"Fonem",LVCFMT_LEFT,130);
	m_cListKata.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	LoadKata();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKataDlg::OnItemchangedDaftarkata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iIndex = pNMListView->iItem;
	if(iIndex==-1)
		return;
	CString sKata = m_cListKata.GetItemText(iIndex,0);	
	CString sFonem = m_cListKata.GetItemText(iIndex,1);	
	
	m_sKata = sKata;
	m_sFonem = sFonem;		
	
	UpdateData(FALSE);
	*pResult = 0;
}

void CKataDlg::OnSimbol1() 
{
	CString sBuffer,sNewText;
	GetDlgItem(IDC_FONEM)->GetWindowText(sBuffer);	
	sNewText.Format("%sê",sBuffer);
	GetDlgItem(IDC_FONEM)->SetWindowText(sNewText);	
	
}

void CKataDlg::OnSimbol2() 
{
	CString sBuffer,sNewText;
	GetDlgItem(IDC_FONEM)->GetWindowText(sBuffer);	
	sNewText.Format("%së",sBuffer);
	GetDlgItem(IDC_FONEM)->SetWindowText(sNewText);		
}

void CKataDlg::OnSimbol3() 
{
	CString sBuffer,sNewText;
	GetDlgItem(IDC_FONEM)->GetWindowText(sBuffer);	
	sNewText.Format("%sö",sBuffer);
	GetDlgItem(IDC_FONEM)->SetWindowText(sNewText);	
}

void CKataDlg::SetSPAW(CSPAW *pSPAW)
{
	m_pSPAW = pSPAW;
}

void CKataDlg::SetDB(CADODatabase *pDb)
{
	m_pDb = pDb;
}

void CKataDlg::OnPaint() 
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

void CKataDlg::SetStyle()
{
	m_cDelete.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cDelete.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cDelete.FontStyle(_T("MS Sans Serif"));
	m_cTambah.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cTambah.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cTambah.FontStyle(_T("MS Sans Serif"));
	m_cSimbol1.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cSimbol1.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cSimbol1.FontStyle(_T("MS Sans Serif"));
	m_cSimbol2.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cSimbol2.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cSimbol2.FontStyle(_T("MS Sans Serif"));
	m_cSimbol3.SetTransparent(IDB_BUTTON, 6, 3, 36, 69, 85, 26);
    m_cSimbol3.SetTextColor(RGB(0,220,0), RGB(0,255,0));
	m_cSimbol3.FontStyle(_T("MS Sans Serif"));
}


