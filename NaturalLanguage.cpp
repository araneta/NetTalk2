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
	m_fLog.Open("NatLanglog.log",CFile::modeWrite|CFile::modeCreate);
}

CNaturalLanguage::~CNaturalLanguage()
{
	m_fLog.Close();
}

void CNaturalLanguage::SetText(CString sText)
{
	m_sText = sText;
	//jadi huruf kecil
	m_sText.MakeLower();	
}

BOOL CNaturalLanguage::IsSingkatan(CString sText)
{	

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
		MessageBox(NULL,"Error on opening table TSingkatan","NETtalk2",MB_OK);
	}
	delete pRs;
	return bResult;
}

void CNaturalLanguage::SetConnDB(CADODatabase *db)
{
	m_pDb = db;		
	LoadTVokal();
	LoadTSimbol();
	LoadTGugus();
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
		MessageBox(NULL,"Error on opening table TSimbol","NETtalk2",MB_OK);
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
		MessageBox(NULL,"Error on opening table TSingkatan","NETtalk2",MB_OK);
	}
	delete pRs;
}


/************************************************************************/
/* bertugas sebagai alur utama dalam class CNaturalLanguage

/************************************************************************/

void CNaturalLanguage::Run()
{	
	int iPanjang = m_sText.GetLength();			
	if(iPanjang==0)
		return;
	
	m_spaw->Stop(FALSE);
	CString sKataTemp="";	
	BOOL bSingkatan=FALSE;		
	int start=0,katake=-1;	
	m_sSukuKataAll = "";
	
#ifndef _TDPSOLA 
	katake=0;
	CreateResultFile(katake);
#endif

	for(int i=0;i<iPanjang;i++)
	{			
		if(m_playkata->stop == 1)
			break;
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
				CString sKecuali = IsPengecualian(sKataTemp);	
				//pengecekan terhadap huruf 'e' atau 'o', jika ada maka pengenalan 
				//dengan 1-5 SPAW dan jaringan syaraf tiruan backpropagation				
				CString sFonem;								
				if((sKataTemp.FindOneOf("oe")!=-1)&&(sKecuali==""))
				{						
					sFonem = m_spaw->Text2Fonem(sKataTemp);															
					//sebelum dipenggal ganti gugus konsonan dengan karakter khusus												
					sFonem = GantiGugus(sFonem);					
					BuatFileKata(sFonem);															
				}else
				{						
					//sebelum dipenggal ganti gugus konsonan dengan karakter khusus										
					if(sKecuali!=""){						
						sKataTemp = GantiGugus(sKecuali);												
					}else{
						sKataTemp = GantiGugus(sKataTemp);
					}
					BuatFileKata(sKataTemp);					
				}													
				//pengaturan jeda
				//jika spasi maka jeda pendek. jika (,) atau (:) maka jeda sedang. jika akhir kalimat jeda panjang  
				// # untuk jeda pendek, ## untuk jeda sedang, ### untuk jeda panjang
				//jeda pendek								
				if(m_sText.GetAt(i)==' ')
				{										
					TambahFileSukuKata("#");
				}
				//jeda sedang 
				else if(m_sText.GetAt(i)==','||m_sText.GetAt(i)==':')
				{					
					TambahFileSukuKata("##");					
				}
				else if(TandaAkhirKalimat(m_sText.GetAt(i)))
				{	
					TambahFileSukuKata("###");										
				}
				
				KATA kata;
				kata.start = start;																					
				kata.end = i;
				start = i+1;										
				
				EnterCriticalSection(&m_playkata->guard);					
				m_playkata->ready = 0;			
				m_playkata->m_vKata.push_back(kata);
				m_playkata->unprocessed++;
				m_playkata->ready = 1;
				LeaveCriticalSection(&m_playkata->guard);
				
				katake++;												
				CreateResultFile(katake);
				
			}
			sKataTemp = "";
		}			
	}
	
	EnterCriticalSection(&m_playkata->guard);					
	m_playkata->finish = 1;
	LeaveCriticalSection(&m_playkata->guard);
	
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
	CADORecordset * pRs = new CADORecordset(m_pDb);				
	CString sCmd,sKata,sPengecualian,sHasil="";
	sCmd.Format("SELECT kata, pengucapan FROM TPengecualian WHERE kata = '%s'",sText);			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		if(!pRs->IsEOF())
		{															
			pRs->GetFieldValue("pengucapan", sPengecualian);													
			pRs->GetFieldValue("kata", sKata);													
			if(sKata==sText)
				sHasil = sPengecualian;
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TPengecualian","NETtalk2",MB_OK);
	}
	delete pRs;
	return sHasil;
}

