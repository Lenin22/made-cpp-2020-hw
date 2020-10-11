#include "matrix.h"
#include <cmath>
#include <cstring>

using namespace task;

task::Matrix::Matrix() : rows_(1), cols_(1), data(new double[1]) {
	data[0] = 1;
}

task::Matrix::Matrix(std::size_t rows, std::size_t cols) : rows_(rows),
														   cols_(cols),
														   data(new double[rows*cols]) {
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			if(i == j)
				data[i*cols_ + j] = 1;
			else
				data[i*cols_ + j] = 0;
		}
	}
}

task::Matrix::Matrix(const Matrix & copy) {
	cols_ = copy.cols_;
	rows_ = copy.rows_;
	data = new double[cols_*rows_];
	std::memcpy(data, copy.data, sizeof(double)*cols_*rows_);
}

Matrix & task::Matrix::operator=(const Matrix & a) {
	double* buffer = new double[a.rows()*a.cols()];
	std::memcpy(buffer, a.data, a.rows() * a.cols() * sizeof(double));
	delete[] data;
	data = new double[a.rows()*a.cols()];
	data = buffer;
	rows_ = a.rows();
	cols_ = a.cols();
	return *this;
}

double & task::Matrix::get(size_t row, size_t col) {
	if (row < rows_ && col < cols_) {
		return data[row*cols_+col];
	} else {
		throw OutOfBoundsException();
	}
}

const double & task::Matrix::get(size_t row, size_t col) const {
	if (row < rows_ && col < cols_) {
		return data[row*cols_ + col];
	} else {
		throw OutOfBoundsException();
	}
}

void task::Matrix::set(size_t row, size_t col, const double & value) {
	if (row < rows_ && col < cols_) {
		data[row*cols_ + col] = value;
	} else {
		throw OutOfBoundsException();
	}
}

void task::Matrix::resize(size_t new_rows, size_t new_cols) {
	double* buffer = new double[cols_*rows_];
	memcpy(buffer, data, cols_*rows_ * sizeof(double));
	delete [] data;
	data = new double[new_rows*new_cols];
	for (std::size_t i = 0; i < new_rows; i++) {
		for (std::size_t j = 0; j < new_cols; j++) {
			if (i < rows_ && j < cols_) {
				data[i*new_cols + j] = buffer[i*cols_ + j];
			} else {
				data[i*new_cols + j] = 0;
			}
		}
	}
	cols_ = new_cols;
	rows_ = new_rows;
	delete [] buffer;
}

Vector task::Matrix::operator[](std::size_t row) {
	return Vector(data+cols_*row, cols_);
}

const Vector task::Matrix::operator[](std::size_t row) const {
	return Vector(data+row*cols_, cols_);
}

Matrix & task::Matrix::operator+=(const Matrix & a) {
	if (cols_ != a.cols_ || rows_ != a.rows_)
		throw SizeMismatchException();
	Matrix result = *this + a;
	*this = result;
	return *this;
}

Matrix & task::Matrix::operator-=(const Matrix & a) {
	if (cols_ != a.cols_ || rows_ != a.rows_)
		throw SizeMismatchException();
	Matrix result = *this + (-a);
	*this = result;
	return *this;
}

Matrix & task::Matrix::operator*=(const Matrix & a) {
	if (cols_ != a.rows_)
		throw SizeMismatchException();
	Matrix result = *this * a;
	*this = result;
	return *this;
}

Matrix & task::Matrix::operator*=(const double & number) {
	Matrix result = *this * number;
	*this = result;
	return *this;
}

Matrix task::Matrix::operator+(const Matrix & a) const {
	if (cols_ != a.cols_ || rows_ != a.rows_)
		throw SizeMismatchException();

	Matrix result(rows_, cols_);
	for (int i = 0; i < result.rows_; i++) {
		for (int j = 0; j < result.cols_; j++) {
			result[i][j] = data[i*cols_ + j] + a[i][j];
		}
	}
	return result;
}

Matrix task::Matrix::operator-(const Matrix & a) const {
	return *this + (-a);
}

Matrix task::Matrix::operator*(const Matrix & a) const {
	if (cols_ != a.rows_)
		throw SizeMismatchException();

	Matrix result(rows_, a.cols_);
	for (int i = 0; i < result.rows_; i++) {
		for (int j = 0; j < result.cols_; j++) {
			result[i][j] = 0;
			for (int k = 0; k < cols_; k++) {
				result[i][j] += data[i*cols_ + k] * a[k][j];
			}
		}
	}
	return result;
}

