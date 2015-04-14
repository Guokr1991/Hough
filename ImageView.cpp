// ImageView.cpp : implementation file
//

#include "stdafx.h"
#include "ImageView.h"
#include "ImageDoc.h"

//{{
#include "MainFrm.h"
//}}
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageView

IMPLEMENT_DYNCREATE(CImageView, CFormView)

BEGIN_MESSAGE_MAP(CImageView, CFormView)
	//{{AFX_MSG_MAP(CImageView)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CImageView::CImageView()
	: CFormView(CImageView::IDD)
{
	//{{AFX_DATA_INIT(CImageView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	InitData();
}

CImageView::CImageView( LPCTSTR lpszTemplateName )
	:CFormView(lpszTemplateName) 
{
	InitData();
}

CImageView::CImageView( UINT nIDTemplate )
	:CFormView(nIDTemplate)
{
	InitData();
}
//��ʼ����Ա����
void CImageView::InitData()
{
	m_nUndoStackDeep = 5;//Undo/Redo��ջ���
	m_nAutoCommand=NULL;
	m_bAutoProcessing=FALSE;
	m_nRegionCombineMode=RGN_COPY;
	m_nRegionFillMode=WINDING;
	m_nZoomFactor=100; //100%
	m_nMouseFunction=MOUSE_REGION;
    m_nRegionTool=RGNMARK_FREEHAND;

	m_stkUnRedo.SetStackDeep(m_nUndoStackDeep);
}

CImageView::~CImageView()
{
	m_Region.Clear();
	m_Region.DeleteObject();
}

void CImageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CImageView diagnostics

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CFormView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CImageDoc* CImageView::GetDocument() // non-debug version is inline
{
//  Ϊ�����������أ�ȥ���ö��� 
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDoc)));
	return (CImageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageView message handlers
//�ĵ�������ͻ�������任
void CImageView::ClientToDoc(CPoint& point)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.DPtoLP(&point);
	point.x+=GetScrollPos(SB_HORZ),
	point.y+=GetScrollPos(SB_VERT);

}

void CImageView::ClientToDoc(CRect& rect)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.DPtoLP(rect);
	rect.left+=GetScrollPos(SB_HORZ),
	rect.right+=GetScrollPos(SB_HORZ),
	rect.top+=GetScrollPos(SB_VERT);
	rect.bottom+=GetScrollPos(SB_VERT);

	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom <= rect.top);
}

//�ú���������
void CImageView::ClientToDoc(CRgn& rgn)
{
	rgn.OffsetRgn(GetScrollPos(SB_HORZ),
	              GetScrollPos(SB_VERT));
}

void CImageView::DocToClient(CPoint& point)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.LPtoDP(&point);
	point.x-=GetScrollPos(SB_HORZ),
	point.y-=GetScrollPos(SB_VERT);

}

void CImageView::DocToClient(CRect& rect)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.LPtoDP(rect);
	rect.left-=GetScrollPos(SB_HORZ),
	rect.right-=GetScrollPos(SB_HORZ),
	rect.top-=GetScrollPos(SB_VERT);
	rect.bottom-=GetScrollPos(SB_VERT);
	rect.NormalizeRect();
}

//�ú���������
void CImageView::DocToClient(CRgn& rgn)
{
	rgn.OffsetRgn(-GetScrollPos(SB_HORZ),
	              -GetScrollPos(SB_VERT));

}

/////////////////////////////////////////////////////////////////////////////
// CImageView ����
void CImageView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);

	// TODO: calculate the total size of this view
    CSize size=GetDocument()->GetDocSize();
	SetScrollSizes(MM_TEXT, size);

	//���ó�ʼ����Ϊȫͼ��
	CRgn rgnImage;
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
	m_Region.DeleteObject();
	m_Region.Attach(rgnImage.Detach());

}

