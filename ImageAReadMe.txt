            IPX1.0 ����Ӧ�ü򻯰�ʹ��˵��
               ���ݣ�1999��3��13��

һ�����˵��

	����ImageA��һ����򵥵�ͼ�����ܣ����Ľ���Ŀ����ʹ
���̺ͿƼ���Ա���о�ͼ�����㷨ʱ���ƹ�C,C++,VisualC++�ļ�
��ϸ�ڣ�ֱ����֤�Լ����㷨��
	ImageA�ǺϷʹ�ҵ��ѧ��ͼ������IPX��򻯵İ汾,����
�����汾�ǣ�
	1���򻯰棨S�棩���ṩ����ȫ��ͼ������������������ͼ���ʽ��
����루BMP,GIF,TIF,JPEG,PCX,BIF,PNG,TGA������������ӡ��
��һЩ�����Ĵ����㷨��
	2����ȫ�棨F�棩����S��Ļ����ϣ�������ʮ���ֱ�ע���ܣ�����ͼ��
ͼ�����ݿ⣬����ͼ����ͨ�öԻ���ֱ��ͼ,����Ƶ��׽�������ð���
������ʽ�����������

	ImageA����VisualC++ 6.0��д�ģ���һ���Ĺ���Ҳ��Ҫ��VC6.0����
�½��С���������ʮ���˽�VisualC++,ֻҪ���������C�ͼ��˽�VisualC++
������ʹ�ã����ɼ����Լ��Ĵ���

�������

	ImageA��������:
	CAboutDlg    ���ڶԻ���
	   ���ܣ� ��ʾ������Ϣ

	CChildFrame  �ӿ�ܴ���
	   ���ܣ� ����һ������2 X 2�ʷֵĶ��ĵ�����(MDI)�Ӵ���

	CMainFrame   ����ܴ���
		���ܣ�1����������������ڣ��˵�����������״̬��
		      2����Ӧ��ɫ��ı���Ϣ

	CImageAppA   ͼ��Ӧ�ó���
		�����Ӧ��IPX��CImageApp
		���ܣ�����Ӧ�ó�����ע���ĵ�ģ��

	CImageDocA   ͼ���ĵ�
		�����Ӧ��IPX��CImageDoc
		���ܣ�1�� ����BMP��ʽͼ���ļ�������ͼ����Ϣ���Ƿ�Ҷȣ�
			      ͼ��������С����ɫ�壩
			  2������ͼ������ΪBMP��ʽͼ���ļ�
	CImageViewA  ͼ����ͼ
		�����Ӧ��IPX��CImageview
		���ܣ�1����ʾ�ʹ�ӡͼ��
			  2����׼��������������������ƣ�ճ����������������
			            ���������
			  3����Ӧ�û����ʵ��ͼ����
			  4������ͼ���ĵ�(CImageDocA)����
			  5���û��������
	CGrayDoc     �Ҷ�ͼ���ĵ�
		�����Ӧ��IPX��CGrayDoc������������CBMPDoc��ΪCImageDocA
		���ܣ����ͼ���Ƿ�Ϊ�Ҷ�ͼ��

	DIBAPI       һ���豸�޹�λͼ(DIB)������

