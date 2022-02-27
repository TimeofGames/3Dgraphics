#include <stdexcept>
#include <cmath>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <winbase.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#define PI 3.14159265
#define width 1080
#define height 720

using namespace std;

class Complex{
private:
	float real;
	float imagi;
public:
	Complex(float real, float imagi) {
		this->real = real;
		this->imagi = imagi;
	}
	void fourth() {
		float temp = (pow(real, 4) - 6 * pow(real, 2) * pow(imagi, 2) + pow(imagi, 4));
		imagi = 4 * pow(real, 3) * imagi - 4 * real * pow(imagi, 3);
		real = temp;
	}

	float magnitude() {
		return sqrt(pow(real,2) + pow(imagi,2));
	}

	void add(Complex other) {
		real += other.real;
		imagi += other.imagi;
	}
};


void FourthMandelbrod( HWND hWnd) {
	int iterations = 10;
	HDC hdc = GetDC(hWnd);
	for (int x = 0; x<width;x++) {
		for (int y = 0; y < height;y++) {
			float a = (x - (float)(width / 2)) / (float)(width / 4);
			float b = (y - (float)(height/ 2)) / (float)(height/ 4);
			Complex c = Complex(a, b);
			Complex z = Complex(0, 0);
			int iter = 0;
			do {
				iter++;
				z.fourth();
				z.add(c);
				if (z.magnitude() > 2.0) { 
					break; 
				}
			} while (iter < iterations);
			if (iter < iterations) {
				SetPixel(hdc, x, y, RGB(iter*25, iter*25, iter*25));
			}
			else {
				SetPixel(hdc, x, y, RGB(0, 0, 0));
			}
		}
	}
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{


	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		width,					  // initial x size
		height,					  // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	//FourthMandelbrod(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
} 

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	
	case WM_PAINT:
		FourthMandelbrod(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

