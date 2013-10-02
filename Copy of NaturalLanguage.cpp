// NaturalLanguage.cpp: implementation of the CNaturalLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NETtalk2.h"
#include "NaturalLanguage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "LearningDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNaturalLanguage::CNaturalLanguage()
{

}

CNaturalLanguage::~CNaturalLanguage()
{

}

void CNaturalLanguage::SetText(CString sText)
{
	m_sText = sText;
	//jadi huruf kecil
	m_sText.MakeLower();
	m_speech->Init();
}

BOOL CNaturalLanguage::IsSingkatan(CString sText)
{	
	/*
	BOOL bResult = FALSE;
	vector<CString>::iterator iter;
	for(iter= m_vSingkatan.begin();iter!=m_vSingkatan.end();iter++)
	{
		if(*iter==sText){
			bResult = TRUE;
			break;
		}
	}
	*/
	CADORecordset * pRs = new CADORecordset(m_pDb);				
	CString sCmd;
	sCmd.Format("SELECT singkatan FROM TSingkatan WHERE singkatan = '%s'",sText);
	BOOL bResult = FALSE;
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		if(!pRs->IsEOF())
		{						
			bResult = TRUE;
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TSingkatan","NETtalk",MB_OK);
	}
	delete pRs;
	return bResult;
}

void CNaturalLanguage::SetConnDB(CADODatabase *db)
{
	m_pDb = db;		
	LoadTVokal();
	LoadTSimbol();
	LoadTGugusKonsonan();
}
void CNaturalLanguage::LoadTSimbol()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);		
	
	m_vSimbol.clear();
	CString sCmd;
	sCmd.Format("SELECT simbol FROM TSimbol");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{						
			CString simbol;
			pRs->GetFieldValue("simbol", simbol);						
			m_vSimbol.push_back(simbol);			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TSimbol","NETtalk",MB_OK);
	}
	delete pRs;
}
void CNaturalLanguage::LoadTSingkatan()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_vSingkatan.clear();
	CString sCmd;
	sCmd.Format("SELECT singkatan FROM TSingkatan");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{						
			CString sSingkatan;
			pRs->GetFieldValue("singkatan", sSingkatan);						
			m_vSingkatan.push_back(sSingkatan);			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TSingkatan","NETtalk",MB_OK);
	}
	delete pRs;
}


/************************************************************************/
/* bertugas sebagai alur utama dalam class CNaturalLanguage

/************************************************************************/
void CNaturalLanguage::Run()
{
	vector<KATA> vKata;
	vector<int> vJeda;
	
	int iPanjang = m_sText.GetLength();			
	if(iPanjang==0)
		return;
	
	CString sKataTemp="";
	
	BOOL bSingkatan=FALSE;	
	m_spaw->Stop(FALSE);
	int start=0;
	for(int i=0;i<iPanjang;i++)
	{	
		sKataTemp+=m_sText.GetAt(i);

		//kata : string yang dipisahkan oleh spasi atau enter atau akhir kalimat		
		if((m_sText.GetAt(i)=='\n')||(m_sText.GetAt(i)==' ')||(i==m_sText.GetLength()-1 )||TandaAkhirKalimat(m_sText.GetAt(i)))
		{						
			sKataTemp.TrimRight();						
			if(sKataTemp.GetLength() > 0)			
			{				

				//hapus tanda baca 								
				sKataTemp = HapusSimbol(sKataTemp);				
				//cek apakah ada dalam table pengecualian 
				sKataTemp = IsPengecualian(sKataTemp);																
		
				//pengecekan terhadap huruf 'e' atau 'o', jika ada maka pengenalan 
				//dengan 1-5 SPAW dan jaringan syaraf tiruan backpropagation
				CString sFonem;				
				vector<SUKUKATA> vSukuKata;				
				if(sKataTemp.FindOneOf("oe")!=-1)
				{
					sFonem = m_spaw->Text2Fonem(sKataTemp);											
					//sebelum dipenggal ganti gugus konsonan dengan karakter khusus
					sFonem = GantiGugusKonsonan(sFonem);									
					vSukuKata = ToSukuKata(sFonem);
				}else
				{						
					//sebelum dipenggal ganti gugus konsonan dengan karakter khusus
					sKataTemp = GantiGugusKonsonan(sKataTemp);									
					vSukuKata = ToSukuKata(sKataTemp);				
				}													
				//pengaturan jeda
				//jika spasi maka jeda pendek. jika (,) atau (:) maka jeda sedang. jika akhir kalimat jeda panjang  
				// # untuk jeda pendek, ## untuk jeda sedang, ### untuk jeda panjang
				//jeda pendek				
				SUKUKATA sukukata;
				if(m_sText.GetAt(i)==' ')
				{					
					sukukata.sSukuKata="#";
					vSukuKata.push_back(sukukata);
				}
				//jeda sedang 
				else if(m_sText.GetAt(i)==','||m_sText.GetAt(i)==':')
				{
					sukukata.sSukuKata="##";
					vSukuKata.push_back(sukukata);
				}
				
				if(vSukuKata.size()>0)
				{
					KATA kata;

					kata.sKata = sKataTemp;
					kata.vSukuKata = vSukuKata;
					kata.start = start;																					
					kata.end = i;
					start = i+1;										
					
					//vKata.push_back(kata);
				}				
			}
			sKataTemp = "";
		}

		//kalimat : string yang diakhiri dengan (.) atau (?) atau (!) atau \n
		//dan jika kata terakhir sebelum (.) bukan singkatan

		//pengecekan apakah akhir kalimat					
		if(TandaAkhirKalimat(m_sText.GetAt(i)))
		{			
			//ambil kata terakhir (sebelum spasi) dari satu kalimat				
			if(vKata.size()>0)
			{				
				CString sKataTerakhir = vKata[vKata.size()-1].sKata;					
				bSingkatan = IsSingkatan(sKataTerakhir);										
				if(!bSingkatan)
				{																
					//pengaturan jeda panjang, jeda antar kalimat				
					SUKUKATA sukukata;
					sukukata.sSukuKata = "###";				
					vKata[vKata.size()-1].vSukuKata.push_back(sukukata);
				
					KALIMAT kalimat;					
					kalimat.iTipeKalimat = TipeKalimat(m_sText.GetAt(i));
					kalimat.vKata = vKata;
					m_speech->Play(kalimat);					
					vKata.clear();				
				}										
			}					
		}		
	}
}

