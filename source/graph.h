#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <stdexcept>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>

template <class T>
class Node {
	T _data;
public:
	Node(const T &data);
	void setData(const T &data) { _data = data; }
	inline const T &getData() const { return _data; }
};

template <class T>
class Edge {
	T _annotation;
public:
	Edge(const T &annotation);
	void setAnnotation(const T &annotation) { _annotation = annotation; }
	inline const T &getAnnotation() const { return _annotation; }
};

template <class N, class E>
class Graph {
	std::vector<Node<N>*> nodes;
	std::vector<Edge<E>*> edges;
	std::map<const Node<N>*,std::list<Edge<E>*>> incident_edges;
	std::map<const Edge<E>*,std::pair<Node<N>*,Node<N>*>> incident_nodes;
public:
	~Graph();
	const Node<N>* addNode(const N &data);
	const Edge<E>* addEdge(const E &annotation, const N &d1, const N &d2);
	const Edge<E>* addEdge(const E &annotation, const Node<N> *n1, const Node<N> *n2);
	void deleteNode(const N &data);
	void deleteNode(const Node<N> *node);
	void deleteEdge(const Edge<E> *edge);
	void deleteEdge(const E &annotation, const N &d1, const N &d2);
	void deleteEdge(const E &annotation, const Node<N> *n1, const Node<N> *n2);
	void deleteEdges(const N &d1, const N &d2);
	void deleteEdges(const Node<N> *n1, const Node<N> *n2);
	inline const std::vector<Node<N>*>& getNodes() const { return nodes; }
	inline const std::vector<Edge<E>*>& getEdges() const { return edges; }
	bool connected(const E &annotation, const N &d1, const N &d2) const;
	bool connected(const E &annotation, const Node<N> *n1, const Node<N> *n2) const;
	template <class Function>
	Function breadthFirst(const Node<N> * start, Function fn) const;
	template <class Function>
	Function breadthFirst(const N &start, Function fn) const;
private:
	typename std::vector<Node<N>*>::const_iterator getNode(const N &data) const;
	typename std::vector<Edge<E>*>::const_iterator getEdge(const E &annotation, const Node<N> *n1, const Node<N> *n2) const;
	const Edge<E>* addEdge(const E &annotation, typename std::vector<Node<N>*>::const_iterator it1, typename std::vector<Node<N>*>::const_iterator it2);
	void deleteNode(typename std::vector<Node<N>*>::const_iterator it_node);
	void deleteEdge(typename std::vector<Edge<E>*>::const_iterator it_edge);
};

template <class T>
Node<T>::Node(const T &data) : _data(data) {}

template <class T>
Edge<T>::Edge(const T &annotation) : _annotation(annotation) {}

template <class N, class E>
Graph<N,E>::~Graph() {
	for (auto it = nodes.begin(); it != nodes.end(); it++)
		delete *it;
	for (auto it = edges.begin(); it != edges.end(); it++)
		delete *it;
}

template <class N, class E>
const Node<N> * Graph<N,E>::addNode(const N &data) {
	Node<N> *n = new Node<N>(data);
	nodes.push_back(n);
	incident_edges[n] = std::list<Edge<E>*>();
	return n;
}

template <class N, class E>
const Edge<E>* Graph<N,E>::addEdge(const E &annotation, const N &d1, const N &d2) {
	return addEdge(annotation, getNode(d1), getNode(d2));
}

template <class N, class E>
const Edge<E> * Graph<N,E>::addEdge(const E &annotation, const Node<N> * n1, const Node<N> * n2) {
	auto it_n1 = std::find(nodes.begin(), nodes.end(), n1);
	auto it_n2 = std::find(nodes.begin(), nodes.end(), n2);
	if (it_n1 == nodes.end() || it_n2 == nodes.end())
		throw std::invalid_argument("Graph::addEdge: node is not in the graph");
	if (connected(annotation, *it_n1, *it_n2))
		throw std::invalid_argument("Graph::addEdge: edge exists");
	return addEdge(annotation, it_n1, it_n2);
}

template <class N, class E>
void Graph<N,E>::deleteNode(const N &data) {
	deleteNode(getNode(data));
}

template <class N, class E>
void Graph<N,E>::deleteNode(const Node<N> *node) {
	auto it_node = std::find(nodes.begin(), nodes.end(), node);
	if (it_node == nodes.end())
		throw std::invalid_argument("Graph::deleteNode: node is not in the graph");
	deleteNode(it_node);
}

template <class N, class E>
void Graph<N,E>::deleteEdge(const Edge<E> * edge) {
	auto it_edge = std::find(edges.begin(), edges.end(), edge);
	if (it_edge == edges.end())
		throw std::invalid_argument("Graph::deleteEdge: edge is not in the graph");
	deleteEdge(it_edge);
}

