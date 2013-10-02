// BackPro.cpp: implementation of the CBackPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NETtalk2.h"
#include "BackPro.h"
#include "LearningDlg.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackPro::CBackPro()
{	

	m_bStop = FALSE;		
	m_iPrevId = -1;
	m_iJumHuruf =0;
	m_iJumKata = 0;	
	CString sFileName;	
	sFileName.Format("Backprolog-%d.txt",GetTickCount());
	m_fLog.Open(sFileName,CFile::modeWrite|CFile::modeCreate);
}

void CBackPro::SetParam( UINT uiNumHidden,double dEta, double dTargetError,UINT uiMaxEpoch)
{
	
	m_uiNumHidden = uiNumHidden;
	m_dEta = dEta;
	m_dTargetError = dTargetError;
	m_uiMaxEpoch = uiMaxEpoch;	

	//CString sText;
	//sText.Format("hidden : %d, learning rate : %2.3f, target error : %2.3f, maxepoch : %d\r\n",uiNumHidden,dEta,dTargetError,uiMaxEpoch);
	//WriteLog(sText);

	int i;
	//inisialisasi bobot input ke hidden
		
	m_vWeightInput2Hidden.resize(m_uiNumInput);		
	m_vDeltaWeightInput2Hidden.resize(m_uiNumInput);
	m_v2WeightInput2Hidden.resize(m_uiNumInput);	
	for(i=0;i<m_uiNumInput;i++)
	{				
		m_v2WeightInput2Hidden[i].resize(m_uiNumHidden);
		m_vWeightInput2Hidden[i].resize(m_uiNumHidden);					
		m_vDeltaWeightInput2Hidden[i].resize(m_uiNumHidden);				
	}	
////////////////////////////////////////////////

	//inisialisasi hidden ke output
		
	m_vWeightHidden2Output.resize(m_uiNumHidden);		
	m_vDeltaWeightHidden2Output.resize(m_uiNumHidden);	
	m_v2WeightHidden2Output.resize(m_uiNumHidden);
	for(i=0;i<m_uiNumHidden;i++)
	{		
		 m_v2WeightHidden2Output[i].resize(m_uiNumOutput);
		m_vWeightHidden2Output[i].resize(m_uiNumOutput);	
		m_vDeltaWeightHidden2Output[i].resize(m_uiNumOutput);
	}
	
///////////////////////////////////////////
	m_vOutputError.resize(m_uiNumOutput);
	m_vHiddenError.resize(m_uiNumHidden);

	//inisialisasi variable untuk backpropagation
	y.resize(m_uiNumOutput);
	y_in.resize(m_uiNumOutput);
	z.resize(m_uiNumHidden);
	z_in.resize(m_uiNumHidden);
	errorhiddenlayer.resize(m_uiNumHidden);

}
void CBackPro::Reset()
{
	m_vLearningData.clear();	
	m_iPrevId = -1;
	m_iJumHuruf = 0;
	m_iJumKata = 0;
}

CBackPro::~CBackPro()
{
	Reset();
	m_fLog.Close();
}

void CBackPro::AddData(INPUT _input)
{	
	m_vLearningData.push_back(_input);		
	if(_input.id!=m_iPrevId)
	{		
		PATTERNKATA pattern;
		pattern.start = m_vLearningData.size()-1;
		m_vPatternKata.push_back(pattern);
		if(_input.id>0)
		{
			m_vPatternKata[m_iPrevId].jumhuruf = m_iJumHuruf;			
		}
		m_iJumHuruf = 0;
		m_iPrevId = _input.id;
		m_iJumKata++;
	}
	m_iJumHuruf++;
}
#define UNIFORM_PLUS_MINUS_ONE ( (double)(2.0 * rand())/RAND_MAX - 1.0 )

