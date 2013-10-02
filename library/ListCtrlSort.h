#pragma once

#include "deelx.h"
#define MATCH_NUMBERS _T("[+-]?\\d+(\\.\\d+)?")

#if !defined(__AFXCMN_H__) && !defined(__ATLCTRLS_H__)
#error neither afxcmn.h nor atlctrls.h being included!
#endif

#define LVM_SORTITEMSEX          (LVM_FIRST + 81)
template<class T>
struct ListCtrlSortHelper
{
	ListCtrlSortHelper() : nSortColumn_(0), bSortAscending_(true){}
	int nSortColumn_;
	BOOL bSortAscending_;

	static int Compare(CString const& csText1, CString const& csText2)
	{
		static CRegexp reg(MATCH_NUMBERS);
		int nEnd1 = 0, nEnd2 = 0;
		
		while(1)
		{
			MatchResult ret1 = reg.Match(csText1, nEnd1);
			MatchResult ret2 = reg.Match(csText2, nEnd2);
			
			CString const& csSubText1 = ret1.IsMatched() ? csText1.Mid(nEnd1, ret1.GetStart() - nEnd1) : csText1;
			CString const& csSubText2 = ret2.IsMatched() ? csText2.Mid(nEnd2, ret2.GetStart() - nEnd2) : csText2;
			
			if (csSubText1 != csSubText2)
				return csSubText1 > csSubText2 ? 1 : -1;
			
			/* Nico 07-4-25 1342 
				既然两个字符串相等, 而两个字符串又已经消耗完, 那自然可以返回了.
			*/
			if (! ret1.IsMatched() && ! ret2.IsMatched()) return 0;
			
			if (ret1.IsMatched() && ret2.IsMatched())
			{
				/* Nico 07-4-25 1355 
					GetGroupStart() 返回正则表达式中 "(\\.\\d+)" 这一个组的起始位置. 如果没有匹配, 则返回 -1; (可查看 deelx 的说明文档) 
					这个组是否匹配意味着捕获的数字是否带有小数点以及小数部分.
					如果捕获的两个数字有一个带有小数部分. 则按浮点数处理. 否则按整数处理
				*/
				if (ret1.GetGroupStart(1) >= 0 || ret2.GetGroupStart(1) >= 0)
				{
					double dNum1 = _tcstod(csText1.Mid(ret1.GetStart(), ret1.GetEnd() - ret1.GetStart()), 0);
					double dNum2 = _tcstod(csText2.Mid(ret2.GetStart(), ret2.GetEnd() - ret2.GetStart()), 0);
					if (dNum1 != dNum2) return dNum1 > dNum2 ? 1 : -1;
				}
				else
				{
					__int64 nNum1 = _ttoi64(csText1.Mid(ret1.GetStart(), ret1.GetEnd() - ret1.GetStart()));
					__int64 nNum2 = _ttoi64(csText2.Mid(ret2.GetStart(), ret2.GetEnd() - ret2.GetStart()));
					if (nNum1 != nNum2) return nNum1 > nNum2 ? 1 : -1;
				}
				
				nEnd1 = ret1.GetEnd();
				nEnd2 = ret2.GetEnd();
			}
			else
			{
				/* Nico 07-4-25 1501 
					现在两个里面肯定有且只有一个 IsMatch() 不为真
				*/
				return ret1.IsMatched() ? 1 : -1;
			}
		}
		return 0;
	}

	static int CALLBACK CompareProc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
	{
		T* pT = (T*)(ListCtrlSortHelper*)lParamData;

		CString csText1, csText2;
		LV_ITEM lvi;
		lvi.iSubItem = pT->nSortColumn_;
		lvi.cchTextMax = MAX_PATH;
		lvi.pszText = csText1.GetBufferSetLength(MAX_PATH);
		csText1.ReleaseBuffer(SendMessage(pT->m_hWnd, LVM_GETITEMTEXT, lParam1, (LPARAM)&lvi));
		lvi.pszText = csText2.GetBufferSetLength(MAX_PATH);
		csText2.ReleaseBuffer(SendMessage(pT->m_hWnd, LVM_GETITEMTEXT, lParam2, (LPARAM)&lvi));

		return pT->bSortAscending_ ? Compare(csText1, csText2) : Compare(csText2, csText1);
	}
	
	LRESULT OnColumnClick_(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
	{
		LRESULT lres = 0;
		OnColumnClick(pNMHDR, &lres);
		return lres;
	}

	void OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
	{
		T* pT = (T*)this;
		*pResult = 0;
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		const int nCol = pNMListView->iSubItem;
		nSortColumn_ = nCol;
		bSortAscending_ = (nCol == nSortColumn_ ? ! bSortAscending_ : TRUE);

		SendMessage(pT->m_hWnd, LVM_SORTITEMSEX, (WPARAM)this, (LPARAM)T::CompareProc);
		SendMessage(pT->m_hWnd, LVM_FIRST + 140/*LVM_SETSELECTEDCOLUMN*/, nSortColumn_, 0);
	}
};

#ifdef __AFXCMN_H__
/*
class ListViewSort : public CListView, public ListCtrlSortHelper<ListViewSort>
{
	void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult ) 
	{ ListCtrlSortHelper<ListViewSort>::OnColumnClick(pNMHDR, pResult);}

	DECLARE_DYNCREATE(ListViewSort)
	DECLARE_MESSAGE_MAP()
};
*/
class ListCtrlSort : public CListCtrl, public ListCtrlSortHelper<ListCtrlSort>
{
	void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult ) 
	{ ListCtrlSortHelper<ListCtrlSort>::OnColumnClick(pNMHDR, pResult);}
	
	DECLARE_DYNAMIC(ListCtrlSort)
	DECLARE_MESSAGE_MAP()
};

#endif