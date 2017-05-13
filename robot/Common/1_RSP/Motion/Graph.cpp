#include "Graph.h"
#include "core/ArdFramework.h"
#include "Log.h"

using namespace ard;

#define MAX_NODES       30 // !! CAUTION : Keep in sync with CommonMsg.proto and RemoteControl.proto
#define MAX_LINKS      200 // !! CAUTION : Keep in sync with CommonMsg.proto and RemoteControl.proto

#define START_POINT_ID 0
#define TARGET_POINT_ID 1

//User helper to create a new node, node 0 is reserved for start point
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
    NEW_NODE(    0,     0);//0 = START_POINT_ID
    NEW_NODE(    0,     0);//1 = TARGET_POINT_ID
    NEW_NODE(    0,   760);//2
    NEW_NODE(    0,   415);//3
    NEW_NODE(    0,    70);//4
    NEW_NODE(  530,   160);//5
    NEW_NODE(  340,  -190);//6
    NEW_NODE( 1016,   116);//7
    NEW_NODE(  890,   -65);//8
    NEW_NODE(  756,  -244);//9
    NEW_NODE( 1155,  -350);//10
    NEW_NODE(  875,  -535);//11
    NEW_NODE(  515,   625);//12
    NEW_NODE( 1160,   180);//13

    m_graph.nodes.count = i;
    ASSERT_TEXT(i <= MAX_NODES, "Too many nodes defined.");


    /**
     * Links
     */

    m_graph.links.list = new GraphLink[MAX_LINKS];
    memset(m_graph.links.list, 0, MAX_LINKS*sizeof(GraphLink));

    LinkId j = 0;
    NEW_LINK( START_POINT_ID , TARGET_POINT_ID ); //fake link
    NEW_LINK( TARGET_POINT_ID , START_POINT_ID ); //fake link
    NEW_LINK( 2 , 3 );
    NEW_LINK( 2 , 4 );
    NEW_LINK( 2 , 5 );
    NEW_LINK( 2 , 6 );
    NEW_LINK( 2 , 8 );
    NEW_LINK( 2 , 9 );
    NEW_LINK( 2 , 10 );
    NEW_LINK( 2 , 12 );
    NEW_LINK( 3 , 2 );
    NEW_LINK( 3 , 4 );
    NEW_LINK( 3 , 5 );
    NEW_LINK( 3 , 6 );
    NEW_LINK( 3 , 7 );
    NEW_LINK( 3 , 8 );
    NEW_LINK( 3 , 9 );
    NEW_LINK( 3 , 10 );
    NEW_LINK( 3 , 12);
    NEW_LINK( 4 , 2 );
    NEW_LINK( 4 , 3 );
    NEW_LINK( 4 , 5 );
    NEW_LINK( 4 , 6 );
    NEW_LINK( 4 , 7 );
    NEW_LINK( 4 , 8 );
    NEW_LINK( 4 , 9 );
    NEW_LINK( 4 , 10 );
    NEW_LINK( 4 , 12 );
    NEW_LINK( 4 , 13 );
    NEW_LINK( 5 , 2 );
    NEW_LINK( 5 , 3 );
    NEW_LINK( 5 , 4 );
    NEW_LINK( 5 , 6 );
    NEW_LINK( 5 , 7 );
    NEW_LINK( 5 , 8 );
    NEW_LINK( 5 , 9 );
    NEW_LINK( 5 , 10 );
    NEW_LINK( 5 , 12 );
    NEW_LINK( 5 , 13 );
    NEW_LINK( 6 , 2 );
    NEW_LINK( 6 , 3 );
    NEW_LINK( 6 , 4 );
    NEW_LINK( 6 , 5 );
    NEW_LINK( 6 , 7 );
    NEW_LINK( 6 , 8 );
    NEW_LINK( 6 , 9 );
    NEW_LINK( 6 , 10 );
    NEW_LINK( 6 , 12 );
    NEW_LINK( 6 , 13 );
    NEW_LINK( 7 , 3 );
    NEW_LINK( 7 , 4 );
    NEW_LINK( 7 , 5 );
    NEW_LINK( 7 , 6 );
    NEW_LINK( 7 , 8 );
    NEW_LINK( 7 , 9 );
    NEW_LINK( 7 , 10 );
    NEW_LINK( 7 , 11 );
    NEW_LINK( 7 , 13 );
    NEW_LINK( 8 , 2 );
    NEW_LINK( 8 , 3 );
    NEW_LINK( 8 , 4 );
    NEW_LINK( 8 , 5 );
    NEW_LINK( 8 , 6 );
    NEW_LINK( 8 , 7 );
    NEW_LINK( 8 , 9 );
    NEW_LINK( 8 , 10 );
    NEW_LINK( 8 , 11 );
    NEW_LINK( 8 , 13 );
    NEW_LINK( 9 , 2 );
    NEW_LINK( 9 , 3 );
    NEW_LINK( 9 , 4 );
    NEW_LINK( 9 , 5 );
    NEW_LINK( 9 , 6 );
    NEW_LINK( 9 , 7 );
    NEW_LINK( 9 , 8 );
    NEW_LINK( 9 , 10 );
    NEW_LINK( 9 , 11 );
    NEW_LINK( 9 , 12 );
    NEW_LINK( 9 , 13 );
    NEW_LINK( 10 , 2 );
    NEW_LINK( 10 , 3 );
    NEW_LINK( 10 , 4 );
    NEW_LINK( 10 , 5 );
    NEW_LINK( 10 , 6 );
    NEW_LINK( 10 , 7 );
    NEW_LINK( 10 , 8 );
    NEW_LINK( 10 , 9 );
    NEW_LINK( 10 , 11 );
    NEW_LINK( 10 , 13 );
    NEW_LINK( 11 , 7 );
    NEW_LINK( 11 , 8 );
    NEW_LINK( 11 , 9 );
    NEW_LINK( 11 , 10 );
    NEW_LINK( 11 , 12 );
    NEW_LINK( 11 , 13 );
    NEW_LINK(12 , 2 );
    NEW_LINK(12 , 3 );
    NEW_LINK(12 , 4 );
    NEW_LINK(12 , 5 );
    NEW_LINK(12 , 6 );
    NEW_LINK(12 , 9 );
    NEW_LINK(12 , 11 );
    NEW_LINK(13 , 4 );
    NEW_LINK(13 , 5 );
    NEW_LINK(13 , 6 );
    NEW_LINK(13 , 7 );
    NEW_LINK(13 , 8 );
    NEW_LINK(13 , 9 );
    NEW_LINK(13 , 10 );
    NEW_LINK(13 , 11 );
    m_graph.links.count = j;
    ASSERT_TEXT(j <= MAX_LINKS, "Too many links defined.");
}