void CBackPro::InitWeight()
{			
	int i,j;
	for(j=0;j<m_uiNumHidden;j++)
	{		
		for(i=0;i<m_uiNumInput;i++)
		{
			m_vWeightInput2Hidden[i][j] = 0.05 * UNIFORM_PLUS_MINUS_ONE;		
		}		
	}
	
	for(j=0;j<m_uiNumOutput;j++)
	{		
		for(i=0;i<m_uiNumHidden;i++)
		{
			m_vWeightHidden2Output[i][j] = 0.05 * UNIFORM_PLUS_MINUS_ONE;							
		}		
	}

}
void CBackPro::Normalize()
{
	//normalisasi input 
	vector<INPUT>::iterator iterInput;		
	int i;	
	for (iterInput = m_vLearningData.begin();iterInput!=m_vLearningData.end();iterInput++)
	{
		INPUT & inputx = *iterInput;
		for(i=0;i<m_uiNumInput;i++)
		{		
			inputx.input[i] = (0.8 * inputx.input[i])+0.1;											
		}	
	}
}

double CBackPro::Activation( double z_in)
{		
	return (1.7159*tanh(0.66666667*z_in));	
}

double CBackPro::ActivationDerivative( double y_in)
{	
	double x = cosh(0.66666667*y_in);
	return (0.66666667*1.7159/(x*x));
}

vector<double> CBackPro::Test(vector<double> input)
{
	//CatatBobot();
	int i,j;
	for(i=0;i<m_uiNumInput;i++)
	{		
		input[i] = (0.8 * input[i])+0.1;											
	}
	//operasi pada hidden layer	
	for(i=0;i<m_uiNumHidden;i++)
	{
		z_in[i] = 0;
		for( j=0;j<m_uiNumInput;j++)
		{				
			z_in[i] += m_vWeightInput2Hidden[j][i]*input[j];	
			
		}					
		//pengaktifan
		z[i] = Activation(z_in[i]);				
	}
	//operasi pada output layer
	CString sText;	
	double max;
	int sel=0;
	for(i=0;i<m_uiNumOutput;i++)
	{		
		y_in[i] = 0;
		
		for(j=0;j<m_uiNumHidden;j++)
		{
			y_in[i]+= m_vWeightHidden2Output[j][i]*z[j];					
		}		
		//pengaktifan
		y[i] = Activation(y_in[i]);								
		
		if(i==0)
		{
			max = y[0];
			sel = 0;
		}else
		{
			if(y[i] > max)
			{
				max = y[i];
				sel = i;
			}
		}
	}	
	//winner takes all
	for(i=0;i<m_uiNumOutput;i++)
	{
		if(i!=sel)
		{
			y[i] = 0;
		}else{
			y[i] = 1;
		}
	}
	return y;
}
void CBackPro::Learning(CProgressCtrl * pProg,CEdit * pStatus)
{	
	
	CString sText="",sBuff;
	if(pStatus!=NULL)
	{		
		pStatus->GetWindowText(sBuff);
		sText.Format("%sTotal data : %d kata, %d window\r\n",sBuff, m_iJumKata,m_vLearningData.size());
		pStatus->SetWindowText(sText);			
	}
	
	if(pProg != NULL)
	{
		pProg->SetRange(0,m_uiMaxEpoch);
		pProg->SetPos(0);
	}	

	m_uiIterations = 0;		
	m_vPatternKata[m_iPrevId].jumhuruf = m_iJumHuruf;				
												
	double dEtaDecay=0.7941833,dEtaMin=0.00005;											

	Normalize();	
	InitWeight();
	CatatBobot();
	
	int sample=0,start,jumhuruf,index;	
	
	for (int i=0;i<m_iJumKata;i++)
	{
		m_vOrder.push_back(i);
	}
	m_dMinMSE = 99;
	m_dMinErrAbs = 99;

	random_shuffle(m_vOrder.begin(),m_vOrder.end());
	push_heap(m_vOrder.begin(),m_vOrder.end());

	Randomizex();
	int step=0;
	while (1)
	{						
		if(m_bStop){
			m_vWeightHidden2Output = m_vBestWeightHidden2Output;
			m_vWeightInput2Hidden = m_vBestWeightInput2Hidden;
			if(pStatus!=NULL)
			{
				CString sBuff,sText;
				pStatus->GetWindowText(sBuff);
				sText.Format("%s%d Best ES : %3.4f AvgErr : %3.4f\r\n",sBuff,m_uiIterations, m_dMinMSE,m_dMinErrAbs);
				//WriteLog(sText);
				pStatus->SetWindowText(sText);				
			}	
			break;
		}
		if((step%120000==0) && (step!=0))			
		{							
			m_dEta*=dEtaDecay;
			if(m_dEta<dEtaMin)
				m_dEta = dEtaMin;							
		}
		if (sample == m_iJumKata){	
			TotalError(pStatus);				
			sample = 0;					
			m_uiIterations++;		
			if(pProg != NULL)
			{				
				pProg->SetPos(m_uiIterations);
			}	
			if((m_uiIterations >m_uiMaxEpoch) || (m_dMSE<m_dTargetError)){
				if(pStatus!=NULL)
				{
					CString sBuff,sText;
					pStatus->GetWindowText(sBuff);
					sText.Format("%s%d Convergen Best ES : %3.4f AvgErr : %3.4f\r\n",sBuff,m_uiIterations, m_dMinMSE,m_dMinErrAbs);
					pStatus->SetWindowText(sText);				
				}	
				break;	
			}			
			random_shuffle(m_vOrder.begin(),m_vOrder.end());
			push_heap(m_vOrder.begin(),m_vOrder.end());
			Randomizex();
			
		}		
		int kata = m_vOrder[sample];
		start = m_vPatternKata[kata].start;
		jumhuruf = m_vPatternKata[kata].jumhuruf;
		//tiap kata							
		ResetDeltaWeight();
		//tiap seri window			
		for (index=start;index<(start+jumhuruf);index++)		
		{			 
			INPUT inputx = m_vLearningData[index];					 
			FeedForward(inputx);		
			CalculateError(inputx);				
			step++;
			UpdateWeight();		
		}					
		sample++;	
	}	
}