void CImageView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(pDC);
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->GetHDIB();

	//1998.12.26���ñ���ɫ
	CBrush brush(HS_DIAGCROSS,RGB(192,192,192) );
	if(!pDC->IsPrinting())
	{
		pDC->SetBkColor(RGB(255,255,255));
		CRect rectWnd;
		GetClientRect(&rectWnd);
		rectWnd.right*=5;rectWnd.bottom*=5;//��ʱ�ô��²�
		pDC->FillRect(&rectWnd,&brush);
	}
	if (hDIB != NULL)
	{
		//�ɾ��ȡ��λͼָ��	
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
		//�ɾ��ȡ��λͼ���	
		int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
		int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
		
		::GlobalUnlock((HGLOBAL) hDIB);
		

		//ȷ����ʾ�����С
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		
	
		if (pDC->IsPrinting())   // printer DC
		{
			// ȡ�ô�ӡ��ֽ�Ŵ�С(in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			// ȡ�ô�ӡ�� pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			//
			// Best Fit case -- create a rectangle which preserves
			// the DIB's aspect ratio, and fills the page horizontally.
			//
			// The formula in the "->bottom" field below calculates the Y
			// position of the printed bitmap, based on the size of the
			// bitmap, the width of the page, and the relative size of
			// a printed pixel (cyInch / cxInch).
			//
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
		}
		else   // not printer DC
		{
			rcDest = rcDIB;
			//{{  ͼ��������ʾ
		    rcDest.bottom=int(rcDest.bottom * m_nZoomFactor/100);    
			rcDest.right =int(rcDest.right *  m_nZoomFactor/100); 
			//}}
		}
		
		//��ʾ
	    pDC->FillRect(&rcDest,&brush);
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());

		//����ѡ������ı߿�
		if(!pDC->IsPrinting())
		{
			CBrush brush(RGB(0,128,128));
			pDC->SetROP2(R2_NOT);
			pDC->FrameRgn(&m_Region,&brush,1,1);
			pDC->SetROP2(R2_COPYPEN);
		}

	}
}
//ͨ�ô���ʼ����
//����    :����m_entryImage,���õȴ����,׼��Undo
//����ֵ   TRUE  �ɹ�
//         FALSE ʧ��
BOOL CImageView::BeginProcessing()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_entryImage.hDIB=NULL;					//Դλͼ���
	m_entryImage.hNewDIB=NULL;				//����Ŀ��λͼ���
	m_entryImage.pImage=NULL;				//Դλͼ���ݿ�ʼλ�����ڻҶ�ͼ��
	m_entryImage.pNewImage=NULL;			//Ŀ��λͼ���ݿ�ʼλ�����ڻҶ�ͼ��
	m_entryImage.hBitmap=NULL;				//Դ��ʱ�м��豸���λͼ���
	m_entryImage.hNewBitmap=NULL;			//Ŀ����ʱ�м��豸���λͼ���
	m_entryImage.hMemDC=NULL;				//Դλͼ�豸
	m_entryImage.hNewDC=NULL;				//Ŀ��λͼ�豸
	m_entryImage.rectImage=CRect(0,0,1,1);		//��Χѡ���������С����
	m_entryImage.sizeImage=CSize(1,1);		//ͼ���С

	if(pDoc->GetHDIB()==NULL) return FALSE;
	//�����Χ�������С����
	if(m_Region.m_hObject == NULL ) return FALSE;
	m_Region.GetRgnBox(&(m_entryImage.rectImage));
	if(m_entryImage.rectImage.right-2<=m_entryImage.rectImage.left ||
		m_entryImage.rectImage.bottom-2<=m_entryImage.rectImage.top)
	{
		AfxMessageBox(_T("ѡ���Ĵ���������Ч"));
		return FALSE;
	}
	BeginWaitCursor();
