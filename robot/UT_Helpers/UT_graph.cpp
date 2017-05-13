#include "BSP.hpp"
#include "Motion/Graph.h"

using namespace ard;

bool UT_graph1()
{
	bool res = true;
	
	Graph graph;
	uint8_t idA = 0;
	uint8_t idB = 9;
	Point A = graph.getNode(idA);
	Point B = graph.getNode(idB);
	//res &= graph.computePathBetweenNodes(idA,idB);
    int size = sizeof(graph);

	return res;
}


bool UT_graph()
{
	bool res = true;

	res &= UT_graph1();

	return res;
}

