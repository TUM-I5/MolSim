/*
 * Vector
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#ifndef VECTOR_
#define VECTOR_

#include <cmath>
#include <sstream>
#include <iostream>

namespace utils {
template <typename type, int length>
class Vector;
}


/** Global operators first */

template <typename type, int length>
std::ostream& operator<<(std::ostream& stream, const utils::Vector<type, length>& v) {

	stream << "[";
	for (int i = 0; i < length; i++) {
		stream << v.content[i] << ";";
	}
	stream << "]";
	return stream;
}


template <typename type, int length>
utils::Vector<type, length> operator*(double scalar, const utils::Vector<type, length>& v) {
	return v * scalar;
}

/** Vector class definition */

template <typename type, int length>
class utils::Vector {

	friend std::ostream& operator<< <type,length>(std::ostream& stream, const Vector& v);
	friend Vector operator* <type,length>(double scalar, const Vector& v);

private:

	type content[length];

public:
	Vector() {
	}

	Vector(type arg) {
		for (int i = 0; i < length; i++) {
			content[i] = arg;
		}
	}

	Vector(type args[length]) {
		for (int i = 0; i < length; i++) {
			content[i] = args[i];
		}
	}

	Vector(const Vector& other) {
		for (int i = 0; i < length; i++) {
			content[i] = other[i];
		}
	}

	Vector operator+(const Vector& rhs) const {
		type result[length];

		for (int i = 0; i < length; i++) {
			result[i] = this->content[i] + rhs.content[i];
		}
		return Vector(result);
	}

	Vector operator-(const Vector& rhs) const {
		type result[length];

		for (int i = 0; i < length; i++) {
			result[i] = this->content[i] - rhs.content[i];
		}
		return Vector(result);
	}

	Vector operator*(double scalar) const{
		type result[length];

		for (int i = 0; i < length; i++) {
			result[i] = this->content[i] * scalar;
		}
		return Vector(result);
	}

	double L2Norm() const {
		double square_sum = 0;
		for (int i = 0; i < length; i++) {
			square_sum += (this->content[i] * this->content[i]);
		}
		return sqrt(square_sum);
	}

	bool equals(const Vector& rhs) const {
		for (int i = 0; i < length; i++) {
			if (rhs.content[i] != this->content[i]) {
				return false;
			}
		}
		return true;
	}

	Vector& operator=(const Vector& rhs) {
		if(this != &rhs) {
			for (int i = 0; i < length; i++) {
				content[i] = rhs.content[i];
			}
		}
		return *this;
	}

	Vector& operator=(double rhs) {
		for (int i = 0; i < length; i++) {
			content[i] = rhs;
		}
		return *this;
	}

	type& operator[](int i) {
		return content[i];
	}

	const type& operator[](int i) const {
			return content[i];
	}

	bool operator==(const Vector& rhs) const {
		for (int i = 0; i < length; i++) {
			if(content[i] != rhs.content[i]) {
				return false;
			}
		}
		return true;
	}

	std::string toString() const {
		std::stringstream str;
		str << *this;
		return str.str();
	}
};


#endif /* VECTOR_ */
