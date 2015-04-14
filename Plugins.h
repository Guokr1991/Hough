//Plugins.h ͼ�����㷨API
//���ݣ�1998-1999
//1999-01-30��ViSC����ImagePro
//1999-04-03��ImageProժ¼��������������ΪImageS�ķ���

#if !defined(_Pligins_Effects_include)
#define _Plugins_Effects_include

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ImageRegion.h"

BOOL WINAPI GrayImageBinarize(BYTE FAR *pOldImage,
						  int OldWidth,
						  int OldHeight,
						  BYTE FAR *pNewImage,
						  int NewWidth,
						  int NewHeight,
						  UINT Threshold ,    //����
						  CImageRegion *rgn);            //Gray��ֵЧ��
BOOL WINAPI ImageBinarize(HDC hOldImage,
						  int OldWidth,
						  int OldHeight,
						  HDC hNewImage,
						  int NewWidth,
						  int NewHeight,
						  UINT Threshold ,    //����
						  CImageRegion *rgn);            //��ֵЧ��




#endif //_Plugins_Effects_include