Graph::Graph()
{
    reset();
    defineGraph();
    setAllValid();

    m_info = new GraphDijkstraInfo[m_graph.nodes.count];
    ASSERT_TEXT(m_info, "heap exhausted");

    ASSERT_TEXT( m_graph.links.list[0].target == TARGET_POINT_ID, "Start point shall be connected to target point by default.");
    ASSERT_TEXT( m_graph.links.list[1].target == START_POINT_ID, "Target point shall be connected to start point by default.");

    //Compute the number of links to the node and the starting link index
    LinkId currentLinkId = 0;
    uint8_t nbLinks = 0;
    for(NodeId currentNodeId = 0; currentNodeId < m_graph.nodes.count ; currentNodeId++)
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
            //Check link reciprocity (only when source is smaller than target)
            if( m_graph.links.list[currentLinkId].source < m_graph.links.list[currentLinkId].target )
            {
                bool found = false;
                for(int j = currentLinkId+1; j < m_graph.links.count ; j++)
                {
                    if(m_graph.links.list[currentLinkId].source == m_graph.links.list[j].target
                    && m_graph.links.list[currentLinkId].target == m_graph.links.list[j].source)
                    {
                        found = true;
                        break;
                    }
                } 
                ASSERT_TEXT(found, (String("A link (")
                    + int(m_graph.links.list[currentLinkId].source) + ", "
                    + int(m_graph.links.list[currentLinkId].target)
                    + ") has no reciproque").c_str());
            }

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
    m_state.way_count = 0;
    memset(m_state.way, 0, MAX_NODES*sizeof(uint8_t));
    memset(m_info, 0, MAX_LINKS*sizeof(GraphDijkstraInfo));

    m_state.state = eGraphState_GS_IDLE;

    m_graph.links.list[0].target = TARGET_POINT_ID;
    m_graph.links.list[1].source = TARGET_POINT_ID;
}

void Graph::setAllValid()
{
    for(int i = 0 ; i < m_graph.links.count ; i++)
    {
        m_state.valid[i] = true;
    }
}

