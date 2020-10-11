#pragma once

#include <vector>
#include <iostream>


namespace task {

const double EPS = 1e-6;


class OutOfBoundsException : public std::exception {};
class SizeMismatchException : public std::exception {};

class Vector {
public:
	Vector(double* ptr, std::size_t size);
	double& operator[](std::size_t idx);
	const double& operator[](std::size_t idx) const;
	double* vec_data;
private:
	std::size_t vec_size;
};

class Matrix {
public:
    Matrix();
    Matrix(std::size_t rows, std::size_t cols);
    Matrix(const Matrix& copy);
    Matrix& operator=(const Matrix& a);

    double& get(size_t row, size_t col);
    const double& get(size_t row, size_t col) const;
    void set(size_t row, size_t col, const double& value);
    void resize(size_t new_rows, size_t new_cols);

    Vector operator[](std::size_t row);
    const Vector operator[](std::size_t row) const;

    Matrix& operator+=(const Matrix& a);
    Matrix& operator-=(const Matrix& a);
    Matrix& operator*=(const Matrix& a);
    Matrix& operator*=(const double& number);

    Matrix operator+(const Matrix& a) const;
    Matrix operator-(const Matrix& a) const;
    Matrix operator*(const Matrix& a) const;
    Matrix operator*(const double& a) const;

    Matrix operator-() const;
    Matrix operator+() const;

    double det() const;
    void transpose();
    Matrix transposed() const;
    double trace() const;

    std::vector<double> getRow(size_t row);
    std::vector<double> getColumn(size_t column);

    bool operator==(const Matrix& a) const;
    bool operator!=(const Matrix& a) const;

	const std::size_t rows() const {
		return rows_;
	}

	const std::size_t cols() const {
		return cols_;
	}

private:
	double* data;
	std::size_t rows_;
	std::size_t cols_;
};


Matrix operator*(const double& a, const Matrix& b);

std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
std::istream& operator>>(std::istream& input, Matrix& matrix);



}  // namespace task