//	ReplaceHandleForUndo((HGLOBAL)pDoc->GetHDIB());
	m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
	
	m_entryImage.sizeImage.cx=pDoc->GetDocSize().cx;
	m_entryImage.sizeImage.cy=pDoc->GetDocSize().cy;
	return TRUE;
}
///////////////////////////////////////////////////
//�Ҷȴ���ʼ����
//����ֵ   TRUE  �ɹ�
//         FALSE ʧ��
//         pNewImage��pImage�ĸ���
///////////////////////////////////////////////////
BOOL CImageView::BeginProcessingGray()
{

	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BeginProcessing();
	
	LPBITMAPINFOHEADER  lpBi,lpNewBi;

	m_entryImage.hDIB=pDoc->GetHDIB();
	lpBi=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)	m_entryImage.hDIB);//ȡ��ԭ��λͼ
	m_entryImage.pImage=(BYTE FAR*)::FindDIBBits((LPSTR)lpBi);

	m_entryImage.hNewDIB=(HDIB)::CopyHandle((HGLOBAL)m_entryImage.hDIB);//����һ����λͼ
	lpNewBi=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)m_entryImage.hNewDIB);
	m_entryImage.pNewImage=(BYTE FAR*)::FindDIBBits((LPSTR)lpNewBi);
	return TRUE;
}
///////////////////////////////////////////////////
//��ɫ����ʼ����
//����ֵ   TRUE  �ɹ�
//         FALSE ʧ��
//         hNewBitmap��hBitmap�ĸ���
//         hNewDC����hNewBitmap, hMemDC����hBitmap

BOOL CImageView::BeginProcessingColor()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BeginProcessing();
	
	HDC hDC=::GetDC(NULL);
	m_entryImage.hMemDC=::CreateCompatibleDC(hDC);
	m_entryImage.hNewDC=::CreateCompatibleDC(hDC);
	
	m_entryImage.hBitmap=::DIBToBitmap(pDoc->GetHDIB(), 
		//HPALETTE(pDoc->GetDocPalette()->m_hObject));
		NULL);
	m_entryImage.hNewBitmap=::DIBToBitmap(pDoc->GetHDIB(),
		//HPALETTE(pDoc->GetDocPalette()->m_hObject));
		NULL);
/*	//{{test
	m_entryImage.hBitmap=(HBITMAP)::CopyHandle(pDoc->GetHDIB());
	m_entryImage.hNewBitmap=(HBITMAP)::CopyHandle(pDoc->GetHDIB());//����һ����λͼ
	//}}
*/
	if(!m_entryImage.hBitmap || !m_entryImage.hNewBitmap) return FALSE;
	if(!::SelectObject(m_entryImage.hMemDC,m_entryImage.hBitmap))	return FALSE;
	if(!::SelectObject(m_entryImage.hNewDC,m_entryImage.hNewBitmap)) return FALSE;

	::SelectClipRgn(m_entryImage.hNewDC,m_Region);
	::DeleteDC(hDC);

	return TRUE;
}

BOOL CImageView::EndProcessing(BOOL bSuccess)
{
	EndWaitCursor();
	if(bSuccess)
	{
		UpdateDocument(m_entryImage.hNewDIB);
		return TRUE;
	}
	return FALSE;
}

BOOL CImageView::EndProcessingGray(BOOL bSuccess )
{
	::GlobalUnlock((HGLOBAL)m_entryImage.hDIB);
	::GlobalUnlock((HGLOBAL)m_entryImage.hNewDIB);
	return EndProcessing(bSuccess );
}

BOOL CImageView::EndProcessingColor(BOOL bSuccess )
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_entryImage.hNewDIB=::BitmapToDIB(m_entryImage.hNewBitmap, 
		//HPALETTE(pDoc->GetDocPalette()->m_hObject));// ʹ���ĵ��ĵ�ɫ��
		NULL);
//	m_entryImage.hNewDIB=(HDIB)::CopyHandle(m_entryImage.hNewBitmap);						  
	BOOL bFlag=FALSE;
	if(m_entryImage.hNewDIB!=NULL)	
	{
		bFlag=EndProcessing(bSuccess);
	}
	else
	{
		EndWaitCursor();
	}

	::DeleteDC(m_entryImage.hMemDC);
	::DeleteDC(m_entryImage.hNewDC);
	::DeleteObject(m_entryImage.hBitmap);
	::DeleteObject(m_entryImage.hNewBitmap);
	::DeleteObject(m_entryImage.hNewDIB);
	return bFlag;

}

