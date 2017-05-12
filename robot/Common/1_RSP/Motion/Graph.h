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
        uint8_t is_best;
    } GraphDijkstraInfo;


    class Graph
    {
        public:
            Graph();

            //! lancement de l'algo dijkstra entre 2 points identifiés par idA et idB
            //it's a disktra algorithm with a reduced heuristic d'(x,y) = d(x,y) + h(y) - h(x)
            //@return true if the path is found, false if no solution exists.
            bool computeShortestPath(NodeId idA, NodeId idB);

            //! calcule la distance entre le point et tout les noeuds du graph
            //! resultat dans un tableau trié de la plus petite distance à la plus grande
            NodeId getShortestNodeId(Point pos);

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

            //return a way point in the path, as determined by its rank in the waypoint list.
            //0 being the start point, and n the target point
            Point getWayPoint(uint8_t rank) const;

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
            uint8_t way_count;
            
            GraphDijkstraInfo* m_info;//GRAPH_NUM_NODE
            
            void defineGraph();
    };
}

#endif
