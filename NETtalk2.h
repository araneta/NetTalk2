// NETtalk2.h : main header file for the NETTALK2 application
//

#if !defined(AFX_NETTALK2_H__DE5AB55A_43CF_41B1_9CFB_2C08C76CD0B0__INCLUDED_)
#define AFX_NETTALK2_H__DE5AB55A_43CF_41B1_9CFB_2C08C76CD0B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "LIBRARY\ado2.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CNETtalk2App:
// See NETtalk2.cpp for the implementation of this class
//

class CNETtalk2App : public CWinApp
{
public:
	CNETtalk2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNETtalk2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNETtalk2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETTALK2_H__DE5AB55A_43CF_41B1_9CFB_2C08C76CD0B0__INCLUDED_)
