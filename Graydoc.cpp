// GrayDoc.cpp: implementation of the CGrayDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "GrayDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGrayDoc,CImageDoc)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrayDoc::CGrayDoc()
{
	m_bGray = TRUE;
}

CGrayDoc::~CGrayDoc()
{

}

BOOL CGrayDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if(!CImageDoc::OnOpenDocument(lpszPathName))
		return FALSE;
	if(m_hDIB==NULL)
		return FALSE;
	LPSTR lpbi=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);

	WORD wBitCount;  // DIB bit count
	DWORD dwCompression;
	if (IS_WIN30_DIB(lpbi))    //Windows 3.0λͼ
	{
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
		dwCompression=((LPBITMAPINFOHEADER)lpbi)->biCompression;
	}
	else                       //OS/2λͼ
	{
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
		dwCompression=BI_RGB;  //δѹ��
	}
	::GlobalUnlock((HGLOBAL)m_hDIB);

	if(wBitCount != 8 || dwCompression != BI_RGB)
	{
		AfxMessageBox("����256���Ҷ�λͼ");
		return FALSE;
	}
	else
	{
		m_bGray=TRUE;
		return TRUE;
	}
}

BOOL CGrayDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return CImageDoc::OnSaveDocument(lpszPathName);	
}
