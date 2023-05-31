
// pictureboardDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "pictureboard.h"
#include "pictureboardDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SQUARE	1 // 사각형
#define CIRCLE	2 // 원
#define TRIANGLE	3 // 삼각형
#define STRAIGHT	4 // 직선


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CpictureboardDlg 대화 상자



CpictureboardDlg::CpictureboardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PICTUREBOARD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpictureboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpictureboardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SQUARE, ID_STRAIGHT, &CpictureboardDlg::OnUpdate)
	ON_COMMAND(ID_CIRCLE, &CpictureboardDlg::OnCircle)
	ON_COMMAND(ID_TRIANGLE, &CpictureboardDlg::OnTriangle)
	ON_COMMAND(ID_SQUARE, &CpictureboardDlg::OnSquare)
	ON_COMMAND(ID_STRAIGHT, &CpictureboardDlg::OnStraight)
	ON_COMMAND_RANGE(ID_COLOR_1, ID_COLOR_5, OnDropDownCommand)
END_MESSAGE_MAP()


// CpictureboardDlg 메시지 처리기

BOOL CpictureboardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	InitToolBar();
	m_nDrawMode = 0;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CpictureboardDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CpictureboardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CpictureboardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpictureboardDlg::InitToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | TBSTYLE_DROPDOWN |
		CBRS_SIZE_DYNAMIC | CBRS_GRIPPER | CBRS_BORDER_ANY) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		EndDialog(IDCANCEL);
	}

	// 드롭다운 메뉴 생성
	CMenu dropDownMenu;
	dropDownMenu.CreatePopupMenu();
	dropDownMenu.AppendMenu(MF_STRING, ID_COLOR_1, _T("Menu Item 1"));
	dropDownMenu.AppendMenu(MF_STRING, ID_COLOR_2, _T("Menu Item 2"));
	dropDownMenu.AppendMenu(MF_STRING, ID_COLOR_3, _T("Menu Item 3"));
	dropDownMenu.AppendMenu(MF_STRING, ID_COLOR_4, _T("Menu Item 4"));
	dropDownMenu.AppendMenu(MF_STRING, ID_COLOR_5, _T("Menu Item 5"));

	// 드롭다운 버튼 생성
	TBBUTTONINFO tbbi;
	m_wndToolBar.GetToolBarCtrl().GetButtonInfo(0, &tbbi);
	tbbi.fsStyle |= TBSTYLE_DROPDOWN;
	tbbi.idCommand = ID_MENU_ITEM_1;  // 드롭다운 버튼의 명령 ID 설정
	m_wndToolBar.GetToolBarCtrl().SetButtonInfo(0, &tbbi);

	// 드롭다운 메뉴 연결
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	//m_wndToolBar.GetToolBarCtrl().SetCmdID(ID_MENU_ITEM_1);  // 드롭다운 메뉴의 명령 ID 설정
	//m_wndToolBar.GetToolBarCtrl().SetDropDownMenu(0, dropDownMenu.GetSafeHmenu());





	CRect rcClientStart;
	CRect rcClientNow;
	
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);

	CPoint ptOffset(rcClientNow.left - rcClientStart.left, rcClientNow.top - rcClientStart.top);
	CRect rcChild;
	CWnd* pwndChild = GetWindow(GW_CHILD);
	
	while (pwndChild)
	{
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, false);
		pwndChild = pwndChild->GetNextWindow();
	}
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, false);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}


void CpictureboardDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_wndToolBar.GetSafeHwnd() != nullptr)
	{
		// 현재 창의 너비에 맞게 ToolBar의 크기를 조정합니다.
		CRect rectToolBar;
		m_wndToolBar.GetWindowRect(rectToolBar); // 현재 위치와 크기 정보 가져옴
		ScreenToClient(rectToolBar); // 클라이언트 영역의 좌표로 변환
		rectToolBar.right = rectToolBar.left + cx;
		m_wndToolBar.MoveWindow(rectToolBar);
	}

}


BOOL CpictureboardDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return true;
}


void CpictureboardDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	rect_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CpictureboardDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CClientDC dc(this); // DC 생성
	CPen my_pen(PS_SOLID, 5, RGB(0, 0, 255)); // 굵기가 5인 팬을 생성한다.
	dc.SelectObject(&my_pen); // 생성한 팬을 DC에 연결한다.
	SelectObject(dc, GetStockObject(NULL_BRUSH));// 안이 투명한 도형을 그려주기 위해 NULL 브러쉬를 만든다.


	if (m_nDrawMode == SQUARE)
	{
		dc.Rectangle(rect_start_pos.x, rect_start_pos.y, point.x, point.y);
	}
	else if (m_nDrawMode == CIRCLE)
	{
		dc.Ellipse(rect_start_pos.x, rect_start_pos.y, point.x, point.y); // 원 그리기
	}
	else if (m_nDrawMode == TRIANGLE)
	{
		POINT arPoint[] = { rect_start_pos.x, rect_start_pos.y , rect_start_pos.x - 40, point.y + 45, point.x , point.y }; // 삼각형 함수 그리기
		dc.Polygon(arPoint, 3);
	}
	else if (m_nDrawMode == STRAIGHT)
	{
		dc.MoveTo(rect_start_pos.x, rect_start_pos.y);
		dc.LineTo(point.x, point.y);
	}

	if (nFlags & MK_CONTROL); // 컨트롤 키가 눌려졌을때

	CDialogEx::OnLButtonUp(nFlags, point);
}



void CpictureboardDlg::OnSquare()
{
	InitCheckButton();
	m_nDrawMode = SQUARE;
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_SQUARE, true);
}


void CpictureboardDlg::OnCircle()
{
	InitCheckButton();
	m_nDrawMode = CIRCLE;
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_CIRCLE, true);
}

void CpictureboardDlg::OnTriangle()
{
	InitCheckButton();
	m_nDrawMode = TRIANGLE;
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_TRIANGLE, true);
}

void CpictureboardDlg::OnStraight()
{
	InitCheckButton();
	m_nDrawMode = STRAIGHT;
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_STRAIGHT, true);
}

void CpictureboardDlg::InitCheckButton()
{
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_SQUARE, false);
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_CIRCLE, false);
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_TRIANGLE, false);
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_STRAIGHT, false);
}

void CpictureboardDlg::OnDropDownCommand(UINT nID)
{
	switch (nID)
	{
	case ID_COLOR_1:
		break;
	}
}

