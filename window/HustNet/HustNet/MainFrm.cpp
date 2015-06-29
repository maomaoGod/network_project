/**
*MainFrm.cpp : CMainFrame 类的实现
*/

#include "stdafx.h"
#include "HustNet.h"
#include "MainFrm.h"
#include "NetSet.h"
#include "UIctrl.h"
#include <sys/stat.h>
#include <sys/types.h>   
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
*CMainFrame
*/
extern  CMainFrame *pframe;
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(DISPATCH,Dispatch)
	ON_WM_CLOSE()
	ON_COMMAND(ID_NETSET, &CMainFrame::OnNETSET)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	/**
	*状态行指示器
	*/
	ID_SEPARATOR,           
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/**
*CMainFrame 构造/析构
*/

CMainFrame::CMainFrame()
{
	/**
	*TODO:  在此添加成员初始化代码
	*/
	if (_access("cache", 0) == 0){
		if (_access("cache/html",0)==0)
			;
		else 
			CreateDirectory(_T("cache/html"), NULL);
	}
	else{
		CreateDirectory(_T("cache"), NULL);
		CreateDirectory(_T("cache/html"), NULL);
	}
	flag = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		/**
		*未能创建
		*/
		return -1;      
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	// TODO:  如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	//set the state bar Msg

	return 0;
}
/**
*@brief 调整框体类的属性
*@param cs  窗体属性结构体
*@note 函数在窗体即将创建时被触发。通过窗体属性结构体的引用，函数设置框架
*的高度和宽度分别为屏幕分辨率的3/4和5/8，并保证窗口的显示位置正好居中
*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~FWS_ADDTOTITLE;
	/**
	*cs.cx = 900;
	*cs.cy = 600;
	*设置高、宽 (屏幕分辨率的3/4)*/
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN) * 3 / 4;
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN) * 5 / 8;
	/**
	*设置窗口位置 (居中)
	*/
	cs.y = (cs.cy*4/3 - cs.cy) / 2;
	cs.x = (cs.cx*8/5 - cs.cx) / 2;
	return TRUE;
}

/**
*CMainFrame 诊断
*/

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


/**
*@author ACM2012
*@note实现窗口分割\n
*函数在框架类创建客户区时被触发。函数通过CSplitterWnd对客户区进行分割，首先使用\n
*CSplitterWnd将框架客户区分为两部分，分割的结果将1个客户区分为1*2的两个窗格，右侧\n
*窗格为响应窗口RespondView窗口区；然后再使用CSplitterWnd将左侧(0,0)窗格划分为2*1两\n
*个窗格,上侧窗格为CmdView窗口区，下侧窗格为LogView窗口区。分割完成后将对应的View\n
*动态创建到指定窗格
*/

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	/**
	*TODO:  在此添加专用代码和/或调用基类
	*/
	CRect rect;
	GetClientRect(&rect);
	BOOL bCreateSplit = m_splitter.CreateStatic(this, 1, 2);
	if (!s_splitter.CreateStatic(&m_splitter, 2, 1, WS_CHILD | WS_VISIBLE, m_splitter.IdFromRowCol(0, 0)))
	{
		return FALSE;
	}
	s_splitter.CreateView(0, 0, RUNTIME_CLASS(CmdView), CSize(rect.Width() / 2, rect.Height() / 2), pContext);
	s_splitter.CreateView(1, 0, RUNTIME_CLASS(LogView), CSize(rect.Width() / 2, rect.Height() / 4), pContext);
	m_splitter.CreateView(0, 1, RUNTIME_CLASS(RespondView), CSize(rect.Width() / 2, rect.Height()), pContext);
	CWnd *pwnd;
	pframe = this;
	pwnd = this->s_splitter.GetPane(0, 0);
	pcmd = DYNAMIC_DOWNCAST(CmdView, pwnd);
	pwnd = this->s_splitter.GetPane(1, 0);
	plog = DYNAMIC_DOWNCAST(LogView, pwnd);
	pwnd = this->m_splitter.GetPane(0, 1);
	prespond = DYNAMIC_DOWNCAST(RespondView, pwnd);
	flag = true;
	return bCreateSplit;
}