void Graph::setStartPoint(const Point& source)
{
    //Update graph with source point position
    GraphNode* node = &m_graph.nodes.list[START_POINT_ID];
    node->x = source.x;
    node->y = source.y;
    m_state.startPoint.x = node->x;
    m_state.startPoint.y = node->y;

    //compute entry point : the shortest graph point
    NodeId entry = getShortestNodeId(source);

    //Update link list with start<->entry link
    m_graph.links.list[0].target = entry;
}


void Graph::setTargetPoint(const Point& target)
{
    //Update graph with source point position
    GraphNode* node = &m_graph.nodes.list[TARGET_POINT_ID];
    node->x = target.x;
    node->y = target.y;
    m_state.targetPoint.x = node->x;
    m_state.targetPoint.y = node->y;
    
    //compute entry point : the shortest graph point
    NodeId exit = getShortestNodeId(target);
    
    //Update link list with start<->entry link
    m_graph.links.list[1].target = exit;
}

bool Graph::computeShortertPath(const Point& source, const Point& target)
{
    ASSERT(m_state.state != eGraphState_GS_COMPUTING);
    m_state.state = eGraphState_GS_COMPUTING;

    DelayMs startCompute = millis();

    //Compute entry/exit point
    setStartPoint(source);
    setTargetPoint(target);

    //Compute path
    bool res = computePathBetweenNodes(START_POINT_ID, TARGET_POINT_ID);

    //Display results
    DelayMs endCompute = millis();
    LOG_INFO(String("   --> from ") + (int)getWayPointId(0) + " to " + (int)getWayPointId(getWayPointNb()) + " computed in " + String(endCompute - startCompute) + " ms.");

    return res;
}

bool Graph::computePathBetweenNodes(uint8_t idSource, uint8_t idTarget)
{
    m_state.state = eGraphState_GS_COMPUTING;
    uint32_t i;
    uint32_t j;

    // init
    for( i=0 ; i < m_graph.nodes.count; i++)
    {
        m_info[i].distance = 0xFFFF;
        m_info[i].prev_node = idSource;
        m_info[i].is_best = 0;
    }

    // Le point de depart est a une distance de 0 de lui même et il n'y a pas mieux
    m_info[0].distance = 0;
    i = idSource;

    while( i != idTarget)
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
        i = idSource;
        for(j = 0; j<m_graph.nodes.count; j++)
        {
            if( ! m_info[j].is_best && m_info[j].distance < best_dist)
            {
                best_dist = m_info[j].distance;
                i = j;
            }
        }
        if(i == idSource)
        {
            // rien de trouvé
            return false;
        }
    }

    m_info[i].is_best = 1;

    // on met le chemin dans l'ordre de a vers b dans le tableau m_way
    m_state.way_count = 2;
    i = idTarget;
    while(m_info[i].prev_node != idSource)
    {
        i = m_info[i].prev_node;
        m_state.way_count++;
    }

    setWayPoint(0, idSource);
    j = m_state.way_count - 1;
    setWayPoint(j, idTarget);
    i = idTarget;
    while(m_info[i].prev_node != idSource)
    {
        i = m_info[i].prev_node;
        j--;
        setWayPoint(j, i);
    }

    m_state.state = eGraphState_GS_COMPUTED;
    
    // affichage debug
    for(i=0; i < m_state.way_count; i++)
    {
        String s = String("    graph path : [" + String(i) + "] : id=" + int(m_state.way[i]-1) + " p=" + getWayPoint(i).toString());
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

    for(i = TARGET_POINT_ID+1; i< m_graph.nodes.count; i++)
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

NodeId Graph::getWayPointId(uint8_t rank) const
{
    ASSERT_TEXT(rank < m_state.way_count, "You requested a waypoint which is not in the list");
    ASSERT_TEXT(m_state.state == eGraphState_GS_COMPUTED, "You request a path without having computed it");
    //-1 because data is stocked as a string
    return m_state.way[rank]-1;
}

void Graph::setWayPoint(uint8_t rank, NodeId newId)
{
    m_state.way[rank] = newId+1;
}

Point Graph::getWayPoint(uint8_t rank) const
{
    return getNode(getWayPointId(rank));
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
        //1+ as data is serialized as a string in protobug due to memory alignement problems
        //on classic table and space used by other tables. The 1+ ensures that data is non nul
        //hence serialized in payload as a null character would define the end of the chain
        //The tradeoff is to always send the full list but to have a full list size which is reduced
        links.sources[i] = 1+ m_graph.links.list[i].source;
        links.targets[i] = 1+ m_graph.links.list[i].target;
    }
}

