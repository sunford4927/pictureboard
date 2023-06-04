
// pictureboardDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "pictureboard.h"
#include "pictureboardDlg.h"
#include "afxdialogex.h"
#include "DrawBox.h"
#include <locale.h> // 글자 깨짐 방지

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PRINTPATH "/"
#define MAX_FOLYLINE 10;

#define NONE 0

#define DRAW 1
#define	CHOICE 2

#define SQUARE	3 // 사각형
#define CIRCLE	4 // 원
#define TRIANGLE	5 // 삼각형
#define STRAIGHT	6 // 직선


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
	ON_COMMAND(ID_IMGSAVE, &CpictureboardDlg::OnImageSave)
//	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_IMGLOAD, &CpictureboardDlg::OnImgload)
	ON_COMMAND(ID_RESET, &CpictureboardDlg::OnEraseBkgnd)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_COLOR, &CpictureboardDlg::ChoiceView)
	ON_COMMAND_RANGE(ID_COLOR_1, ID_COLOR_1 +1, OnDropDownMenu)
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
	//m_pos = nullptr;
	m_gR = 0;
	m_gG = 0;
	m_gB = 0;
	InitToolBar();
	m_subMode = NONE;
	m_mainMode = NONE;
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
		AreaDraw();
		//CDialogEx::OnPaint();
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
	m_cx = cx;
	m_cy = cy;
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
	m_rect_start_pos = point;
	m_prevXY = point;

	if (m_mainMode == NONE)
	{
		POSITION pos = m_ModelList.GetHeadPosition();
		_SModel next;
		int idx = 0;
		while (pos)
		{
			next = m_ModelList.GetNext(pos);
			if (
				point.x >= next.m_cpStart.x &&
				point.y >= next.m_cpStart.y &&
				point.x <= next.m_cpEnd.x &&
				point.y <= next.m_cpEnd.y
				)
			{
				next.m_isClick = true;
				m_prevXY = point;
				m_pos = idx;
				m_mainMode = CHOICE;
				m_subMode = next.m_iDrawMode;
			}
			idx++;
		}
	}
}


void CpictureboardDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC lc(this);
	CPen clear_pen(PS_SOLID, 5, RGB(255, 255, 255));
	lc.SelectObject(&clear_pen);
	SelectObject(lc, GetStockObject(NULL_BRUSH));
	if (m_mainMode == DRAW)
	{
		OnDraw(point);
	}

	if (m_mainMode == CHOICE)
	{
		CPoint move_pos;
		CPoint prev_pos;
		// 마우스가 이동한 거리를 계산
		m_rect_start_pos = m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_cpStart;

		move_pos.x = point.x - m_prevXY.x;
		move_pos.y = point.y - m_prevXY.y;

		prev_pos = m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_cpEnd;

		m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_cpStart.x += move_pos.x;
		m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_cpStart.y += move_pos.y;

		m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_cpEnd.x += move_pos.x;
		m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_cpEnd.y += move_pos.y;
		switch (m_subMode)
		{
		case SQUARE:
			SquareDraw(lc, m_rect_start_pos, prev_pos);
			AreaDraw();
			m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_isClick = false;
			m_mainMode = NONE;
			break;
		case CIRCLE:
			CircleDraw(lc, m_rect_start_pos, prev_pos);
			AreaDraw();
			m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_isClick = false;
			m_mainMode = NONE;
			break;
		case TRIANGLE:
			TriangleDraw(lc, m_rect_start_pos, prev_pos);
			AreaDraw();
			m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_isClick = false;
			m_mainMode = NONE;
			break;
		case STRAIGHT:
			StraightDraw(lc, m_rect_start_pos, prev_pos);
			AreaDraw();
			m_ModelList.GetAt(m_ModelList.FindIndex(m_pos)).m_isClick = false;
			m_mainMode = NONE;
			break;
		}

	}
	
	
	if (nFlags & MK_CONTROL); // 컨트롤 키가 눌려졌을때

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CpictureboardDlg::OnSquare()
{
	if (m_subMode == SQUARE) {
		// 버튼이 이미 선택된 상태이므로 선택 해제한다.
		m_mainMode = NONE;
		m_subMode = NONE;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_SQUARE, FALSE);
	}
	else {
		// 다른 버튼들의 선택을 해제하고 현재 버튼을 선택한다.
		InitCheckButton(ID_CIRCLE, ID_TRIANGLE, ID_STRAIGHT);
		m_mainMode = DRAW;
		m_subMode = SQUARE;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_SQUARE, TRUE);
	}
}

