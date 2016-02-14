#ifndef _MATRIX
#define _MATRIX

class Matrix {
public:
	Matrix();
	Matrix(int rows, int cols, float data[]);
	Matrix(int rows, int cols);
	Matrix(Matrix &other);
	~Matrix();
	void operator*=(Matrix other);
	void operator+=(Matrix other);
	void toConsole();
	float get(int row, int column);
	void set(int row, int column, float value);
	float* copy();

	static Matrix* unitMatrix( int size );
	
	float *matrix;

private:
	int rowCount;
	int columnCount;

	void initialize( int rows, int cols, float data[] );
};

#endif