/////////////////////////////////////////////////////////////////////////////
// CImageView ������

void CImageView::OnEditClear() 
{
	if (OpenClipboard())
	{
		EmptyClipboard();
		CloseClipboard();
	}
	
}

void CImageView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
	//��������Ϊ�գ�ID_EDIT_CLEAR�˵������
}
void CImageView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CImageDoc* pDoc = GetDocument();
	// Clean clipboard of contents, and copy the DIB.

	if (OpenClipboard())
	{
		BeginWaitCursor();
		EmptyClipboard();
		SetClipboardData (CF_DIB, ::CopyHandle((HANDLE) pDoc->GetHDIB()) );
		CloseClipboard();
		EndWaitCursor();
	}
	
}

void CImageView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
    //���ڴ�DIBΪ�գ�ID_EDIT_COPY�˵������
	
}

void CImageView::OnEditCut() 
{
	// TODO: Add your command handler code here
	CImageDoc* pDoc = GetDocument();
	// Clean clipboard of contents, and copy the DIB.
	BeginWaitCursor();
	if (pDoc->GetHDIB() != NULL)
	{
	 
		if (OpenClipboard())
			{
			EmptyClipboard();
			SetClipboardData (CF_DIB, ::CopyHandle((HANDLE) pDoc->GetHDIB()) );
			CloseClipboard();
			}
		pDoc->ReplaceHDIB((HDIB)NULL); // and free the old DIB
		pDoc->InitDIBData();    // set up new size & palette
		pDoc->SetModifiedFlag(TRUE);

		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
//		OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
	EndWaitCursor();
	
}

void CImageView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
	//���ڴ�DIBΪ�գ�ID_EDIT_CUT�˵������
	
}

void CImageView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	HDIB hNewDIB = NULL;

	if (OpenClipboard())
	{
		BeginWaitCursor();

		hNewDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));

		CloseClipboard();

		if (hNewDIB != NULL)
		{
			CImageDoc* pDoc = GetDocument();
			pDoc->ReplaceHDIB(hNewDIB); // and free the old DIB
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
//			OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
	
}

void CImageView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
	//��������Ϊ�գ�ID_EDIT_PASTE�˵������
	
}
void CImageView::OnEditRedo() 
{
	CImageDoc* pDoc = GetDocument();

	HGLOBAL hNew = NULL;
	HGLOBAL hData = pDoc->GetHDIB() ;//�ڴ�׼��Undo����

	if (m_stkUnRedo.HasRedo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Redo(hData);

		if (hNew != NULL)
		{
			//�ڴ˴����ø�Redo���
			pDoc->ReplaceHDIB((HDIB)hNew); // and free the old DIB
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
}

void CImageView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_stkUnRedo.HasRedo());
    //��Redo���ݾ��Ϊ�գ�ID_EDIT_REDO�˵������
}
void CImageView::OnEditSelectAll() 
{
	// TODO: Add your command handler code here
	//��������Ϊȫͼ��
	if(GetDocument()==NULL) return;
    CSize size=GetDocument()->GetDocSize();
	CRgn rgnImage;
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
	m_Region.DeleteObject();
	m_Region.Attach(rgnImage.Detach());
	GetDocument()->UpdateAllViews(NULL);  //������ʾ�����Ե
}

void CImageView::OnEditUndo() 
{
	CImageDoc* pDoc = GetDocument();

	HGLOBAL hNew = NULL;
	HGLOBAL hData = pDoc->GetHDIB() ;//�ڴ�׼��Undo����

	if (m_stkUnRedo.HasUndo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Undo(hData);
		
		if (hNew != NULL)
		{
			//�ڴ˴����ø�Undo���
			pDoc->ReplaceHDIB((HDIB)hNew); // and free the old DIB
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
}

void CImageView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_stkUnRedo.HasUndo());
    //��Undo���ݾ��Ϊ�գ�ID_EDIT_UNDO�˵������
	
}