����ʹ�÷���

	����Լ���ͼ�����㷨�ķ���
	1����ImageA����
	2��Ϊ���㷨��Ӳ˵���  ��VC��Դ��ͼ���򿪲˵�IDR_MAINFRAME
	3���ڡ������Ӳ˵��¼����Լ��Ĳ˵�����磺
	     ID     :ID_IMAGE_MIRROR
	     Caption:����
	     Prompt :���ҷ�תͼ��\n����
	4��Ϊ�ò˵�����Ӵ��������� 
	     CImageViewA::OnImageMirror()
	5����д�������� ��
		//�Ƽ�ʹ��������д������������ȫ����֪������ı��ϸ�ڶ�
		//ֱ�Ӽ����Լ����㷨
		void CImageViewA::OnColorMirror() //����
		{
			// TODO: Add your command handler code here
			CImageDocA* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
	
			if(pDoc->IsGray())
			{
				//�Ҷ�ͼ�������ϣ���ԻҶ�ͼ��Ͳ�ɫʹ��ͳһ�Ĵ���
				//��һ�ο���Ϊ�ա��ԻҶ�ͼ��Ĵ����ٶȴ�Լ�ڲ�ɫ�����
				//4�����ϣ��������ֻ����256���Ҷ�ͼ���Ƽ�д����һ��
				BeginProcessingGray();
				DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
				for(DWORD i=0;i<w;i++)
					for(DWORD j=0;j<h;j++)
					{
						//�Ҷȵ㴦��,�����Խ��öλ�Ϊ�Լ��Ĵ���
						SetGrayPixel(w-i-1,j,GetGrayOldPixel(i,j));
					}
				EndProcessingGray();
			}
			else
			{
				//��ɫͼ�������ֻϣ���ԻҶ�ͼ������һ�ο���Ϊ��
				BeginProcessingColor(); 
				DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
				for(DWORD i=0;i<w;i++)
					for(DWORD j=0;j<h;j++)
					{
						//��ɫ�㴦��,�����Խ��öλ�Ϊ�Լ��Ĵ���
						SetPixel(w-i-1,j,GetOldPixel(i,j));
					}
				EndProcessingColor(); 
			}
		}
	6����Ӳ˵��������Ϣ������������
	     CImageViewA::OnUpdateImageMirror()
	7����д�˵�����º������壬����
		void CImageViewA::OnUpdateColorNegative(CCmdUI* pCmdUI) 
		{
			CImageDocA* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pCmdUI->Enable(pDoc->GetHDIB() != NULL ); 	
		}


�ġ��û��ֲ�
	�ڱ����а������û��ڼ����Լ��Ĵ���ʱ����Ҫ�˽����ͺ�����Ϣ
	1�������˽����Ϣ
		��С���еĺ�����������ģ����ڱ���б�ȻҪʹ��
		 ��CImageViewA::GetGrayOldPixel

			BYTE GetGrayOldPixel( int x, int y);
			����
				x  ����X 
				y  ����Y
	
			����ֵ
				�õ�ĻҶ�ֵ 0 - 255
			����˵��
				ȡ�Ҷ�ͼ����ǰָ��λ�õ�����ֵ

		��	CImageViewA::GetOldPixel

			COLORREF GetOldPixel(int x, int y);
			����
				x  ����X 
				y  ����Y
	
			����ֵ
				�õ����ɫֵ
			����˵��
				ȡ(��ɫ)ͼ����ǰָ��λ�õ�����ֵ

		�� CImageViewA::SetPixel

		   void SetPixel(int x, int y, COLORREF color)
			����
				x  ����X 
				y  ����Y
				color ָ������ɫֵ���紿��ɫRGB(255,0,0) 
	
			����ֵ
				��
			����˵��
				����(��ɫ)ͼ��ָ��λ�õ�����ֵ
		�� CImageViewA::SetGrayPixel
		  
		   void SetGrayPixel(int x, int y, BYTE color)
			����
				x  ����X 
				y  ����Y
				color ָ���ĻҶ�ֵ�����ɫ255 
			����ֵ
				��
			����˵��
				���ûҶ�ͼ��ָ��λ�õ�����ֵ
	
	2��ͼ��������Ϣ
	     ֪������4�����������Ѿ���������Լ����㷨��ʵ���ˣ�
	   �����������ʹ���Ƽ��Ĵ�����д�����˽����º���������ġ�
	  ��BOOL CImageViewA::BeginProcessing();//ͨ�ô���ʼ����
	  ��BOOL CImageViewA::EndProcessing();//ͨ�ô����������
	        ������������ϣ�����û�����

	  ��BOOL CImageViewA::BeginProcessingGray();//�Ҷȴ���ʼ����
	  ��BOOL CImageViewA::EndProcessingGray();//�Ҷȴ����������
	  ��BOOL CImageViewA::BeginProcessingColor();//��ɫ����ʼ����
	  ��BOOL CImageViewA::EndProcessingColor();//��ɫ�����������
			���ĸ�������װ��Ԥ����ʹ�����ĵ����¡���ʾ���µĹ���
			

	  ��BOOL CImageViewA::EndUseColor();//��ɫͼ��ʹ�ý���
	  ��BOOL CImageViewA::EndUseGray();//�Ҷ�ͼ��ʹ�ý���
	  ��BOOL CImageViewA::BeginUseColor();//��ɫͼ��ʹ�ÿ�ʼ
	  �� BOOL CImageViewA::BeginUseGray();	//�Ҷ�ͼ��ʹ�ÿ�ʼ
	     �����ֻ��ʹ�ö�����Ҫ����ԭͼ�����ݣ���ʹ��
		       BeginUseGray    ----   EndUseGray
			   BeginUseColor   ----   EndUseColor
		����Ҫʹ��
			   BeginProcessingColor  ----   EndProcessingColor
		       BeginProcessingGray   ----   EndProcessingGray
		��������д���롰ʹ�÷������и�����һ����
	
	3��λͼ��̽ӿ�DIBAPI
	     ����һ��ʮ�����õ�λͼ���������˽����麯����ʹ��������
       ����ͼ������IPX�򻯰����ȫ���У��Ƽ�ʹ����VPic����ͼ����
	   ��DIBAPI��Ȼ�Ƿǳ���Ҫ�ġ��������ڽ�һ����װ����API(Ӧ�ñ�̽ӿ�)
		   ����API�Ļ�����VC��diblook�п����ҵ������Ƕ������������䣬
       �����������м���bug
			
			�ڴ˸���DIBAPI�ļ�˵������ϸ��Ϣ�뿴Դ������ÿһ������
       ǰ��˵��

	   ��BitmapToDIB  ��λͼ���HBITMAPת��Ϊ�豸�޹�λͼ���HDIB
	   ��CopyHandle   ���ƾ��
	   ��Create24BitsBitmap  ����һ��24λɫλͼ
	   ��Create8BitsBitmap  ����һ��8λɫλͼ
	   ��CreateBIPalette   ����һ��BITMAPINFO�ṹ������ɫ��
	   ��CreateDIBPlaette  ����һ��DIB������ɫ�� 
	   ��DIBHeight         ��ȡλͼ��
	   ��DIBNumColors      ��ȡλͼ��ɫ��
	   ��DIBToBitmap       ���豸�޹�λͼ���HDIBת��Ϊλͼ���HBITMAP
	   ��DIBWidth          ��ȡλͼ��
	   ��FindDIBBits       ��ȡλͼͼ�����ݿ�ʼ����ָ�� 
	   ��GetGrayPixel      ��ȡ�Ҷ�ͼ��ָ��������ֵ
	   ��PaintDIB          ����λͼ
	   ��PaletteSize       ��ȡ��ɫ��ı��ش�С
	   ��ReadDIBFile       ��һ�ļ�����λͼ
	   ��SaveDIB           ��λͼд��һ���ļ�  
	   ��SetGrayPixel      ���ûҶ�λͼָ��������ֵ

		