BOOL CNaturalLanguage::IsVokal(char c)
{
	list<CString>::iterator iter;
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
		return 9;
	if(from	==0 && (!IsVokal(sText.GetAt(0)) && !IsVokal(sText.GetAt(1)) && IsVokal(sText.GetAt(2)) && sText.GetLength()==3))
		return 11;
	if(from==0 && (!IsVokal(sText.GetAt(0)) && !IsVokal(sText.GetAt(1)) && !IsVokal(sText.GetAt(2))  && IsVokal(sText.GetAt(3)) && sText.GetLength()==4))
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
//////////////////////////////////////////////////////////////////////////
	
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
		MessageBox(NULL,"Error on opening table TVokal","NETtalk2",MB_OK);
	}
	delete pRs;
}

BOOL CNaturalLanguage::IsSimbol(char c)
{
	list<CString>::iterator iter;
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


void CNaturalLanguage::LoadTGugus()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_vGugus.clear();
	CString sCmd;
	sCmd.Format("SELECT gugus, simbol FROM TGugus");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{						
			GUGUS Gugus;
			pRs->GetFieldValue("gugus", Gugus.sGugus);						
			pRs->GetFieldValue("simbol", Gugus.sFonem);						
			m_vGugus.push_back(Gugus);			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TGugus","NETtalk2",MB_OK);
	}
	delete pRs;
}

CString CNaturalLanguage::GantiGugus(CString sText)
{
	CString sTemp = sText;
	list<GUGUS>::iterator iter;
	for (iter=m_vGugus.begin();iter!=m_vGugus.end();iter++)
	{
		GUGUS Gugus = (GUGUS)*iter;				
		sTemp.Replace(Gugus.sGugus,Gugus.sFonem);
	}
	return sTemp;
}

