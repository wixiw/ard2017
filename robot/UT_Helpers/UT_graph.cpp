#include "BSP.hpp"
#include "Motion/Graph.h"

using namespace ard;

bool UT_graph1()
{
	bool res = true;
	char buffer[GRAPH_NUM_NODE][100];
	
	Graph graph;
	uint8_t idA = 0;
	uint8_t idB = 9;
	Point A = graph.getNode(idA);
	Point B = graph.getNode(idB);
	res &= graph.computeShortestPath(idA,idB);
    int size = sizeof(graph);
	for(int i = 0; i < graph.getWayPointNb() ; i++)
	{
	    String s = String("[") + String(i) + "]: " + graph.getWayPoint(i).toString();
	    strcpy(buffer[i], s.c_str());
	}

	return res;
}


bool UT_graph()
{
	bool res = true;

	res &= UT_graph1();

	return res;
}

