// Hough.cpp: implementation of the CHough class.
// Hough�任�� �����ݣ�1999��4��5��
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "Hough.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "math.h"
#include "progdlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BUFFER_SIZE_1 4096
#define BUFFER_SIZE_2 8192

typedef struct tagFileHeader
{
	//�ļ�ͷ����,Ҳ������ƫ�Ƶ�ַ,=sizeof(SFileHeader)
	long hsize;
	//��img_width,img_height���Լ�������󳤶ȣ�Ϊn_x*n_y/8���ֽڡ�
	WORD img_width;
	WORD img_height;
	/*
	��theta_size,roh_size���Լ������������������ŷ�˳��Ϊ����
	ͬһ�Ƕȣ��ٰ������ŷš���s=s_min+ds*k,r=r_min+dr*l�ľ���
	���ŷ����Ϊ��n=k*n_r+l+1,ƫ�Ƶ�ַΪ��
	offset=sizeof(�ļ�ͷ)+(n-1)*msize
	*/
	WORD theta_size;
	WORD roh_size;//ÿ��theta�Ƕȶ���roh_size������
	double theta_min;//�Ƕȵ���Сֵ
	double theta_max;//�Ƕȵ����ֵ
	double roh_min;//�������Сֵ
	double roh_max;//��������ֵ
	DWORD msize;//ʹ��Hough2�洢�㷨��ÿ�������ʵ�ʴ洢����Ϊsize=n_x*n_y/8���ֽڡ�
}SFileHeader;


CHough::CHough()
{

}

CHough::~CHough()
{

}

//1999-04-05�����ݣ������Hough�任ʵ��
//ֱ�� rho : 0 - sqrt(w*w + h*h)
//     theta: 0 - ��
//�����ͼ����Hough�ռ�ͼ�񣬳����ı��ˡ�
//ע�⣺ֻ�ܶԻҶ�ͼ����
BOOL CHough::Hough()
{
	if( !IsValid() ) return FALSE;
	unsigned w = GetWidth();//ͼ���
	unsigned h = GetHeight(); //ͼ���
	unsigned Rho_max = unsigned(sqrt(w*w + h*h));//Houghͼ�������rho����������
	unsigned Theta_max = 360;   //Houghͼ���:�Ƕ�theta����������
	double PI = 3.1415927;   

	unsigned *arrayHough;//Hough�ռ�����
	arrayHough = new unsigned[Rho_max * Theta_max]; 
	memset(arrayHough,0,sizeof(unsigned)* Rho_max * Theta_max);

	BYTE thresh=127;//��ֵ����ֵ
	  //��ͼ���ֵ������Hough�任
	  //��ֵ��������Ҳ�����ѭ��������ˡ�
	  //��ֵ��ѡȡ����ʹ�ø���Ч�ķ�����
	CProgressDlg dlg(0,w,1);
	dlg.SetWindowText("���ڼ���Hough����");
	DWORD k,m;
	for(unsigned i = 0 ; i < w; i ++)
	{
	    if(dlg.CheckCancelButton())
		{
			delete arrayHough;
			return FALSE;
		}
       	dlg.StepIt();
		for(unsigned j = 0; j < h; j++)
		{
			BYTE color = GetGrayPixel(i,j);
			if(color > thresh)
			{
				for(k = 0 ; k < Theta_max ; k++)
					//�Ƕ�ȡֵ��Χ -pi/4 - 3*pi/4
				{
					double angle= k * (PI) /Theta_max - PI/4; 
					unsigned rho = unsigned(fabs(i * cos(angle)+ j * sin(angle)));
					arrayHough[Theta_max * rho + k] ++;
				}
			}
		}
	}

	//��Hough�ռ���������Houghͼ��

	//�½�Houghͼ��
	VGrayPic picHough;
	picHough.CreateImage(Rho_max,Theta_max,BPP_8);
	picHough.SetPixelDepth(BPP_8,GrayScale);
	unsigned nMaxHough=0;//Houghͼ�����ֵ
	
	CProgressDlg dlg2(0,Rho_max,1);
	dlg2.SetWindowText("��������Houghͼ�����ֵ");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg2.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			nMaxHough = max(nMaxHough,arrayHough[Theta_max * k + m]);
		}
	}
	if(nMaxHough == 0 ) nMaxHough =1;
	//��nMaxHough��һ��
	CProgressDlg dlg3(0,Rho_max,1);
	dlg3.SetWindowText("���ڴ�Hough�ռ���������Houghͼ��");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg3.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			picHough.SetGrayPixel(k,m,arrayHough[Theta_max * k + m]*255/nMaxHough);
		}
	}
	//����ǰͼ���û�ΪHoughͼ��
	delete arrayHough;
	return CreateFrom(picHough);
}

