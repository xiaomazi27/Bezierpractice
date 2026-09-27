# MFC + OpenGL 三阶贝塞尔曲线
基于MFC框架与OpenGL绘制三阶贝塞尔曲线，支持鼠标交互。

## 功能
1. 绘制黄色三阶贝塞尔曲线
2. 绘制绿色控制多边形
3. 红色圆点为控制点，**鼠标左键按住红点可拖动控制点，曲线实时重绘更新**
4. 窗口大小自适应投影变换

## 开发环境
- Visual Studio
- MFC
- OpenGL

## 编译运行步骤
1. 使用VS打开 `Bezierhomework.sln`
2. 项目属性 → 链接器 → 输入 → 附加依赖项添加：`opengl32.lib;glu32.lib`
3. 编译并运行项目

## 核心文件
- `BezierhomeworkView.h / BezierhomeworkView.cpp`：OpenGL绘制逻辑、鼠标拖拽控制点交互代码