CString CNaturalLanguage::HapusSimbol(CString sSimbol)
{
	CString sTemp = sSimbol;	
	list<CString>::iterator iter;		
	for (iter=m_vSimbol.begin();iter!=m_vSimbol.end();iter++)
	{
		CString xc = *iter;					
		sTemp.Remove(xc.GetAt(0));				
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

CString CNaturalLanguage::GetFileSukuKata(CString sSukuKata)
{
	CADORecordset * pRs = new CADORecordset(m_pDb);				
	CString sCmd;
	CString sFileName;			
	sCmd.Format("SELECT FileName FROM TSukuKata WHERE SukuKata = '%s'",sSukuKata);
	
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		if(!pRs->IsEOF())
		{									
			pRs->GetFieldValue("FileName", sFileName);												
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TSukuKata","NETtalk2",MB_OK);
	}
	return sFileName;
}

void CNaturalLanguage::CreateResultFile(int index)
{
	CString sFile;
	m_sResultFileName.Format("%s\\hasil%d.wav",m_sFolderPath,index);	
#ifdef _TDPSOLA
	m_tdpsola.Save2File(m_sResultFileName);	
#else
	sFile.Format("%s\\voice\\%s",m_sFolderPath,GetFileSukuKata("#"));
	CopyFile(sFile,m_sResultFileName,FALSE);
#endif			
	
}
/*
void CNaturalLanguage::BuatFileKata(CString sKata)
{		
	//DFSA tingkat pertama
	
	int currstate=0;
	list<SUKUKATA> vSukukata,vSukukata2;
	list<SUKUKATA>::iterator iter;
	CString sSukuTemp="",sHasil;
	SUKUKATA sukukata,hasil;
	for (int i=0;i<sKata.GetLength();i++)
	{		
		currstate = FirstDFSA(currstate,sKata.GetAt(i));					
		if(currstate==0)
		{					
			sukukata.sSukuKata = sSukuTemp;											
			vSukukata.push_back(sukukata);						
			sSukuTemp="";
			currstate = FirstDFSA(currstate,sKata.GetAt(i));								
		}
		sSukuTemp += sKata.GetAt(i);						
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
			TambahFileSukuKata(sSukuTemp);			
			sSukuTemp="";
			currstate = ThirdDFSA(currstate,hasil.sSukuKata);		
		}
		sSukuTemp += hasil.sSukuKata;	
	}
	if (sSukuTemp.GetLength()>0)
	{										
		TambahFileSukuKata(sSukuTemp);		
	}	
}
*/
void CNaturalLanguage::BuatFileKata(CString sKata)
{		
	//DFSA tingkat pertama
	
	int currstate=0;
	list<CString> vSukukata,vSukukata2;
	list<CString>::iterator iter;
	CString sSukuTemp="";
	CString sSukuKata,sHasil;
	for (int i=0;i<sKata.GetLength();i++)
	{		
		currstate = FirstDFSA(currstate,sKata.GetAt(i));					
		if(currstate==0)
		{					
			sSukuKata = sSukuTemp;											
			vSukukata.push_back(sSukuKata);						
			sSukuTemp="";
			currstate = FirstDFSA(currstate,sKata.GetAt(i));								
		}
		sSukuTemp += sKata.GetAt(i);						
	}	
	if (sSukuTemp.GetLength()>0)
	{			
		sSukuKata = sSukuTemp;		
		vSukukata.push_back(sSukuKata);
	}
	
	//DFSA tingkat kedua
	sSukuTemp="";currstate=0;	
	for(iter=vSukukata.begin();iter!=vSukukata.end();iter++)
	{
		sHasil = *iter;				
		currstate = SecondDFSA(currstate,sHasil);					
		if(currstate==0)
		{								
			sSukuKata= sSukuTemp;				
			vSukukata2.push_back(sSukuKata);						
			sSukuTemp="";
			currstate = SecondDFSA(currstate,sHasil);			
		}
		sSukuTemp += sHasil;			
	}
	if (sSukuTemp.GetLength()>0)
	{
		sSukuKata = sSukuTemp;			
		vSukukata2.push_back(sSukuKata);
	}

	//DFSA tingkat ketiga
	sSukuTemp="";currstate=0;	
	for(iter=vSukukata2.begin();iter!=vSukukata2.end();iter++)
	{
		sHasil = *iter;
		currstate = ThirdDFSA(currstate,sHasil);							
		if(currstate==0)
		{							
			TambahFileSukuKata(sSukuTemp);			
			sSukuTemp="";
			currstate = ThirdDFSA(currstate,sHasil);		
		}
		sSukuTemp += sHasil;	
	}
	if (sSukuTemp.GetLength()>0)
	{										
		TambahFileSukuKata(sSukuTemp);		
	}	
}

#ifdef _TDPSOLA
void CNaturalLanguage::TambahFileSukuKata(CString sSukuTemp)
{		
	m_sSukuKataAll += "-";
	m_sSukuKataAll +=  sSukuTemp; 
	m_pLog->SetWindowText(m_sSukuKataAll);
	
	CString sFileSuku = GetFileSukuKata(sSukuTemp);
	CString sFilePath;			

	if(sFileSuku != ""){
		sFilePath.Format("%s\\voice\\%s",m_sFolderPath,sFileSuku);		
		m_tdpsola.AddSignalFromFile(sFilePath);		
		//CString sText;
		//sText.Format("Exists %s\r\n",sSukuTemp);
		//WriteLog(sText);
	}else
	{		
		CString sText;
		sText.Format("Not Exists %s\r\n",sSukuTemp);
		WriteLog(sText);
	}	
}
#else

void CNaturalLanguage::TambahFileSukuKata(CString sSukuTemp)
{	
	CString sFileSuku = GetFileSukuKata(sSukuTemp);
	CString sFilePath;			
	
	if(sFileSuku != ""){
		sFilePath.Format("%s\\voice\\%s",m_sFolderPath,sFileSuku);
		
		CFile fresult;
		CFile fsuku;
		if(!fresult.Open(m_sResultFileName,CFile::modeReadWrite))
		{
			//MessageBox(NULL,"Error opening result","NETtalk2",MB_OK);
			return;
		}
		if(!fsuku.Open(sFilePath,CFile::modeRead))
		{
			//MessageBox(NULL,"Error opening file suku kata","NETtalk2",MB_OK);
			return;
		}
		
		WAVE_HDR wavesuku,waveresult;
		CHUNK_HDR chunksuku,chunkresult;
		DATA_HDR datasuku,dataresult;
		WAVEFORMATPE wavformsuku,waveformresult;
		
		DWORD nsampsuku = GetHeader(&fsuku,&wavesuku,&chunksuku,&wavformsuku,&datasuku);
		DWORD nsampresult = GetHeader(&fresult,&waveresult,&chunkresult,&waveformresult,&dataresult);
		if(nsampresult==0 || nsampsuku==0)
			return;

		dataresult.data_size =  (nsampsuku+nsampresult)*wavformsuku.nBlockAlign;
		waveresult.chunk_size = dataresult.data_size+sizeof(DATA_HDR)+sizeof(CHUNK_HDR)+sizeof(WAVEFORMATPE );
	
		fresult.Seek(0,CFile::begin);
		fresult.Write(&waveresult,sizeof(WAVE_HDR));
		fresult.Write(&chunksuku,sizeof(CHUNK_HDR));
		fresult.Write(&wavformsuku,sizeof(WAVEFORMATPE ));
		fresult.Write(&dataresult,sizeof(DATA_HDR));

		WORD nBlockAlign = wavformsuku.nBlockAlign;			
		short int int_data[4],j;			
		fsuku.Seek(44,CFile::begin);
		fresult.Seek(0,CFile::end);						

		
		for(DWORD x=0;x<nsampsuku;x++)
		{					
			if(fsuku.Read(int_data,nBlockAlign)>0)
			{
				j = int_data[0];							
				fresult.Write(&j,nBlockAlign);											
			}					
		}				

		fresult.Close();
		fsuku.Close();
		
		CString sText;
		sText.Format("Exists %s\r\n",sSukuTemp);
		WriteLog(sText);
	}else
	{
		CString sText;
		sText.Format("Not Exists %s\r\n",sSukuTemp);
		WriteLog(sText);
	}
	
}
#endif
DWORD CNaturalLanguage::GetHeader(CFile * file, WAVE_HDR *whdr, CHUNK_HDR *chdr, WAVEFORMATPE  *wfmt,DATA_HDR * dthdr)
{
	file->Read(whdr,sizeof(WAVE_HDR));	
	if(strnicmp(whdr->chunk_id,"RIFF",4) != 0) {		
		MessageBox(NULL,"Error in RIFF header","NETtalk2",MB_OK);		
		return 0;
	}	
	file->Read(chdr,sizeof(CHUNK_HDR));
	if(strnicmp(chdr->form_type,"WAVEfmt ",8) != 0) {
		MessageBox(NULL,"Error in WAVEfmt form_type","NETtalk2",MB_OK);				
		return 0;
	}
	if(chdr->hdr_size != sizeof(WAVEFORMATPE )) {
		MessageBox(NULL,"Error in WAVEfmt header size","NETtalk2",MB_OK);						
		return 0;
	}
	file->Read(wfmt,sizeof(WAVEFORMATPE ));
	if(wfmt->wFormatTag != WAVE_FORMAT_PCM) {
		MessageBox(NULL,"Error in WAVEfmt header - not PCM","NETtalk2",MB_OK);								
		return 0;
	}
	if(wfmt->nChannels > 4 || wfmt->nBlockAlign > 8) {
		MessageBox(NULL,"Error in WAVEfmt header - Channels/BlockSize","NETtalk2",MB_OK);								
        return 0;
	}
	file->Read(dthdr,sizeof(DATA_HDR));
	char dataTypeCheck[5] = "data";
	for(int i = 0 ; i < 4 ; i++) {
		if(dthdr->data_type[i] != dataTypeCheck[i]) {
			MessageBox(NULL,"Data type not audio 'data'","NETtalk2",MB_OK);											
			return 0;
		}
	}
	DWORD number_of_samples = dthdr->data_size/wfmt->nBlockAlign;	
	return number_of_samples;
}

void CNaturalLanguage::WriteLog(CString sText)
{
	m_fLog.Seek(0,CFile::end);
	m_fLog.Write(sText,sText.GetLength());
}
void CNaturalLanguage::SetStatusCtrl(CRichEditCtrl * pText,CRichEditCtrl * pLog)
{
	m_pLog = pLog;
	m_pText = pText;
}
void CNaturalLanguage::SetSharedVar(PLAYKATA * playkata)
{
	m_playkata = playkata;
}

void CNaturalLanguage::ReloadData()
{
	LoadTVokal();
	LoadTSimbol();
	LoadTGugus();
	LoadTSingkatan();
}
