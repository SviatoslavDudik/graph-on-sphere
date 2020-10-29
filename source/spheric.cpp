#include "spheric.h"


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

