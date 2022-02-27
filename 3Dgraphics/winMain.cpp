#include <stdexcept>
#include <cmath>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <winbase.h>
#include <winuser.h>
#include <ctime>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#define PI 3.14159265
#define WIDHT 900
#define HEIGHT 506

using namespace std;

class affinsMatrix2D {
private:
	float matrix[3][3];
	float** retMatrix;
	int affinsType;

	void reloadMatrix() {
		for (int i = 0; i < 3; i++ ) {
			for (int j = 0; j < 3; j++ ) {
				if (i != j) { matrix[i][j] = 0; }
				else { matrix[i][j] = 1; }
			}
		}
	}
	float degreeToRad(float degree) {
		return degree * PI / 180;
	}
	void placeHolder() {
		retMatrix = (float**)malloc(sizeof(float*) * 3);
		for (int i = 0; i < 3;i++) {
			retMatrix[i] = (float*)malloc(sizeof(float) * 3);
		}
	}
public:
	affinsMatrix2D(){
		this->reloadMatrix();
		this->placeHolder();
	}

	void rotate(float angle) {
		this->reloadMatrix();
		angle = degreeToRad(angle);
		matrix[0][0] = cos(angle);
		matrix[1][1] = cos(angle);
		matrix[0][1] = sin(angle);
		matrix[1][0] = -sin(angle);
		affinsType = 2;
	}
	void move_on(int x = 0, int y = 0) {
		this->reloadMatrix();
		matrix[0][2] = x;
		matrix[1][2] = y;
		affinsType = 0;
	}
	void scaling(float multuX = 1, float multuY = 1) {
		this->reloadMatrix();
		matrix[0][0] = 1/multuX;
		matrix[1][1] = 1/multuY;
		affinsType = 1;
	}
	float** getMatrix() {
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {retMatrix[i][j] = matrix[i][j];}
		}
		return retMatrix;
	}
	int getType() {
		return affinsType;
	}
};

class affinsMatrix3D {
private:
	float matrix[4][4];
public:
	void rotate(float angle) {

	}
	void move_on(int x, int y, int z) {

	}
	void augmentation(float multu) {

	}
};

class point2D {
private:
	float x, y, z;
public:
	point2D() {
		this->x = 1;
		this->y = 1;
		this->z = 1;
	}
	point2D(float x, float y, float z = 1) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float get(int i) {
		switch (i)
		{
		case 0:
			return x;
			
		case 1:
			return y;
		case 2:
			return z;
		default:
			break;
		}
	}
	void set(int i, float arg) {
		switch (i)
		{
		case 0:
			x = arg;
			break;
		case 1:
			y = arg;
			break;
		default:
			break;
		}
	}
};

class point3D {
private:
	float x, y, z, w;
public:
	point3D(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 1;
	}
};

class point2DArray{
private:
	point2D* pointArray;
	int realSize;
	int maxSize;
	float xMin = INFINITY;
	float yMin = INFINITY;
	float xMax = -INFINITY;
	float yMax = -INFINITY;
	point2D center;

	void checkBorders(point2D point) {
		if (point.get(0) < xMin) { xMin = point.get(0); }
		if (point.get(0) > xMax) { xMax = point.get(0); }
		if (point.get(1) < yMin) { yMin = point.get(1); }
		if (point.get(1) > yMax) { yMax = point.get(1); }
	}

	void setCenter() {
		center = point2D(((xMax + xMin) / 2), ((yMax + yMin) / 2), 1);
	}

public:
	point2DArray(int size) {
		this->maxSize = size;
		this->realSize = 0;
		pointArray = (point2D*)malloc(maxSize * sizeof(point2D));
	}
	point2DArray operator *(affinsMatrix2D inputMatrix) {
		point2DArray result = point2DArray(maxSize);
		float** matrix = inputMatrix.getMatrix();
		switch (inputMatrix.getType())
		{
		case 0:
			for (int i = 0;i < realSize;i++) {
				point2D localPoint;
				for (int j = 0;j < 3;j++) {
					float localResult = 0;
					for (int k = 0;k < 3;k++) {
						localResult += pointArray[i].get(k) * matrix[j][k];
					}
					localPoint.set(j, localResult);
				}
				result.add(localPoint);
			}
			for (int j = 0;j < 3;j++) {
				float localResult = 0;
				for (int k = 0;k < 3;k++) {
					localResult += center.get(k) * matrix[j][k];
				}
				center.set(j, localResult);
			}
			return result;
		case 1:
			for (int i = 0;i < realSize;i++) {
				point2D localPoint;
				for (int j = 0;j < 3;j++) {
					float localResult = 0;
					for (int k = 0;k < 3;k++) {
						localResult += (pointArray[i].get(k) - center.get(k)) * matrix[j][k];
					}
					localResult += center.get(j);
					localPoint.set(j, localResult);
				}
				result.add(localPoint);
			}
			result.setCenter(center);
			return result;
		case 2:

			for (int i = 0;i < realSize;i++) {
				point2D localPoint;
				for (int j = 0;j < 3;j++) {
					float localResult = 0;
					for (int k = 0;k < 3;k++) {
						localResult += (pointArray[i].get(k) - center.get(k)) * matrix[j][k];
					}
					localResult += center.get(j);
					localPoint.set(j, localResult);
				}
				result.add(localPoint);
			}
			result.setCenter(center);
			return result;
		default:
			break;
		}
	}
	void add(point2D point) { //переделать
		if (maxSize == realSize) {
			throw invalid_argument("array overload");
		}
		else {
			pointArray[realSize] = point;
			checkBorders(point);
			setCenter();
			realSize++;
		}
	}
	point2D getPoint(int i) {
		return pointArray[i];
	}
	int getSize() {
		return realSize;
	}
	void setCenter(point2D center) {
		this->center = center;
	}
	point2D getCenter() {
		return this->center;
	}
	int getWidth() {
		return xMax - xMin;
	}
};

