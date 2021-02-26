#ifndef SPHERIC_H
#define SPHERIC_H

#include <algorithm>
#include <cmath>
#include <stdexcept>

template <int N>
class Spheric_Base {
	int _r;
	double _angles[N-1];
public:
	Spheric_Base();
	Spheric_Base(int r, const double angles[N-1]);
	inline double getRadius() const { return _r; }
	inline double getAngle(int i) const { return _angles[i]; }
	void setRadius(int r) { _r = r; }
	void setAngle(int i, double angle);

	void rotate(int i, double angle);
	void rotate(double angles[N-1]);
	double norm() const;
	bool operator==(const Spheric_Base<N> &p) const;
	bool operator!=(const Spheric_Base<N> &p) const;
	Spheric_Base<N> operator*(double d) const;
	template <int M>
	friend Spheric_Base<M> operator*(double d, const Spheric_Base<M> &p);
private:
	void invertAllExcept(int i);
};

template <int N>
class Spheric : public Spheric_Base<N> {
public:
	Spheric() : Spheric_Base<N>() {}
	Spheric(int r, const double angles[N-1]) : Spheric_Base<N>(r, angles) {}
	const Spheric<N>& operator=(const Spheric_Base<N> &p);
};

template <>
class Spheric<3> : public Spheric_Base<3> {
public:
	Spheric() : Spheric_Base<3>() {}
	Spheric(int r, const double angles[2]) : Spheric_Base<3>(r, angles) {}
	Spheric(int r, double latitude, double longitude);
	inline double getLatitude() const { return M_PI/2 - getAngle(0); }
	inline double getLongitude() const { return getAngle(1); }
	void setLatitude(double latitude);
	void setLongitude(double longitude);
	const Spheric<3>& operator=(const Spheric_Base<3> &p);
};

template <int N>
Spheric_Base<N>::Spheric_Base() : _r(0) {
	for (int i = 0; i < N-1; i++)
		_angles[i] = 0;
}

template <int N>
Spheric_Base<N>::Spheric_Base(int r, const double angles[N-1]) : _r(r) {
	for (int i = 0; i < N-1; i++)
		if (angles[i] < 0 || angles[i] >= M_PI)
			throw std::invalid_argument("Spheric::Spheric: angle out of bounds");
	std::copy(angles, angles + N-1, _angles);
}

template <int N>
void Spheric_Base<N>::setAngle(int i, double angle) {
	if (i < 0 || i >= N-1)
		throw std::out_of_range("Spheric::setAngle: i is out of range");
	if (angle < 0 || angle >= M_PI)
		throw std::invalid_argument("Spheric::setAngle: angle out of bounds");
	_angles[i] = angle;
}

template <int N>
void Spheric_Base<N>::rotate(int i, double angle) {
	if (i < 0 || i >= N-1)
		throw std::out_of_range("Spheric::rotate: i is out of range");
	bool invert = false;
	_angles[i] += std::fmod(angle, 2*M_PI);
	if (_angles[i] >= 2*M_PI) {
		_angles[i] -= 2*M_PI;
	}
	else if (_angles[i] <= -2*M_PI) {
		_angles[i] += 2*M_PI;
	}

	if (_angles[i] >= M_PI) {
		_angles[i] -= M_PI;
		invert = true;
	}
	else if (_angles[i] < 0) {
		_angles[i] += M_PI;
		invert = true;
	}

	if (invert)
		invertAllExcept(i);
}

template <int N>
void Spheric_Base<N>::rotate(double angles[N-1]) {
	for (int i = 0; i < N-1; i++) {
		rotate(i, angles[i]);
	}
}

template <int N>
double Spheric_Base<N>::norm() const {
	return std::abs(_r);
}

template <int N>
const Spheric<N>& Spheric<N>::operator=(const Spheric_Base<N> &p) {
	this->setRadius(p.getRadius());
	for (int i = 0; i < N-1; i++)
		this->setAngle(i, p.getAngle(i));
	return *this;
}

template <int N>
bool Spheric_Base<N>::operator==(const Spheric_Base<N> &p) const {
	bool same = true;
	for (int i = 0; i < N-1; i++)
		same &= _angles[i] == p._angles[i];
	return same && _r == p._r;
}

template <int N>
bool Spheric_Base<N>::operator!=(const Spheric_Base<N> &p) const {
	return !(*this == p);
}

template <int N>
Spheric_Base<N> Spheric_Base<N>::operator*(double d) const {
	return Spheric<N>(_r*d, _angles);

}

template <int M>
Spheric_Base<M> operator*(double d, const Spheric_Base<M> &p) {
	return p*d;
}

template <int N>
void Spheric_Base<N>::invertAllExcept(int i) {
	bool invert = N % 2 == 1;
	for (int j = 0; j < N-1; j++) {
		if (j == i) // skip _angles[i]
			continue;
		// invert angles relative to pi/2
		_angles[j] = M_PI - _angles[j];
		if (_angles[j] == M_PI) {
			_angles[j] = 0;
			invert = !invert;
		}
	}
	if (invert)
		_r *= -1;
}

class Cartesian {
public:
	double x,y,z;
};

Cartesian convertCartesian(const Spheric<3> &p);
double distanceGrandCercle(const Spheric<3> &p1, const Spheric<3> &p2);
Spheric<3> coordsEarth(double latitude, double longitude);

#endif
