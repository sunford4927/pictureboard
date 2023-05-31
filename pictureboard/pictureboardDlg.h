
// pictureboardDlg.h: 헤더 파일
//

#pragma once


// CpictureboardDlg 대화 상자
class CpictureboardDlg : public CDialogEx
{
// 생성입니다.
public:
	CpictureboardDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICTUREBOARD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void InitToolBar();
	CToolBar m_wndToolBar;
	void OnDropDownCommand(UINT nID);
	void OnUpdate(CCmdUI* pCmdUI);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CPoint rect_start_pos;
	int m_nDrawMode;

	afx_msg void OnSquare();
	afx_msg void OnCircle();
	afx_msg void OnTriangle();
	void InitCheckButton();
	afx_msg void OnStraight();
};
