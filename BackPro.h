// BackPro.h: interface for the CBackPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKPRO_H__E2F48542_1E04_4B71_9935_52B7A6B17724__INCLUDED_)
#define AFX_BACKPRO_H__E2F48542_1E04_4B71_9935_52B7A6B17724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <malloc.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <functional>

typedef struct tag_INPUT
{
	vector<double> input;
	vector<double> output;	
	int id;
}INPUTPATTERN;
typedef struct tag_PATTERN
{
	int start,jumhuruf;
}PATTERNKATA;
class CBackPro  
{
public:	
	void SetInputOutputLength(UINT uiNumInput, UINT uiNumOutput);
	void Load();
	void SetConnDB(CADODatabase *db);
	void Save(CProgressCtrl * pProg);
	void SetParam(UINT uiNumHidden,double dEta, double dTargetError,UINT uiMaxEpoh);
	void InitWeight();
	void Reset();
	void AddData(INPUTPATTERN _input);	
	void Learning(CProgressCtrl * pProg,CEdit * pStatus);
	CBackPro();	
	vector<double> Test(vector<double> input);	
	BOOL m_bStop;	
	virtual ~CBackPro();	
private:			
	void GetTotalHessian();
	void DevideHessian(int d);
	void EraseHessian();
	void CalculateHessian(INPUTPATTERN inputx);
	void TotalError(CEdit * pStatus);
	void UpdateWeight();
	void ResetDeltaWeight();
	void CalculateError(INPUTPATTERN inputx);
	void FeedForward(INPUTPATTERN inputx);
	void CatatBobot();
	void Normalize();
	void WriteLog(CString sText);
	CFile m_fLog;
	vector<int> m_vOrder;
	vector<PATTERNKATA> m_vPatternKata;
	vector<INPUTPATTERN> m_vLearningData;	
	vector<double> y;
	vector<double> y_in;
	vector<double> z;
	vector<double> z_in;
	vector<double> errorhiddenlayer;		
	vector< vector<double> > m_vWeightInput2Hidden;
	vector< vector<double> > m_vBestWeightInput2Hidden;	
	vector< vector<double> > m_vErrorWeightInput2Hidden;				
	vector< vector<double> > m_vWeightHidden2Output;
	vector< vector<double> > m_vBestWeightHidden2Output;	
	vector< vector<double> > m_vErrorWeightHidden2Output;

	vector< vector<double> > m_v2WeightInput2Hidden;
	vector< vector<double> > m_v2WeightHidden2Output;

	vector<double> m_vOutputError;
	vector<double> m_vHiddenError;	
	double m_dEta;
	double m_dTargetError;
	double m_dMSE;
	double m_dErrAbs;
	double m_dMinMSE;
	double m_dMinErrAbs;
	double ActivationDerivative( double y_in);
	double Activation( double z_in);
	UINT m_uiMaxEpoch;
	UINT m_uiNumInput;	
	UINT m_uiNumHidden;
	UINT m_uiNumOutput;
	UINT m_uiIterations;	
	int m_iPrevId;		
	int m_iJumHuruf;
	int m_iJumKata;
	CADODatabase *m_pDb;	
};

#endif // !defined(AFX_BACKPRO_H__E2F48542_1E04_4B71_9935_52B7A6B17724__INCLUDED_)