�塢����
	1����˵���Ƿǳ���Ҫ�ģ��������������ʱ������ϵ��
	2��������������ִ���(bug)�������֪ͨ�ҡ�
	3����ImageA�л��۵ľ�����IPX�������汾���ǿ��Լ���ʹ�õģ����ֺ���
�����ڸ߰汾�ж���ͼ�������¼��ݡ�				
       4�����������ᳫ���㷨ʵ�ֺ����봦����Ϣ��Ӧ�������룬���磺
		void CImageViewA::OnColorNegative() //��ƬЧ��
		{
			CImageDocA* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
	
			if(pDoc->IsGray())
			{
				BeginProcessingGray();
				DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
				GrayImageNegative(m_entryImage.pImage,w,h,
				                  m_entryImage.pNewImage,w,h);
				EndProcessingGray();
			}
		}
		BOOL WINAPI GrayImageNegative(BYTE FAR *pOldImage,
						  int OldWidth,
						  int OldHeight,
						  BYTE FAR *pNewImage,
						  int NewWidth,
						  int NewHeight)
		{
			if(OldWidth!=NewWidth || OldHeight!=NewHeight 
			    || pOldImage==NULL || pNewImage==NULL)
				return FALSE;
			for(int i=0;i<OldWidth;i++)
				for(int j=0;j<OldHeight;j++)
				{
					SetGrayPixel(pNewImage,i,j,NewWidth,NewHeight,
					     255-GetGrayPixel(pOldImage,i,j,OldWidth,OldHeight))
					//*(pNewImage+j*NewWidth+i)=
					//		0xFF-*(pOldImage+j*OldWidth+i);
				}
			return TRUE;
		}
				
	�ҵ���ϵ��ַ��
	   �Ϸʹ�ҵ��ѧ�о���¥404
	        0551-4655210-2947 ��404
	   ������3¥���������ϢѧԺ����ʵ����
	        0551-4658414-8313
	   BaoJie@990.net
		




