// NaturalLanguage.h: interface for the CNaturalLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NATURALLANGUAGE_H__63B9D4AB_D0E5_42DD_82C3_F0B734091A39__INCLUDED_)
#define AFX_NATURALLANGUAGE_H__63B9D4AB_D0E5_42DD_82C3_F0B734091A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "1_5SPAW.h"
#include "Speech.h"
class CNaturalLanguage  
{	
public:
	enum TipeKalimat {Berita,Tanya,Perintah};			
	void SetSpeech(CSpeech * pSpeech);
	void LoadSPAW();
	void SetSPAW(CSPAW *pSpaw);
	void Run();
	void SetConnDB(CADODatabase * db);
	void SetText(CString sText);
	BOOL IsSingkatan(CString sText);			
	CString KembalikanGugusKonsonan(CString sText);
	CNaturalLanguage();
	virtual ~CNaturalLanguage();
	CSpeech * m_speech;	
private:	
	CString HapusSimbol(CString sSimbol);
	CString GantiGugusKonsonan(CString sText);
	CString IsPengecualian(CString sText);
	void LoadTGugusKonsonan();
	void LoadTSimbol();
	void LoadTVokal();
	void AddKata(CString sKata);
	void LoadTSingkatan();	
	BOOL IsSimbol(char c);
	BOOL TandaAkhirKalimat(char c);
	BOOL IsVokal(char c);
	CSPAW * m_spaw;	
	int ThirdDFSA(int from,CString sText);
	int SecondDFSA(int from, CString sText);
	int FirstDFSA(int from,char c);		
	int TipeKalimat(char lastsimbol);
	vector<SUKUKATA> ToSukuKata(CString sText);		
	vector<CString> m_vSingkatan;
	vector<CString> m_vVokal;
	vector<CString> m_vSimbol;
	vector<GUGUSKONSONAN> m_vGugusKonsonan;
	vector<KECUALI> m_vPengecualian;	
	CString m_sText;	
	CADODatabase *m_pDb;

};

#endif // !defined(AFX_NATURALLANGUAGE_H__63B9D4AB_D0E5_42DD_82C3_F0B734091A39__INCLUDED_)