class point3DArray{
private:
	point3D* pointArray;
public:
	point3DArray Multiplication() {

	}
};

class background {
private:
	HBITMAP bg;
	int error;
public:
	background() {
		bg = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/Background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		error = GetLastError(); 
	};
	~background() {
		DeleteObject(bg);
	}
	void printBackground(HDC hdc) {
		HDC memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, bg);
		BitBlt(hdc, 0, 0, WIDHT, HEIGHT, memdc, 0, 0, SRCCOPY);
	}
};

class sprite {
private:
	HBITMAP middleStage = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/middleStage.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP leftStage1 = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/leftStage1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP leftStage2 = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/leftStage2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP leftStage3 = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/leftStage3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP rightStage1 = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/rightStage1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP rightStage2 = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/rightStage2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP rightStage3 = (HBITMAP)LoadImageW(NULL, L"C:/Users/arseniy/source/repos/3Dgraphics/3Dgraphics/lab2Images/rightStage3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	int stage;
	bool direction;
	int x = 300;
	int y = 100;
	int width = 288;
	int height = 334;
	HDC hdc;
	HDC memdc;
	
	void delSprite() {
		BitBlt(hdc, x, y, width, height, memdc, 0, 0, SRCINVERT);
	}

	void printSprite() {
		BitBlt(hdc, x, y, width, height, memdc, 0, 0, SRCINVERT);
	}
	void nextStage() {
		if (stage > 0 && stage < 6) {
			if (direction) {
				stage++;
			}
			else {
				stage--;
			}
		}
		else {
			direction = !direction;
			if (direction) {
				stage++;
			}
			else {
				stage--;
			}
		}
	}

	void setMEMDC() {
		switch (stage)
		{
		case 0:
			SelectObject(memdc, leftStage3);
			break;
		case 1:
			SelectObject(memdc, leftStage2);
			break;
		case 2:
			SelectObject(memdc, leftStage1);
			break;
		case 3:
			SelectObject(memdc, middleStage);
			break;
		case 4:
			SelectObject(memdc, rightStage1);
			break;
		case 5:
			SelectObject(memdc, rightStage2);
			break;
		case 6:
			SelectObject(memdc, rightStage3);
			break;
		default:
			break;
		}
	}

public:
	sprite(HDC hdc, int startStage = 3, bool startDirection = false) {
		stage = startStage;
		direction = startDirection;
		this->hdc = hdc;
		memdc = CreateCompatibleDC(hdc);
		setMEMDC();
		printSprite();
	}

	void printNext() {
		delSprite();
		nextStage();
		setMEMDC();
		printSprite();
	}

};

VOID OnPaint(HDC hdc,point2D startPoint, point2D endPoint, Color color)
{
	Graphics graphics(hdc);
	Pen      pen(color);
	graphics.DrawLine(&pen, startPoint.get(0), startPoint.get(1), endPoint.get(0), endPoint.get(1));
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
		WIDHT,					  // initial x size
		HEIGHT,					  // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	background   bg;
	bg.printBackground(GetDC(hWnd));
	sprite sp = sprite(GetDC(hWnd));

	long startTime = time(nullptr);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (startTime != time(nullptr)) {
			startTime = time(nullptr);
			sp.printNext();
		}
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
} 

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc = GetDC(hWnd);
	PAINTSTRUCT  ps;
	switch (message)
	{
	case WM_PAINT:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}