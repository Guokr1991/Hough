// ImageApp.cpp: implementation of the CImageApp class.
// ����,1998.12.27
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "ImageApp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageApp::CImageApp()
{

}

CImageApp::~CImageApp()
{

}
//1998.12.21 ȡ��App�����ĵ���ָ������
CPtrArray * CImageApp::EnumAllDocuments( CDocument *pDocToExclude /*=NULL*/)
{
    CPtrArray* pArray = NULL;
//    POSITION pos = m_templateList.GetHeadPosition();
    POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();

    pArray = new CPtrArray;        
	BOOL bFound = FALSE;
	while (pos != NULL)
	{
//		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
        POSITION posDoc = pTemplate->GetFirstDocPosition();
        while(posDoc != NULL)
        {
			bFound = TRUE; 
            CDocument* pDoc = pTemplate->GetNextDoc(posDoc);
            if( pDoc != pDocToExclude )
				pArray->Add(pDoc); 
        }
	}
	if(bFound)
		return(pArray);
	else
		return NULL;
}

/* �ļ�-����Ϣ��Ӧ

  ������ע����ĵ�ģ�壬����ļ�����չ���б�
  ����Ӧ�ĵ�ģ���֮
  ������Ӧ�ĵ�ģ�壬�õ�һ��ע����ĵ�ģ���

  ��������֮��ʵ���Լ��ġ��򿪡��Ի���
*/
void CImageApp::OnFileOpen() 
{
	//ȡ��ע����ĵ���չ��
	CString strType;	
	POSITION curTemplatePos = GetFirstDocTemplatePosition();
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::fileNewName);
		strType+=str;  //��    λͼ�ļ�(*.bmp)
		strType+="|*";
		curTemplate->GetDocString(str, CDocTemplate::filterExt);
		strType+=str;// ��    .BMP
		strType+="|";
	}
	strType+="�����ļ�(*.*)|*.*||";
  

	CFileDialog dlg(TRUE,
		            NULL,
					"δ����",
					OFN_ALLOWMULTISELECT|OFN_EXPLORER,
					strType);

   if(dlg.DoModal() != IDOK)
		return;

	CString  strFileName=dlg.GetPathName();
	LPTSTR lpStr=strFileName.GetBuffer(256);
	strFileName.ReleaseBuffer();

	//�����ļ���չ��������Ӧ���ĵ�ģ��
	CString strExt=dlg.GetFileExt(); 
	strExt.MakeUpper();

	curTemplatePos = GetFirstDocTemplatePosition();
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::filterExt);
		if( str == strExt || str ==  "."+strExt)
		{
			curTemplate->OpenDocumentFile(lpStr);
			return;
		}
	}
	//δע����ļ����ͣ������ĵ�ģ���(��һ��ע����ĵ�ģ��)
	curTemplatePos = GetFirstDocTemplatePosition();
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		curTemplate->OpenDocumentFile(lpStr);
		return;
	}
	AfxMessageBox("û����ע����ĵ�ģ��");
}

/*1998.12.28 ���ݣ� �µ�ͼ���ĵ�
����  ��strTemplate �ĵ�ģ��ı�־�ַ���
		hDIB        ͼ���λͼ���
*/
CImageDoc * CImageApp::CreateDoc(CString strTemplate, HDIB hDIB)
{
	CImageDoc *pNewDoc=NULL;
	POSITION curTemplatePos = GetFirstDocTemplatePosition();

	BOOL bSuccess=FALSE;
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::docName);
		if(str == strTemplate)
		{
			pNewDoc=(CImageDoc *)(curTemplate->OpenDocumentFile(NULL));
			       //CImageDoc
			bSuccess=TRUE;
			break;
		}
	}
	if(bSuccess)
	{
		pNewDoc->ReplaceHDIB(hDIB);
		if(hDIB) pNewDoc->SetModifiedFlag(TRUE);
		return pNewDoc;
	}
	else
	{
		AfxMessageBox("û����ע����ĵ�ģ��");
		return NULL;
	}

}
//1999-01-29 ,���ݣ���һλͼ�����ĵ�
CImageDoc * CImageApp::CreateDoc(CString strTemplate,HBITMAP hBitmap )
{
	return CreateDoc(strTemplate,BitmapToDIB(hBitmap,NULL));
}

//1999-02-06�����ݣ��رյ�ǰ�򿪵������ĵ� 
void CImageApp::OnFileCloseAll()
{
    CPtrArray* pDocArray = NULL;
    pDocArray = EnumAllDocuments();
	if(pDocArray == NULL) return;

	int result=MessageBox(NULL,"�Զ������������޸���?","�ر������ĵ�",MB_YESNO);
	if(result == IDYES)
		OnFileSaveAll();
//	HideApplication( );
	CloseAllDocuments(FALSE);
}

void CImageApp::OnFileSaveAll()
{
	// TODO: Add your command handler code here
    CPtrArray* pDocArray;
    pDocArray = EnumAllDocuments();
    for(int i = 0; i < pDocArray->GetSize(); i++)
    {
		((CImageDoc*) pDocArray->GetAt(i))->OnFileSave();
                
	}
}

BOOL CImageApp::InitInstance()
{
	AfxEnableControlContainer();

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}



	return CWinApp::InitInstance();
}




