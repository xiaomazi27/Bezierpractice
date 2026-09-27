
// BezierhomeworkView.cpp: CBezierhomeworkView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Bezierhomework.h"
#endif

#include "BezierhomeworkDoc.h"
#include "BezierhomeworkView.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBezierhomeworkView

IMPLEMENT_DYNCREATE(CBezierhomeworkView, CView)

BEGIN_MESSAGE_MAP(CBezierhomeworkView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBezierhomeworkView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

END_MESSAGE_MAP()

// CBezierhomeworkView 构造/析构

CBezierhomeworkView::CBezierhomeworkView() noexcept
{
    m_hRC = NULL;
    m_pDC = nullptr;
    bOpenGLInit = FALSE;

    // 初始化4个控制点
    p[0][0] = 50;  p[0][1] = 200;
    p[1][0] = 150; p[1][1] = 50;
    p[2][0] = 300; p[2][1] = 300;
    p[3][0] = 400; p[3][1] = 180;
    selectedPoint = -1;
}

CBezierhomeworkView::~CBezierhomeworkView()
{
}

BOOL CBezierhomeworkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBezierhomeworkView 绘图

void CBezierhomeworkView::OnDraw(CDC* /*pDC*/)
{
	CBezierhomeworkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    if (!bOpenGLInit) return;
    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawBezierCurve();

    SwapBuffers(m_pDC->GetSafeHdc());
    wglMakeCurrent(NULL, NULL);

}


// CBezierhomeworkView 打印


void CBezierhomeworkView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBezierhomeworkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CBezierhomeworkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CBezierhomeworkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CBezierhomeworkView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBezierhomeworkView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBezierhomeworkView 诊断

#ifdef _DEBUG
void CBezierhomeworkView::AssertValid() const
{
	CView::AssertValid();
}

void CBezierhomeworkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBezierhomeworkDoc* CBezierhomeworkView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBezierhomeworkDoc)));
	return (CBezierhomeworkDoc*)m_pDocument;
}
#endif //_DEBUG


// CBezierhomeworkView 消息处理程序
int CBezierhomeworkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_pDC = new CClientDC(this);
    InitOpenGL();
    return 0;
}

void CBezierhomeworkView::InitOpenGL()
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };
    int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
    SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd);
    m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    bOpenGLInit = TRUE;
}

void CBezierhomeworkView::OnDestroy()
{
    if (m_hRC)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
    }
    if (m_pDC) delete m_pDC;
    CView::OnDestroy();
}

void CBezierhomeworkView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    if (bOpenGLInit && cx > 0 && cy > 0)
    {
        wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
        glViewport(0, 0, cx, cy);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, cx, cy, 0);
        glMatrixMode(GL_MODELVIEW);
        Invalidate(FALSE);
    }
}

void CBezierhomeworkView::DrawBezierCurve()
{
	// 绘制黄色贝塞尔曲线
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (float t = 0.0f; t <= 1.0f; t += 0.01f)
	{
		float mt = 1 - t;
		float x = mt * mt * mt * p[0][0] + 3 * mt * mt * t * p[1][0] + 3 * mt * t * t * p[2][0] + t * t * t * p[3][0];
		float y = mt * mt * mt * p[0][1] + 3 * mt * mt * t * p[1][1] + 3 * mt * t * t * p[2][1] + t * t * t * p[3][1];
		glVertex2f(x, y);
	}
	glEnd();

	// 红色控制点
	glColor3f(1, 0, 0);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		glVertex2fv(p[i]);
	glEnd();

	// 绿色控制线
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++)
		glVertex2fv(p[i]);
	glEnd();
}

// 鼠标左键按下：拾取控制点
void CBezierhomeworkView::OnLButtonDown(UINT nFlags, CPoint point)
{
    selectedPoint = -1;
    // 检测鼠标点距离哪个控制点近，阈值10像素
    for (int i = 0; i < 4; i++)
    {
        float dx = point.x - p[i][0];
        float dy = point.y - p[i][1];
        if (sqrt(dx * dx + dy * dy) < 10.0f)
        {
            selectedPoint = i;
            break;
        }
    }
    CView::OnLButtonDown(nFlags, point);
}

// 鼠标移动：拖动选中的点
void CBezierhomeworkView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (selectedPoint != -1 && (nFlags & MK_LBUTTON))
    {
        p[selectedPoint][0] = point.x;
        p[selectedPoint][1] = point.y;
        Invalidate(FALSE); // 触发窗口重绘，刷新曲线
    }
    CView::OnMouseMove(nFlags, point);
}

// 左键松开：取消选中
void CBezierhomeworkView::OnLButtonUp(UINT nFlags, CPoint point)
{
    selectedPoint = -1;
    CView::OnLButtonUp(nFlags, point);
}


