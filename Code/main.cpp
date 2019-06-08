//雷霆战机
//author 君爸爸
//version 1.0
//2019/6/7
#include<Windows.h>
#include<Mmsystem.h>		//多媒体设备接口
#include<stdlib.h>
#pragma comment(lib,"winmm.lib")		//库文件
#pragma comment(lib,"Msimg32.lib")

//全局常量
const int CREATEDIJI = 101;

//全局变量
typedef struct Node {
	int x;
	int y;
	struct Node *pnext;
}Mine,Diji,Zidan;

Diji *pDiji = NULL;
Mine *pMine = NULL;
Zidan *pZidan = NULL;
int g_iBK = 0;

//全局函数的声明
void CreateMine(HWND hwnd);
void CreateZidan(HWND hwnd);
void ShowZidan(Zidan *phead, HWND hwnd);
void Move_Zidan(Zidan *phead);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance,
	LPSTR lpCmdLine, int nShowCmd) {
	//窗口创建4步
	//1.设计窗口
	WNDCLASSEX wnd = { 0 };				//定义一个窗口
	wnd.cbSize = sizeof(WNDCLASSEX);		//窗口的字节数大小
	wnd.hbrBackground = (HBRUSH)5;			//窗口背景颜色
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance;				//实例句柄
	wnd.hCursor = NULL;					//光标
	wnd.hIcon = NULL;						//像标
	wnd.lpfnWndProc = WndProc;				//窗口的过程函数
	wnd.style = CS_HREDRAW | CS_VREDRAW;	//窗口的样式
	wnd.lpszMenuName = NULL;				//菜单栏
	wnd.lpszClassName = L"For our dreams";	//窗口的类名称

	//2.注册窗口（就是把设计号的窗口给系统）
	RegisterClassEx(&wnd);

	//3.正式的创建窗口
	HWND hwnd = CreateWindow(L"For our dreams",			//窗口类名称
							L"君爸爸.雷霆战机",		//窗口标题
							WS_OVERLAPPEDWINDOW,		//窗口的弹出样式
							500, 225, 600, 600,		//窗口的坐标的大小
							0, 0, hInstance, 0);

	//4.显示窗口，更新窗口
	ShowWindow(hwnd, nShowCmd);		//显示
	UpdateWindow(hwnd);				//更新

	//文件名 窗口句柄 文件类型 异步处理（音乐播放的同时继续往下运行代码） 循环播放 
	PlaySound(L"123.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//消息循环 只要不点击退出 总是在不断的接收客户发出的指令 并且对这些指令做出相应的动作
	MSG msg = { 0 };							//定义一个消息
	while (GetMessageA(&msg, 0, 0, 0)) {		//获取消息
		TranslateMessage(&msg);				//翻译消息
		DispatchMessage(&msg);					//把处理过的消息传递给系统
	}

	return 0;
}

//窗口的过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{ 
		HDC hdc;
		HDC memdc;
		HBITMAP hBitmap;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		hBitmap = (HBITMAP)LoadImage(NULL, L"Image1.bmp", IMAGE_BITMAP,
								600, 600, LR_LOADFROMFILE);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);

		//BitBlt(hdc, 0, 0, 600, 600, memdc, 0, 0, SRCCOPY);		//放置背景图
		BitBlt(hdc, 0, g_iBK, 600, 600 - g_iBK, memdc, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, 600, g_iBK, memdc, 0, 600 - g_iBK, SRCCOPY);
		g_iBK += 5;
		if (g_iBK == 600) {
			g_iBK = 0;
		}
		 
		CreateMine(hwnd);
		ShowZidan(pZidan, hwnd);
		Move_Zidan(pZidan);
		break;

	case WM_CREATE:
		pMine = (struct Node*)malloc(sizeof(struct Node));
		pMine->x = 275;
		pMine->y = 375;
		SetTimer(hwnd, CREATEDIJI, 200, NULL);
		break;
	/*
	case WM_LBUTTONDOWN:								//鼠标左键按下
		MessageBox(NULL,L"hello world1",NULL,NULL);		//消息盒子
		break;

	case WM_LBUTTONDBLCLK:								//鼠标双击
		break;
	*/

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			pMine->y -= 20;
			break;
		case VK_DOWN:
			pMine->y += 20;
			break;
		case VK_LEFT:
			pMine->x -= 20;
			break;
		case VK_RIGHT:
			pMine->x += 20;
			break;
		case VK_SPACE:
			CreateZidan(hwnd);
			break;
		}
		break;

	case WM_DESTROY:									//关闭程序按键
		PostQuitMessage(0);							//后台退出程序
		break;
	}
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void CreateMine(HWND hwnd) {
	HDC hdc;				//设备上下文 DC
	HDC memdc;			//内存 DC
	HBITMAP hBitmap;		//位图的句柄（图片）

	hdc = GetDC(hwnd);
	hBitmap = (HBITMAP)LoadImage(NULL, L"Image.bmp", IMAGE_BITMAP,
								60, 60, LR_LOADFROMFILE);

	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hBitmap);

	//BitBlt(hdc, pMine->x, pMine->y, 60, 60, memdc, 0, 0, SRCCOPY);	//图片放置
	TransparentBlt(hdc, pMine->x, pMine->y, 60, 60, memdc,			//去背景
				0, 0, 60, 60, RGB(0, 0, 0));
}

//做子弹
void CreateZidan(HWND hwnd) {
	Zidan *u;
	u = (Zidan*)malloc(sizeof(Zidan));
	u->x = pMine->x;
	u->y = pMine->y;
	u->pnext = NULL;
	if (NULL == pZidan) {
		pZidan = u;
	}
	else {
		u->pnext = pZidan;
		pZidan = u;
	}
}

//显示子弹
void ShowZidan(Zidan *phead,HWND hwnd) {
	HDC hdc;				//设备上下文 DC
	HDC memdc;			//内存 DC
	HBITMAP hBitmap;		//位图的句柄（图片）

	hdc = GetDC(hwnd);
	hBitmap = (HBITMAP)LoadImage(NULL, L"Image2.bmp", IMAGE_BITMAP,
		60, 25, LR_LOADFROMFILE);

	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hBitmap);

	while (phead != NULL) {
		TransparentBlt(hdc, phead->x, phead->y, 60, 25, memdc,			//去背景
			0, 0, 60, 25, RGB(0, 0, 0));
		phead = phead->pnext;
	}
}

void Move_Zidan(Zidan *phead) {			//子弹移动
	while (phead != NULL) {
		phead->y -= 10;
		phead = phead->pnext;
	}
}