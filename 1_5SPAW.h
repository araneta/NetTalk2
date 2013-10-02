// 1_5SPAW.h: interface for the C1_5SPAW class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_1_5SPAW_H__F704F070_7C1C_478E_9A64_8897FEEE572A__INCLUDED_)
#define AFX_1_5SPAW_H__F704F070_7C1C_478E_9A64_8897FEEE572A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BackPro.h"		
typedef struct tag_FONEM
{
	vector<double> binaryfeature;
	CString sFonem;
}FONEM;

typedef struct tag_ABJAD
{
	vector<double> binaryfeature;
	CString sAbjad;
}ABJAD;
typedef struct tag_FONEMKATA
{
	CString sKata;
	CString sFonem;	
}FONEMKATA;

class CSPAW  
{
private:
	void WriteLog(CString sText);
	CFile m_fLog;
	UINT	m_uiWinSize;
	UINT	m_pos;	
	vector<double> AbjadToBinary(vector<char> vWindowx);	
	vector<double> FonemToBinary(char c);		
	BOOL m_bStop;
	int m_binaryfonemlength,m_binaryabjadlength;		
	void LoadTAbjad();
	void LoadTFonem();
	CString BinaryFonemToString(vector<double> vBit);		
public:
	CBackPro * m_BpFonem;
	void SaveBackPro(CProgressCtrl * pProg);
	void Initialize(UINT uiWindowSize,UINT pos,UINT unitHidden, double learningrate, double targeterror,UINT maxepoch);
	void SetConnDB(CADODatabase *db);
	void Training(CProgressCtrl * pProg,CEdit * pStatus);	
	void Test(CEdit * pStatus);
	void LoadBackPro();
	void SetBackPro(CBackPro * pBp);
	void Stop(BOOL bTrue);
	CADODatabase * m_pDb;	
	vector<FONEM> m_vFonem;
	vector<ABJAD> m_vAbjad;	
	CString Text2Fonem(CString sTex);	
	CSPAW();
	virtual ~CSPAW();
};

#endif // !defined(AFX_1_5SPAW_H__F704F070_7C1C_478E_9A64_8897FEEE572A__INCLUDED_)
