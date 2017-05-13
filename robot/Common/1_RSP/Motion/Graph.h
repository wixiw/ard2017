#ifndef GRAPH_H
#define GRAPH_H

//! @file graph.h
//! @brief Graph de points de passage pour l'evitement
//! @author Atlantronic

#include <stdint.h>
#include "core/ArdMaths.h"
#include "RemoteControl.pb.h"

namespace ard
{
    typedef uint8_t NodeId;
    typedef uint16_t LinkId;

    typedef struct
    {
        NodeId source;
        NodeId target;
        uint16_t distance;
    } GraphLink;

    typedef struct
    {
        int16_t x;
        int16_t y;
        uint8_t link_id;
        uint8_t link_num;
    } GraphNode;

    typedef struct
    {
        uint8_t count;
        GraphNode* list;
    } GraphNodeList;

    typedef struct
    {
        uint8_t count;
        GraphLink* list;
    } GraphLinkList;

    typedef struct
    {
        GraphNodeList nodes;
        GraphLinkList links;
    } GraphDefinition;

    typedef struct
    {
        uint16_t distance;
        NodeId prev_node;
        NodeId next_node;
        uint8_t is_best;
    } GraphDijkstraInfo;

    /**
     * Nodes id 0 and 1 reserved for start/end point which are dynamically modified
     */
    class Graph
    {
        public:
            Graph();

            //Find the best path between 2 points on the table
            bool computeShortertPath(const PointCap& source, const PointCap& target, eDir sens);

            //! calcule la distance entre le point et tout les noeuds du graph
            //! resultat dans un tableau trié de la plus petite distance à la plus grande
            // les points de départ et d'arrivee sont exclut
            NodeId getShortestNodeId(Point pos, uint16_t* distance);

            void setValidLink(LinkId id, bool valid)
            {
                m_state.valid[id] = valid;
            }

            Point getNode(NodeId id) const
            {
                return Point(m_graph.nodes.list[id].x, m_graph.nodes.list[id].y);
            }

            GraphLink const& getLink(LinkId id) const
            {
                return m_graph.links.list[id];
            }

            uint8_t getWayPointNb() const{return m_state.way_count;};

            //return a way point id in the computed path.
            NodeId getWayPointId(uint8_t rank) const;

            //return a way point in the computed path, as determined by its rank in the waypoint list.
            //0 being the start point, and n the target point
            PointCap getWayPoint(uint8_t rank) const;

            //Reset any ongoing computation
            void reset();

            //Reset links validity
            void setAllValid();

            //Serialization has to be splitted due to data volume
            void serializeNodes(apb_GraphNodes& nodes) const;
            void serializeLinks(apb_GraphLinks& links) const;
            apb_GraphState const& serializeState() const {return m_state;};

        protected:
            GraphDefinition m_graph;
            apb_GraphState m_state;
            
            GraphDijkstraInfo* m_info;//GRAPH_NUM_NODE
            


            //builder function to set up nodes and links
            void defineGraph();

            //Setter to insert a point in the path trajectory
            void setWayPoint(uint8_t rank, NodeId id);

            //walk throught the path and optimize headings
            void optimizeHeadings(eDir sens);

            //! lancement de l'algo dijkstra entre 2 points identifiés par idA et idB
            //it's a disktra algorithm with a reduced heuristic d'(x,y) = d(x,y) + h(y) - h(x)
            //@return true if the path is found, false if no solution exists.
            bool computePathBetweenNodes(NodeId idA, NodeId idB);

            //Find the graph node which is the closest from start/target points and update graph
            NodeId setStartPoint(const PointCap& source);
            NodeId setTargetPoint(const PointCap& target);

            //Defines the disktra distance, in our case it's a reduced distance so that we are as efficient as an A*
            //it's possible because we are in the monotone case
            //with d(x,y) the euclidian distance between 2 nodes, and h(x) the heuristic to the target = d(x,target)
            // h(a) <= d(a,b) +h(b) (due to the triangle inequality)
            //so we use d'(a,b) = d(a,b) + h(b) - h(a)
            //see https://en.wikipedia.org/wiki/A*_search_algorithm
            uint16_t computeDistance(NodeId a, NodeId b);
    };
}

#endif