int CNaturalLanguage::TipeKalimat(char lastsimbol)
{
	if(lastsimbol=='.')
		return TipeKalimat::Berita;
	else if(lastsimbol=='?')
		return TipeKalimat::Tanya;
	else if(lastsimbol=='!')
		return TipeKalimat::Perintah;
	return TipeKalimat::Berita;
}


CString CNaturalLanguage::IsPengecualian(CString sText)
{	
	/*
	vector<KECUALI>::iterator iter;		
	for (iter=m_vPengecualian.begin();iter!=m_vPengecualian.end();iter++)
	{
		KECUALI kecuali = *iter;
		//if(sText.GetLength()==kecuali.sKata.GetLength())
		sText.Replace(kecuali.sKata,kecuali.sPengucapan);	
	}
	*/
	CADORecordset * pRs = new CADORecordset(m_pDb);				
	CString sCmd,sKata,sPengecualian;
	sCmd.Format("SELECT kata,pemenggalan FROM TPengecualian WHERE kata = '%s'",sText);			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		if(!pRs->IsEOF())
		{															
			pRs->GetFieldValue("pemenggalan", sPengecualian);													
			pRs->GetFieldValue("kata", sKata);													
			sText.Replace(sKata,sPengecualian);
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TPengecualian","NETtalk",MB_OK);
	}
	delete pRs;
	return sText;
}

vector<SUKUKATA> CNaturalLanguage::ToSukuKata(CString sText)
{	
	//DFSA tingkat pertama
	int currstate=0;
	vector<SUKUKATA> vSukukata,vSukukata2,vSukukata3;
	vector<SUKUKATA>::iterator iter;
	CString sSukuTemp="",sHasil;
	SUKUKATA sukukata,hasil;
	for (int i=0;i<sText.GetLength();i++)
	{
		if(!IsSimbol(sText.GetAt(i)))
		{
			currstate = FirstDFSA(currstate,sText.GetAt(i));					
			if(currstate==0)
			{					
				sukukata.sSukuKata = sSukuTemp;											
				vSukukata.push_back(sukukata);						
				sSukuTemp="";
				currstate = FirstDFSA(currstate,sText.GetAt(i));								
			}
			sSukuTemp += sText.GetAt(i);					
		}
	}	
	if (sSukuTemp.GetLength()>0)
	{			
		sukukata.sSukuKata = sSukuTemp;		
		vSukukata.push_back(sukukata);
	}
	

	//DFSA tingkat kedua
	sSukuTemp="";currstate=0;	
	for(iter=vSukukata.begin();iter!=vSukukata.end();iter++)
	{
		hasil = *iter;				
		currstate = SecondDFSA(currstate,hasil.sSukuKata);					
		if(currstate==0)
		{								
			sukukata.sSukuKata = sSukuTemp;				
			vSukukata2.push_back(sukukata);						
			sSukuTemp="";
			currstate = SecondDFSA(currstate,hasil.sSukuKata);			
		}
		sSukuTemp += hasil.sSukuKata;			
	}
	if (sSukuTemp.GetLength()>0)
	{
		sukukata.sSukuKata = sSukuTemp;			
		vSukukata2.push_back(sukukata);
	}

	//DFSA tingkat ketiga
	sSukuTemp="";currstate=0;	
	for(iter=vSukukata2.begin();iter!=vSukukata2.end();iter++)
	{
		hasil = *iter;
		currstate = ThirdDFSA(currstate,hasil.sSukuKata);					
		if(currstate==0)
		{				
			sukukata.sSukuKata = sSukuTemp;							
			vSukukata3.push_back(sukukata);						
			sSukuTemp="";
			currstate = ThirdDFSA(currstate,hasil.sSukuKata);		
		}
		sSukuTemp += hasil.sSukuKata;	
	}
	if (sSukuTemp.GetLength()>0)
	{						
		sukukata.sSukuKata = sSukuTemp;					
		vSukukata3.push_back(sukukata);
	}
	return vSukukata3;
	
}

