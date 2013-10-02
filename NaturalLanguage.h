// NaturalLanguage.h: interface for the CNaturalLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NATURALLANGUAGE_H__63B9D4AB_D0E5_42DD_82C3_F0B734091A39__INCLUDED_)
#define AFX_NATURALLANGUAGE_H__63B9D4AB_D0E5_42DD_82C3_F0B734091A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "1_5SPAW.h"
#include "TDPSOLA.h"

class CNaturalLanguage  
{	
public:
	void ReloadData();
	enum TipeKalimat {Berita,Tanya,Perintah};			
	void LoadSPAW();
	void SetSPAW(CSPAW *pSpaw);
	void Run();
	void SetConnDB(CADODatabase * db);
	void SetText(CString sText);
	BOOL IsSingkatan(CString sText);			
	//CString KembalikanGugusKonsonan(CString sText);
	CNaturalLanguage();
	virtual ~CNaturalLanguage();	
	CString	m_sFolderPath;	

	void SetStatusCtrl(CRichEditCtrl * pText,CRichEditCtrl * pLog);
	void SetSharedVar(PLAYKATA * playkata);
private:
	DWORD GetHeader(CFile * file, WAVE_HDR *whdr, CHUNK_HDR *chdr, WAVEFORMATPE  *wfmt,DATA_HDR * dthdr);
	void TambahFileSukuKata(CString sSuku);
	void BuatFileKata(CString sKata);
	void CreateResultFile(int index);
	CString m_sResultFileName;

	CString GetFileSukuKata(CString sSukuKata);
	CString HapusSimbol(CString sSimbol);
	CString GantiGugus(CString sText);
	CString IsPengecualian(CString sText);
	void LoadTGugus();
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
	list<CString> m_vSingkatan;
	list<CString> m_vVokal;
	list<CString> m_vSimbol;
	list<GUGUS> m_vGugus;
	list<KECUALI> m_vPengecualian;	
	CString m_sText;	
	CADODatabase *m_pDb;
	CFile m_fLog;
	CTDPSOLA m_tdpsola;

	CRichEditCtrl * m_pText;
	CRichEditCtrl * m_pLog;
	PLAYKATA * m_playkata;
	CString m_sSukuKataAll;
protected:
	void WriteLog(CString sText);
};
#endif // !defined(AFX_NATURALLANGUAGE_H__63B9D4AB_D0E5_42DD_82C3_F0B734091A39__INCLUDED_)