void CpictureboardDlg::OnCircle()
{
	if (m_subMode == CIRCLE) {
		// 버튼이 이미 선택된 상태이므로 선택 해제한다.
		m_subMode = NONE;
		m_mainMode = NONE;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_CIRCLE, FALSE);
	}
	else {
		// 다른 버튼들의 선택을 해제하고 현재 버튼을 선택한다.
		m_mainMode = DRAW;
		InitCheckButton(ID_SQUARE, ID_TRIANGLE, ID_STRAIGHT);
		m_subMode = CIRCLE;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_CIRCLE, TRUE);
	}
}

void CpictureboardDlg::OnTriangle()
{
	if (m_subMode == TRIANGLE) {
		// 버튼이 이미 선택된 상태이므로 선택 해제한다.
		m_subMode = NONE;
		m_mainMode = NONE;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_TRIANGLE, FALSE);
	}
	else {
		// 다른 버튼들의 선택을 해제하고 현재 버튼을 선택한다.
		m_mainMode = DRAW;
		InitCheckButton(ID_SQUARE, ID_CIRCLE, ID_STRAIGHT);
		m_subMode = TRIANGLE;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_TRIANGLE, TRUE);
	}
}

void CpictureboardDlg::OnStraight()
{
	if (m_subMode == STRAIGHT) {
		// 버튼이 이미 선택된 상태이므로 선택 해제한다.
		m_subMode = NONE;
		m_mainMode = NONE;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_STRAIGHT, FALSE);
	}
	else {
		// 다른 버튼들의 선택을 해제하고 현재 버튼을 선택한다.
		InitCheckButton(ID_SQUARE, ID_CIRCLE, ID_TRIANGLE);
		m_subMode = STRAIGHT;
		m_mainMode = DRAW;
		m_wndToolBar.GetToolBarCtrl().CheckButton(ID_STRAIGHT, TRUE);
	}
}


void CpictureboardDlg::InitCheckButton(int value1,int value2,int value3)
{
	//m_wndToolBar.GetToolBarCtrl().CheckButton(ID_SQUARE, FALSE);
	m_wndToolBar.GetToolBarCtrl().CheckButton(value1, FALSE);
	m_wndToolBar.GetToolBarCtrl().CheckButton(value2, FALSE);
	m_wndToolBar.GetToolBarCtrl().CheckButton(value3, FALSE);
}





//void CpictureboardDlg::OnContextMenu(CWnd* pWnd, CPoint point)
//{
//	// 컨텍스트 메뉴 생성
//	CMenu menu;
//	menu.CreatePopupMenu();
//
//	// 메뉴를 추가
//	menu.AppendMenu(MF_STRING, 2001, _T("Menu 1"));
//	menu.AppendMenu(MF_STRING, 2001, _T("Menu 1"));
//	menu.AppendMenu(MF_STRING, 2001, _T("Menu 1"));
//	// 컨텍스트  메뉴를 x,y 좌표에 출력합니다.
//	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, AfxGetMainWnd());
//}


CpictureboardDlg::_SModel CpictureboardDlg::OnMakeModel(int type, CPoint startPoint, CPoint endPoint)
{
	_SModel model;
	model.m_iDrawMode = type;
	model.m_cpStart = startPoint;
	model.m_cpEnd = endPoint;
	model.m_iCnt+=1;
	model.m_isClick =FALSE;
	model.m_sR = m_gR;
	model.m_sG = m_gG;
	model.m_sB = m_gB;
	return model;
}

