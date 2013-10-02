// 1_5SPAW.cpp: implementation of the C1_5SPAW class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NETtalk2.h"
#include "1_5SPAW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPAW::CSPAW()
{
	CString sFile;
	sFile.Format("SPAWlog%d.log",GetTickCount());
	m_fLog.Open(sFile,CFile::modeWrite|CFile::modeCreate);
}

CSPAW::~CSPAW()
{
	m_fLog.Close();
}

CString CSPAW::BinaryFonemToString(vector<double> vBit)
{
	vector<FONEM>::iterator iterFonem;
	vector<double> vResult;	
	CString sResult = "";
	for(iterFonem=m_vFonem.begin(); iterFonem!= m_vFonem.end();iterFonem++)
	{
		FONEM fonem = (FONEM)*iterFonem;		
		
		BOOL bSama = TRUE;
		for(int i=0;i<m_binaryfonemlength;i++)
		{					
			
			if(vBit[i] != fonem.binaryfeature[i])
			{
				bSama = FALSE;
				break;
			}
		}
		if(bSama)
		{
			sResult += fonem.sFonem;
		}
	}
	return sResult;
}
vector<double> CSPAW::FonemToBinary(char c)
{
	vector<FONEM>::iterator iterFonem;
	vector<double> vResult;	

	for(iterFonem=m_vFonem.begin(); iterFonem!= m_vFonem.end();iterFonem++)
	{
		FONEM fonem = (FONEM)*iterFonem;
		if(fonem.sFonem.GetAt(0)==c)			
		{					
			for(int i=0;i<m_binaryfonemlength;i++)
			{					
				vResult.push_back(fonem.binaryfeature[i]);					
			}					
		}
	}		
				
	return vResult;
}
vector<double> CSPAW::AbjadToBinary(vector<char> vWindowx)
{
	vector<ABJAD>::iterator iterAbjad;
	vector<char>::iterator iterWindowx;	
	vector<double> vResult;	
	
	for (iterWindowx=vWindowx.begin();iterWindowx!=vWindowx.end();iterWindowx++)
	{		
		for(iterAbjad=m_vAbjad.begin(); iterAbjad!= m_vAbjad.end();iterAbjad++)
		{			
			ABJAD abjad = (ABJAD)*iterAbjad;			
			if(abjad.sAbjad.GetAt(0)==*iterWindowx)			
			{														
				for(int i=0;i<m_binaryabjadlength;i++)
				{										
					vResult.push_back(abjad.binaryfeature[i]);
				}				
			}
		}
	}			
	return vResult;
}

void CSPAW::Initialize(UINT uiWindowSize,UINT pos,UINT unitHidden, double learningrate, double targeterror,UINT maxepoch)
{
	m_uiWinSize = uiWindowSize;
	m_pos = pos;
	m_bStop = FALSE;
	
	m_binaryfonemlength = 30;
	m_binaryabjadlength = 27;		

	LoadTFonem();
	LoadTAbjad();	
		
	m_BpFonem->SetInputOutputLength(m_uiWinSize*m_binaryabjadlength,m_binaryfonemlength);
	m_BpFonem->SetParam(unitHidden,learningrate,targeterror,maxepoch);	
	m_BpFonem->m_bStop = FALSE;
}
void CSPAW::LoadTAbjad()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	m_vAbjad.clear();
	CString sCmd;
	sCmd.Format("SELECT abjad, binaryfeature FROM TAbjad");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while(!pRs->IsEOF())
		{			
			ABJAD data;
			CString sBinaryFeature;
			CString sAbjad;
			pRs->GetFieldValue("abjad", data.sAbjad);			
			pRs->GetFieldValue("binaryfeature",sBinaryFeature);												
			data.binaryfeature.resize(m_binaryabjadlength);			
			for(int i=0;i<m_binaryabjadlength;i++)
			{
				char sChar[1];
				sprintf(sChar,"%c",sBinaryFeature.GetAt(i));
				data.binaryfeature[i] = atoi(sChar);							
			}			
			m_vAbjad.push_back(data);			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TAbjad","NETtalk",MB_OK);
	}
	delete pRs;
}