Matrix task::Matrix::operator*(const double & a) const {
	Matrix result(rows_, cols_);
	for (int i = 0; i < result.rows_; i++) {
		for (int j = 0; j < result.cols_; j++) {
			result[i][j] = data[i*cols_ + j] * a;
		}
	}
	return result;
}

Matrix task::Matrix::operator-() const {
	Matrix result(rows_, cols_);
	for (int i = 0; i < result.rows_; i++) {
		for (int j = 0; j < result.cols_; j++) {
			result[i][j] = -data[i*cols_ + j];
		}
	}
	return result;
}

Matrix task::Matrix::operator+() const {
	return Matrix(*this);
}

double determinant(Matrix matrix, int n) {
	double det = 0;
	Matrix submatrix(matrix.rows(), matrix.cols());
	if (n == 1) {
		return matrix[0][0];
	} else if (n == 2) {
		return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
	} else {
		for (int x = 0; x < n; x++) {
			int subi = 0;
			for (int i = 1; i < n; i++) {
				int subj = 0;
				for (int j = 0; j < n; j++) {
					if (j == x)
						continue;
					submatrix[subi][subj] = matrix[i][j];
					subj++;
				}
				subi++;
			}
			det = det + (pow(-1, x) * matrix[0][x] * determinant(submatrix, n - 1));
		}
	}
	return det;
}

double task::Matrix::det() const {
	if (cols_ != rows_)
		throw SizeMismatchException();
	return determinant(*this, cols_);
}

void task::Matrix::transpose() {
	Matrix tr = transposed();
	*this = tr;
}

Matrix task::Matrix::transposed() const {
	Matrix result(cols_, rows_);
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			result[j][i] = data[i*cols_ + j];
		}
	}
	return result;
}

double task::Matrix::trace() const {
	if (cols_ != rows_)
		throw SizeMismatchException();
	double result = 0;
	for (std::size_t i = 0; i < cols_; i++) {
		result += data[i*cols_ + i];
	}
	return result;
}

std::vector<double> task::Matrix::getRow(size_t row) {
	std::vector<double> result(&data[row*cols_], &data[(row + 1)*cols_]);
	return result;
}

std::vector<double> task::Matrix::getColumn(size_t column) {
	std::vector<double> result(rows_);
	for (std::size_t i = 0; i < rows_; i++) {
		result[i] = data[i * cols_ + column];
	}
	return result;
}

bool task::Matrix::operator==(const Matrix & a) const {
	if(cols_ != a.cols_ || rows_ != a.rows_)
		return false;
	for (std::size_t i = 0; i < cols_*rows_; i++) {
		if (fabs(data[i]-a.data[i]) > EPS) {
			return false;
		}
	}
	return true;
}

bool task::Matrix::operator!=(const Matrix & a) const {
	return !(*this == a);
}

task::Vector::Vector(double* ptr, std::size_t size) :
vec_data(ptr), vec_size(size) {}

double& task::Vector::operator[](std::size_t idx) {
	if (idx < vec_size) {
		return vec_data[idx];
	} else {
		throw std::out_of_range("");
	}
}

const double & task::Vector::operator[](std::size_t idx) const {
	if (idx < vec_size) {
		return vec_data[idx];
	} else {
		throw std::out_of_range("");
	}
}

Matrix task::operator*(const double & a, const Matrix & b) {
	Matrix result(b.rows(), b.cols());
	for (int i = 0; i < result.rows(); i++) {
		for (int j = 0; j < result.cols(); j++) {
			result[i][j] = b[i][j] * a;
		}
	}
	return result;
}

std::ostream & task::operator<<(std::ostream & output, const Matrix & matrix) {
	for (std::size_t i = 0; i < matrix.rows(); i++) {
		for (std::size_t j = 0; j < matrix.cols(); j++) {
			output << matrix[i][j] << " ";
		}
		output << "\n";
	}
	return output;
}

std::istream & task::operator>>(std::istream & input, Matrix & matrix) {
	size_t cols, rows;
	input >> rows >> cols;
	matrix.resize(rows, cols);
	for (std::size_t i = 0; i < rows; i++) {
		for (std::size_t j = 0; j < cols; j++) {
			input >> matrix[i][j];
		}
	}
	return input;
}
