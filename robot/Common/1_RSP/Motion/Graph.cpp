#include "Graph.h"
#include <math.h>
#include "core/ArdFramework.h"
#include "Log.h"

using namespace ard;

#define MAX_NODES       30 // !! CAUTION : Keep in sync with CommonMsg.proto and RemoteControl.proto
#define MAX_LINKS      200 // !! CAUTION : Keep in sync with CommonMsg.proto and RemoteControl.proto

//User helper to create a new node
#define NEW_NODE(XXX,YYY) \
m_graph.nodes.list[i].x = XXX;\
m_graph.nodes.list[i].y = YYY;\
++i;

#define NEW_LINK(XXX,YYY) \
m_graph.links.list[j].source = XXX;\
m_graph.links.list[j].target = YYY;\
++j;

void Graph::defineGraph()
{
    /**
     * Nodes
     */

    m_graph.nodes.list = new GraphNode[MAX_NODES];
    memset(m_graph.nodes.list, 0, MAX_NODES*sizeof(GraphNode));
    NodeId i = 0;

        //table vertical line :
    NEW_NODE(    0,   760);//0
    NEW_NODE(    0,   415);//1
    NEW_NODE(    0,    70);//2
    NEW_NODE(  530,   160);//3
    NEW_NODE(  340,  -190);//4
    NEW_NODE( 1016,   116);//5
    NEW_NODE(  890,   -65);//6
    NEW_NODE(  756,  -244);//7
    NEW_NODE( 1155,  -350);//8
    NEW_NODE(  875,  -535);//9
    NEW_NODE(  515,   625);//10
    NEW_NODE( 1160,   180);//11

    m_graph.nodes.count = i;


    /**
     * Links
     */

    m_graph.links.list = new GraphLink[MAX_LINKS];
    memset(m_graph.links.list, 0, MAX_LINKS*sizeof(GraphLink));

    LinkId j = 0;
    NEW_LINK( 0 , 1 );
    NEW_LINK( 0 , 2 );
    NEW_LINK( 0 , 3 );
    NEW_LINK( 0 , 4 );
    NEW_LINK( 0 , 6 );
    NEW_LINK( 0 , 7 );
    NEW_LINK( 0 , 8 );
    NEW_LINK( 0 ,10 );
    NEW_LINK( 1 , 0 );
    NEW_LINK( 1 , 2 );
    NEW_LINK( 1 , 3 );
    NEW_LINK( 1 , 4 );
    NEW_LINK( 1 , 5 );
    NEW_LINK( 1 , 6 );
    NEW_LINK( 1 , 7 );
    NEW_LINK( 1 , 8 );
    NEW_LINK( 1 ,10 );
    NEW_LINK( 2 , 0 );
    NEW_LINK( 2 , 1 );
    NEW_LINK( 2 , 3 );
    NEW_LINK( 2 , 4 );
    NEW_LINK( 2 , 5 );
    NEW_LINK( 2 , 6 );
    NEW_LINK( 2 , 7 );
    NEW_LINK( 2 , 8 );
    NEW_LINK( 2 ,10 );
    NEW_LINK( 2 ,11 );
    NEW_LINK( 3 , 0 );
    NEW_LINK( 3 , 1 );
    NEW_LINK( 3 , 2 );
    NEW_LINK( 3 , 4 );
    NEW_LINK( 3 , 5 );
    NEW_LINK( 3 , 6 );
    NEW_LINK( 3 , 7 );
    NEW_LINK( 3 , 8 );
    NEW_LINK( 3 ,10 );
    NEW_LINK( 3 ,11 );
    NEW_LINK( 4 , 0 );
    NEW_LINK( 4 , 1 );
    NEW_LINK( 4 , 2 );
    NEW_LINK( 4 , 3 );
    NEW_LINK( 4 , 5 );
    NEW_LINK( 4 , 6 );
    NEW_LINK( 4 , 7 );
    NEW_LINK( 4 , 8 );
    NEW_LINK( 4 ,10 );
    NEW_LINK( 4 ,11 );
    NEW_LINK( 5 , 1 );
    NEW_LINK( 5 , 2 );
    NEW_LINK( 5 , 3 );
    NEW_LINK( 5 , 4 );
    NEW_LINK( 5 , 6 );
    NEW_LINK( 5 , 7 );
    NEW_LINK( 5 , 8 );
    NEW_LINK( 5 , 9 );
    NEW_LINK( 5 ,11 );
    NEW_LINK( 6 , 0 );
    NEW_LINK( 6 , 1 );
    NEW_LINK( 6 , 2 );
    NEW_LINK( 6 , 3 );
    NEW_LINK( 6 , 4 );
    NEW_LINK( 6 , 5 );
    NEW_LINK( 6 , 7 );
    NEW_LINK( 6 , 8 );
    NEW_LINK( 6 , 9 );
    NEW_LINK( 6 ,11 );
    NEW_LINK( 7 , 0 );
    NEW_LINK( 7 , 1 );
    NEW_LINK( 7 , 2 );
    NEW_LINK( 7 , 3 );
    NEW_LINK( 7 , 4 );
    NEW_LINK( 7 , 5 );
    NEW_LINK( 7 , 6 );
    NEW_LINK( 7 , 8 );
    NEW_LINK( 7 , 9 );
    NEW_LINK( 7 ,10 );
    NEW_LINK( 7 ,11 );
    NEW_LINK( 8 , 0 );
    NEW_LINK( 8 , 1 );
    NEW_LINK( 8 , 2 );
    NEW_LINK( 8 , 3 );
    NEW_LINK( 8 , 4 );
    NEW_LINK( 8 , 5 );
    NEW_LINK( 8 , 6 );
    NEW_LINK( 8 , 7 );
    NEW_LINK( 8 , 9 );
    NEW_LINK( 8 ,11 );
    NEW_LINK( 9 , 5 );
    NEW_LINK( 9 , 6 );
    NEW_LINK( 9 , 7 );
    NEW_LINK( 9 , 8 );
    NEW_LINK( 9 ,10 );
    NEW_LINK( 9 ,11 );
    NEW_LINK(10 , 0 );
    NEW_LINK(10 , 1 );
    NEW_LINK(10 , 2 );
    NEW_LINK(10 , 3 );
    NEW_LINK(10 , 4 );
    NEW_LINK(10 , 7 );
    NEW_LINK(10 , 9 );
    NEW_LINK(11 , 2 );
    NEW_LINK(11 , 3 );
    NEW_LINK(11 , 4 );
    NEW_LINK(11 , 5 );
    NEW_LINK(11 , 6 );
    NEW_LINK(11 , 7 );
    NEW_LINK(11 , 8 );
    NEW_LINK(11 , 9 );
    m_graph.links.count = j;
}