void CBackPro::Save(CProgressCtrl * pProg)
{
	CADORecordset * pRs = new CADORecordset(m_pDb);			
	//simpan parameter untuk pembelajaran	
	
	if(pRs->Open("TPembelajaran", CADORecordset::openTable))		
	{							
		pRs->AddNew();					
		
		pRs->SetFieldValue(1, m_dEta);
		pRs->SetFieldValue(2, (int)m_uiNumHidden);
		pRs->SetFieldValue(3, (int)m_uiMaxEpoch);
		pRs->SetFieldValue(4, m_dTargetError);
		pRs->SetFieldValue(5, (int)m_uiNumInput);
		pRs->SetFieldValue(6, (int)m_uiNumOutput);
		pRs->Update();
		pRs->Close();		
		
	}	
	if(pProg!=NULL)
	{
		pProg->SetRange(0,m_uiMaxEpoch);
		pProg->SetPos(0);
	}
	int i,j;
	m_pDb->Execute("DELETE from TBobot");
	//simpan parameter untuk semua bobot	
	if(pRs->Open("TBobot", CADORecordset::openTable))		
	{
		//input ke hidden layer
		for( j=0;j<m_uiNumHidden;j++)
		{
			if(m_bStop)
				break;
			for( i= 0;i<m_uiNumInput;i++)
			{
				pRs->AddNew();							
				pRs->SetFieldValue(1, 1);
				pRs->SetFieldValue(2, i+1);
				pRs->SetFieldValue(3, j+1);
				pRs->SetFieldValue(4, m_vWeightInput2Hidden[i][j]);
				pRs->Update();								
			}					
			if(pProg!=NULL)
			{
				pProg->SetPos(j);
			}
		}
		//hidden ke output layer
		for (j=0;j<m_uiNumOutput;j++)
		{
			if(m_bStop)
				break;
			for (i=0;i<m_uiNumHidden;i++)
			{
				pRs->AddNew();							
				pRs->SetFieldValue(1, 2);
				pRs->SetFieldValue(2, i+1);
				pRs->SetFieldValue(3, j+1);
				pRs->SetFieldValue(4, m_vWeightHidden2Output[i][j]);
				pRs->Update();	
			}			
			if(pProg!=NULL)
			{
				pProg->SetPos(j);
			}
		}
		pRs->Close();
	}														
	if(pProg!=NULL)
	{
		pProg->SetPos(0);
	}
	delete pRs;
}

void CBackPro::SetConnDB(CADODatabase *db)
{
	m_pDb = db;
}