/////////////////////////////////////////////////////////////////////////////
// CImageView ��ӡ
/*MFC��Ĭ�ϴ�ӡ����
ID_FILE_PRINT						 \
   -> CView::OnFilePrint()            \
ID_FILE_PRINT_PREVIEW                  | ==>  
   -> CView::OnFilePrintPreview()     /
         CView::DoPrintPreview       /
										
			CMyView::OnPreparePrinting(CPrintInfo* pInfo )
				CView::DoPreparePrinting( CPrintInfo* pInfo )
					�Ի��� CPrintDialog.DoModal();
					������ӡ�豸������
					��ӡ���̶Ի���AFX_IDD_PRINTDLG
			CMyView::OnBeginPrinting( CDC* pDC, CPrintInfo* pInfo )
			CDC::StartDoc( LPDOCINFO lpDocInfo )
	 			����> CMyView::OnPrepareDC( CDC* pDC, CPrintInfo* pInfo = NULL )
	 			��    CDC::StartPage()
	 			��    CMyView::OnPrint( CDC* pDC, CPrintInfo* pInfo )
	 			��              CMyView::OnDraw(CDC* pDC)
	 			��					{
     			��						if(pDC->IsPrinting())  {.......}
     			��					}
	 			����  CDC::EndPage()
			CDC::EndDoc()
CMyView::OnEndPrinting( CDC* pDC, CPrintInfo* pInfo )
*/	

BOOL CImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CView::OnBeginPrinting(pDC,pInfo);
}

void CImageView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CView::OnEndPrinting(pDC,pInfo);
}

void CImageView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add code to print the controls
	CView::OnPrint(pDC,  pInfo);//call OnDraw(pDC)
}

void CImageView::UpdateDocument(HDIB hNewDIB)
{
	if (hNewDIB != NULL)
	{
		CImageDoc* pDoc = GetDocument();
		CSize sizeOld= pDoc->GetDocSize();
		pDoc->ReplaceHDIB(hNewDIB); // and free the old DIB
//		pDoc->InitDIBData();    // set up new size & palette
//		pDoc->SetModifiedFlag(TRUE);
//		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
//		OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
//		pDoc->UpdateAllViews(NULL);
		CSize size=pDoc->GetDocSize();
		SetScrollSizes(MM_TEXT, size);

		//����С�ı�����������
		if(size.cx == sizeOld.cx &&	size.cy == sizeOld.cy)
			return;
		else
		{
			CRgn rgnImage;
			rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
			m_Region.DeleteObject();
			m_Region.Attach(rgnImage.Detach());
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageView ��괦��

void CImageView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		LButtonDblClkRegion(nFlags,point);
		break;
	};
}
void CImageView::LButtonDblClkRegion(UINT nFlags, CPoint point)
{	//ȡ����ǰѡ�����򣬻ָ�Ϊȫͼ��
	m_Region.Clear();
	OnEditSelectAll();
	CFormView::OnLButtonDblClk(nFlags, point);
}

void CImageView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Pressing the mouse button in the view window starts a new stroke
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		LButtonDownRegion(nFlags,point);
		break;
	};
}

void CImageView::LButtonDownRegion(UINT nFlags, CPoint point)
{
	if( m_nRegionTool == RGNMARK_NONE)
		return;
	SetCapture();       // Capture the mouse until button up.
	m_ptPrev = point;   // Serves as the MoveTo() anchor point for the
						// LineTo() the next point, as the user drags the
						// mouse.
	switch(m_nRegionTool)
	{
	case RGNMARK_FREEHAND:
		m_Region.Clear();
		// Add first point to the new stroke
		m_Region.m_pointArray.Add(point);
		break;
	case RGNMARK_RECT:
	case RGNMARK_ROUNDRECT:
	case RGNMARK_ELLIPSE:
		m_ptPrev2 = point;
		break;
	};

	
}

void CImageView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		LButtonUpRegion(nFlags,point);
		break;
	};
}

