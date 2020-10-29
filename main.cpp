#include "source/earth_map.h"
#include <iostream>

int main() {
	/* Spheric<3> a = coordsEarth(51.507222, -0.1275); //london */
	/* Spheric<3> b = coordsEarth(51.45, -2.583333); //bristol */
	Spheric<3> a = coordsEarth(38.725267, -9.150019); //lisbon
	Spheric<3> b = coordsEarth(-18.933333, 47.516667); //antananarivo
	std::cout << distance(a, b) << std::endl;
	return 0;
}

/* struct _distance { */
/* 	std::map<const Node<int>*,int> distance; */
/* 	Node<int> *goal; */
/* 	Node<int>* operator()(const Node<int> *from, const Node<int> *to, const Edge<int> *edge) { */
/* 		int val = distance[from] + edge->getAnnotation(); */
/* 		try { */
/* 			int& d = distance.at(to); */
/* 			d = std::min(d, val); */
/* 		} */
/* 		catch (std::out_of_range e) { */
/* 			distance[to] = val; */
/* 		} */
/* 		if (from == goal) */
/* 			return goal; */
/* 		return nullptr; */
/* 	} */
/* }; */

/* int main() { */
/* 	Graph<int,int> graph; */
/* 	Node<int>*n1 = graph.addNode(1); */
/* 	Node<int>*n2 = graph.addNode(2); */
/* 	Node<int>*n3 = graph.addNode(3); */
/* 	Node<int>*n4 = graph.addNode(4); */
/* 	Node<int>*n5 = graph.addNode(5); */
/* 	Edge<int>*e = graph.addEdge(1, n1, n2); */
/* 	graph.addEdge(2, n1, n3); */
/* 	graph.addEdge(3, n2, n4); */
/* 	graph.addEdge(15, n2, n5); */
/* 	graph.addEdge(5, n4, n5); */
/* 	std::vector<Node<int>*> nodes = graph.getNodes(); */
/* 	for (auto it = nodes.begin(); it != nodes.end(); it++) */
/* 		std::cout << (*it)->getData() << std::endl; */
/* 	std::cout << std::endl; */
/* 	int d = 0; */
/* 	struct _distance aux; */
/* 	aux.distance[n1] = 0; */
/* 	aux.goal = n2; */
/* 	aux = graph.breadthFirst(n1, aux); */
/* 	for (auto it = aux.distance.begin(); it != aux.distance.end(); it++) */
/* 		std::cout << it->first->getData() << "  " << it->second << std::endl; */
/* 	std::cout << "Distance to n1: " << aux.distance[n1] << std::endl; */
/* 	std::cout << "Distance to n2: " << aux.distance[n2] << std::endl; */
/* 	std::cout << "Distance to n3: " << aux.distance[n3] << std::endl; */
/* 	std::cout << "Distance to n4: " << aux.distance[n4] << std::endl; */
/* 	std::cout << "Distance to n5: " << aux.distance[n5] << std::endl; */
/* 	return 0; */
/* } */
