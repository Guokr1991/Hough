//����ImagePower���̣����ݣ�1998��8��-9��
// ProgDlg.h : ��������ͷ�ļ�

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

#ifndef __PROGDLG_H__
#define __PROGDLG_H__

/*
��:

// Don't forget to include the header file for the progress dialog class:
#include "progdlg.h"

void OnCalculate()
{
  CProgressDlg dlg;  dlg.Create();  // By default Create will disable the application's main window
   // For a progress dialog with a range that contains 10 steps ( (upper-lower) / step)
   // we would use a loop with ten calls to StepIt(). Note that the range or the step size
  // could be changed programmatically with SetRange or SetStep.

  ��		CProgressDlg dlg(0,Width,1,
			theApp.m_bProgClock���������̿ɸ�ΪTRUE��"FALSE��ѡ�����������);

	for(int i=0; i<10; i++)

  {
    if(dlg.CheckCancelButton())
      if(AfxMessageBox(_T("Are you sure you want to Cancel?"),MB_YESNO)==IDYES)
        break;
    DoSingleCalculation(i);  // Some function or block of code that performs a single
                                            // iteration of the processing
    dlg.StepIt();
   };
  // At this point either the operation has completed or the user has cancelled.
}

*/

class CProgressDlg : public CDialog
{
// Construction / Destruction
public:
	void EnableClock(BOOL bClock=TRUE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    CProgressDlg(int nLower =0,int nUpper =100,int nStep=1,BOOL bClock=FALSE);
    ~CProgressDlg();

    BOOL Create(CWnd *pParent=NULL);

    // Checking for Cancel button
    BOOL CheckCancelButton();
    // Progress Dialog manipulation
//    void SetStatus(LPCTSTR lpszMessage);
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
        
// Dialog Data
    //{{AFX_DATA(CProgressDlg)
	enum { IDD = CG_IDD_PROGRESS };
	CStatic	m_Clock;
    CProgressCtrl	m_Progress;
	//}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProgressDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bClock;
	CToolTipCtrl m_tooltip;
//	UINT m_nCaptionID;
    int m_nLower;
    int m_nUpper;
    int m_nStep;
    
    BOOL m_bCancel;
    BOOL m_bParentDisabled;

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    // Generated message map functions
    //{{AFX_MSG(CProgressDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __PROGDLG_H__