/*
������ȡ��
void Process(HDC hDC,CRect rect)
{
	for(int x=rect.left;x<=rect.right;x++)
		for(int y=rect.top;y<=rect.bottom;y++)
		{
			::SetPixelV(hDC,x,y,0x00FFFFFF-::GetPixel(hDC,x,y));
		}
}
*/
void CImageView::LButtonUpRegion(UINT nFlags, CPoint point)
{
	if( m_nRegionTool == RGNMARK_NONE)
		return;

	CImageDoc* pDoc = GetDocument();
	CSize size=pDoc->GetDocSize();
	CClientDC dc(this);

	CRgn rgn;//��ͼ����������
	CRgn rgnImage;//ȫͼ������
//	rgnImage.DeleteObject();
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);

	if (GetCapture() != this)
		return; // If this window (view) didn't capture the mouse,
				// then the user isn't drawing in this window.

	// Mouse button up is interesting in the Scribble application
	// only if the user is currently drawing a new stroke by dragging
	// the captured mouse.
	switch(m_nRegionTool)
	{
	case RGNMARK_FREEHAND:
//		dc.MoveTo(m_ptPrev);
//		dc.LineTo(point);
		m_Region.m_pointArray.Add(point);

		//��������
		if(!m_Region.CreateRegion(&rgn,m_nRegionFillMode))
		{
			AfxMessageBox(_T("����ѡ��ʧ��"));
			return; 
		}
	
		break;
	case RGNMARK_RECT:
		rgn.CreateRectRgn(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		break;
	case RGNMARK_ELLIPSE:
		rgn.CreateEllipticRgn(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		break;
	case RGNMARK_ROUNDRECT:
		rgn.CreateRoundRectRgn(m_ptPrev.x,m_ptPrev.y,point.x,point.y,
			20,20);//Բ�ǰ뾶20
		break;
	};
	
	if(rgn.m_hObject == NULL)
	{
		rgn.CreateRectRgn(0,0,0,0);
	}
	ClientToDoc(rgn);  //ת��Ϊ�ĵ�����
	//��������ͼ��Ľ�
	rgn.CombineRgn(&rgn,&rgnImage,RGN_AND);
	//��������ϲ�ģʽȷ��Ŀ������
	if(m_nRegionCombineMode== RGN_DIFF)
		rgn.CombineRgn(&m_Region,&rgn,m_nRegionCombineMode);
	else
		rgn.CombineRgn(&rgn,&m_Region,m_nRegionCombineMode);
	//�õ�ǰ����Ϊ������
	m_Region.DeleteObject();
	m_Region.Attach(rgn.Detach());

	pDoc->UpdateAllViews(NULL);
	ReleaseCapture();   // Release the mouse capture established at
							// the beginning of the mouse drag.
	
	AutoProcessing();//�Զ�����
	return;
}

void CImageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		MouseMoveRegion(nFlags,point);
		break;
	};
}