Graph::Graph()
{
    reset();
    defineGraph();
    setAllValid();

    m_info = new GraphDijkstraInfo[m_graph.nodes.count];
    ASSERT_TEXT(m_info, "heap exhausted");

    //Compute the number of links to the node and the starting link index
    LinkId currentLinkId = 0;
    uint8_t nbLinks = 0;
    for(NodeId currentNodeId = 0 ; currentNodeId < m_graph.nodes.count ; currentNodeId++)
    {
        //The starting index is the current linkId
        m_graph.nodes.list[currentNodeId].link_id = currentLinkId;
        ASSERT_TEXT(m_graph.links.list[currentLinkId].source == currentNodeId, "It's not expected to have source node which are not connected or not in growing order.");
        nbLinks = 0;

        //Walk through the links where this node is the source node
        while(m_graph.links.list[currentLinkId].source == currentNodeId)
        {
            ASSERT_TEXT(m_graph.links.list[currentLinkId].source != m_graph.links.list[currentLinkId].target, "A source can't also be a target");
            ASSERT_TEXT(currentLinkId < m_graph.links.count, "The number of links is ill defined");
            nbLinks++;
            currentLinkId++;
        }
        m_graph.nodes.list[currentNodeId].link_num = nbLinks;
    }
    ASSERT_TEXT(currentLinkId == m_graph.links.count, "The number of links is ill defined");

    //Compute distances between nodes
    for(LinkId linkId = 0 ; linkId < m_graph.links.count ; linkId++)
    {
        GraphLink* link = &m_graph.links.list[linkId];
        ASSERT_TEXT(link->source < m_graph.nodes.count, (String("A source node id[") + link->source + "] is too big").c_str());
        ASSERT_TEXT(link->target < m_graph.nodes.count, (String("A source node id[") + link->target + "] is too big").c_str());
        Point source = Point(m_graph.nodes.list[link->source].x, m_graph.nodes.list[link->source].y);
        Point target = Point(m_graph.nodes.list[link->target].x, m_graph.nodes.list[link->target].y);
        link->distance = source.distanceTo(target);
    }
}

void Graph::reset()
{
    m_state.way_count = m_graph.nodes.count;
    memset(m_state.way, 0, MAX_NODES*sizeof(uint8_t));
    memset(m_info, 0, MAX_LINKS*sizeof(GraphDijkstraInfo));

    m_state.state = eGraphState_GS_IDLE;
}

