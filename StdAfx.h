// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8AA0F96B_836B_4AF4_A408_D0728169B10E__INCLUDED_)
#define AFX_STDAFX_H__8AA0F96B_836B_4AF4_A408_D0728169B10E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#if defined(_WIN64)
 typedef unsigned __int64 ULONG_PTR;
#else
 typedef unsigned long ULONG_PTR;
#endif

typedef ULONG_PTR DWORD_PTR;

#include "library/ado2.h"

class CLearningDlg;
class CBackPro;
#include <vector>
#include <list>
#include "Mmsystem.h"
#pragma comment( lib, "Winmm.lib" )

using namespace std;

typedef struct tag_SUKUKATA
{
	CString sSukuKata; // dalam bentuk fonem	
}SUKUKATA;
typedef struct tag_KATA
{	
	int start,end;
}KATA;

typedef struct tag_PLAYKATA
{
	CString sFolderPath;
	CRITICAL_SECTION guard;	
	volatile int ready,processed,unprocessed,finish,stop;	
	vector<KATA> m_vKata;
}PLAYKATA;

typedef struct tag_GUGUS
{
	CString sGugus;
	CString sFonem;
}GUGUS;
typedef struct tag_KECUALI
{
	CString sKata;
	CString sPengucapan;
}KECUALI;


typedef struct tag_WAVE_HDR{
    char chunk_id[4];
    DWORD chunk_size;
} WAVE_HDR;

typedef struct tag_CHUNK_HDR{
    char form_type[8];
    DWORD hdr_size;
} CHUNK_HDR;

/* specific waveform format structure for PCM data */
typedef struct tag_waveformatpe {
    WORD    wFormatTag;        // format type 
    WORD    nChannels;         // number of channels (i.e. mono, stereo, etc.) 
    DWORD   nSamplesPerSec;    // sample rate
    DWORD   nAvgBytesPerSec;   // for buffer estimation 
    WORD    nBlockAlign;       // block size of data 
    WORD    wBitsPerSample;
} WAVEFORMATPE;

typedef struct tag_DATA_HDR{
    char data_type[4];
    DWORD data_size;
} DATA_HDR;

#define WINDOWSIZE 7
#define CENTERWINDOWPOS 2
#define PI   3.141592
#define ALPHA 0.8
#define BETA 1
#define _TDPSOLA
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8AA0F96B_836B_4AF4_A408_D0728169B10E__INCLUDED_)
