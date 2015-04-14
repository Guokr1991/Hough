// UnRedoStack.cpp: implementation of the CUnRedoStack class.
//	����/������ջ�����ݣ�1999��3��17��
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "UnRedoStack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnRedoStack::CUnRedoStack()
{
	m_bEnableRedo = TRUE;
	m_nDeep = 5;
}

CUnRedoStack::~CUnRedoStack()
{
	while (!m_listUndo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listUndo.RemoveHead());
	}
	while (!m_listRedo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listRedo.RemoveHead());
	}
}

//1999-03-17�����ݣ����ƾ��
HGLOBAL CUnRedoStack::CopyHandle(HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

//1999-03-17�����ݣ���Undoջ����¾��
void CUnRedoStack::PushUndo(HGLOBAL handle)
{
	if(m_listUndo.GetCount() >= (int)m_nDeep)
		::GlobalFree(m_listUndo.RemoveTail());
	m_listUndo.AddHead(handle);
}
//1999-03-17�����ݣ���Redoջ����¾��
void CUnRedoStack::PushRedo(HGLOBAL handle)
{
	if(m_listRedo.GetCount() >= (int)m_nDeep)
		::GlobalFree(m_listRedo.RemoveTail());
	m_listRedo.AddHead(handle);
}

/* 1999-03-17�����ݣ�ִ��Undo
   hData:�����浽Redoջ�е�����
ִ�в���
   1��Redo = hData
   2����m_listUndo��ȡ���
ע��hData�ڱ����ƺ���ջ�����Կ�������ջ���ͷţ���Ӱ��
    Undo/Redo  
*/
HGLOBAL CUnRedoStack::Undo(HGLOBAL hData)
{
	if(m_bEnableRedo)
	{
		PushRedo(CopyHandle(hData));
	}
	return m_listUndo.IsEmpty()?NULL:m_listUndo.RemoveHead();
}

/* 1999-03-17�����ݣ�ִ��Redo
   hData:�����浽Undoջ�е�����
ִ�в���
   1��Undo = hData
   2����m_listRedo��ȡ���
ע��hData�ڱ����ƺ���ջ�����Կ�������ջ���ͷţ���Ӱ��
    Undo/Redo  
*/
HGLOBAL CUnRedoStack::Redo(HGLOBAL hData)
{
	PushUndo(CopyHandle(hData));
	return m_listRedo.IsEmpty()?NULL:m_listRedo.RemoveHead();
}

//1999-03-18,���ݣ����ö�ջ��ȣ���ɾ��������ʷ���ݣ�
void CUnRedoStack::SetStackDeep(UINT nNewDeep)
{
	while (!m_listUndo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listUndo.RemoveHead());
	}
	while (!m_listRedo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listRedo.RemoveHead());
	}
	m_nDeep = nNewDeep;
}
