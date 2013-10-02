// TDPSOLA.h: interface for the CTDPSOLA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDPSOLA_H__48BDE52D_FBED_4D16_9592_B0F1111E334A__INCLUDED_)
#define AFX_TDPSOLA_H__48BDE52D_FBED_4D16_9592_B0F1111E334A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTDPSOLA  
{
public:
	void Save2File(CString sFile);
	void Reset();
	void AddSignalFromFile(CString sFile);
	CTDPSOLA();
	virtual ~CTDPSOLA();		
	void PSOLA(double dAlpha /*time stretching factor*/,double dBeta/*pitch shifting factor*/);
private:	
	void WriteLog(CString sText);
	void LowpassFilter(vector<short> & vSignal);
	void Normalize();
	short Round(float d);
	void PitchDetector();
	vector<short> Block(vector<short> vData, long start, long end);
	short Max(vector<short> vData, long & pos);
	vector<short> m_vSignal;		
	vector<long> m_vPitchMarks;
	vector<short> m_vResult;
	void GetData(CFile * fin, WAVEFORMATPE * wavfmt,long nSamples);
	long GetHeader(CFile * file, WAVE_HDR * whdr, CHUNK_HDR * chdr,WAVEFORMATPE * wfmt,DATA_HDR * dthdr);
	CFile m_fLog;
//	long m_outputsize;
};

#endif // !defined(AFX_TDPSOLA_H__48BDE52D_FBED_4D16_9592_B0F1111E334A__INCLUDED_)
