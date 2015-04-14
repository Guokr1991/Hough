// UnRedoStack.h: interface for the CUnRedoStack class.
//	����/������ջ�����ݣ�1999��3��17��
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_UNREDOSTACK_H__149B1A21_DC4C_11D2_A8BD_44B1F7C00000__INCLUDED_)
#define AFX_UNREDOSTACK_H__149B1A21_DC4C_11D2_A8BD_44B1F7C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>       //ģ����


/*ʹ�÷���
��������
CUnRedoStack CYourView::m_stkUnRedo;

void CYourView::BeginProcessing()
{
	//......

	HGLOBAL hData = ..... ;//�ڴ�׼��Undo����
	m_stkUnRedo.PushUndo(hData);

	//......

}

void CYourView::OnEditUndo() 
{
	HGLOBAL hNew = NULL;
	HGLOBAL hData = ..... ;//�ڴ�׼��Undo����

	if (m_stkUnRedo.HasUndo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Undo(hData);
		
		if (hNew != NULL)
		{
			//�ڴ˴����ø�Undo���
		}
		EndWaitCursor();
	}
}

void CImageView::OnEditRedo() 
{
	HGLOBAL hNew = NULL;
	HGLOBAL hData = ..... ;//�ڴ�׼��Undo����

	if (m_stkUnRedo.HasRedo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Redo(hData);

		if (hNew != NULL)
		{
			//�ڴ˴����ø�Redo���
		}
		EndWaitCursor();
	}
}

*/

class CUnRedoStack : public CObject  
{
public:
	HGLOBAL Redo(HGLOBAL hData);
	HGLOBAL Undo(HGLOBAL hData);
	BOOL HasUndo(){	return !m_listUndo.IsEmpty();}
	BOOL HasRedo(){	return !m_listRedo.IsEmpty();}

	void PushUndo(HGLOBAL handle);
	void PushRedo(HGLOBAL handle);
	HGLOBAL CopyHandle (HGLOBAL h);
	CUnRedoStack();
	virtual ~CUnRedoStack();

	void SetStackDeep(UINT nNewDeep);
	UINT GetStackDeep(){return m_nDeep;}

	BOOL m_bEnableRedo;//�Ƿ���������
protected:
	UINT m_nDeep;//��ջ���
	CTypedPtrList<CPtrList,HGLOBAL>     m_listUndo;
	CTypedPtrList<CPtrList,HGLOBAL>     m_listRedo;

};

#endif // !defined(AFX_UNREDOSTACK_H__149B1A21_DC4C_11D2_A8BD_44B1F7C00000__INCLUDED_)
