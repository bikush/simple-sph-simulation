
#include <iostream>
#include "Matrix.h"

using namespace std;

// TODO: REMOVE THIS CRAPPY CLASS
void Matrix::initialize( int rows, int cols, float data[]){
	rowCount = rows;
	columnCount = cols;

	matrix = new float[ rowCount * columnCount ];

	for(int row = rowCount-1; row>=0; row--){
		int rowOffset = row * columnCount;
		for(int column = columnCount-1; column>=0; column--){
			matrix[rowOffset + column] = data[rowOffset + column];
		}
	}
}


Matrix::Matrix(){
	rowCount = 0;
	columnCount = 0;
	matrix = 0;
}

Matrix::Matrix(int rows, int cols, float data[]) {
	initialize( rows, cols, data );
}

Matrix::Matrix(int rows, int cols)
{
	rowCount = rows;
	columnCount = cols;

	matrix = new float[ rowCount * columnCount ];
	for( int i = 0; i<rowCount*columnCount; i++ ) matrix[i] = 0.0f;
}

Matrix::Matrix(Matrix &other) {
	initialize( other.rowCount, other.columnCount, other.matrix );	
}

Matrix::~Matrix() {
	delete [] matrix;
}

void Matrix::operator*=(Matrix other){

	if(columnCount != other.rowCount){
		cerr << "Matrix: pomnozi: U matrixma se ne podudara broj "
				"stupaca prve i broj redaka druge matrice." << endl;
		return;
	}

	float *matRezultat = new float[ rowCount * other.columnCount ];
	int matRezRedova = rowCount;
	int matRezStupaca = other.columnCount;

	// Svaki redak prve matrice, množi se sa svakim stupcem druge matrice
	for(int row = matRezRedova-1; row>=0; row--){
		int rowOffset = row * columnCount;
		for(int column = matRezStupaca-1; column>=0; column--){
			int columnOffset = (other.rowCount-1) * other.columnCount + column;
			float rezultat = 0.0;
			for(int elemStupca = columnCount-1; elemStupca>=0; elemStupca--){
				rezultat += matrix[rowOffset + elemStupca] *
						other.matrix[columnOffset - other.columnCount * (columnCount-1-elemStupca)];
			}
			matRezultat[row * matRezStupaca + column] = rezultat;
		}
	}

	delete [] matrix;
	matrix = matRezultat;
	rowCount = matRezRedova;
	columnCount = matRezStupaca;
}

/*
 * Metoda koja æe ovu matricu zbrojiti sa ulaznom (drugom) matricom, a
 * rezultat æe prepisati ovu matricu.
 */
void Matrix::operator+=(Matrix other){

	if(columnCount != other.columnCount){
		cerr << "Matrix: zbroji: U matricama se ne podudara broj stupaca." << endl;
		return;
	}
	if(rowCount != other.rowCount){
		// Damn, Croatian
		cerr << "Matrix: zbroji: U matricama se ne podudara broj redova." << endl;
		return;
	}

	for(int row = 0; row<rowCount; row++){
		int rowOffset = row * columnCount;
		for(int column = 0; column<columnCount; column++){
			matrix[rowOffset + column]+=other.matrix[rowOffset + column];
		}
	}

}

/*
 * Metoda za jednostavan toConsole na standardni izlaz.
 */
void Matrix::toConsole(){

	cout << "Redova: " << rowCount << "\nStupaca: " << columnCount << endl;
	for(int row = 0; row<rowCount; row++){
		int rowOffset = row * columnCount;
		for(int column = 0; column<columnCount; column++){
			cout << "\t" << matrix[rowOffset + column];
		}
		cout<<endl;
	}

}

/*
 * Glupa metoda za dohvaæanje nekog elementa matrice.
 * Ovo je æisto zbog praktiènosti, ništa više.
 */
float Matrix::get(int row, int column){
	return matrix[(row-1)*columnCount+column-1];
}

void Matrix::set(int row, int column, float value)
{
	matrix[(row-1)*columnCount+column-1]=value;
}

float* Matrix::copy()
{
	int size = rowCount * columnCount;
	float* out = new float[ size ];
	memcpy( out, matrix, size * sizeof(float) );
	return out;
}

Matrix* Matrix::unitMatrix( int size )
{
	Matrix* out = new Matrix(size, size);
	for( int i=0; i<size; i++ ) out->set(i,i,1.0f);
	return out;
}