BOOL CNaturalLanguage::IsVokal(char c)
{
	vector<CString>::iterator iter;
	for (iter=m_vVokal.begin();iter!=m_vVokal.end();iter++)
	{
		CString xc = *iter;		
		if(c==xc.GetAt(0))
			return TRUE;
	}	
	return FALSE;
}

int CNaturalLanguage::FirstDFSA(int from, char c)
{
	if(from==0 && c==' ')
		return 1;
	if(from==1 && c==' ')
		return 1;
	if(from==0 && IsVokal(c))
		return 2;
	if(from==0 && c=='n')
		return 3;
	if(from==0 && c=='k')
		return 4;
	if(from==0 && c=='s')
		return 5;
	if(from==0 && c!='n' && c!='k' && c!='s' && !IsVokal(c))
		return 7;
	if(from==3 && (c=='g'||c=='y'))
		return 6;
	if(from==3 && IsVokal(c))
		return 8;
	if(from==4 && c=='h')
		return 6;
	if(from==4 && IsVokal(c))
		return 8;
	if(from==5 && c=='y')
		return 6;
	if(from==5 && IsVokal(c))
		return 8;
	if(from==7 && IsVokal(c))
		return 8;
	//ilegal move
	return 0;
}		

int CNaturalLanguage::SecondDFSA(int from,CString sText)
{	
	if(from==0 && sText.GetAt(0)==' ' && sText.GetLength()==1)
		return 1;
	if(from==1 && sText.GetAt(0)==' ' && sText.GetLength()==1)
		return 1;
	if(from==0 && IsVokal(sText.GetAt(0)) && sText.GetLength()==1)
		return 2;
	if(from==0 && !IsVokal(sText.GetAt(0)) && IsVokal(sText.GetAt(1))&& sText.GetLength()==2)
		return 7;
	if(from==0 && !IsVokal(sText.GetAt(0))&& sText.GetLength()==1)
		return 4;			
	if(from==2 && !IsVokal(sText.GetAt(0))&& sText.GetLength()==1)
		return 3;
	if(from==4 && !IsVokal(sText.GetAt(0)) && IsVokal(sText.GetAt(1))&& sText.GetLength()==2)
		return 5;
	if(from==4 && !IsVokal(sText.GetAt(0))&& sText.GetLength()==1)
		return 4;	
	if(from==7 && !IsVokal(sText.GetAt(0))&& sText.GetLength()==1)
		return 8;
	if(from==5 && !IsVokal(sText.GetAt(0))&& sText.GetLength()==1)
		return 6;
	//ilegal move
	return 0;
}

int CNaturalLanguage::ThirdDFSA(int from, CString sText)
{
	if(from==0 && sText.GetAt(0)==' ' && sText.GetLength()==1)
		return 1;
	if(from==1 && sText.GetAt(0)==' ' && sText.GetLength()==1)
		return 1;
	if(from==0 && IsVokal(sText.GetAt(0)) && !IsVokal(sText.GetAt(1)) && sText.GetLength()==2)
		return 2;
	if(from==0 && !IsVokal(sText.GetAt(0)) && IsVokal(sText.GetAt(1)) && !IsVokal(sText.GetAt(2)) && sText.GetLength()==3)
		return 4;
	if(from==0 && !IsVokal(sText.GetAt(0)) && !IsVokal(sText.GetAt(1)) && IsVokal(sText.GetAt(2)) && !IsVokal(sText.GetAt(3)) && sText.GetLength()==4)
		return 6;
	if(from==0 && IsVokal(sText.GetAt(0)) && sText.GetLength()==1)
		return 8;
	if(from==0 && !IsVokal(sText.GetAt(0)) && IsVokal(sText.GetAt(1)) && sText.GetLength()==2)
		return 8;
	if(from	==0 && (!IsVokal(sText.GetAt(0)) && !IsVokal(sText.GetAt(1)) && IsVokal(sText.GetAt(2)) && sText.GetLength()==3) || (!IsVokal(sText.GetAt(0)) && !IsVokal(sText.GetAt(1)) && !IsVokal(sText.GetAt(2))  && IsVokal(sText.GetAt(3)) && sText.GetLength()==4))
		return 11;
	if(from==2 && !IsVokal(sText.GetAt(0)) && sText.GetLength()==1)
		return 3;
	if(from==4 && !IsVokal(sText.GetAt(0)) && sText.GetLength()==1)
		return 5;
	if(from==6 && !IsVokal(sText.GetAt(0)) && sText.GetLength()==1)
		return 7;
	if(from==9 && IsVokal(sText.GetAt(0)) && sText.GetLength()==1)
		return 10;	
	if(from==11 && IsVokal(sText.GetAt(0)) && sText.GetLength()==1)
		return 12;	
	//ilegal move
	return 0;
}

