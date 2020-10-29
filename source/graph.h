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
	void setData(const T& data) { _data = data; }
	inline const T& getData() const { return _data; }
};

template <class T>
class Edge {
	T _annotation;
public:
	Edge(const T &annotation);
	void setAnnotation(const T& annotation) { _annotation = annotation; }
	inline const T& getAnnotation() const { return _annotation; }
};

template <class N, class E>
class Graph {
	std::vector<Node<N>*> nodes;
	std::vector<Edge<E>*> edges;
	std::map<const Node<N>*,std::list<Edge<E>*>> incident_edges;
	std::map<const Edge<E>*,std::pair<Node<N>*,Node<N>*>> incident_nodes;
public:
	~Graph();
	Node<N>* addNode(const N& data);
	Edge<E>* addEdge(const E& annotation, const Node<N> *n1, const Node<N> *n2);
	void deleteNode(const Node<N> *node);
	void deleteEdge(const Edge<E> *edge);
	void deleteEdge(const E& annotation, const Node<N> *n1, const Node<N> *n2);
	inline const std::vector<Node<N>*>& getNodes() const { return nodes; }
	inline const std::vector<Edge<E>*>& getEdges() const { return edges; }
	bool connected(const E &annotation, const Node<N> *n1, const Node<N> *n2) const;
	const Edge<E>* getEdge(const E &annotation, const Node<N> *n1, const Node<N> *n2) const;
	template <class Function>
	Function breadthFirst(Node<N> * start, Function fn);
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
Node<N> * Graph<N,E>::addNode(const N& data) {
	Node<N> *n = new Node<N>(data);
	nodes.push_back(n);
	incident_edges[n] = std::list<Edge<E>*>();
	return n;
}

template <class N, class E>
Edge<E> * Graph<N,E>::addEdge(const E& annotation, const Node<N> * n1, const Node<N> * n2) {
	auto it_n1 = std::find(nodes.begin(), nodes.end(), n1);
	auto it_n2 = std::find(nodes.begin(), nodes.end(), n2);
	if (it_n1 == nodes.end() || it_n2 == nodes.end())
		throw std::invalid_argument("Graph::addEdge: node is not in the graph");
	if (connected(annotation, *it_n1, *it_n2))
		throw std::invalid_argument("Graph::addEdge: edge exists");
	Edge<E> *e = new Edge<E>(annotation);
	edges.push_back(e);
	incident_nodes[e] = std::make_pair(*it_n1,*it_n2);
	incident_edges[n1].push_back(e);
	return e;
}

template <class N, class E>
void Graph<N,E>::deleteNode(const Node<N> * node) {
	auto it_node = std::find(nodes.begin(), nodes.end(), node);
	if (it_node == nodes.end())
		throw std::invalid_argument("Graph::deleteNode: node is not in the graph");
	nodes.erase(it_node);
	std::list<Edge<E>*> list_edges = incident_edges[node];
	for (auto it = list_edges.begin(); it !=  list_edges.end(); it++)
		deleteEdge(*it);
	incident_edges.erase(node);
	delete node;
}

template <class N, class E>
void Graph<N,E>::deleteEdge(const Edge<E> * edge) {
	auto it_edge = std::find(edges.begin(), edges.end(), edge);
	if (it_edge == edges.end())
		throw std::invalid_argument("Graph::deleteEdge: edge is not in the graph");
	edges.erase(it_edge);
	incident_nodes.erase(edge);
	delete edge;
}

template <class N, class E>
void Graph<N,E>::deleteEdge(const E& annotation, const Node<N> *n1, const Node<N> *n2) {
	deleteEdge(getEdge(annotation, n1, n2));
}

template <class N, class E>
bool Graph<N,E>::connected(const E &annotation, const Node<N> *n1, const Node<N> *n2) const {
	return getEdge(annotation, n1, n2) != nullptr;
}

template <class N, class E>
const Edge<E>* Graph<N,E>::getEdge(const E &annotation, const Node<N> *n1, const Node<N> *n2) const {
	std::list<Edge<E>*> edges = incident_edges.at(n1);
	for (auto it = edges.cbegin(); it != edges.cend(); it++) {
		if ((*it)->getAnnotation() == annotation) {
			if (incident_nodes.at(*it).second == n2) {
				return *it;
			}
		}
	}
	return nullptr;
}

template <class N, class E>
template <class Function>
Function Graph<N,E>::breadthFirst(Node<N> * start, Function fn) {
	std::queue<Node<N>*> queue;
	std::set<Node<N>*> discovered;
	discovered.insert(start);
	queue.push(start);
	while (!queue.empty()) {
		Node<N> * curr = queue.front();
		queue.pop();
		std::list<Edge<E>*> edges = incident_edges.at(curr);
		for (auto it = edges.cbegin(); it != edges.cend(); it++) {
			Node<N> * node = incident_nodes.at(*it).second;
			if (fn(curr, node, *it) == curr)
				return fn;
			if (discovered.find(node) == discovered.end()) {
				discovered.insert(node);
				queue.push(node);
			}
		}
	}
	return fn;
}

#endif
