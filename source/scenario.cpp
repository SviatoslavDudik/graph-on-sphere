#include "scenario.h"

Scenario::Scenario() {
	initScenario();
}

void Scenario::initScenario() {
	map.addPlace("bordeaux", 44.84, -0.58);
	map.addPlace("brest", 48.39, -4.49);
	map.addPlace("calais", 50.948056, 1.856389);
	map.addPlace("douvres", 45.9897, 5.3739);
	map.addPlace("edinburgh", 55.953, -3.189);
	map.addPlace("lehavre", 49.49, 0.1);
	map.addPlace("londres", 51.507222, -0.1275);
	map.addPlace("paris", 48.856613, 2.352222);
	map.addPlace("plymouth", 50.371389, -4.142222);
	map.addPlace("portsmouth", 50.805833, -1.087222);
	map.addPlace("quimper", 47.9967, -4.0964);
	map.addPlace("rennes", 48.1147, -1.6794);

	map.addConnection("edinburgh", "londres", TRAIN);
	map.addConnection("londres", "plymouth", TRAIN);
	map.addConnection("londres", "portsmouth", TRAIN);
	map.addConnection("londres", "douvres", TRAIN);
	map.addConnection("plymouth", "brest", BOAT);
	map.addConnection("portsmouth", "lehavre", BOAT);
	map.addConnection("douvres", "calais", BOAT);
	map.addConnection("brest", "rennes", TRAIN);
	map.addConnection("brest", "bordeaux", BOAT);
	map.addConnection("lehavre", "paris", BOAT);
	map.addConnection("calais", "paris", TRAIN);
	map.addConnection("rennes", "quimper", TRAIN);
	map.addConnection("rennes", "paris", TRAIN);
	map.addConnection("bordeaux", "quimper", TRAIN);
	map.addConnection("bordeaux", "paris", TRAIN);

}
