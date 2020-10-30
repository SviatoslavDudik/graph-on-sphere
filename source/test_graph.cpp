#include "test_graph.h"
#include "graph.h"
#include <assert.h>
#include <iostream>

void allTests() {
	testNode();
	testEdge();
	testGraph();
}

void testNode() {
	Node<int> n = 42;
	assert(n.getData() == 42);
	n.setData(5);
	assert(n.getData() == 5);
}

void testEdge() {
	Edge<std::string> e("abc");
	assert(e.getAnnotation() == "abc");
	e.setAnnotation("def");
	assert(e.getAnnotation() == "def");
}

void testGraphAddDelete() {
	Graph<int,int> g;
	const std::vector<Node<int>*>& nodes = g.getNodes();
	const std::vector<Edge<int>*>& edges = g.getEdges();
	const Node<int> *n1 = g.addNode(1);
	const Node<int> *n2 = g.addNode(2);
	g.addNode(3);
	g.addEdge(1, 1, 2);
	g.addEdge(2, 1, 3);
	assert(std::find(nodes.begin(), nodes.end(), n1) != nodes.end());
	assert(edges.size() == 2);
	assert(nodes.size() == 3);
	g.deleteNode(2);
	assert(std::find(nodes.begin(), nodes.end(), n2) == nodes.end());
	assert(edges.size() == 1);
	assert(nodes.size() == 2);
	g.deleteNode(n1);
	assert(edges.size() == 0);
	assert(nodes.size() == 1);

	n1 = g.addNode(1);
	n2 = g.addNode(2);
	const Edge<int> *e = g.addEdge(1, n1, n2);
	assert(std::find(edges.begin(), edges.end(), e) != edges.end());
	assert(edges.size() == 1);
	assert(nodes.size() == 3);
	g.deleteEdge(e);
	assert(std::find(edges.begin(), edges.end(), e) == edges.end());
	assert(edges.size() == 0);
	assert(nodes.size() == 3);

	e = g.addEdge(1, 1, 2);
	g.addEdge(2, 1, 2);
	g.addEdge(3, 1, 2);
	g.addEdge(4, 1, 2);
	g.deleteEdge(1, 1, 2);
	assert(std::find(edges.begin(), edges.end(), e) == edges.end());
	assert(edges.size() == 3);
	g.deleteEdge(2, n1, n2);
	assert(edges.size() == 2);
	g.addEdge(0, 1, 3);
	g.addEdge(-1, 1, 3);
	g.deleteEdges(n1, n2);
	assert(edges.size() == 2);
	g.deleteEdges(1, 3);
	assert(edges.size() == 0);
}

struct _test_breadthFirst {
	int a[3] = {0};
	int b[7] = {0};
	int i = 0;
	const Node<int>* operator()(const Node<int> *from, const Node<int> *to, const Edge<int> *by) {
		// because of graph structure used for tests
		if (from->getData() < to->getData()) {
			assert(by->getAnnotation() >= i);
			i = by->getAnnotation();
		}
		a[by->getAnnotation()-1] += 1;
		b[from->getData()-1] += 1;
		return nullptr;
	}
};

void testGraphUtils() {
	Graph<int,int> g;
	const Node<int> *n1 = g.addNode(1);
	const Node<int> *n2 = g.addNode(2);
	g.addEdge(1, n1, n2);
	assert(g.connected(1, n1, n2));
	assert(!g.connected(0, n1, n2));
	assert(!g.connected(1, n2, n1));
	assert(g.connected(1, 1, 2));
	assert(!g.connected(0, 1, 2));
	assert(!g.connected(0, 2, 1));
	for (int i = 3; i<=7; i++)
		g.addNode(i);
	g.addEdge(1, 2, 1);
	g.addEdge(1, 1, 3);
	g.addEdge(1, 3, 1);
	g.addEdge(1, 1, 4);
	g.addEdge(1, 4, 1);
	g.addEdge(2, 3, 4);
	g.addEdge(2, 4, 3);
	g.addEdge(2, 2, 4);
	g.addEdge(2, 4, 2);
	g.addEdge(2, 2, 5);
	g.addEdge(2, 5, 2);
	g.addEdge(3, 5, 6);
	g.addEdge(3, 6, 5);
	g.addEdge(3, 5, 7);
	g.addEdge(3, 7, 5);
	g.addEdge(3, 6, 7);
	g.addEdge(3, 7, 6);
	struct _test_breadthFirst test;
	test = g.breadthFirst(1, test);
	// graph was build in a way to satisfy these asserts
	for (int i = 0; i<3; i++)
		assert(test.a[i] == 6);
	for (int i = 0; i<7; i++)
		assert(test.b[i] >= 2);
	struct _test_breadthFirst test2;
	test2 = g.breadthFirst(n1, test2);
	for (int i = 0; i<3; i++)
		assert(test2.a[i] == 6);
	for (int i = 0; i<7; i++)
		assert(test2.b[i] >= 2);
}

void testGraph() {
	testGraphAddDelete();
	testGraphUtils();
}