void CNaturalLanguage::SetSPAW(CSPAW *pSpaw)
{
	m_spaw = pSpaw;
}

void CNaturalLanguage::LoadSPAW()
{
	m_spaw->LoadBackPro();
}

void CNaturalLanguage::LoadTVokal()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_vVokal.clear();
	CString sCmd;
	sCmd.Format("SELECT vokal FROM TVokal");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{						
			CString vokal;
			pRs->GetFieldValue("vokal", vokal);						
			m_vVokal.push_back(vokal);			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TVokal","NETtalk",MB_OK);
	}
	delete pRs;
}

BOOL CNaturalLanguage::IsSimbol(char c)
{
	vector<CString>::iterator iter;
	for (iter=m_vSimbol.begin();iter!=m_vSimbol.end();iter++)
	{
		CString xc = *iter;		
		if(c==xc.GetAt(0))
			return TRUE;
	}
	
	return FALSE;
}


BOOL CNaturalLanguage::TandaAkhirKalimat(char c)
{
	if (c== '.'||c== '?'||c== '!' ||c=='\n'||c=='\0')	
	{
		return TRUE;
	}
	return FALSE;
}	

void CNaturalLanguage::SetSpeech(CSpeech *pSpeech)
{
	m_speech = pSpeech;
}

void CNaturalLanguage::LoadTGugusKonsonan()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_vGugusKonsonan.clear();
	CString sCmd;
	sCmd.Format("SELECT gugus, fonem FROM TGugusKonsonan");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{						
			GUGUSKONSONAN Gugus;
			pRs->GetFieldValue("gugus", Gugus.sGugus);						
			pRs->GetFieldValue("fonem", Gugus.sFonem);						
			m_vGugusKonsonan.push_back(Gugus);			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TGugus","NETtalk",MB_OK);
	}
	delete pRs;
}

CString CNaturalLanguage::GantiGugusKonsonan(CString sText)
{
	CString sTemp = sText;
	vector<GUGUSKONSONAN>::iterator iter;
	for (iter=m_vGugusKonsonan.begin();iter!=m_vGugusKonsonan.end();iter++)
	{
		GUGUSKONSONAN Gugus = (GUGUSKONSONAN)*iter;				
		sTemp.Replace(Gugus.sGugus,Gugus.sFonem);
	}
	return sTemp;
}

CString CNaturalLanguage::HapusSimbol(CString sSimbol)
{
	CString sTemp = sSimbol;	
	vector<CString>::iterator iter;		
	for (iter=m_vSimbol.begin();iter!=m_vSimbol.end();iter++)
	{
		CString xc = *iter;					
		sTemp.Remove(xc.GetAt(0));				
	}
	return sTemp;

}

CString CNaturalLanguage::KembalikanGugusKonsonan(CString sText)
{
	CString sTemp = sText;
	vector<GUGUSKONSONAN>::iterator iter;
	for (iter=m_vGugusKonsonan.begin();iter!=m_vGugusKonsonan.end();iter++)
	{
		GUGUSKONSONAN Gugus = (GUGUSKONSONAN)*iter;				
		sTemp.Replace(Gugus.sFonem,Gugus.sGugus);
	}
	return sTemp;
}
void CNaturalLanguage::AddKata(CString sKata)
{
	CADORecordset * pRs = new CADORecordset(m_pDb);					
	CString sCmd;	
	sCmd.Format("SELECT * FROM TKata WHERE Kata = '%s'",sKata);		
	BOOL bExist = FALSE;
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{
		if(pRs->GetRecordCount()>0){
			bExist = TRUE;
		}		
		pRs->Close();
	}

	if(pRs->Open("TKata", CADORecordset::openTable) && !bExist)		
	{			
		pRs->AddNew();							
		pRs->SetFieldValue(0, sKata);
		pRs->SetFieldValue(1, sKata);
		pRs->Update();
		pRs->Close();				
	}					
}
