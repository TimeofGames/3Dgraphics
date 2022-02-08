#include <list>

using namespace std;

class affinsMatrix2D {
private:
	float matrix[3][3];
public:
	void rotate(float angle) {

	}
	void move_on(int x, int y) {

	}
	void augmentation(float multu) {

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
	float x, y;
public:
	point2D(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

class point3D {
private:
	float x, y, z;
public:
	point3D(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class point2DArray{
private:
	list<point2D> pointArray;
public:
	point3DArray Multiplication() {

	}
};

class point3DArray{
private:
	list<point3D> pointArray;
public:
	point3DArray Multiplication() {

	}
};