void CSPAW::LoadTFonem()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);				
	m_vFonem.clear();
	CString sCmd;
	sCmd.Format("SELECT fonem, binaryfeature FROM TFonem");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{									
		while(!pRs->IsEOF())
		{			
			FONEM data;
			CString sBinaryFeature;			
			pRs->GetFieldValue("fonem", data.sFonem);	
			pRs->GetFieldValue("binaryfeature",sBinaryFeature);												
			data.binaryfeature.resize(m_binaryfonemlength);
			for(int i=0;i<m_binaryfonemlength;i++)
			{
				char sChar[1];
				sprintf(sChar,"%c",sBinaryFeature.GetAt(i));
				data.binaryfeature[i] = atoi(sChar);											
			}			
			
			m_vFonem.push_back(data);			
			
			pRs->MoveNext();			
		}		
		pRs->Close();					
	}else
	{
		MessageBox(NULL,"Error on opening table TFonem","NETtalk",MB_OK);
	}

	delete pRs;
}
void CSPAW::Training(CProgressCtrl * pProg,CEdit * pStatus)
{	
	int * posx = new int[m_uiWinSize];
	int pos = abs((int)((m_uiWinSize/2)-m_pos));		
	vector<char> vWindowx;			
	vWindowx.resize(m_uiWinSize);			
	
	m_BpFonem->Reset();
	m_BpFonem->InitWeight();	
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	CString sCmd;
	sCmd.Format("SELECT distinct kata, fonem FROM TKata order by kata");
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{	
		int index = 0,w;
		while(!pRs->IsEOF())
		{	
			if(m_bStop)
			{
				break;
			}						
			
			CString sKata,sFonem;
			pRs->GetFieldValue("fonem", sFonem);	
			pRs->GetFieldValue("kata",sKata);		
			sKata.TrimLeft();sKata.TrimRight();
			sFonem.TrimLeft();sFonem.TrimRight();
			
			int nLength = sKata.GetLength();								
			for( w=0;w<m_uiWinSize;w++)
			{
				posx[w] = w - pos;		
			}	
			CString sWindow="";
			CString sBuffer,sText;			
			for(int i=0;i<nLength;i++)
			{							
				for(int j=0;j<m_uiWinSize;j++)
				{
					if((posx[j]<0) ||(posx[j]>=nLength))
					{
						vWindowx[j] = '-';
					}else{
						vWindowx[j] = sKata.GetAt(posx[j]);																				
					}						
				}			
				vector<double> vBinaryInput = AbjadToBinary(vWindowx);												
				char cfonem = sFonem.GetAt(i);
				vector<double> vBinaryTarget = FonemToBinary(cfonem);												
				
				INPUTPATTERN input;
				input.input = vBinaryInput;
				input.output = vBinaryTarget;
				input.id = index;
				
				m_BpFonem->AddData(input);			
				
				for(w=0;w<m_uiWinSize;w++)
				{
					posx[w]++;
				}			
			}
			index++;						
			pRs->MoveNext();
		}
		
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TKata","NETtalk",MB_OK);
	}
	delete pRs;
	
	m_BpFonem->Learning(pProg,pStatus);		
	delete[] posx;		
	
}
void CSPAW::Test(CEdit * pStatus)
{
	int * posx = new int[m_uiWinSize];
	int pos = (m_uiWinSize/2)-m_pos;	
	vector<char> vWindowx;			
	vWindowx.resize(m_uiWinSize);	
	UINT uiTrue=0,uiFalse=0,w;
	CString sBuffer,sText;
	
	if(pStatus!=NULL){		
		pStatus->GetWindowText(sBuffer);
		sText.Format("%s\r\ntesting ....\r\n",sBuffer);
		pStatus->SetWindowText(sText);
	}
	
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	CString sCmd;
	//0 : test kata yang ada dalam db
	//1 : test kata yang belum dikenali
	for(int idx=0;idx<2;idx++)
	{
		if(idx==0){
			sCmd.Format("SELECT kata, fonem FROM TKata order by kata");
			WriteLog("Kata dalam Db\r\n");
		}else {
			sCmd.Format("SELECT kata, fonem FROM TKataSemua WHERE kata NOT IN (SELECT kata FROM TKata) order by kata");			
			WriteLog("\r\nKata tidak ada dalam Db\r\n");
		}
		
		UINT jumkata = 0;		
		vector<int> vPos;
		if(pRs->Open(sCmd,CADORecordset::openQuery))
		{			
			if(idx==1)
			{								
				jumkata = 0;
				uiFalse = 0;
				uiTrue = 0;
				int nKata = pRs->GetRecordCount();
				for(int i=0;i<50;i++)
				{
					vPos.push_back((int)(rand() % nKata));
				}								
			}
			while(!pRs->IsEOF())
			{							
				if(idx==1)
				{					
					if(jumkata==50)
						break;							
					int recpos = vPos[jumkata];					
					pRs->SetAbsolutePosition(recpos);										
				}
				CString sKata,sRightFonem;
				pRs->GetFieldValue("fonem", sRightFonem);	
				pRs->GetFieldValue("kata",sKata);		
							
				int nLength = sKata.GetLength();
				for( w=0;w<m_uiWinSize;w++)
				{
					posx[w] = w - pos;		
				}	
				CString sFonem = "";
				for(int i=0;i<nLength;i++)
				{							
					for(int j=0;j<m_uiWinSize;j++)
					{
						if((posx[j]<0) ||(posx[j]>=nLength))
						{
							vWindowx[j] = '-';
						}else{
							vWindowx[j] = sKata.GetAt(posx[j]);																				
						}
					}								
					vector<double> vBinaryInput = AbjadToBinary(vWindowx);
					vector<double> vBineryOutput = m_BpFonem->Test(vBinaryInput);
					
					CString sFonemx=BinaryFonemToString(vBineryOutput);				
					
					if(sFonemx=="")
					{
						sFonemx = sKata.GetAt(i);
					}
							
					sFonem += sFonemx;		
					for(w=0;w<m_uiWinSize;w++)
					{
						posx[w]++;
					}			
				}
				CString sFalse = "false";
				if(sRightFonem == sFonem){
					sFalse = "true";
					uiTrue++;
				}else
				{
					uiFalse++;	
					sText.Format("%s %s      %s\r\n",sFonem,sRightFonem,sFalse);
					WriteLog(sText);
				}					
				jumkata++;
								
				pRs->MoveNext();
			}
			
			if(pStatus!=NULL)
			{				
				pStatus->GetWindowText(sBuffer);
				CString sTest;
				if(idx==0)//kata dalam db
					sTest = "Kata dalam database\r\n";
				else if(idx==1)
					sTest = "Kata tidak ada dalam database\r\n";
				
				sText.Format("%s %s benar : %d, salah : %d, keakuratan : %2.2f percent\r\n",sBuffer,sTest,uiTrue,uiFalse, (float)uiTrue*100/jumkata);
				WriteLog(sText);
				pStatus->SetWindowText(sText);
			}		
			pRs->Close();			
		}else
		{
			MessageBox(NULL,"Error on opening table TKata","NETtalk",MB_OK);
		}
		
	}
	
	delete pRs;	
	
	delete[] posx;
}

