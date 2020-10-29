#ifndef EARTH_MAP_H
#define EARTH_MAP_H

#include "graph.h"
#include "spheric.h"
#include <string>

enum connectionType { TRAIN, BOAT };

class Place {
	std::string _name;
	Spheric<3> _location;
public:
	Place(std::string name, Spheric<3> location);
	inline const std::string& getName() const { return _name; }
	inline const Spheric<3>& getLocation() const { return _location; }
};

class EarthMap : private Graph<Place, connectionType> {
	std::map<const std::string, Node<Place>*> places;

public:
	void addPlace(const std::string &name, double latitude, double longitude);
	void deletePlace(const std::string &name);
	void addConnection(const std::string &name1, const std::string &name2, const connectionType &ct);
	void removeConnection(std::string name1, std::string name2, connectionType ct);
	long distance(const std::string &name1, const std::string &name2);
private:
	Node<Place>* getPlace(const std::string &name);
};


#endif
