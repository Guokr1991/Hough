//Plugins.cpp ͼ�����㷨API
//���ݣ�1998-1999
//1999-01-30��ViSC����ImagePro
//1999-04-03��ImageProժ¼��������������ΪImageS�ķ���

#include "stdafx.h"
#include "Plugins.h"
#include "dibapi.h"


#include "Resource.h"
#include "ProgDlg.h"

#include "math.h"
#include "stdlib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Gray��ֵЧ��
BOOL WINAPI GrayImageBinarize(BYTE FAR *pOldImage,
						  int OldWidth,
						  int OldHeight,
						  BYTE FAR *pNewImage,
						  int NewWidth,
						  int NewHeight,
						  UINT Threshold,     //����
						  CImageRegion *rgn
						  )
{
	if(OldWidth!=NewWidth || OldHeight!=NewHeight 
	    || pOldImage==NULL || pNewImage==NULL)
		return FALSE;

	UINT Color;
	CRect rect;
	rgn->GetRgnBox(&rect);

	CProgressDlg dlg(0,rect.Width(),1);
	for(long i=rect.left;i<rect.right;i++)
	{
		for(long j=rect.top;j<rect.bottom;j++)
			{
				if(rgn->PtInRegion(i,j))
				{
					Color=GetGrayPixel(pOldImage,i,j,OldWidth,OldHeight);
					if(Color>Threshold)  Color=255;
					else                 Color=0;
					SetGrayPixel(pNewImage,i,j,NewWidth,NewHeight,(BYTE)Color);
				}
			}
		if(dlg.CheckCancelButton())
		{ return FALSE; }
		dlg.StepIt();
	}
	return TRUE;
}

//���ݣ�1999-01-30
//��ֵЧ��
BOOL WINAPI ImageBinarize(HDC hOldImage,
						  int OldWidth,
						  int OldHeight,
						  HDC hNewImage,
						  int NewWidth,
						  int NewHeight,
						  UINT Threshold ,    //����
						  CImageRegion *rgn)            //��ֵЧ��
{
	if(OldWidth!=NewWidth || OldHeight!=NewHeight 
	    || hOldImage==NULL || hNewImage==NULL)
		return FALSE;
	CRect rect;
	rgn->GetRgnBox(&rect);
	::SelectClipRgn(hNewImage,HRGN(rgn->m_hObject));

	//����ԭͼ
	COLORREF Color;
	UINT Lumin;
	
	CProgressDlg dlg(0,rect.Width(),1);
	for(long i=rect.left;i<rect.right;i++)
	{
		for(long j=rect.top;j<rect.bottom;j++)
			{
//				if(rgn->PtInRegion(i,j))
				{
					Color=::GetPixel(hOldImage,i,j);
					Lumin=(298*GetRValue(Color)+
					       588*GetGValue(Color)+
						   114*GetBValue(Color)
						  )/1000;
					if(Lumin>Threshold)  
						::SetPixelV(hNewImage,i,j, RGB(255,255,255) );
					else 
						::SetPixelV(hNewImage,i,j, RGB(0,0,0) );
				}
			}
		if(dlg.CheckCancelButton())
		{ return FALSE; }
		dlg.StepIt();
	}
	return TRUE;

}