void CpictureboardDlg::OnDraw(CPoint endPoint)
{
	_SModel model;
	CClientDC dc(this); // DC 생성
	CPen my_pen(PS_SOLID, 5, RGB(m_gR, m_gG, m_gB)); // 굵기가 5인 팬을 생성한다.
	dc.SelectObject(&my_pen); // 생성한 팬을 DC에 연결한다.
	SelectObject(dc, GetStockObject(NULL_BRUSH));// 안이 투명한 도형을 그려주기 위해 NULL 브러쉬를 만든다.

	if (m_mainMode == DRAW)
	{
		if (m_subMode == SQUARE)
		{
			SquareDraw(dc, m_rect_start_pos, endPoint);
			model = OnMakeModel(m_subMode, m_rect_start_pos, endPoint);
			m_ModelList.AddTail(model);
		}
		else if (m_subMode == CIRCLE)
		{
			CircleDraw(dc, m_rect_start_pos, endPoint);
			model = OnMakeModel(m_subMode, m_rect_start_pos, endPoint);
			m_ModelList.AddTail(model);
		}
		else if (m_subMode == TRIANGLE)
		{
			TriangleDraw(dc, m_rect_start_pos, endPoint);
			model = OnMakeModel(m_subMode, m_rect_start_pos, endPoint);
			m_ModelList.AddTail(model);
		}
		else if (m_subMode == STRAIGHT)
		{
			StraightDraw(dc, m_rect_start_pos, endPoint);
			model = OnMakeModel(m_subMode, m_rect_start_pos, endPoint);
			m_ModelList.AddTail(model);
		}
	}
}

void CpictureboardDlg::OnDraw(int type, CPoint startPoint, CPoint endPoint,int R, int G, int B)
{
	CClientDC dc(this); // DC 생성
	CPen my_pen(PS_SOLID, 5, RGB(R, G, B)); // 굵기가 5인 팬을 생성한다.
	dc.SelectObject(&my_pen); // 생성한 팬을 DC에 연결한다.
	SelectObject(dc, GetStockObject(NULL_BRUSH));// 안이 투명한 도형을 그려주기 위해 NULL 브러쉬를 만든다.

	
	if (type == SQUARE)
	{
		SquareDraw(dc, startPoint, endPoint);
	}
	else if (type == CIRCLE)
	{
		CircleDraw(dc, startPoint, endPoint);
	}
	else if (type == TRIANGLE)
	{
		TriangleDraw(dc, startPoint, endPoint);
	}
	else if (type == STRAIGHT)
	{
		StraightDraw(dc,startPoint,endPoint);
	}
}