/*1999-04-07�����ݣ�
��ɢHough�任Ȩֵ������㡣�㷨�ο���ɡ�����
��������ɢHough�任���㷨ʵ�֡�
ԭ�������
������lpszWeightFile   Ȩֵ������
	  imageWidth       Ȩֵ�����Ӧ��ͼ���
	  imageHeight	   Ȩֵ�����Ӧ��ͼ��ߡ�	
*/		
BOOL CHough::Hough2CalculateWeight(LPCTSTR lpszWeightFile,
								   WORD imageWidth, 
								   WORD imageHeight)
{
	/*
	�ú�������hough�任Ȩֵ������Ҫ������������ͼ���Ⱥ͸߶���������
	�ٶ�ͼ��ռ��������£�
	�����꣺[0,imageWidth];
	�����꣺[0,imageHeight];
	��ɢ���̶�Ϊ��dx=ʵ��ͼ��ߴ�/ͼ��������=1;
	hough�ռ䷶Χ�������£�
	�Ƕ�theta��	(-PI/2,PI];
	����roh:		[0,sqrt(imageWidth^2+imageHeight^2];
	��ɢ���̶�Ϊ��
	�Ƕ�theta,��ɢ��imageWidth����
	��ɢ����ĽǶ�Ϊ��-PI/2+dt,-PI/2+2dt,...,-PI/2+size*dt;
	����roh,��ɢ��imageHeight����
	��ɢ����ľ���Ϊ:0,dr,2dr,3dr,...,(size-1)dr,rmax;
	note:Ҫ��imageWidth �� imageHeight ���ܹ���8����.
	ĿǰҪ��������ͼ��ҪС��1024*1024
	*/
	SFileHeader sfHeader;
	CFile weightFile;CFileException e;
	WORD length,m_t,m_r,buffer[BUFFER_SIZE_1],buffer_point;
	double dt,dr,theta,roh;
	double cc,ss;
	WORD t,r,x,y;
	double x_cc,y_ss;
	double PI = 3.1415927;   
//here is changed
	sfHeader.hsize		=	sizeof(SFileHeader);
	sfHeader.img_width	=	imageWidth;
	sfHeader.img_height	=	imageHeight;
	sfHeader.theta_size	=	imageWidth;
	sfHeader.roh_size	=	imageHeight;
	sfHeader.theta_min	=	0;
	sfHeader.theta_max	=	PI/2;
	sfHeader.roh_min	=	0;
	sfHeader.roh_max	=	sqrt(imageWidth*imageWidth+imageHeight*imageHeight);
	sfHeader.msize		=	imageWidth*imageHeight/8;

	if(!weightFile.Open(lpszWeightFile,CFile::modeCreate|CFile::modeWrite,&e))
	{
		return FALSE;
	}
	//save header
	weightFile.Write(&sfHeader,sizeof(SFileHeader));

	//begin calculate weight
	dt=(sfHeader.theta_max-sfHeader.theta_min)/sfHeader.theta_size;
	dr=(sfHeader.roh_max-sfHeader.roh_min)/(sfHeader.roh_size-1);
	theta=sfHeader.theta_min;

	CProgressDlg dlg(0,sfHeader.theta_size,1);
	dlg.SetWindowText("���ڼ���Ȩֵ����");
	TRACE1("\n Total step : %d",sfHeader.theta_size);
	for(t=1;t<=sfHeader.theta_size;t++)
	{
	    if(dlg.CheckCancelButton())
		{
			weightFile.Close();
			return FALSE;
		}
		dlg.StepIt();
		TRACE1("\n step : %d",t);
		theta+=dt;
		roh=sfHeader.roh_min-dr;
		cc=cos(theta);ss=sin(theta);
		for(r=0;r<sfHeader.roh_size;r++)
		{
			roh+=dr;
			//begin calculate one weight matrix
			buffer_point=0;
			y_ss=-ss;
			for(y=0;y<sfHeader.img_height;y++)
			{
				y_ss+=ss;
				x_cc=-cc;
				for(x=0;x<sfHeader.img_width;x++)
				{
					x_cc+=cc;
					if(fabs(roh-x_cc-y_ss)-dr/2<0)
					{
						buffer[buffer_point*2]	=y;
						buffer[buffer_point*2+1]=x;
						buffer_point++;
					}
				}
			}
			if(buffer_point)
			{
				//note,buffer_point start from zero,don't need substract 1
				length=(buffer_point*2+3)*sizeof(WORD);//every block,number of bytes
				m_t=t-1;m_r=r;
				//save one weight matrix
				weightFile.Write(&length,sizeof(WORD));
				weightFile.Write(&m_t,sizeof(WORD));
				weightFile.Write(&m_r,sizeof(WORD));
				weightFile.Write(buffer,(buffer_point*2)*sizeof(WORD));
			}
			//end of one calculate
		}
	}
	weightFile.Close();
	return TRUE;
}
/*1999-04-07�����ݣ�
�µ���ɢHough�任���㡣�㷨�ο���ɡ�����
��������ɢHough�任���㷨ʵ�֡�
ԭ�������
������lpszWeightFile   Ȩֵ������
*/		

