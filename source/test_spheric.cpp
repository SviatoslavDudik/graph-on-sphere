#include "test_spheric.h"
#include "spheric.h"
#include <assert.h>

void testSpheric4D();

void testSpheric() {
	testSpheric4D();
}

bool equals(double val, double ref, double rel_error) {
	if (std::abs(val-ref) >= ref*rel_error)
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