void CBackPro::Load()
{
	CADORecordset * pRs = new CADORecordset(m_pDb);				
	
	CString sCmd;
	sCmd.Format("SELECT LearningRate, HiddenUnit, MaxEpoch, TargetError, NumInput, NumOutput FROM  TPembelajaran");					
	MessageBox(NULL,"load TPembelajaran","NETtalk2",MB_OK);			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		if(!pRs->IsEOF())
		{			
			double eta,targeterror;
			int numhidden,maxepoch,numinput,numoutput;
			pRs->GetFieldValue("LearningRate",eta);			
			pRs->GetFieldValue("HiddenUnit", numhidden);	
			pRs->GetFieldValue("MaxEpoch", maxepoch);
			pRs->GetFieldValue("TargetError", targeterror);
			pRs->GetFieldValue("NumInput", numinput);
			pRs->GetFieldValue("NumOutput", numoutput);		
			m_dEta = eta;
			m_dTargetError = targeterror;
			m_uiNumHidden = numhidden;
			m_uiNumOutput = numoutput;
			m_uiNumInput = numinput;
			m_uiMaxEpoch = maxepoch;
			
			SetParam(m_uiNumHidden,m_dEta,m_dTargetError,m_uiMaxEpoch);
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TPembelajaran","NETtalk2",MB_OK);
	}
	//load bobot input to hidden
	MessageBox(NULL,"load inpu t2 hidden","NETtalk2",MB_OK);			
	sCmd.Format("SELECT DariNeuron, KeNeuron, Bobot FROM  TBobot Where IDLayer = 1");					
			
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while (!pRs->IsEOF() && !m_bStop)
		{			
			int darineuron,keneuron;
			double bobot;
			pRs->GetFieldValue("DariNeuron",darineuron);			
			pRs->GetFieldValue("KeNeuron", keneuron);	
			pRs->GetFieldValue("Bobot", bobot);
			m_vWeightInput2Hidden[darineuron-1][keneuron-1] = bobot;			
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TBobot","NETtalk2",MB_OK);
	}
	
	sCmd.Format("SELECT DariNeuron, KeNeuron, Bobot FROM  TBobot Where IDLayer = 2");					
	MessageBox(NULL,"hidden 2 output","NETtalk2",MB_OK);					
	if(pRs->Open(sCmd,CADORecordset::openQuery))
	{							
		while (!pRs->IsEOF() && !m_bStop)
		{			
			int darineuron,keneuron;
			double bobot;
			pRs->GetFieldValue("DariNeuron",darineuron);			
			pRs->GetFieldValue("KeNeuron", keneuron);	
			pRs->GetFieldValue("Bobot", bobot);
			m_vWeightHidden2Output[darineuron-1][keneuron-1] = bobot;
			pRs->MoveNext();
		}
		pRs->Close();			
	}else
	{
		MessageBox(NULL,"Error on opening table TBobot","NETtalk2",MB_OK);
	}
	delete pRs;
}

void CBackPro::SetInputOutputLength(UINT uiNumInput, UINT uiNumOutput)
{
	m_uiNumInput = uiNumInput;
	m_uiNumOutput = uiNumOutput;
}

void CBackPro::WriteLog(CString sText)
{	
	m_fLog.Seek(0,CFile::end);
	m_fLog.Write(sText,sText.GetLength());
}

void CBackPro::CatatBobot()
{
	int i,j;
	CString sText;
	WriteLog("bobot input ke hidden\r\n");
	for (i=0;i<m_uiNumInput;i++)
	{		
		for(j=0;j<m_uiNumHidden;j++)	
		{
			sText.Format("[%d,%d][%2.4f],",i,j,m_vWeightInput2Hidden[i][j]);
			WriteLog(sText);
		}			
		sText.Format("\r\n");
		WriteLog(sText);
	}
	sText.Format("\r\n");
	WriteLog(sText);
	WriteLog("bobot hidden ke output\r\n");
	for (i=0;i<m_uiNumHidden;i++)
	{				
		for(j=0;j<m_uiNumOutput;j++)		
		{
			sText.Format("[%d,%d][%2.4f],",i,j,m_vWeightHidden2Output[i][j]);
			WriteLog(sText);
		}		
		sText.Format("\r\n");
		WriteLog(sText);	
	}
	sText.Format("\r\n");
	WriteLog(sText);
		
}


