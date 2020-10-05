#pragma once
#include <vector>
#include <iostream>

namespace task {

	double ERR_EPS = 1e-7;

	template<class T>
	std::vector<T> operator+(const std::vector<T>& a) {
		std::vector<T> result(a);
		return result;
	}

	template<class T>
	std::vector<T> operator-(const std::vector<T>& a) {
		std::vector<T> result(a.size());
		for (size_t i = 0; i < result.size(); i++) {
			result[i] = -a[i];
		}
		return result;
	}

	template<class T>
	std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
		std::vector<T> result(a.size());
		for (unsigned int i = 0; i < a.size(); i++) {
			result[i] = a[i] + b[i];
		}
		return result;
	}

	template<class T>
	std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
		return a + (-b);
	}

	template<class T>
	T operator*(const std::vector<T>& a, const std::vector<T>& b) {
		T result = 0;
		for (unsigned int i = 0; i < a.size(); i++) {
			result += a[i] * b[i];
		}
		return result;
	}

	template<class T>
	std::vector<T> operator%(const std::vector<T>& a, const std::vector<T>& b) {
		std::vector<T> result(3);
		result[0] = a[1] * b[2] - a[2] * b[1];
		result[1] = a[2] * b[0] - a[0] * b[2];
		result[2] = a[0] * b[1] - a[1] * b[0];
		return result;
	}

	template<class T>
	double length2(const std::vector<T>& a) {
		double result = 0;
		for (const T& elem : a) {
			result += elem * elem;
		}
		return result;
	}

	template<class T>
	bool operator||(const std::vector<T>& a, const std::vector<T>& b) {
		double len2_a = length2(a);
		double len2_b = length2(b);
		if (len2_a != 0 && len2_b != 0) {
			double dot = a * b;
			double cos2fi = dot*dot / (len2_a * len2_b);
			return 1-cos2fi < ERR_EPS;
		}
		return false;
	}

	template<class T>
	bool operator&&(const std::vector<T>& a, const std::vector<T>& b) {
		double dot = a * b;		
		double cos2fi = dot*dot / (length2(a) * length2(b));
		return (1 - cos2fi < ERR_EPS) && (dot > 0);
	}

	template<class T>
	std::istream& operator>> (std::istream& is, std::vector<T>& data) {
		size_t size;
		is >> size;
		data.resize(size);
		T value;
		for (size_t i = 0; i < size; i++) {
			is >> value;
			data[i] = value;
		}
		return is;
	}

	template<class T>
	std::ostream& operator<< (std::ostream& os, std::vector<T> data) {
		bool start = true;
		for (T& elem : data) {
			if (!start) {
				os << ' ';
			}
			os << elem;
			start = false;
		}
		os << '\n';
		return os;
	}

	template<class T>
	void reverse(std::vector<T>& data) {
		for (size_t i = 0; i < data.size() / 2; i++) {
			T buf = data[i];
			data[i] = data[data.size() - 1 - i];
			data[data.size() - 1 - i] = buf;
		}
	}

	std::vector<int> operator|(const std::vector<int>& a,
							   const std::vector<int>& b) {
		std::vector<int> result(a.size());
		for (size_t i = 0; i < result.size(); i++) {
			result[i] = a[i] | b[i];
		}
		return result;
	}

	std::vector<int> operator&(const std::vector<int>& a,
							   const std::vector<int>& b) {
		std::vector<int> result(a.size());
		for (size_t i = 0; i < result.size(); i++) {
			result[i] = a[i] & b[i];
		}
		return result;
	}
}  // namespace task