void CImageView::MouseMoveRegion(UINT nFlags, CPoint point) 
{
	if((nFlags & MK_LBUTTON) !=0  )//�������
	{
		//do somthing
	}
	else//���δ����
	{
		return;
	}
	if (GetCapture() != this)
			return; // If this window (view) didn't capture the mouse,
					// then the user isn't drawing in this window.

	CImageDoc* pDoc = GetDocument();
	CSize size=pDoc->GetDocSize();
	CClientDC dc(this);

	CRgn rgn;
	CRgn rgnImage;
		CBrush* pOldBrush;
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
	switch(m_nRegionTool)
	{
	case RGNMARK_FREEHAND:
		m_Region.m_pointArray.Add(point);

		//����ǰ�ü�����Ϊ������
		dc.SelectClipRgn(&rgnImage);
	
		dc.MoveTo(m_ptPrev);
		dc.LineTo(point);
		m_ptPrev = point;
		break;
	case RGNMARK_RECT:
		//����ϴλ���
		pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.SetROP2(R2_NOT);//��ɫ
		dc.Rectangle(m_ptPrev.x,m_ptPrev.y,m_ptPrev2.x,m_ptPrev2.y);
		   //��������
		m_ptPrev2 = point;
		dc.Rectangle(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		dc.SelectObject(pOldBrush);
		break;
	case RGNMARK_ELLIPSE:
		//����ϴλ���
		pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.SetROP2(R2_NOT);//��ɫ
		dc.Ellipse(m_ptPrev.x,m_ptPrev.y,m_ptPrev2.x,m_ptPrev2.y);
		   //��������
		m_ptPrev2 = point;
		dc.Ellipse(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		dc.SelectObject(pOldBrush);
		break;
	case RGNMARK_ROUNDRECT:
		//����ϴλ���
		pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.SetROP2(R2_NOT);//��ɫ
		dc.RoundRect(m_ptPrev.x,m_ptPrev.y,m_ptPrev2.x,m_ptPrev2.y,
			20,20);//Բ�ǰ뾶20
		   //��������
		m_ptPrev2 = point;
		dc.RoundRect(m_ptPrev.x,m_ptPrev.y,point.x,point.y,
			20,20);
		dc.SelectObject(pOldBrush);
		break;
	};
	return;
}




BOOL CImageView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->GetHDIB();

/*	//1998.12.26���ñ���ɫ
	CClientDC dc(this);
	dc.SetBkColor(RGB(255,255,255));
	CBrush brush(HS_DIAGCROSS,RGB(192,192,192) );
	CRect rectWnd;
	GetClientRect(&rectWnd);
	dc.FillRect(&rectWnd,&brush);
*/
//	Invalidate();
	return CFormView::OnScroll(nScrollCode, nPos, bDoScroll);
}


/*1998.12.27 �Ҷ�ͼ��ʹ�ã�����
	m_entryImage.hDIB			//Դλͼ���
	m_entryImage.pImage			//Դλͼ���ݿ�ʼλ�����ڻҶ�ͼ��
	m_entryImage.rectImage		//��Χѡ���������С����
	m_entryImage.sizeImage		//ͼ���С

*/
BOOL CImageView::BeginUseGray()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->GetHDIB()==NULL) return FALSE;

	m_entryImage.hDIB=NULL;					//Դλͼ���
	m_entryImage.hNewDIB=NULL;				//����Ŀ��λͼ���
	m_entryImage.pImage=NULL;				//Դλͼ���ݿ�ʼλ�����ڻҶ�ͼ��
	m_entryImage.pNewImage=NULL;			//Ŀ��λͼ���ݿ�ʼλ�����ڻҶ�ͼ��
	m_entryImage.hBitmap=NULL;				//Դ��ʱ�м��豸���λͼ���
	m_entryImage.hNewBitmap=NULL;			//Ŀ����ʱ�м��豸���λͼ���
	m_entryImage.hMemDC=NULL;				//Դλͼ�豸
	m_entryImage.hNewDC=NULL;				//Ŀ��λͼ�豸
	m_entryImage.rectImage=CRect(0,0,1,1);		//��Χѡ���������С����
	m_entryImage.sizeImage=CSize(1,1);		//ͼ���С

	//�����Χ�������С����
	m_Region.GetRgnBox(&(m_entryImage.rectImage));

	m_entryImage.sizeImage.cx=pDoc->GetDocSize().cx;
	m_entryImage.sizeImage.cy=pDoc->GetDocSize().cy;


	LPBITMAPINFOHEADER  lpBi;
	m_entryImage.hDIB=pDoc->GetHDIB();
	lpBi=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)	m_entryImage.hDIB);//ȡ��ԭ��λͼ
	m_entryImage.pImage=(BYTE FAR*)::FindDIBBits((LPSTR)lpBi);

	return TRUE;
}