void CBackPro::FeedForward(INPUT inputx)
{
	int i,j;
	/*
	 Feed forward
	*/
	//operasi pada hidden layer
	/************************************************************************/						
	CString sText;
	for(i=0;i<m_uiNumHidden;i++)
	{		
		z_in[i] = 0;
		for( j=0;j<m_uiNumInput;j++)
		{				
			z_in[i] += m_vWeightInput2Hidden[j][i]*inputx.input[j];	
			
		}					
		//pengaktifan		
		z[i] = Activation(z_in[i]);				
		sText.Format("[%d][%2.4f , %2.4f]",i,z_in[i],z[i]);
		WriteLog(sText);
	}		
	//operasi pada output layer
	/************************************************************************/	
	WriteLog("feed forward hidden 2 output\r\n");
	
	for(i=0;i<m_uiNumOutput;i++)
	{				
		y_in[i] = 0;		
		for(j=0;j<m_uiNumHidden;j++)
		{
			y_in[i]+= m_vWeightHidden2Output[j][i]*z[j];					
		}		
		//pengaktifan
		y[i] = Activation(y_in[i]);	
		sText.Format("[%d][%2.4f , %2.4f]",i,y_in[i],y[i]);
		WriteLog(sText);
	}
}

void CBackPro::CalculateError(INPUT inputx)
{
	int i,j;
	double err,selisih;
	WriteLog("error hidden 2 output\r\n");
	CString sText;
	for(i=0;i<m_uiNumOutput;i++)
	{
		//error pada tiap output			
		selisih = inputx.output[i]-y[i];				
		m_vOutputError[i] = selisih*ActivationDerivative(y_in[i]);
		err = m_vOutputError[i];
		sText.Format("[%d][%2.4f]",i,err);
		WriteLog(sText);
		for(j=0;j<m_uiNumHidden;j++)
		{			
			m_vDeltaWeightHidden2Output[j][i] = err*z[j];
			sText.Format("[%d,%d][%2.4f]",j,i,m_vDeltaWeightHidden2Output[j][i]);
			WriteLog(sText);
		}
	}		
	
	//hitung error pada hidden layer	
	for(i=0;i<m_uiNumHidden;i++)
	{
		errorhiddenlayer[i] = 0;
		for(j=0;j<m_uiNumOutput;j++)
		{						
			errorhiddenlayer[i] += m_vOutputError[j]*m_vWeightHidden2Output[i][j];
		}
		m_vHiddenError[i] = errorhiddenlayer[i]*ActivationDerivative(z_in[i]);				
	}
	WriteLog("error input 2 hidden\r\n");
	//hitung delta weight input 2 hidden		
	for(j=0;j<m_uiNumHidden;j++)
	{		
		err = m_vHiddenError[j];
		sText.Format("[%d][%2.4f]",j,err);
		WriteLog(sText);
		for(i=0;i<m_uiNumInput;i++)	
		{															
			m_vDeltaWeightInput2Hidden[i][j] = err*inputx.input[i];			
			sText.Format("[%d,%d][%2.4f]",i,j,m_vDeltaWeightInput2Hidden[i][j]);
			WriteLog(sText);
		}		
	}	
}

void CBackPro::ResetDeltaWeight()
{
	//reset error pada tiap layer
	//output layer
	int i,j;
	for(i=0;i<m_uiNumOutput;i++)
	{																			
		m_vOutputError[i] =0;	
		for(j=0;j<m_uiNumHidden;j++)
		{
			m_vDeltaWeightHidden2Output[j][i] = 0;
		}	
	}
	//hidden layer
	for (i=0;i<m_uiNumHidden;i++)
	{
		m_vHiddenError[i] = 0;
		for(j=0;j<m_uiNumInput;j++)
		{
			m_vDeltaWeightInput2Hidden[j][i] = 0;
		}	
	}
}