void CpictureboardDlg::OnImageSave()
{
	CString szFilter = _T("Image (*.BMP, *.PNG, *.JPG) | *.BMP;*.PNG;*.JPG | All Files(*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	CString title;
	if (IDOK == dlg.DoModal())
	{
		title.Format(L"%s", dlg.m_pOFN->lpstrFileTitle);

		CClientDC dc(this);
		CImage img;
		int color_depth = ::GetDeviceCaps(dc, BITSPIXEL);
		img.Create(m_cx, m_cy-25, color_depth, 10);
		BitBlt(img.GetDC(), 0, -25, m_cx, m_cy, dc, 0, 0, SRCCOPY);
		img.Save(title+".png", Gdiplus::ImageFormatPNG);
		img.ReleaseDC();

	}
		CStdioFile file;
		setlocale(LC_ALL, "korean");
		file.Open(title+".obj", CFile::modeCreate | CFile::modeWrite, nullptr);
		CString str = _T("");
		POSITION pos = m_ModelList.GetHeadPosition();
		_SModel next;
		while (pos)
		{
			next = m_ModelList.GetNext(pos);

			str.Format(_T("%d,%d,%d,%d,%d,%d,%d\n"),
				next.m_iCnt,
				next.m_iDrawMode,
				next.m_cpStart.x,
				next.m_cpStart.y,
				next.m_cpEnd.x,
				next.m_cpEnd.y,
				next.m_isClick
			);
			file.SeekToEnd();
			file.WriteString(str);
		}

		file.Close();


}


/*
	CFileDialog(
	BOOL bOpenFileDialog, 
	LPCTSTR lpszDefExt = null, 
	LPCTSTR lpszFileName =NULL, 
	DWORD dwFlags = OFNpHIDEREADONLY | OFN_OVERWRITERPROMPT,
	CWnd * pParentWnd=NULL
	);

	BOOL bOpenFileDialog : 이 값이 TRUE면 열기 다이얼로그 이고 FALSE이면 파일 저장 다이얼로그 박스이다
	LPCTSTR lpszDefExt = null : 파일열기시 확장자 지정 // 저장시 자동 확장자 붙음 예)"hwp"
	LPCTSTR lpszFileName =NULL : default 파일명으로 저장할 때.. 예)"NoName"
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITERPROMPT : 아래의 플래그 값을 OR연산자로 조합하여 이용할 수있다
	//
		OFN_ALLOWMULTSELECT : 파일이름 리스트박스에서 파일을 여러개 선택할 수 잇도록 지정한다.
		OFN_FILEMUSTEXIST : 사용자가 이미 존재하는 파일만을 선택할 수 있도록 한다. 이 프래그가 지정되면 OFN_PATHNUSTEXIST도 자동 지정
		OFN_HIDEREADONLY : 읽기전용파일은 출력하지 않음.
		OFN_OVERWRITERPROMPT : 기존의 파일을 덮어쓰도록 한다
		OFN_PATHMUSTEXIST : 존재하지 않는 디렉토리나 파일을 선택시 경고 메시지 박스가 나타난다.
		LPCTSTR lpszFilter = NULL - 파일 형식 박스에 나타날 파일의 형태를 지정
			예) “실행파일(*.EXE|*.EXE|모든파일(*.*)|*.*||”파일형식 박스에 나타나는 내용은 다음과 같다.
		실행파일(*.EXE)
		모든파일(*.*)
	//
	CWnd * pParentWnd=NULL - 대부분 널 값을 준다 다이얼로그 박스의 부모 윈도우를 지정
*/





void CpictureboardDlg::OnImgload()
{
	CImage image;
	CString szFilter = _T("Image (*.BMP, *.PNG, *.JPG) | *.BMP;*.PNG;*.JPG | All Files(*.*)|*.*||");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (CString)szFilter, NULL);
	CString title ,rawname;
	int lastidx;
	if (IDOK == dlg.DoModal())

	{

		CClientDC dc(this);
		title.Format(L"%s", dlg.m_pOFN->lpstrFileTitle);
		lastidx = title.ReverseFind('.');
		if (lastidx != -1)
		{
			rawname = title.Left(lastidx);
		}
		image.Load(rawname+".png");
		image.Draw(dc, 0, 25);

	}

	/// <summary>
	/// 사용자의 눈에 txt 파일을 숨기기위해서는 절대경로를 따르 지정해 줘야함
	/// </summary>

	_SModel model;
	CStdioFile file;
	file.Open(rawname+_T(".obj"), CFile::modeRead | CFile::shareDenyNone);
	CString str_oneLine;
	while (file.ReadString(str_oneLine))
	{
		CStringArray strArray;
		OnSplit(str_oneLine, _T(","), strArray);
		model.m_iCnt = _ttoi(strArray.GetAt(0));
		model.m_iDrawMode = _ttoi(strArray.GetAt(1));
		model.m_cpStart.x = _ttoi(strArray.GetAt(2));
		model.m_cpStart.y = _ttoi(strArray.GetAt(3));
		model.m_cpEnd.x = _ttoi(strArray.GetAt(4));
		model.m_cpEnd.y = _ttoi(strArray.GetAt(5));
		model.m_isClick = strArray.GetAt(6) == "1"? TRUE :FALSE;
		m_ModelList.AddTail(model);
	}
}

void CpictureboardDlg::OnSplit(CString value, CString phraser, CStringArray& strs)
{
	int count = 0;

	CString tempStr = value;

	int length = value.GetLength();

	while (length)
	{
		int find = tempStr.Find(phraser);
		if (find != -1)
		{
			CString temp = tempStr.Left(find);
			int varLen = _tcslen(phraser);
			strs.Add(tempStr.Left(find));
			tempStr = tempStr.Mid(find + varLen);
			count++;
		}
		else
		{
			length = 0;
		}
	}
}

void CpictureboardDlg::OnReset(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
}


void CpictureboardDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
		_SModel model;
		CClientDC dc(this); // DC 생성
		CClientDC lc(this);
		CPen clear_pen(PS_SOLID, 5, RGB(255, 255, 255));
		lc.SelectObject(&clear_pen);
		SelectObject(lc, GetStockObject(NULL_BRUSH));
		CPen my_pen(PS_SOLID, 5, RGB(m_gR, m_gG, m_gB)); // 굵기가 5인 팬을 생성한다.
		dc.SelectObject(&my_pen); // 생성한 팬을 DC에 연결한다.
		SelectObject(dc, GetStockObject(NULL_BRUSH));// 안이 투명한 도형을 그려주기 위해 NULL 브러쉬를 만든다.

	if (m_mainMode == DRAW && m_subMode != NONE && nFlags == MK_LBUTTON)
	{

		if (m_subMode == SQUARE)
		{
			SquareDraw(lc, m_rect_start_pos, m_prevXY);
			SquareDraw(dc, m_rect_start_pos, point);
			m_prevXY = point;
			AreaDraw();

		}
		else if (m_subMode == CIRCLE)
		{
			CircleDraw(lc, m_rect_start_pos, m_prevXY);
			CircleDraw(dc, m_rect_start_pos, point);
			m_prevXY = point;
			AreaDraw();
		}
		else if (m_subMode == TRIANGLE)
		{
			TriangleDraw(lc, m_rect_start_pos, m_prevXY);
			TriangleDraw(dc, m_rect_start_pos, point);

			m_prevXY = point;
			AreaDraw();
		}
		else if (m_subMode == STRAIGHT)
		{
			StraightDraw(lc, m_rect_start_pos, m_prevXY);
			StraightDraw(dc, m_rect_start_pos, point);
			m_prevXY = point;
			AreaDraw();
		}
	}

}

void CpictureboardDlg::AreaDraw()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	POSITION pos = m_ModelList.GetHeadPosition();
	_SModel next;
	while (pos)
	{
		next = m_ModelList.GetNext(pos);
		OnDraw(next.m_iDrawMode, next.m_cpStart, next.m_cpEnd, next.m_sR, next.m_sG, next.m_sB);
	}
}

void CpictureboardDlg::SquareDraw(CClientDC &dc, CPoint startPoint, CPoint endPoint)
{
	dc.Rectangle(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
}

void CpictureboardDlg::CircleDraw(CClientDC &dc, CPoint startPoint, CPoint endPoint)
{
	dc.Ellipse(startPoint.x, startPoint.y, endPoint.x, endPoint.y); // 원 그리기
}

void CpictureboardDlg::TriangleDraw(CClientDC &dc, CPoint startPoint, CPoint endPoint)
{
	POINT arPoint[] = { startPoint.x, startPoint.y , startPoint.x - 40, endPoint.y + 45, endPoint.x , endPoint.y }; // 삼각형 함수 그리기
	dc.Polygon(arPoint, 3);
}

void CpictureboardDlg::StraightDraw(CClientDC &dc,CPoint startPoint, CPoint endPoint)
{

	dc.MoveTo(startPoint.x, startPoint.y);
	dc.LineTo(endPoint.x, endPoint.y);
}



void CpictureboardDlg::OnColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 컨텍스트 메뉴 생성
	CMenu menu;
	menu.CreatePopupMenu();

	// 메뉴를 추가
	menu.AppendMenu(MF_STRING, 2001, _T("Menu 1"));
	menu.AppendMenu(MF_STRING, 2001, _T("Menu 1"));
	menu.AppendMenu(MF_STRING, 2001, _T("Menu 1"));
	// 컨텍스트  메뉴를 x,y 좌표에 출력합니다.
	menu.TrackPopupMenu(TPM_LEFTALIGN, m_rect_start_pos.x, m_rect_start_pos.y, AfxGetMainWnd());
}

void CpictureboardDlg::ChoiceView()
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		COLORREF color = dlg.GetColor();
		m_gR = GetRValue(color);
		m_gG = GetGValue(color);
		m_gB = GetBValue(color);
	}
}



void CpictureboardDlg::OnDropDownMenu(UINT nID)
{
	CRect rect;
	CWnd* pWnd = GetDlgItem(nID);
	pWnd->GetWindowRect(&rect);
	m_Color_DropDown.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON , rect.left, rect.bottom, this);

}