void Graph::setAllValid()
{
    for(int i = 0 ; i < m_graph.links.count ; i++)
    {
        m_state.valid[i] = true;
    }
}

bool Graph::computeShortestPath(uint8_t a, uint8_t b)
{
//TODO    ASSERT_TEXT(m_state.state == eGraphState_GS_IDLE, "You have to reset the graph before doing any computation.");

    uint32_t i;
    uint32_t j;

    // init
    for( i=0 ; i < m_graph.nodes.count; i++)
    {
        m_info[i].distance = 0xFFFF;
        m_info[i].prev_node = a;
        m_info[i].is_best = 0;
    }

    // a est a une distance de 0 de lui même et il n'y a pas mieux
    m_info[a].distance = 0;
    i = a;

    while( i != b)
    {
        m_info[i].is_best = 1;

        uint32_t max = m_graph.nodes.list[i].link_id + m_graph.nodes.list[i].link_num;
        for(j = m_graph.nodes.list[i].link_id; j < max; j++)
        {
            if( m_state.valid[j])
            {
                int connected_node = m_graph.links.list[j].target;
                // calcul de la distance en passant par la
                uint16_t dist = m_info[i].distance + m_graph.links.list[j].distance + 200; //200 is a penalty to penalize manoeuvers
                if( m_info[connected_node].distance > dist)
                {
                    // on a trouvé un chemin plus court pour aller vers "connected_node"
                    m_info[connected_node].distance = dist;
                    m_info[connected_node].prev_node = i;
                }
            }
        }

        uint16_t best_dist = 0xFFFF;
        i = a;
        for(j = 0; j<m_graph.nodes.count; j++)
        {
            if( ! m_info[j].is_best && m_info[j].distance < best_dist)
            {
                best_dist = m_info[j].distance;
                i = j;
            }
        }
        if(i == a)
        {
            // rien de trouvé
            return false;
        }
    }

    m_info[i].is_best = 1;

    // on met le chemin dans l'ordre de a vers b dans le tableau m_way
    m_state.way_count = 2;
    i = b;
    while(m_info[i].prev_node != a)
    {
        i = m_info[i].prev_node;
        m_state.way_count++;
    }

    m_state.way[0] = a+1;
    j = m_state.way_count - 1;
    m_state.way[j] = b+1;
    i = b;
    while(m_info[i].prev_node != a)
    {
        i = m_info[i].prev_node;
        j--;
        m_state.way[j] = i+1;
    }

    m_state.state = eGraphState_GS_COMPUTED;
    
    // affichage debug
    for(i=0; i < m_state.way_count; i++)
    {
        String s = String("chemin - graph : [" + String(i) + "] : id=" + int(m_state.way[i]-1) + " p=" + getWayPoint(i).toString());
        LOG_INFO(s);
    }

    return true;
}

NodeId Graph::getShortestNodeId(Point pos)
{
    int i;
    uint16_t dist = 0;
    NodeId shortestId = 0;
    uint16_t shortestDist = 0xFFFF;

    for(i = 0; i< m_graph.nodes.count; i++)
    {
        dist = pos.distanceTo(Point(m_graph.nodes.list[i].x, m_graph.nodes.list[i].y));

        if( dist < shortestDist )
        {
            shortestDist = dist;
            shortestId = i;
        }
    }

    return shortestId;
}

Point Graph::getWayPoint(uint8_t rank) const
{
    ASSERT_TEXT(rank < m_state.way_count, "You requested a waypoint which is not in the list");
    ASSERT_TEXT(m_state.state == eGraphState_GS_COMPUTED, "You request a path without having computed it");
    return getNode(m_state.way[rank]-1);
}

void Graph::serializeNodes(apb_GraphNodes& nodes) const
{
    nodes.nodes_count = m_graph.nodes.count;

    for( uint32_t i = 0 ; i < nodes.nodes_count; i++)
    {
        nodes.nodes[i].x = m_graph.nodes.list[i].x;
        nodes.nodes[i].y = m_graph.nodes.list[i].y;
    }
}

void Graph::serializeLinks(apb_GraphLinks& links) const
{
    links.count = m_graph.links.count;

    for( uint32_t i = 0 ; i < m_graph.links.count; i++)
    {
        links.sources[i] = 1+ m_graph.links.list[i].source;
        links.targets[i] = 1+ m_graph.links.list[i].target;
    }
}