BOOL CHough::Hough2(LPCTSTR lpszWeightFile)
{
	if( !IsValid() ) return FALSE;

	CFile weightFile;
	CFileException e;
	WORD wBuffer[BUFFER_SIZE_2];
	int *arrayHough;//store data from weight file.working stream.
	UINT nBytesNeed,nBytesNeed0,nBytesRead,nBottom,
		nP1,nP2,nHSuffix=0,nBSuffix=0,
		suffix_x,suffix_y;
	WORD wWidth,wHeight,
		wTheta,wRoh,
		Theta_max,Rho_max,wLen,
		k,m;
	int i;
	
	SFileHeader weightHeader;

	wWidth	=(WORD)GetWidth();
	wHeight	=(WORD)GetHeight();
	//��Ȩֵ�����ļ�
	if(!weightFile.Open(lpszWeightFile,CFile::modeRead,&e))
	{
		AfxMessageBox("��ȡȨֵ�����ļ�ʱ��������");
		return FALSE;
	}

	//��Ȩֵ�����ļ�ͷ.
	weightFile.Read(&weightHeader,sizeof(SFileHeader));
	if((wWidth!=weightHeader.img_width)&&
		(wHeight!=weightHeader.img_width) )
	{
		AfxMessageBox("���任ͼ���С��Ȩֵ��������.");
		weightFile.Close();
		return FALSE;
	}
	Theta_max=weightHeader.theta_size;
	Rho_max=weightHeader.roh_size;
	arrayHough	=new int[Theta_max*Rho_max];
////////////////////////////////////////////////
//��ʼHough�任
	memset(arrayHough,0,sizeof(int)*Theta_max*Rho_max);

	nP2=0;
	nBytesNeed=BUFFER_SIZE_2*2;
	CProgressDlg dlg1(0,weightFile.GetLength()/4096,1);
	dlg1.SetWindowText("���ڼ���Hough�����");
	for(;;)
	{
		nBytesRead	= weightFile.Read(&(wBuffer[nP2]),nBytesNeed);
		nBottom		= nP2 + nBytesRead/2 - 1;
		nBytesNeed0	= nBytesNeed;

		nP1 = 0;
		dlg1.SetPos(weightFile.GetPosition()/4096);
		while( nP1 <= nBottom  )
		{
			wLen = wBuffer[nP1]/2;
			if( nP1 + wLen -1 <= nBottom )
			{
				nP1++;
				wTheta	=wBuffer[nP1++];
				wRoh	=wBuffer[nP1++];
				nHSuffix=wTheta*Rho_max+wRoh;
				for(i=0;i<(wLen-3)/2;i++)
				{
					suffix_y=nP1++;suffix_x=nP1++;
					nBSuffix=wBuffer[suffix_y]*wWidth+wBuffer[suffix_x];
//					arrayHough[wTheta*Rho_max+wRoh]+=I[nBSuffix];
					arrayHough[wTheta*Rho_max+wRoh]+=GetGrayPixel(wBuffer[suffix_x],wBuffer[suffix_y]);
				}
				if(nP1==nBottom+1)
				{
					nBytesNeed=BUFFER_SIZE_2*2;
					nP2=0;
				}
			}
			else
			{
				nP2=0;
				for(;nP1<=nBottom;nP1++)
				{
					wBuffer[nP2++]=wBuffer[nP1];//move data from bottom to top
				}
				nBytesNeed=(BUFFER_SIZE_2-nP2)*2;
			}
		}
		if(nBytesRead<nBytesNeed0) break;
	}
//Hough�任����///////////////////////////////////
	//��Hough�ռ���������Houghͼ��

	//�½�Houghͼ��
	VGrayPic picHough;
	picHough.CreateImage(Theta_max,Rho_max,BPP_8);
	picHough.SetPixelDepth(BPP_8,GrayScale);
	int nMaxHough=0;//Houghͼ�����ֵ
	
	CProgressDlg dlg2(0,Rho_max,1);
	dlg2.SetWindowText("��������Houghͼ�����ֵ");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg2.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			nMaxHough = max(nMaxHough,arrayHough[ k + m * Rho_max]);
		}
	}
	if(nMaxHough == 0 ) nMaxHough =1;
	//��nMaxHough��һ��
	CProgressDlg dlg3(0,Rho_max,1);
	dlg3.SetWindowText("���ڴ�Hough�ռ���������Houghͼ��");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg3.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			picHough.SetGrayPixel(m,k,arrayHough[ k + m * Rho_max]*255/nMaxHough);
		}
	}
	//����ǰͼ���û�ΪHoughͼ��
	delete arrayHough;
	weightFile.Close();;
	return CreateFrom(picHough);

}
