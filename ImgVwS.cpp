// ImgVwS.cpp : implementation of the CImageViewS class
//

#include "stdafx.h"
#include "ImageS.h"

#include "ImgDocS.h"
#include "ImgVwS.h"
#include "GrayDocS.h"

#include "rangeann.h"
#include "progdlg.h"

#include "Plugins.h"
#include "VicPict.h"
#include "VPicEx.h"
#include "VGrayPic.h"

#include "Hough.h"//Hough�任


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageViewS

IMPLEMENT_DYNCREATE(CImageViewS, CImageView)

BEGIN_MESSAGE_MAP(CImageViewS, CImageView)
	//{{AFX_MSG_MAP(CImageViewS)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_COMMAND(ID_COLOR_NEGATIVE, OnColorNegative)
	ON_COMMAND(ID_COLOR_BINARIZE, OnColorBinarize)
	ON_COMMAND(ID_IMAGE_EFFECTS_QUICKEMBOSS, OnImageEffectsQuickemboss)
	ON_COMMAND(ID_COLOR_BRIGHTNESS, OnColorBrightness)
	ON_COMMAND(ID_HOUGH_CLASSIC, OnHoughClassic)
	ON_COMMAND(ID_COLOR_GRAY, OnColorGray)
	ON_COMMAND(ID_HOUGH_NEW_CALC_MAT, OnHoughNewCalcMat)
	ON_COMMAND(ID_HOUGH_NEW, OnHoughNew)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CImageView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CImageView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CImageView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageViewS construction/destruction

CImageViewS::CImageViewS()
	: CImageView(CImageViewS::IDD)
{
	//{{AFX_DATA_INIT(CImageViewS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CImageViewS::~CImageViewS()
{
}

void CImageViewS::DoDataExchange(CDataExchange* pDX)
{
	CImageView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageViewS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CImageViewS::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CImageView::PreCreateWindow(cs);
}

void CImageViewS::OnInitialUpdate()
{
	CImageView::OnInitialUpdate();
//	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CImageViewS printing

BOOL CImageViewS::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageViewS::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CImageView::OnBeginPrinting(pDC,pInfo);
}

void CImageViewS::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CImageView::OnEndPrinting(pDC,pInfo);
}

void CImageViewS::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here
	CImageView::OnPrint(pDC,pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CImageViewS::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CImageViewS diagnostics

#ifdef _DEBUG
void CImageViewS::AssertValid() const
{
	CImageView::AssertValid();
}

void CImageViewS::Dump(CDumpContext& dc) const
{
	CImageView::Dump(dc);
}

CImageDocS* CImageViewS::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDocS))
		||m_pDocument->IsKindOf(RUNTIME_CLASS(CGrayDocS)));
	return (CImageDocS*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageViewS message handlers

/*
1999-04-01�����ݣ���������ƬЧ��

*/
void CImageViewS::OnColorNegative() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->IsGray())
	{
		//�Ҷ�ͼ��
		BeginProcessingGray();
	    DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
		CProgressDlg dlg(0,w,1);
		for(DWORD i=0;i<w;i++)
		{ 
			if(dlg.CheckCancelButton())
			{	EndProcessingGray(FALSE);	break;}
            dlg.StepIt();
			for(DWORD j=0;j<h;j++)
			{
				//�Ҷȵ㴦��
				SetGrayPixel(i,j,255-GetGrayOldPixel(i,j));
			}
		}
		EndProcessingGray();
	}
	else
	{
		//��ɫͼ�������ֻϣ���ԻҶ�ͼ������һ�ο���Ϊ��
		BeginProcessingColor(); 
	    DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
		CProgressDlg dlg(0,w,1);
		for(DWORD i=0;i<w;i++)
		{
			if(dlg.CheckCancelButton())
			{EndProcessingColor(FALSE); 	break;}
            dlg.StepIt();
			for(DWORD j=0;j<h;j++)
			{
				//��ɫ�㴦��
				SetPixel(i,j,0x00FFFFFF-GetOldPixel(i,j));
			}
		}
		EndProcessingColor(); 
	}
}
//1999-01-30 ���ݣ���ֵ��
void CImageViewS::OnColorBinarize() 
{
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRangeDlg dlg;

   dlg.m_strTitle = _T("��ֵ��");
   dlg.m_strLabel = _T("����");
   dlg.m_nValue = 127;
   dlg.m_nMin = 0;
   dlg.m_nMax = 255;
   dlg.m_nPage = 1;

   if(dlg.DoModal() == IDOK)
   {
		BOOL bSuc=FALSE;
 		if(pDoc->IsGray())		BeginProcessingGray();
		else             	    BeginProcessingColor(); 

		DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
		if(pDoc->IsGray())
			bSuc=GrayImageBinarize(m_entryImage.pNewImage, w,h,
							  m_entryImage.pNewImage,w,h,
							  dlg.m_nValue,
							  &m_Region
							  );
		else
		{
			bSuc=ImageBinarize(m_entryImage.hMemDC, w,h,
							  m_entryImage.hNewDC,w,h,
							  dlg.m_nValue,
							  &m_Region
							  );
		}
		if(pDoc->IsGray())		EndProcessingGray(bSuc);
		else				    EndProcessingColor(bSuc); 	
  }
  pDoc->m_bGray = FALSE;
}

