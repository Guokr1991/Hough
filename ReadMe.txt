					 IPX�򻯰�ʹ��˵��
					 ���ݣ�1999��4��5��
һ�����˵��

	����ImageS��һ��ͼ�����ܣ�����ImageA�������汾��
	ImageA�ǺϷʹ�ҵ��ѧ��ͼ������IPX��򻯵İ汾,����
�����汾�ǣ�
	1���򻯰棨S��,�����汾�����ṩ����ȫ��ͼ����������������
��ͼ���ʽ�ı���루BMP,GIF,TIF,JPEG,PCX,BIF,PNG,TGA����������
��ӡ����һЩ�����Ĵ����㷨��
	2����ȫ�棨F�棩����S��Ļ����ϣ�������ʮ���ֱ�ע���ܣ�����ͼ��
ͼ�����ݿ⣬����ͼ����ͨ�öԻ���ֱ��ͼ,����Ƶ��׽�������ð���
������ʽ�����������

	ImageS����VisualC++ 6.0��д�ģ���һ���Ĺ���Ҳ��Ҫ��VC6.0����
�½��С���������ʮ���˽�VisualC++,ֻҪ���������C�ͼ��˽�VisualC++
������ʹ�ã����ɼ����Լ��Ĵ���

	�����û��ͼ����ľ��飬��������ImageA��ʼѧϰ����ImageA
�з����ͼ����Ļ���ϸ�ڣ�ʹ������ֱ���������Լ����㷨����Ӧ��
�ȿ�һ��ImageA��ʹ��˵��ImageAReadme.txt
	ImageA�еĴ���������޸ĵ���ֲ��ImageS�¡�

�����������ԣ�
	ImageS�ĺ�����Victorͼ�����㷨�⣬����һ����⽫����
IPX��ϵ������
	����9��ͼ���ʽ�����10��ͼ���ʽ����
	�������Ρ�Բ�Ρ���Բ��Բ�Ǿ��Ρ���������ѡ������򻯴���
	������׼������֧�֣�cut,paste,copy
	�������⼶��undo��redo��ȱʡ����Ϊ5�㣩
	�����������Ի���
	������ֵѡ��Ի���
	����ǿ���Vic��֧�֣����־��䴦���㷨��
	�����ʷִ���
�������
ImageS��Ϊ���󲿷�
  1��IPX��
		CGrayDoc
		CImageApp
		CImageDoc
		CImageRegion
		CImageView
		CProgressDlg
		CRangeDlg
		CUnRedoStack
		VGrayPic
		VPic
		VPicEx
  2���ض���
		CAboutDlg
		CChildFrame
		CGrayDocS
		CImageAppS
		CImageDocS
		CImageSSrvrItem
		CImageViewS
		CInPlaceFrame
		CMainFrame
ע�⣺
  1��ImageS��ΪOLE�������Ĵ��벢û����ɣ�ֻ�ṩ��MFC������ܣ�
�������Ҫ������ܣ���Ҫ�Լ������Ӧ���롣
 
�ġ�ʹ�÷���

֧�ֵ��ļ���ʽ��
  Open: bif,bmp,jpg,pcx,tga,tif,gif,png
  Save: bif,bmp,jpg,pcx,tga,tif,gif,png,eps
	��������CImageDocS::OnOpenDocument��CImageDocS::OnSaveDocument
��ʵ���ض��ı���롣

ʹ�÷��� 
 ����һ������ͼ��ȡ���õ�
		 ������CImageViewS::OnColorNegative()//��ƬЧ��
		 ���ַ�������ϸ˵����ο�ImageAReadme.txt
 ���������㷨����Ϣ������룬����WINAPI
		 ������CImageViewS::OnColorBinarize()//��ֵ��
               ::GrayImageBinarize
			   ::ImageBinarize 
 ��������ʹ��VPic��VPicEx��ҵ�Ѵ��ڵ��㷨
		������CImageViewS::OnImageEffectsQuickemboss()//���ٸ���
			  ��ʹ��VPicEx::QuickEmboss��
 �����ģ���װ��VPic��VGrayPic���Լ����㷨
		������CImageViewS::OnColorBrightness()
		      VGrayPic::Brighten()
	���ڹ����㷨ʵ�飨ʹ�ûҶ�ͼ����֤�㷨���������Ƽ����ַ�����


�塢How to ����
  1������ڱ�Ĺ�����ʹ��Vic��
       (1)����VPic���빤��(����ͨ������Ȼ�ֱ�Ӹ����ļ�VicPict.h
		  ��VicPict.cpp����Ŀ�깤�̡�)
	   (2)�����Ҫ����VPic��������չ����VPicEx��VGrayPic���빤��
	   (3)��vicdefs.h���빤��
	   (4)��project - setting - link ��ָ�����ӿ�
	             vic32ms.lib  //vic��
				 vctw32ms.lib //Twainɨ����֧��
	   (5)ȷ����windows\system���ִ���ļ�Ŀ¼��������DLL
				vic32.dll
				vctw32.dll

�����Ҹ�
	Ϊ�˱�֤ʵ���Ҹ���Ա������Э��ͳһ���밴��������̵�ԭ��
��¼�������������޸ġ����磬��Ҫ�ڿ���м���Sobel���ӵ�ʵ�֣�
������Ӧ�γ������ĵ���

    1��ע�͡�����صĺ�����ͷд��ʱ�䡢���ߡ����ܡ�����˵��
	   �������ı�Ҫ��Ϣ��   
	   ��
	   //1999-04-05������,Sobel��Ե��ȡ����
	   CImageViewS::OnImageSobel()
	   {}

	   //1999-04-05������,Sobel��Ե��ȡ����
	   //��������
	   //����ֵ��BOOL
	   //		�ɹ� TRUE  ʧ�� FALSE     
	   //��ע��
	   VGrayPic::Sobel()
	   {}
	   
	2��������Ҫ������ɸ�������ϰ������д���ڱʼǱ���д��
	    д����������Щ����������ԭ����漰��ģ�顣�磺

		����  MyPhotoShop ���̼�Ҫ
		��23�ţ�Sobel����
		˵����
			Sobel΢���������ڱ�Ե��⡣�㷨��ֵ�ԡ������ͼ����
		�������㷨�ֲᡷp182��
		    ������㷨���ǳ����㷨���������Լ�������㷨�������㷨
		�Ƚϸ��ӣ����ڴ˶��㷨ԭ�����˵������

		�漰��
		   VGrayPic::Sobel()  
		        ��һ��ͼ��ʵ��Sobel���Ӵ���
		   CImageViewS::OnImageSobel()
		        ��Ӧ�˵���Ϣ
		��Դ��
		   �˵�ID_IMAGE_SOBEL���ڲ˵�IDR_MAINFRAME��
		   ������й�������ť����ɼ��ϣ���
		   ��������ťID_IMAGE_SOBEL(�ڹ�����IDR_MAINFRAME)	
		   ��������������������Դ�����ݼ����Ի����ַ�����
		   ��Ӧ����˵������

