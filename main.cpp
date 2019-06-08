//����ս��
//author ���ְ�
//version 1.0
//2019/6/7
#include<Windows.h>
#include<Mmsystem.h>		//��ý���豸�ӿ�
#include<stdlib.h>
#pragma comment(lib,"winmm.lib")		//���ļ�
#pragma comment(lib,"Msimg32.lib")

//ȫ�ֳ���
const int CREATEDIJI = 101;

//ȫ�ֱ���
typedef struct Node {
	int x;
	int y;
	struct Node *pnext;
}Mine,Diji,Zidan;

Diji *pDiji = NULL;
Mine *pMine = NULL;
Zidan *pZidan = NULL;
int g_iBK = 0;

//ȫ�ֺ���������
void CreateMine(HWND hwnd);
void CreateZidan(HWND hwnd);
void ShowZidan(Zidan *phead, HWND hwnd);
void Move_Zidan(Zidan *phead);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance,
	LPSTR lpCmdLine, int nShowCmd) {
	//���ڴ���4��
	//1.��ƴ���
	WNDCLASSEX wnd = { 0 };				//����һ������
	wnd.cbSize = sizeof(WNDCLASSEX);		//���ڵ��ֽ�����С
	wnd.hbrBackground = (HBRUSH)5;			//���ڱ�����ɫ
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance;				//ʵ�����
	wnd.hCursor = NULL;					//���
	wnd.hIcon = NULL;						//���
	wnd.lpfnWndProc = WndProc;				//���ڵĹ��̺���
	wnd.style = CS_HREDRAW | CS_VREDRAW;	//���ڵ���ʽ
	wnd.lpszMenuName = NULL;				//�˵���
	wnd.lpszClassName = L"For our dreams";	//���ڵ�������

	//2.ע�ᴰ�ڣ����ǰ���ƺŵĴ��ڸ�ϵͳ��
	RegisterClassEx(&wnd);

	//3.��ʽ�Ĵ�������
	HWND hwnd = CreateWindow(L"For our dreams",			//����������
							L"���ְ�.����ս��",		//���ڱ���
							WS_OVERLAPPEDWINDOW,		//���ڵĵ�����ʽ
							500, 225, 600, 600,		//���ڵ�����Ĵ�С
							0, 0, hInstance, 0);

	//4.��ʾ���ڣ����´���
	ShowWindow(hwnd, nShowCmd);		//��ʾ
	UpdateWindow(hwnd);				//����

	//�ļ��� ���ھ�� �ļ����� �첽�������ֲ��ŵ�ͬʱ�����������д��룩 ѭ������ 
	PlaySound(L"123.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//��Ϣѭ�� ֻҪ������˳� �����ڲ��ϵĽ��տͻ�������ָ�� ���Ҷ���Щָ��������Ӧ�Ķ���
	MSG msg = { 0 };							//����һ����Ϣ
	while (GetMessageA(&msg, 0, 0, 0)) {		//��ȡ��Ϣ
		TranslateMessage(&msg);				//������Ϣ
		DispatchMessage(&msg);					//�Ѵ��������Ϣ���ݸ�ϵͳ
	}

	return 0;
}

//���ڵĹ��̺���
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

		//BitBlt(hdc, 0, 0, 600, 600, memdc, 0, 0, SRCCOPY);		//���ñ���ͼ
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
	case WM_LBUTTONDOWN:								//����������
		MessageBox(NULL,L"hello world1",NULL,NULL);		//��Ϣ����
		break;

	case WM_LBUTTONDBLCLK:								//���˫��
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

	case WM_DESTROY:									//�رճ��򰴼�
		PostQuitMessage(0);							//��̨�˳�����
		break;
	}
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void CreateMine(HWND hwnd) {
	HDC hdc;				//�豸������ DC
	HDC memdc;			//�ڴ� DC
	HBITMAP hBitmap;		//λͼ�ľ����ͼƬ��

	hdc = GetDC(hwnd);
	hBitmap = (HBITMAP)LoadImage(NULL, L"Image.bmp", IMAGE_BITMAP,
								60, 60, LR_LOADFROMFILE);

	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hBitmap);

	//BitBlt(hdc, pMine->x, pMine->y, 60, 60, memdc, 0, 0, SRCCOPY);	//ͼƬ����
	TransparentBlt(hdc, pMine->x, pMine->y, 60, 60, memdc,			//ȥ����
				0, 0, 60, 60, RGB(0, 0, 0));
}

//���ӵ�
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

//��ʾ�ӵ�
void ShowZidan(Zidan *phead,HWND hwnd) {
	HDC hdc;				//�豸������ DC
	HDC memdc;			//�ڴ� DC
	HBITMAP hBitmap;		//λͼ�ľ����ͼƬ��

	hdc = GetDC(hwnd);
	hBitmap = (HBITMAP)LoadImage(NULL, L"Image2.bmp", IMAGE_BITMAP,
		60, 25, LR_LOADFROMFILE);

	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hBitmap);

	while (phead != NULL) {
		TransparentBlt(hdc, phead->x, phead->y, 60, 25, memdc,			//ȥ����
			0, 0, 60, 25, RGB(0, 0, 0));
		phead = phead->pnext;
	}
}

void Move_Zidan(Zidan *phead) {			//�ӵ��ƶ�
	while (phead != NULL) {
		phead->y -= 10;
		phead = phead->pnext;
	}
}