/* Nico 07-4-27 1323 
	This file for MFC only!
	No need to compile for WTL.
 */

#include <stdafx.h>
#include "ListCtrlSort.h"
/*
IMPLEMENT_DYNCREATE(ListViewSort, CListView)

BEGIN_MESSAGE_MAP(ListViewSort, CListView)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()
*/
IMPLEMENT_DYNAMIC(ListCtrlSort, CListCtrl)

BEGIN_MESSAGE_MAP(ListCtrlSort, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()
