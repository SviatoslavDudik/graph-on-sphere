#ifndef SCENARIO_H
#define SCENARIO_H

#include "earth_map.h"

class Scenario {
	EarthMap map;
public:
	Scenario();
	EarthMap& getMap();
private:
	void initScenario();
};

#endif