/*1998.12.27 ��ɫͼ��ʹ�ã�����
	m_entryImage.hDIB			//Դλͼ���
	m_entryImage.hBitmap		//Դ��ʱ�м��豸���λͼ���
	m_entryImage.hMemDC			//Դλͼ�豸
	m_entryImage.rectImage		//��Χѡ���������С����
	m_entryImage.sizeImage		//ͼ���С
*/
BOOL CImageView::BeginUseColor()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->GetHDIB()==NULL) return FALSE;

	m_entryImage.hDIB=NULL;					//Դλͼ���
	m_entryImage.hNewDIB=NULL;				//����Ŀ��λͼ���
	m_entryImage.pImage=NULL;				//Դλͼ���ݿ�ʼλ�����ڻҶ�ͼ��
	m_entryImage.pNewImage=NULL;			//Ŀ��λͼ���ݿ�ʼλ�����ڻҶ�ͼ��
	m_entryImage.hBitmap=NULL;				//Դ��ʱ�м��豸���λͼ���
	m_entryImage.hNewBitmap=NULL;			//Ŀ����ʱ�м��豸���λͼ���
	m_entryImage.hMemDC=NULL;				//Դλͼ�豸
	m_entryImage.hNewDC=NULL;				//Ŀ��λͼ�豸
	m_entryImage.rectImage=CRect(0,0,1,1);		//��Χѡ���������С����
	m_entryImage.sizeImage=CSize(1,1);		//ͼ���С

	//�����Χ�������С����
	m_Region.GetRgnBox(&(m_entryImage.rectImage));
	m_entryImage.sizeImage.cx=pDoc->GetDocSize().cx;
	m_entryImage.sizeImage.cy=pDoc->GetDocSize().cy;

	HDC hDC=::GetDC(NULL);
	m_entryImage.hMemDC=::CreateCompatibleDC(hDC);
	
	m_entryImage.hBitmap=::DIBToBitmap(pDoc->GetHDIB(), 
		NULL);
	if(!m_entryImage.hBitmap ) return FALSE;
	if(!::SelectObject(m_entryImage.hMemDC,m_entryImage.hBitmap))	return FALSE;

//	::SelectClipRgn(m_entryImage.hNewDC,m_Region);
	::DeleteDC(hDC);

	return TRUE;
}

/*1998.12.27 �Ҷ�ͼ��ʹ�ý���������
	m_entryImage.hDIB			//Դλͼ���
	m_entryImage.pImage			//Դλͼ���ݿ�ʼλ�����ڻҶ�ͼ��

*/
BOOL CImageView::EndUseGray()
{
	::GlobalUnlock((HGLOBAL)m_entryImage.hDIB);
	return TRUE;
}

/*1998.12.27 ��ɫͼ��ʹ�ý���������
	m_entryImage.hBitmap		//Դ��ʱ�м��豸���λͼ���
	m_entryImage.hMemDC			//Դλͼ�豸
*/
BOOL CImageView::EndUseColor()
{
	::DeleteDC(m_entryImage.hMemDC);
	::DeleteObject(m_entryImage.hBitmap);
	return TRUE;
}

//1998.12.29 �Զ�����
void CImageView::AutoProcessing()
{
	if(m_bAutoProcessing  && (m_nAutoCommand != NULL))
	{
		::PostMessage(this->m_hWnd,WM_COMMAND,m_nAutoCommand,NULL);
	}

}
//1998.2.9
BOOL CImageView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//��ֹ�������£�������˸
//	return CFormView::OnEraseBkgnd(pDC);
}

//1999-02-05�����ݣ���VPic�����ĵ�
//!!!ע�⣬������δ�ı�ԭͼ�ĵ�ɫ�����ɫ�������
//�ڴ����иı��˵�ɫ�����ɫ������ʹ��UpdateDocument(HDIB)
void CImageView::UpdateDocument(VPic *Pic)
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	VPic pic2;
	pic2.CreateFromDIB(pDoc->GetHDIB());
	CDC *pDC=Pic->GetDC();
	CDC *pDC2=pic2.GetDC();
	pDC2->SelectClipRgn(&m_Region);
	pDC2->BitBlt(0,0,pic2.GetWidth(),pic2.GetHeight(),
		     pDC,0,0,SRCCOPY);
	UpdateDocument(BitmapToDIB(pic2.GetHBitmap(), NULL ));
}
