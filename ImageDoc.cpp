// ImageDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ImageDoc.h"
#include "ImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, COleServerDoc)

CImageDoc::CImageDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here
	EnableAutomation();
	AfxOleLockApp();

	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1,1);     // dummy value to make CScrollView happy
	
	m_bGray=FALSE;
}

BOOL CImageDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CImageDoc::~CImageDoc()
{
	AfxOleUnlockApp();

	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
	}
}

COleServerItem* CImageDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	// Instead of returning NULL, return a pointer to a new COleServerItem
	//  derived class that is used in conjunction with this document, then
	//  remove the ASSERT(FALSE) below.
	//  (i.e., return new CMyServerItem.)
	ASSERT(FALSE);			// remove this after completing the TODO
	return NULL;
}


BEGIN_MESSAGE_MAP(CImageDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CImageDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//����λͼ����
void CImageDoc::ReplaceHDIB(HDIB hDIB)
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	m_hDIB = hDIB;
	InitDIBData();
	SetModifiedFlag(TRUE);
	//ȡ����ͼ
	CImageView *pView=NULL;
	POSITION curViewPos = GetFirstViewPosition();
	if(curViewPos != NULL)
		pView = (CImageView *)(GetNextView(curViewPos));
	if(pView != NULL)
	{
		pView->SetScrollSizes(MM_TEXT, GetDocSize());
		UpdateAllViews(NULL);
	}
}
//1999-01-29������
void CImageDoc::ReplaceHDIB(HBITMAP hBitmap)
{
	ReplaceHDIB((HDIB)BitmapToDIB(hBitmap,NULL));
}
//����λͼ�������� ��m_palDIB,m_hDIB,m_sizeDoc
void CImageDoc::InitDIBData()
{
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
	}
	if (m_hDIB == NULL)
	{
		return;
	}
	// Set up document size
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	if (::DIBWidth(lpDIB) > INT_MAX ||::DIBHeight(lpDIB) > INT_MAX)
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		CString strMsg="ͼ���ļ�����";
//		strMsg.LoadString(IDS_DIB_TOO_BIG);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return;
	}
	m_sizeDoc = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
	::GlobalUnlock((HGLOBAL) m_hDIB);

	// Create copy of palette
	m_palDIB = new CPalette;
	if (m_palDIB == NULL)
	{
		// we must be really low on memory
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		return;
	}
	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// DIB may not have a palette
		delete m_palDIB;
		m_palDIB = NULL;
		return;
	}

}



BOOL CImageDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//if (!COleServerDoc::OnOpenDocument(lpszPathName))
	//	return FALSE;
	
	// TODO: Add your specialized creation code here

	//���ļ�
/*	CFile file;
	CFileException fe;
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
*/
	DeleteContents(); //ȷ���ĵ���
	BeginWaitCursor(); //���õȴ����

	// replace calls to Serialize with ReadDIBFile function
        //��λͼ
/*	TRY
	{
		m_hDIB = ::ReadDIBFile(file);
	}
        //�쳣����
	CATCH (CFileException, eLoad)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eLoad,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		m_hDIB = NULL;
		return FALSE;
	}
	END_CATCH
*/
	VPic pic;
	if(!pic.CreateFrom(lpszPathName))
	{
		EndWaitCursor();
		m_hDIB = NULL;
		CString strMsg="װ��ͼ��ʱ��������!";
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	m_hDIB = BitmapToDIB(pic.GetHBitmap(),pic.GetPalette());
	//��ʼ��λͼ����
	InitDIBData();

	EndWaitCursor();

    //������DIB�ļ�����ʾ������Ϣ
	if (m_hDIB == NULL)
	{
		// may not be DIB format
		CString strMsg="װ��ͼ��ʱ��������!";
//		strMsg.LoadString(IDS_CANNOT_LOAD_DIB);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
        
    // ���õ�ǰ�ļ�·����������MRU
	SetPathName(lpszPathName);
	
    // �ļ���ʼʱδ�޸�
    SetModifiedFlag(FALSE);     // start off with unmodified
	return TRUE;
}

BOOL CImageDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	//COleServerDoc::OnSaveDocument(lpszPathName);
/*	CFile file;
	CFileException fe;

	if (!file.Open(lpszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	// replace calls to Serialize with SaveDIB function
	BOOL bSuccess = FALSE;
	TRY
	{
		BeginWaitCursor();
		bSuccess = ::SaveDIB(m_hDIB, file);
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH
*/
	BOOL bSuccess = FALSE;
	BeginWaitCursor();
	VPic pic;
	if(!pic.CreateFromDIB(m_hDIB))
	{
		EndWaitCursor();
		CString strMsg="����ͼ��ʱ��������!";
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	bSuccess = pic.SaveImage(lpszPathName);
	//bSuccess = ::SaveDIB(m_hDIB, file);

	EndWaitCursor();
	if (!bSuccess)
	{
		// may be other-style DIB (load supported but not save)
		//  or other problem in SaveDIB
		CString strMsg="����ͼ��ʱ��������!";
//		strMsg.LoadString(IDS_CANNOT_SAVE_DIB);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return bSuccess;
	}
	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);     // back to unmodified

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////
// CImageDoc diagnostics

#ifdef _DEBUG
void CImageDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CImageDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageDoc serialization

// δ�ã���������OnOpenDocument() & OnSaveDocument()
void CImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageDoc commands

//1998.12.21 ����������OnFileSaveAs,��ImagePower
void CImageDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
    if(GetPathName()!="")
		OnSaveDocument(GetPathName());
	else
		OnFileSaveAs();
}

//1999-01-31 
void CImageDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	OnOpenDocument(GetPathName());
}

//1999-01-30 ����,������ͼ
void CImageDoc::UpdateView()
{
	//ȡ����ͼ
	CImageView *pView=NULL;
	POSITION curViewPos = GetFirstViewPosition();
	if(curViewPos != NULL)
		pView = (CImageView *)(GetNextView(curViewPos));
	if(pView != NULL)
	{
		pView->SetScrollSizes(MM_TEXT, GetDocSize());
		UpdateAllViews(NULL);
	}
}
//1999-02-02�����ݣ���ȡ�ĵ�����ͼ
CImageView * CImageDoc::GetView()
{
	CImageView *pView=NULL;
	POSITION curViewPos = GetFirstViewPosition();
	if(curViewPos != NULL)
		pView = (CImageView *)(GetNextView(curViewPos));
	return pView;
}
