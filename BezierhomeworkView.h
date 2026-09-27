
// BezierhomeworkView.h: CBezierhomeworkView 类的接口
//

#pragma once
#include <gl/gl.h>


class CBezierhomeworkView : public CView
{
protected: // 仅从序列化创建
	CBezierhomeworkView() noexcept;
	DECLARE_DYNCREATE(CBezierhomeworkView)
	HGLRC m_hRC;
	CClientDC* m_pDC;
	BOOL bOpenGLInit;
	void InitOpenGL();
	void DrawBezierCurve();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	GLfloat p[4][2];    // 4个控制点
	int selectedPoint; // 当前选中控制点，-1代表没选中
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);


// 特性
public:
	CBezierhomeworkDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CBezierhomeworkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // BezierhomeworkView.cpp 中的调试版本
inline CBezierhomeworkDoc* CBezierhomeworkView::GetDocument() const
   { return reinterpret_cast<CBezierhomeworkDoc*>(m_pDocument); }
#endif