CString CSPAW::Text2Fonem(CString sText)
{	
	int * posx = new int[m_uiWinSize];
	int pos = (m_uiWinSize/2)-m_pos;		
	vector<char> vWindowx;			
	vWindowx.resize(m_uiWinSize);	
	UINT uiTrue=0,uiFalse=0,w;
	int j=0;	
	int nLength = sText.GetLength();
	for(w=0;w<m_uiWinSize;w++)
	{
		posx[w] = w - pos;		
	}	
	CString sFonem = "";
	for(int i=0;i<nLength;i++)
	{							
		for(int j=0;j<m_uiWinSize;j++)
		{
			if((posx[j]<0) ||(posx[j]>=nLength))
			{
				vWindowx[j] = '-';
			}else{
				vWindowx[j] = sText.GetAt(posx[j]);																				
			}
		}								
		vector<double> vBinaryInput = AbjadToBinary(vWindowx);		
		vector<double> vBineryOutput = m_BpFonem->Test(vBinaryInput);					
		CString sFonemx=BinaryFonemToString(vBineryOutput);
		if(sFonemx=="")
		{
			sFonemx = sText.GetAt(i);
		}
		sFonem += sFonemx;
		
		for(w=0;w<m_uiWinSize;w++)
		{
			posx[w]++;
		}
		
	}			

	delete[] posx;
	return sFonem;
}

void CSPAW::SetConnDB(CADODatabase *db)
{
	m_pDb = db;	
}

void CSPAW::Stop(BOOL bTrue)
{
	m_bStop = bTrue;
	m_BpFonem->m_bStop = bTrue;	
}


void CSPAW::SetBackPro(CBackPro *pBp)
{
	m_BpFonem = pBp;
}

void CSPAW::LoadBackPro()
{
	m_BpFonem->Load();
}

void CSPAW::SaveBackPro(CProgressCtrl * pProg)
{
	m_BpFonem->Save(pProg);
}

void CSPAW::WriteLog(CString sText)
{
	m_fLog.Seek(0,CFile::end);
	m_fLog.Write(sText,sText.GetLength());
}