void CBackPro::UpdateWeight()
{
	//lakukan perubahan bobot untuk hidden layer
	int i,j;	
	CString sText;
	WriteLog("bobot baru input 2 hidden\r\n");
	double divisor,epsilon;
	
	for(j=0;j<m_uiNumHidden;j++)
	{
		for(i=0;i<m_uiNumInput;i++)	
		{			
			//m_vWeightInput2Hidden[i][j] += m_dEta*m_vDeltaWeightInput2Hidden[i][j];																			
			divisor = m_v2WeightInput2Hidden[i][j] + 0.1;
			if(divisor<0.1)
				divisor = 1.0;
			epsilon = m_dEta / divisor;
			m_vWeightInput2Hidden[i][j] +=  epsilon*m_vDeltaWeightInput2Hidden[i][j];																			
			sText.Format("[%d,%d][%2.4f][%2.4f]",i,j,epsilon,m_vWeightInput2Hidden[i][j]);
			WriteLog(sText);
		}	
	}
	//lakukan perubahan bobot untuk output layer
	WriteLog("bobot baru hidden 2 output\r\n");
	for(i=0;i<m_uiNumOutput;i++)
	{
		for(j=0;j<m_uiNumHidden;j++)
		{			
			//m_vWeightHidden2Output[j][i] += m_dEta*m_vDeltaWeightHidden2Output[j][i];						
			divisor = m_v2WeightHidden2Output[j][i] + 0.1;
			if(divisor<0.1)
				divisor = 1.0;
			epsilon = m_dEta / divisor;
			m_vWeightHidden2Output[j][i] +=  epsilon*m_vDeltaWeightHidden2Output[j][i];																			
			sText.Format("[%d,%d][%2.4f][%2.4f]",j,i,epsilon,m_vWeightHidden2Output[j][i]);
			WriteLog(sText);
		}	
	}
}

void CBackPro::TotalError(CEdit * pStatus )
{
	int i,sample,start,jumhuruf,index;
	m_dMSE = 0;
	m_dErrAbs = 0;
	double selisih=0;
	for (sample=0;sample<m_iJumKata;sample++)
	{
		start = m_vPatternKata[sample].start;
		jumhuruf = m_vPatternKata[sample].jumhuruf;				
		//tiap seri window					
		for (index=start;index<(start+jumhuruf);index++)		
		{			 
			INPUT inputx = m_vLearningData[index];					 
			FeedForward(inputx);						
			//hitung output error 			
			
			for(i=0;i<m_uiNumOutput;i++)
			{
				//error pada tiap output
				selisih = fabs(inputx.output[i]-y[i]);
				m_dMSE += pow(selisih,2);
				m_dErrAbs += selisih;										
			}				
		}		
	}
	int nData = m_vLearningData.size();
	m_dMSE /= nData*2;
	m_dErrAbs /= nData;

	if(m_dMinMSE>m_dMSE){
		m_dMinMSE = m_dMSE;
		if(m_dErrAbs<m_dMinErrAbs){
			m_dMinErrAbs = m_dErrAbs;
			m_vBestWeightHidden2Output = m_vWeightHidden2Output;
			m_vBestWeightInput2Hidden = m_vWeightInput2Hidden;
		}
	}
	
	if(pStatus!=NULL)
	{
		CString sBuff,sText;
		pStatus->GetWindowText(sBuff);		
		sText.Format("%s%d MSE : %3.4f AvgErr : %3.4f\r\n",sBuff,m_uiIterations, m_dMSE, m_dErrAbs);
		//WriteLog(sText);
		pStatus->SetWindowText(sText);						
	}	
}