template <class N, class E>
void Graph<N,E>::deleteEdge(const E &annotation, const N &d1, const N &d2) {
	deleteEdge(annotation, *getNode(d1), *getNode(d2));
}

template <class N, class E>
void Graph<N,E>::deleteEdge(const E &annotation, const Node<N> *n1, const Node<N> *n2) {
	deleteEdge(getEdge(annotation, n1, n2));
}

template <class N, class E>
bool Graph<N,E>::connected(const E &annotation, const N &d1, const N &d2) const {
	return connected(annotation, *getNode(d1), *getNode(d2));
}

template <class N, class E>
bool Graph<N,E>::connected(const E &annotation, const Node<N> *n1, const Node<N> *n2) const {
	return getEdge(annotation, n1, n2) != edges.end();
}

template <class N, class E>
void Graph<N,E>::deleteEdges(const N &d1, const N &d2) {
	deleteEdges(*getNode(d1), *getNode(d2));
}

template <class N, class E>
void Graph<N,E>::deleteEdges(const Node<N> *n1, const Node<N> *n2) {
	std::list<Edge<E>*> list_edges = incident_edges[n1];
	for (auto it = list_edges.begin(); it != list_edges.end();) {
		if (incident_nodes[*it].second == n2) {
			Edge<E> *edge = *it;
			it = list_edges.erase(it);
			deleteEdge(edge);
		}
		else {
			it++;
		}
	}
}

template <class N, class E>
template <class Function>
Function Graph<N,E>::breadthFirst(const Node<N> * start, Function fn) const {
	std::queue<const Node<N>*> queue;
	std::set<const Node<N>*> discovered;
	discovered.insert(start);
	queue.push(start);
	while (!queue.empty()) {
		const Node<N> *curr = queue.front();
		queue.pop();
		std::list<Edge<E>*> edges = incident_edges.at(curr);
		for (Edge<E> *edge: edges) {
			const Node<N> *node = incident_nodes.at(edge).second;
			if (fn(curr, node, edge) == curr)
				return fn;
			if (discovered.find(node) == discovered.end()) {
				discovered.insert(node);
				queue.push(node);
			}
		}
	}
	return fn;
}

template <class N, class E>
template <class Function>
Function Graph<N,E>::breadthFirst(const N &start, Function fn) const {
	return breadthFirst(*getNode(start), fn);
}

template <class N, class E>
typename std::vector<Node<N>*>::const_iterator Graph<N,E>::getNode(const N &data) const {
	for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
		if ((*it)->getData() == data) {
			return it;
		}
	}
	return nodes.cend();
}

template <class N, class E>
typename std::vector<Edge<E>*>::const_iterator Graph<N,E>::getEdge(const E &annotation, const Node<N> *n1, const Node<N> *n2) const {
	for (auto it = edges.cbegin(); it != edges.cend(); it++) {
		if ((*it)->getAnnotation() == annotation) {
			std::pair<Node<N>*,Node<N>*> nodes = incident_nodes.at(*it);
			if (nodes.first == n1 && nodes.second == n2) {
				return it;
			}
		}
	}
	return edges.cend();
}

template <class N, class E>
const Edge<E>* Graph<N,E>::addEdge(const E &annotation, typename std::vector<Node<N>*>::const_iterator it1, typename std::vector<Node<N>*>::const_iterator it2) {
	Edge<E> *e = new Edge<E>(annotation);
	edges.push_back(e);
	incident_nodes[e] = std::make_pair(*it1,*it2);
	incident_edges[*it1].push_back(e);
	return e;
}

template <class N, class E>
void Graph<N,E>::deleteNode(typename std::vector<Node<N>*>::const_iterator it_node) {
	if (it_node == nodes.cend())
		throw std::invalid_argument("Graph::deleteNode: node is not in the graph");
	Node<N> *node = *it_node;
	nodes.erase(it_node);
	for (auto it = edges.begin(); it != edges.end(); ) {
		std::pair<Node<N>*,Node<N>*> pair = incident_nodes[*it];
		if (pair.first == node || pair.second == node) {
			Edge<E> *edge = *it;
			auto it2 = edges.erase(it);
			incident_nodes.erase(edge);
			delete edge;
			it = it2;
		}
		else {
			it ++;
		}
	}
	incident_edges.erase(node);
	delete node;
}

template <class N, class E>
void Graph<N,E>::deleteEdge(typename std::vector<Edge<E>*>::const_iterator it_edge) {
	if (it_edge == edges.cend())
		throw std::invalid_argument("Graph::deleteEdge: edge is not in the graph");
	Edge<E> *edge = *it_edge;
	edges.erase(it_edge);
	incident_nodes.erase(edge);
	delete edge;
}

#endif
