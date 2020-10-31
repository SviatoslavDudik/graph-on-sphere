#include "earth_map.h"

#include <climits>

Place::Place(std::string name, Spheric<3> location) :
	_name(name), _location(location) {}

bool Place::operator==(const Place& p) {
	return _name == p._name;

}

const Node<Place>* EarthMap::getPlace(const std::string &name) {
	try {
		return places.at(name);
	}
	catch (std::out_of_range e) {
		return nullptr;
	}
}

void EarthMap::addPlace(const std::string &name, double latitude, double longitude) {
	auto it = getPlace(name);
	if (it == nullptr) {
		Spheric<3> location = coordsEarth(latitude, longitude);
		Place p = Place(name, location);
		const Node<Place> *n = addNode(p);
		places[name] = n;
	}
}
void EarthMap::deletePlace(const std::string &name) {
	auto it = getPlace(name);
	if (it != nullptr) {
		deleteNode(it);
		places.erase(name);
	}
}

void EarthMap::addConnection(const std::string &name1, const std::string &name2, const connectionType &ct) {
	auto it1 = getPlace(name1);
	auto it2 = getPlace(name2);
	if (it1 != nullptr || it2 != nullptr) {
		addEdge(ct, it1, it2);
		addEdge(ct, it2, it1);
	}
}
void EarthMap::removeConnection(std::string name1, std::string name2, connectionType ct) {
	auto it1 = getPlace(name1);
	auto it2 = getPlace(name2);
	if (it1 != nullptr || it2 != nullptr) {
		deleteEdge(ct, it1, it2);
		deleteEdge(ct, it2, it1);
	}

}
struct _distance {
	std::map<const Node<Place>*,long> distances;
	const Node<Place> *goal;
	const Node<Place>* operator()(const Node<Place> *from, const Node<Place> *to, const Edge<connectionType> *edge) {
		edge->getAnnotation();
		long val = LONG_MAX;
		try {
			long val = distances.at(from) + distance(from->getData().getLocation(), to->getData().getLocation());
		}
		catch (std::out_of_range e) {
			// part of the route contains an edge with different connectionType
		}
		// update distance to 'to'
		try {
			long& d = distances.at(to);
			d = std::min(d, val);
		}
		catch (std::out_of_range e) {
			// first visit
			distances[to] = val;
		}
		// if hit the goal then end
		if (from == goal)
			return goal;
		// continue
		return nullptr;
	}
};

long EarthMap::distance(const std::string &name1, const std::string &name2) {
	struct _distance d;
	const Node<Place> *n1 = getPlace(name1);
	const Node<Place> *n2 = getPlace(name2);
	d.distances[n1] = 0;
	d.goal = n2;
	breadthFirst(n1, d);
	try {
		return d.distances.at(n2);
	}
	catch (std::out_of_range e) {
		return -1; // error
	}
}