//1999-02-17�����ݣ����ٸ���
void CImageViewS::OnImageEffectsQuickemboss() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;

    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
	VPicEx pic;
	pic.CreateFromDIB(pDoc->GetHDIB());
	CRect rectRgn;
	if(m_Region.m_hObject == NULL ) return ;
	m_Region.GetRgnBox(&rectRgn);
	pic.SetWorkingArea(rectRgn);

	pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
	pic.QuickEmboss();   

	UpdateDocument(&pic);
	EndWaitCursor();
}


void CImageViewS::OnColorBrightness() 
{
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRangeDlg dlg;

	dlg.m_strTitle = _T("��ֵ��");
	dlg.m_strLabel = _T("����");
	dlg.m_nValue = 0;
	dlg.m_nMin = -255;
	dlg.m_nMax = 255;
	dlg.m_nPage = 1;

   if(dlg.DoModal() == IDOK)
   {
		if(!pDoc->IsGray())
		{
			AfxMessageBox("������������û�б�д��ɫ������");
			return;
		}
	    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
		VGrayPic pic;
		pic.CreateFromDIB(pDoc->GetHDIB());
		CRect rectRgn;
		if(m_Region.m_hObject == NULL ) return ;
		m_Region.GetRgnBox(&rectRgn);
		pic.SetWorkingArea(rectRgn);

		pic.Brighten(dlg.m_nValue);

		UpdateDocument(&pic);
		EndWaitCursor();
   }
}

//1999-04-05,���ݣ�����Hough�任
void CImageViewS::OnHoughClassic() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;

	CHough pic;
	pic.CreateFromDIB(pDoc->GetHDIB());

	if(!pDoc->IsGray())
	{
		AfxMessageBox("���ǻҶ�ͼ�񣬽��ҶȻ�");
		pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
		pDoc->m_bGray = TRUE;
	}
    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
	if(	pic.Hough())
		UpdateDocument(BitmapToDIB(pic.GetHBitmap(), NULL ));
	EndWaitCursor();
}

//1999-04-06,���ݣ��Ҷ�Ч��
void CImageViewS::OnColorGray() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;
    BeginWaitCursor();
	m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
	
	VPic pic;
	pic.CreateFromDIB(pDoc->GetHDIB());
	pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
	pDoc->m_bGray = TRUE;

	UpdateDocument(BitmapToDIB(pic.GetHBitmap(), NULL ));
	EndWaitCursor();
}

//1999-04-08�����ݣ��������-���Hough�任Ȩֵ����
void CImageViewS::OnHoughNewCalcMat() 
{
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CFileDialog OpenDlg(FALSE,
		            "wmt",
					"δ����",
					OFN_EXPLORER,
					"Ȩֵ�����ļ�(*.wmt)|*.wmt");
	if(OpenDlg.DoModal() != IDOK) return;

	WORD imageWidth=256,imageHeight=256;
	if(pDoc->GetHDIB())
	{
		imageWidth = WORD(pDoc->GetDocSize().cx);
		imageHeight = WORD(pDoc->GetDocSize().cy);
	}

	CRangeDlg dlgW,dlgH;
	dlgW.m_strTitle = _T("ͼ���");
	dlgW.m_strLabel = _T("��");
	dlgW.m_nValue = imageWidth;
	dlgW.m_nMin = 0;
	dlgW.m_nMax = 2000;
	dlgW.m_nPage = 10;

	dlgH.m_strTitle = _T("ͼ���");
	dlgH.m_strLabel = _T("��");
	dlgH.m_nValue = imageHeight;
	dlgH.m_nMin = 0;
	dlgH.m_nMax = 2000;
	dlgH.m_nPage = 10;
	
	if(dlgW.DoModal() != IDOK)return;
	if(dlgH.DoModal() != IDOK)return;

	imageWidth = dlgW.m_nValue;
	imageHeight = dlgH.m_nValue;
	
	CHough pic;
	pic.Hough2CalculateWeight(OpenDlg.GetPathName(),
		imageWidth,
		imageHeight);
	::MessageBox(m_hWnd,"��ϲ��ϲ��л��л�أ������������ˡ�","�󹦸��",MB_OK);
}

//1999-04-08�����ݣ�����-���Hough�任
void CImageViewS::OnHoughNew() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;

	CFileDialog OpenDlg(TRUE,
		            "wmt",
					"δ����",
					OFN_EXPLORER,
					"Ȩֵ�����ļ�(*.wmt)|*.wmt");
	if(OpenDlg.DoModal() != IDOK) return;

	CHough pic;
	pic.CreateFromDIB(pDoc->GetHDIB());

	if(!pDoc->IsGray())
	{
		AfxMessageBox("���ǻҶ�ͼ�񣬽��ҶȻ�");
		pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
		pDoc->m_bGray = TRUE;
	}
    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
	if(	pic.Hough2(OpenDlg.GetPathName()))
		UpdateDocument(BitmapToDIB(pic.GetHBitmap(), NULL ));
	EndWaitCursor();
}
