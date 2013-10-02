// PengecualianDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nettalk2.h"
#include "PengecualianDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPengecualianDlg dialog


CPengecualianDlg::CPengecualianDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPengecualianDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPengecualianDlg)
	m_sPengucapan = _T("");
	m_sKata = _T("");
	//}}AFX_DATA_INIT
}


void CPengecualianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPengecualianDlg)
	DDX_Control(pDX, IDC_STPENGUCAPAN, m_cstPengucapan);
	DDX_Control(pDX, IDC_STLIST, m_cstList);
	DDX_Control(pDX, IDC_STKATA, m_cstKata);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_TAMBAH, m_cTambah);
	DDX_Control(pDX, IDC_SIMBOL3, m_cSimbol3);
	DDX_Control(pDX, IDC_SIMBOL2, m_cSimbol2);
	DDX_Control(pDX, IDC_SIMBOL1, m_cSimbol1);
	DDX_Control(pDX, IDC_DAFTARPENGECUALIAN, m_cListPengecualian);
	DDX_Text(pDX, IDC_PENGUCAPAN, m_sPengucapan);
	DDX_Text(pDX, IDC_KATA, m_sKata);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPengecualianDlg, CDialog)
	//{{AFX_MSG_MAP(CPengecualianDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_TAMBAH, OnTambah)
	ON_BN_CLICKED(IDC_SIMBOL1, OnSimbol1)
	ON_BN_CLICKED(IDC_SIMBOL2, OnSimbol2)
	ON_BN_CLICKED(IDC_SIMBOL3, OnSimbol3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DAFTARPENGECUALIAN, OnItemchangedDaftarpengecualian)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPengecualianDlg message handlers

void CPengecualianDlg::LoadPengecualian()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_cListPengecualian.DeleteAllItems();
	CString sCmd;
	sCmd.Format("SELECT kata, pengucapan FROM TPengecualian");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{												
			CString sPemenggalan,sKata;
			pRs->GetFieldValue("kata",sKata);						
			pRs->GetFieldValue("pengucapan", sPemenggalan);						
			int i = m_cListPengecualian.InsertItem(0,sKata);
			m_cListPengecualian.SetItemText(i,1,sPemenggalan);		
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox("Error on opening table TPengecualian","NETtalk2",MB_OK);
	}
	delete pRs;
}

BOOL CPengecualianDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetStyle();		
	m_cListPengecualian.InsertColumn(0,_T("Kata"),LVCFMT_LEFT,130);
	m_cListPengecualian.InsertColumn(1,_T("pengucapan"),LVCFMT_LEFT,130);
	m_cListPengecualian.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	LoadPengecualian();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPengecualianDlg::SetDB(CADODatabase *pDb)
{
	m_pDb = pDb;
}

void CPengecualianDlg::OnDelete() 
{
	int i = m_cListPengecualian.GetSelectionMark();
	if(i==-1)
		return;

	CString sKata;
	sKata = m_cListPengecualian.GetItemText(i,0);
	CString sCmd;
	sCmd.Format("DELETE FROM TPengecualian WHERE Kata = '%s'",sKata);
	m_pDb->Execute(sCmd);	
	LoadPengecualian();
	
}

void CPengecualianDlg::OnTambah() 
{
	UpdateData();
	if(m_sKata.GetLength()==0 || m_sPengucapan.GetLength()==0)
		return;
	CADORecordset * pRs = new CADORecordset(m_pDb);
	CString sCmd;	
	sCmd.Format("SELECT * FROM TPengecualian WHERE Kata = '%s'",m_sKata);		
	BOOL bExist = FALSE;
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{
		if(pRs->GetRecordCount()>0)
			bExist = TRUE;
		pRs->Close();
	}
	
	if(bExist)
	{
		sCmd.Format("UPDATE TPengecualian SET Pemenggalan = '%s' WHERE Kata = '%s'",m_sPengucapan,m_sKata);
		m_pDb->Execute(sCmd.GetBuffer(0));
	}else
	{							
		if(pRs->Open("TPengecualian", CADORecordset::openTable))		
		{							
			pRs->AddNew();					
			pRs->SetFieldValue(0, m_sKata);
			pRs->SetFieldValue(1, m_sPengucapan);
			
			pRs->Update();
			pRs->Close();					
		}	
	}	
	delete pRs;
	m_cListPengecualian.DeleteAllItems();	
	LoadPengecualian();
	
}

void CPengecualianDlg::OnSimbol1() 
{
	CString sBuffer,sNewText;
	GetDlgItem(IDC_PENGUCAPAN)->GetWindowText(sBuffer);	
	sNewText.Format("%sê",sBuffer);
	GetDlgItem(IDC_PENGUCAPAN)->SetWindowText(sNewText);	
	
}

void CPengecualianDlg::OnSimbol2() 
{
	CString sBuffer,sNewText;
	GetDlgItem(IDC_PENGUCAPAN)->GetWindowText(sBuffer);	
	sNewText.Format("%së",sBuffer);
	GetDlgItem(IDC_PENGUCAPAN)->SetWindowText(sNewText);		
}

void CPengecualianDlg::OnSimbol3() 
{
	CString sBuffer,sNewText;
	GetDlgItem(IDC_PENGUCAPAN)->GetWindowText(sBuffer);	
	sNewText.Format("%sö",sBuffer);
	GetDlgItem(IDC_PENGUCAPAN)->SetWindowText(sNewText);	
}



void CPengecualianDlg::OnItemchangedDaftarpengecualian(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int iIndex = pNMListView->iItem;
	if(iIndex==-1)
		return;
	CString sKata = m_cListPengecualian.GetItemText(iIndex,0);	
	CString sPengucapan = m_cListPengecualian.GetItemText(iIndex,1);	
	
	m_sKata = sKata;
	m_sPengucapan = sPengucapan;		
	
	UpdateData(FALSE);
	*pResult = 0;
}

void CPengecualianDlg::OnPaint() 
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

void CPengecualianDlg::SetStyle()
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
