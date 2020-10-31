#include "test_spheric.h"
#include "spheric.h"
#include <assert.h>

#include <iostream>

void testSpheric4D();
void testSpheric3D();

void testSpheric() {
	testSpheric4D();
	testSpheric3D();
}

bool equals(double val, double ref, double rel_error) {
	if (std::abs(val-ref) > std::abs(ref)*rel_error)
		return false;
	return true;
}

void testSpheric4D() {
	Spheric<4> p1;
	assert(p1.getRadius() == 0);
	for (int i = 0; i < 3; i++)
		assert(p1.getAngle(i) == 0);
	p1.setRadius(1);
	assert(p1.getRadius() == 1);
	p1.setAngle(0, M_PI/2);
	assert(p1.getAngle(0) == M_PI/2);

	double angles[3] = {M_PI/2};
	Spheric<4> p2(1, angles);
	assert(p2.getRadius() == 1);
	for (int i = 0; i < 3; i++)
		assert(p2.getAngle(i) == angles[i]);

	p1.rotate(0, M_PI);
	assert(p1.getRadius() == 1);
	assert(p1.getAngle(0) == M_PI/2);
	assert(p1.getAngle(1) == 0);
	assert(p1.getAngle(2) == 0);
	for (int i = 0; i < 3; i++)
		angles[0] = 3*M_PI/2;
	p2.rotate(angles);
	assert(p2.getRadius() == 1);
	for (int i = 0; i < 3; i++)
		assert(p2.getAngle(i) == 0);
	p1.setRadius(-1);
	assert(p1.norm() == 1);
	assert(p1 == p1);
	Spheric<4> p3;
	p3.setRadius(1);
	assert(p2 == p3);
	assert(p1 != p2);
	p1 = 2 * p1;
	assert(p1.getRadius() == -2);
	assert(p1.getAngle(0) == M_PI/2);
	assert(p1.getAngle(1) == 0);
	assert(p1.getAngle(2) == 0);
}

void testSpheric3D() {
	Spheric<3> p1;
	p1.setRadius(1);
	p1.setAngle(1, M_PI/4);
	p1.rotate(0, M_PI);
	assert(p1.getRadius() == -1);
	assert(p1.getAngle(0) == 0);
	assert(p1.getAngle(1) == 3*M_PI/4);

	Spheric<3> p2(1, M_PI/6, -M_PI);
	assert(p2.getRadius() == -1);
	assert(equals(p2.getAngle(0), 2*M_PI/3, 0.00001));
	assert(p2.getAngle(1) == 0);

	assert(equals(p2.getLatitude(), -M_PI/6, 0.00001));
	p2.rotate(1, M_PI/2);
	assert(p2.getLongitude() == M_PI/2);

	p1.setRadius(-1);
	p1.setAngle(0, M_PI/4);
	p1.setAngle(1, 3*M_PI/4);
	Cartesian c = convertCartesian(p1);
	assert(equals(c.x, 0.5, 0.00001));
	assert(equals(c.y, -0.5, 0.00001));
	assert(equals(c.z, -sqrt(2)/2, 0.00001));

	p1.setRadius(1);
	p1.setAngle(0, M_PI/4);
	p1.setAngle(1, M_PI/4);
	p2.setRadius(-1);
	p2.setAngle(0, M_PI/4);
	p2.setAngle(1, M_PI/4);
	assert(equals(distance(p1,p2), M_PI, 0.00001));

	p2.setAngle(0, 3*M_PI/4);
	assert(equals(distance(p1,p2), M_PI/2, 0.00001));

	p1 = coordsEarth(48.856613, 2.352222); // paris
	p2 = coordsEarth(51.507222, -0.1275); // london
	assert(equals(distance(p1,p2), 343680, 0.001));
	p2 = coordsEarth(51.45, -2.583333); //bristol
	assert(equals(distance(p1,p2), 454440, 0.001));
	p2 = coordsEarth(-18.933333, 47.516667); //antananarivo
	assert(equals(distance(p1,p2), 8757070, 0.001));
}