/**
*@author ACM2012
*@note 框体变化时动态调整窗格大小函数在框体大小发生变化时被触发。函数根据窗体的变化
*重新调整子窗格的大小，调整的规则为左侧窗格的宽度为整个客户区大小的1/3，右侧为2/3；
*左侧两个小窗格的高度均为客户区高度的1/2。所有窗格的高和宽均不得小于50像素，否则系统
*隐藏窗格
*/

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	/**
	*TODO:  在此处添加消息处理程序代码
	*/
	if (flag){
		if (nType != SIZE_MINIMIZED){
			CRect rect;
			/**
			*@brief 获取客户区大小
			*/
			GetClientRect(&rect);
			/**
			*@brief 设置窗格大小
			*/
			m_splitter.SetRowInfo(0, cy, 50);
			m_splitter.SetColumnInfo(0, rect.Width() / 3, 50);
			s_splitter.SetColumnInfo(0, rect.Width() / 3, 50);
			/**
			*@brief 更新窗格
			*/
			m_splitter.RecalcLayout();
		}
	}
}


/**
*@author ACM2012
*@param  wparam 标明消息需要发送的目的窗口标号   lparam  消息传送给目的窗口的参数
*@note 函数在接收到DISPATCH消息时函数被触发。函数根据wparam确定消息要被发送的目的窗口，
*并解析lparam中的内容，lparam指向一个CMD结构体，该结构体第一个元素为要发送给目的窗
*口的消息ID，第二个参数为要发送给目的窗口的参数args
*/
LRESULT CMainFrame::Dispatch(WPARAM wparam, LPARAM lparam)
{
	int CMD_ID;
	void *CMD_PARA1, *CMD_PARA2;
	CMD_ID = ((CMD *)lparam)->ID;
	CMD_PARA1 = ((CMD *)lparam)->para1;
	CMD_PARA2 = ((CMD *)lparam)->para2;
	switch (wparam)
	{
	case CMDVIEW: 
		pcmd->SendMessage(CMD_ID, (WPARAM)CMD_PARA1, (LPARAM)CMD_PARA2);
		break;
	case RPVIEW:
		prespond->SendMessage(CMD_ID, (WPARAM)CMD_PARA1, (LPARAM)CMD_PARA2);
		break;
	case LOGVIEW: 
		plog->SendMessage(CMD_ID, (WPARAM)CMD_PARA1, (LPARAM)CMD_PARA2);
		break;
	default: break;
	}
	return 0;
}

/**
*@author ACM2012
*@note 窗体关闭程序,动态效果关闭主窗体
*/
void CMainFrame::OnClose()
{
	/**
	*TODO:  在此添加消息处理程序代码和/或调用默认值
	*/
	CleanRp(NULL);
	if (AfxMessageBox(_T(" 是否退出软件 \n"), MB_OKCANCEL, MB_ICONQUESTION) == IDOK){
		CRect rectDlg;
		this->GetWindowRect(rectDlg);
		while (rectDlg.TopLeft().y+3 < rectDlg.BottomRight().y-3){//rectDlg.Height() > 30
			rectDlg.TopLeft().y += 3;
			rectDlg.BottomRight().y -= 3;
			this->MoveWindow(rectDlg);
		}
		DestroyWindow();//CFrameWnd::OnCloseWithout(0);
	}
	return;
}

/**
*@author ACM2012
*@note 打开网络设置对话框
*/
void CMainFrame::OnNETSET()
{
	/**
	*TODO:  在此添加命令处理程序代码
	*/
	/**
	*@brief 实例化设置对话框
	*/
	NetSet SetDlg;
	/**
	*@brief 模态显示设置对话框
	*/
	SetDlg.DoModal();
}
