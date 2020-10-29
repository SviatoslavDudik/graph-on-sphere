#ifndef SPHERIC_H
#define SPHERIC_H

#include <algorithm>
#include <cmath>
#include <exception>
#include <stdexcept>

template <int N>
class Spheric_Base {
	int _r;
	double _angles[N-1];
public:
	Spheric_Base();
	Spheric_Base(int r, double angles[N-1]);
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
	Spheric(int r, double angles[N-1]) : Spheric_Base<N>(r, angles) {}
};

template <>
class Spheric<3> : public Spheric_Base<3> {
public:
	Spheric() : Spheric_Base<3>() {}
	Spheric(int r, double angles[2]) : Spheric_Base<3>(r, angles) {}
	Spheric(int r, double latitude, double longitude);
	inline double getLatitude() const { return M_PI/2 - getAngle(0); }
	inline double getLongitude() const { return getAngle(1); }
	void setLatitude(double latitude);
	void setLongitude(double longitude);
};

template <int N>
Spheric_Base<N>::Spheric_Base() : _r(0) {
	for (int i = 0; i < N-1; i++)
		_angles[i] = 0;
}

template <int N>
Spheric_Base<N>::Spheric_Base(int r, double angles[N-1]) : _r(r) {
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
	for (int j = 0; j < N-1; j++) {
		if (j == i)
			continue;
		_angles[j] = M_PI - _angles[j];
	}
	if (N % 2 == 1)
		_r *= -1;

}

Spheric<3>::Spheric(int r, double latitude, double longitude) : Spheric_Base<3>() {
	setRadius(r);
	setLatitude(latitude);
	setLongitude(longitude);
}

void Spheric<3>::setLatitude(double latitude) {
	if (latitude > M_PI/2 || latitude < -M_PI/2)
		throw std::invalid_argument("Spheric::setLatitude: latitude is out of bounds");
	setAngle(0, M_PI/2 - latitude);
}

void Spheric<3>::setLongitude(double longitude) {
	if (longitude >= 2*M_PI || longitude <= -M_PI)
		throw std::invalid_argument("Spheric::setLongitude: longitude is out of bounds");
	if (longitude < 0) {
		longitude += M_PI;
		setRadius(-getRadius());
		setAngle(0,M_PI-getAngle(0));
	}
	else if (longitude >= M_PI) {
		longitude -= M_PI;
		setRadius(-getRadius());
		setAngle(0,M_PI-getAngle(0));
	}
	setAngle(1, longitude);
}

class Cartesian {
public:
	double x,y,z;
};

Cartesian convertCartesian(const Spheric<3> &p) {
	Cartesian cart;
	cart.x = p.getRadius() * sin(p.getAngle(0)) * cos(p.getAngle(1));
	cart.y = p.getRadius() * sin(p.getAngle(0)) * sin(p.getAngle(1));
	cart.z = p.getRadius() * cos(p.getAngle(0));
	return cart;
}

double distance(const Spheric<3> &p1, const Spheric<3> &p2) {
	double res;
	const double R = std::abs(p1.getRadius());
	if (R != std::abs(p2.getRadius())) {
		res = -1.0; // error!
	}
	else {
		const double latitude_1 = p1.getLatitude() + ((p1.getRadius() < 0) ? M_PI:0.0);
		const double latitude_2 = p2.getLatitude() + ((p2.getRadius() < 0) ? M_PI:0.0);
		const double dlongitude = (p1.getLongitude() - p2.getLongitude());
		const double dlatitude = (latitude_1 - latitude_2);
		const double sdlatitude = sin(dlatitude / 2.0);
		const double sdlongitude = sin(dlongitude / 2.0);
		const double a = (sdlatitude*sdlatitude) +
			(cos(latitude_1)*cos(latitude_2)*sdlongitude*sdlongitude);
		res = R*2*atan2(sqrt(a), sqrt(1 - a));
	}
	return res;
}

Spheric<3> coordsEarth(double latitude, double longitude) {
	return Spheric<3>(6371008, M_PI*latitude/180, M_PI*longitude/180);
}

#endif
