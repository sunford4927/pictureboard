
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
	CMenu m_Color_DropDown;

	// 도형관리를 위한 구조체 선언
	struct _SModel
	{
		int m_iCnt = 0;
		int m_iDrawMode;
		CPoint m_cpStart;
		CPoint m_cpEnd;
		BOOL m_isClick = FALSE;
		int m_sR;
		int m_sG;
		int m_sB;
	};
	CList<_SModel, _SModel&> m_ModelList;
	_SModel OnMakeModel(int type, CPoint startPoint, CPoint endPoint );

	void OnDraw(int type, CPoint startPoint, CPoint endPoint, int R, int G, int B); // 무효화 영역 발생시 복구
	void OnDraw(CPoint endPoint); // 
	void OnSplit(CString value, CString phraser, CStringArray& strs);
	void AreaDraw();// 마우스 드래그시 미리보기

	void SquareDraw(CClientDC &dc, CPoint startPoint, CPoint endPoint);
	void CircleDraw(CClientDC &dc, CPoint startPoint, CPoint endPoint);
	void TriangleDraw(CClientDC &dc, CPoint startPoint, CPoint endPoint);
	void StraightDraw(CClientDC &dc,CPoint startPoint, CPoint endPoint);
	
	void OnDropDownMenu(UINT nID);
	BOOL m_bChecked;
	CPoint m_prevXY;
	CPoint m_rect_start_pos;
	int m_cx;
	int m_cy;
	int m_pos;
	int m_mainMode;
	int m_subMode;
	int m_gR;
	int m_gG;
	int m_gB;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnImageSave();

	afx_msg void OnSquare();
	afx_msg void OnCircle();
	afx_msg void OnTriangle();
	void InitCheckButton(int value1, int value2, int value3);
	afx_msg void OnStraight();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnImgload();
	afx_msg void OnReset(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	void CpictureboardDlg::ChoiceView();
public:
	afx_msg void OnColor();
};