void CBackPro::CalculateHessian(INPUT inputx)
{
	int i,j;
	double yk,xi,wki;
	vector<double> d2E_d2Yn;		
	vector<double> d2E_d2X;	
	d2E_d2Yn.resize(m_uiNumOutput);	
	d2E_d2X.resize(m_uiNumHidden);		
	//hitung d2E/d2Yin = f'(Yn)^2* d2E/d2X ->1
	CString sText;
	WriteLog("hessian youtput 1\r\n");
	for(i=0;i<m_uiNumOutput;i++)
	{		
		yk = ActivationDerivative(y_in[i]);
		d2E_d2Yn[i] = yk*yk;
		sText.Format("[%d][%2.4f]",i,d2E_d2Yn[i]);
		WriteLog(sText);
	}
	WriteLog("hessian d2E/d2Wk\r\n");
	//hitung d2E/d2Wki = d2E/d2Yin * x^2
	for(i=0;i<m_uiNumOutput;i++)
	{							
		for(j=0;j<m_uiNumHidden;j++)
		{			
			xi = z[j];
			m_v2WeightHidden2Output[j][i] = (0.8*m_v2WeightHidden2Output[j][i] ) + (0.2*d2E_d2Yn[i] * xi*xi);												
			sText.Format("[%d,%d][%2.4f]",j,i,m_v2WeightHidden2Output[j][i]);
			WriteLog(sText);
		}
	}
	//hitung d2E/d2x = sigma(d2E/d2Yn * wki^2)
	WriteLog("hessian d2E/d2x \r\n");
	for(j=0;j<m_uiNumHidden;j++)
	{					
		d2E_d2X[j] = 0;
		for(i=0;i<m_uiNumOutput;i++)		
		{			
			wki = m_vWeightHidden2Output[j][i];
			d2E_d2X[j]+= d2E_d2Yn[i]*wki*wki;
		}		
		sText.Format("[%d][%2.4f]",j,d2E_d2X[j]);
		WriteLog(sText);
	}
//////////////////////////////////	
	vector<double> d2E_d2Yn2;		
	vector<double> d2E_d2X2;

	d2E_d2Yn2.resize(m_uiNumHidden);	
	d2E_d2X2.resize(m_uiNumInput);		
	//hitung d2E/d2Yin = f'(Yn)^2* d2E/d2X
	WriteLog("hessian youtput 2\r\n");
	for(i=0;i<m_uiNumHidden;i++)
	{		
		yk = ActivationDerivative(z_in[i]);
		d2E_d2Yn2[i] = d2E_d2X[i]*yk*yk;
		sText.Format("[%d][%2.4f]",i,d2E_d2Yn2[i]);
		WriteLog(sText);
	}

	//hitung d2E/d2Wki = d2E/d2Yin * x^2
	WriteLog("hessian d2E/d2Wk\r\n");
	for(i=0;i<m_uiNumHidden;i++)
	{							
		for(j=0;j<m_uiNumInput;j++)
		{			
			xi = inputx.input[j];
			m_v2WeightInput2Hidden[j][i] = (0.8*m_v2WeightInput2Hidden[j][i] )+ (0.2*d2E_d2Yn2[i] * xi*xi);						
			sText.Format("[%d,%d][%2.4f]",j,i,m_v2WeightInput2Hidden[j][i]);
			WriteLog(sText);
		}
	}
}

void CBackPro::EraseHessian()
{
	int i,j;
	for(i=0;i<m_uiNumOutput;i++)
	{							
		for(j=0;j<m_uiNumHidden;j++)
		{						
			m_v2WeightHidden2Output[j][i] =0.0;									
		}
	}
	for(i=0;i<m_uiNumHidden;i++)
	{							
		for(j=0;j<m_uiNumInput;j++)
		{				
			m_v2WeightInput2Hidden[j][i] =0.0;
		}
	}
}

void CBackPro::DevideHessian(int d)
{
	int i,j;
	for(i=0;i<m_uiNumOutput;i++)
	{							
		for(j=0;j<m_uiNumHidden;j++)
		{						
			m_v2WeightHidden2Output[j][i] /=d;									
		}
	}
	for(i=0;i<m_uiNumHidden;i++)
	{							
		for(j=0;j<m_uiNumInput;j++)
		{				
			m_v2WeightInput2Hidden[j][i] /=d;
		}
	}
}

void CBackPro::Randomizex()
{	
	EraseHessian();
	for (int sample=0;sample<m_iJumKata/2;sample++)
	{
		int kata = m_vOrder[sample];
		int start = m_vPatternKata[kata].start;
		int jumhuruf = m_vPatternKata[kata].jumhuruf;				
		//tiap seri window			
		for (int index=start;index<(start+jumhuruf);index++)		
		{			 
			INPUT inputx = m_vLearningData[index];					 
			FeedForward(inputx);
			CalculateHessian(inputx);
		}
	}
	
	DevideHessian(m_iJumKata/2);
}
