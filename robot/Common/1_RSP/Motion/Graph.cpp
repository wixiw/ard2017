#include "Graph.h"
#include "core/ArdFramework.h"
#include "Log.h"

using namespace ard;

#define MAX_NODES       30 // !! CAUTION : Keep in sync with CommonMsg.proto and RemoteControl.proto
#define MAX_LINKS      250 // !! CAUTION : Keep in sync with CommonMsg.proto and RemoteControl.proto
#define MAX_PATH_COUNT  15 // !! CAUTION : Keep in sync with CommonMsg.proto and RemoteControl.proto

#define START_POINT_ID 0
#define TARGET_POINT_ID 1

#define PROXIMITY 20 //mm : distance under which way point are skipped

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
    NEW_NODE(  776,  -244);//9
    NEW_NODE( 1155,  -350);//10
    NEW_NODE(  875,  -535);//11
    NEW_NODE(  515,   625);//12
    NEW_NODE( 1160,   180);//13
    NEW_NODE(  260,   340);//14
    NEW_NODE( -530,   160);//15 sym of 5
    NEW_NODE( -340,  -190);//16 sym of 6
    NEW_NODE(-1016,   116);//17 sym of 7
    NEW_NODE( -890,   -65);//18 sym of 8
    NEW_NODE( -756,  -244);//19 sym of 9
    NEW_NODE(-1155,  -350);//20 sym of 10
    NEW_NODE( -875,  -535);//21 sym of 11
    NEW_NODE( -485,   385);//22 nearly sym of 12
    NEW_NODE(-1160,   180);//23 sym of 13

    m_graph.nodes.count = i;
    ASSERT_TEXT(i <= MAX_NODES, "Too many nodes defined.");


    /**
     * Links
     */

    m_graph.links.list = new GraphLink[MAX_LINKS];
    memset(m_graph.links.list, 0, MAX_LINKS*sizeof(GraphLink));

    LinkId j = 2;
    NEW_LINK( 2 , 3 );
    NEW_LINK( 2 , 4 );
    NEW_LINK( 2 , 5 );
    NEW_LINK( 2 , 6 );
    NEW_LINK( 2 , 8 );
    NEW_LINK( 2 , 9 );
    NEW_LINK( 2 , 12 );
    NEW_LINK( 2 , 14 );
    NEW_LINK( 2 , 15 );
    NEW_LINK( 2 , 16 );
    NEW_LINK( 2 , 18 );
    NEW_LINK( 2 , 19 );
    NEW_LINK( 2 , 22 );
    NEW_LINK( 3 , 2 );
    NEW_LINK( 3 , 4 );
    NEW_LINK( 3 , 5 );
    NEW_LINK( 3 , 6 );
    NEW_LINK( 3 , 7 );
    NEW_LINK( 3 , 8 );
    NEW_LINK( 3 , 9 );
    NEW_LINK( 3 , 12);
    NEW_LINK( 3 , 14);
    NEW_LINK( 3 , 15);
    NEW_LINK( 3 , 16);
    NEW_LINK( 3 , 17);
    NEW_LINK( 3 , 18);
    NEW_LINK( 3 , 19);
    NEW_LINK( 3 , 22);
    NEW_LINK( 4 , 2 );
    NEW_LINK( 4 , 3 );
    NEW_LINK( 4 , 5 );
    NEW_LINK( 4 , 6 );
    NEW_LINK( 4 , 7 );
    NEW_LINK( 4 , 8 );
    NEW_LINK( 4 , 9 );
    NEW_LINK( 4 , 12 );
    NEW_LINK( 4 , 13 );
    NEW_LINK( 4 , 14 );
    NEW_LINK( 4 , 15 );
    NEW_LINK( 4 , 16 );
    NEW_LINK( 4 , 17 );
    NEW_LINK( 4 , 18 );
    NEW_LINK( 4 , 19 );
    NEW_LINK( 4 , 22 );
    NEW_LINK( 4 , 23 );
    NEW_LINK( 5 , 2 );
    NEW_LINK( 5 , 3 );
    NEW_LINK( 5 , 4 );
    NEW_LINK( 5 , 6 );
    NEW_LINK( 5 , 7 );
    NEW_LINK( 5 , 8 );
    NEW_LINK( 5 , 9 );
    NEW_LINK( 5 , 10 );
    NEW_LINK( 5 , 11 );
    NEW_LINK( 5 , 12 );
    NEW_LINK( 5 , 13 );
    NEW_LINK( 5 , 14 );
    NEW_LINK( 5 , 15 );
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
    NEW_LINK( 6 , 14 );
    NEW_LINK( 7 , 3 );
    NEW_LINK( 7 , 4 );
    NEW_LINK( 7 , 5 );
    NEW_LINK( 7 , 6 );
    NEW_LINK( 7 , 8 );
    NEW_LINK( 7 , 9 );
    NEW_LINK( 7 , 10 );
    NEW_LINK( 7 , 11 );
    NEW_LINK( 7 , 13 );
    NEW_LINK( 7 , 14 );
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
    NEW_LINK( 8 , 14 );
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
    NEW_LINK( 9 , 14 );
    NEW_LINK( 10 , 5 );
    NEW_LINK( 10 , 6 );
    NEW_LINK( 10 , 7 );
    NEW_LINK( 10 , 8 );
    NEW_LINK( 10 , 9 );
    NEW_LINK( 10 , 11 );
    NEW_LINK( 10 , 13 );
    NEW_LINK( 10 , 14 );
    NEW_LINK( 11 , 5 );
    NEW_LINK( 11 , 7 );
    NEW_LINK( 11 , 8 );
    NEW_LINK( 11 , 9 );
    NEW_LINK( 11 , 10 );
    NEW_LINK( 11 , 12 );
    NEW_LINK( 11 , 13 );
    NEW_LINK( 11 , 14 );
    NEW_LINK(12 , 2 );
    NEW_LINK(12 , 3 );
    NEW_LINK(12 , 4 );
    NEW_LINK(12 , 5 );
    NEW_LINK(12 , 6 );
    NEW_LINK(12 , 9 );
    NEW_LINK(12 , 11 );
    NEW_LINK(12 , 14 );
    NEW_LINK(12 , 15 );
    NEW_LINK(12 , 16 );
    NEW_LINK(12 , 20 );
    NEW_LINK(12 , 22 );
    NEW_LINK(13 , 4 );
    NEW_LINK(13 , 5 );
    NEW_LINK(13 , 6 );
    NEW_LINK(13 , 7 );
    NEW_LINK(13 , 8 );
    NEW_LINK(13 , 9 );
    NEW_LINK(13 , 10 );
    NEW_LINK(13 , 11 );
    NEW_LINK(13 , 18 );
    NEW_LINK(14 , 2 );
    NEW_LINK(14 , 3 );
    NEW_LINK(14 , 4 );
    NEW_LINK(14 , 5 );
    NEW_LINK(14 , 6 );
    NEW_LINK(14 , 7 );
    NEW_LINK(14 , 8 );
    NEW_LINK(14 , 9 );
    NEW_LINK(14 , 10 );
    NEW_LINK(14 , 11 );
    NEW_LINK(14 , 12 );
    NEW_LINK(14 , 15 );
    NEW_LINK(14 , 16 );
    NEW_LINK(15 , 2 );
    NEW_LINK(15 , 3 );
    NEW_LINK(15 , 4 );
    NEW_LINK(15 , 5 );
    NEW_LINK(15 , 12 );
    NEW_LINK(15 , 14 );
    NEW_LINK(15 , 16 );
    NEW_LINK(15 , 17 );
    NEW_LINK(15 , 18 );
    NEW_LINK(15 , 19 );
    NEW_LINK(15 , 20 );
    NEW_LINK(15 , 21 );
    NEW_LINK(15 , 22 );
    NEW_LINK(15 , 23 );
    NEW_LINK(16 , 2 );
    NEW_LINK(16 , 3 );
    NEW_LINK(16 , 4 );
    NEW_LINK(16 , 12 );
    NEW_LINK(16 , 14 );
    NEW_LINK(16 , 15 );
    NEW_LINK(16 , 17 );
    NEW_LINK(16 , 18 );
    NEW_LINK(16 , 19 );
    NEW_LINK(16 , 20 );
    NEW_LINK(16 , 22 );
    NEW_LINK(16 , 23 );
    NEW_LINK(17 , 3 );
    NEW_LINK(17 , 4 );
    NEW_LINK(17 , 15 );
    NEW_LINK(17 , 16 );
    NEW_LINK(17 , 18 );
    NEW_LINK(17 , 19 );
    NEW_LINK(17 , 20 );
    NEW_LINK(17 , 21 );
    NEW_LINK(17 , 23 );
    NEW_LINK(18 , 2 );
    NEW_LINK(18 , 3 );
    NEW_LINK(18 , 4 );
    NEW_LINK(18 , 13 );
    NEW_LINK(18 , 15 );
    NEW_LINK(18 , 16 );
    NEW_LINK(18 , 17 );
    NEW_LINK(18 , 19 );
    NEW_LINK(18 , 20 );
    NEW_LINK(18 , 21 );
    NEW_LINK(18 , 23 );
    NEW_LINK(19 , 2 );
    NEW_LINK(19 , 3 );
    NEW_LINK(19 , 4 );
    NEW_LINK(19 , 15 );
    NEW_LINK(19 , 16 );
    NEW_LINK(19 , 17 );
    NEW_LINK(19 , 18 );
    NEW_LINK(19 , 20 );
    NEW_LINK(19 , 21 );
    NEW_LINK(19 , 22 );
    NEW_LINK(19 , 23 );
    NEW_LINK(20 , 12 );
    NEW_LINK(20 , 15 );
    NEW_LINK(20 , 16 );
    NEW_LINK(20 , 17 );
    NEW_LINK(20 , 18 );
    NEW_LINK(20 , 19 );
    NEW_LINK(20 , 21 );
    NEW_LINK(20 , 23 );
    NEW_LINK(21 , 15 );
    NEW_LINK(21 , 17 );
    NEW_LINK(21 , 18 );
    NEW_LINK(21 , 19 );
    NEW_LINK(21 , 20 );
    NEW_LINK(21 , 22 );
    NEW_LINK(21 , 23 );
    NEW_LINK(22 , 2 );
    NEW_LINK(22 , 3 );
    NEW_LINK(22 , 4 );
    NEW_LINK(22 , 12 );
    NEW_LINK(22 , 15 );
    NEW_LINK(22 , 16 );
    NEW_LINK(22 , 19 );
    NEW_LINK(22 , 21 );
    NEW_LINK(23 , 4 );
    NEW_LINK(23 , 15 );
    NEW_LINK(23 , 16 );
    NEW_LINK(23 , 17 );
    NEW_LINK(23 , 18 );
    NEW_LINK(23 , 19 );
    NEW_LINK(23 , 20 );
    NEW_LINK(23 , 21 );
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
    
    m_graph.links.list[0].source = START_POINT_ID;
    m_graph.links.list[0].target = START_POINT_ID;
    m_graph.nodes.list[START_POINT_ID].link_id = 0;
    m_graph.nodes.list[START_POINT_ID].link_num = 1;
    
    m_graph.links.list[1].source = TARGET_POINT_ID;
    m_graph.links.list[1].target = TARGET_POINT_ID;
    m_graph.nodes.list[TARGET_POINT_ID].link_id = 1;
    m_graph.nodes.list[TARGET_POINT_ID].link_num = 1;
    
    //Compute the number of links to the node and the starting link index
    LinkId currentLinkId = 2;
    uint8_t nbLinks = 0;
    for(NodeId currentNodeId = TARGET_POINT_ID+1; currentNodeId < m_graph.nodes.count ; currentNodeId++)
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
            //Check link reciprocity (when source is smaller than target, search ahead)
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
            //Check link reciprocity (when source is bigger than target, search backward)
            else
            {
                bool found = false;
                for(int j = currentLinkId-1; 1 < j ; j--)
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
    for(LinkId linkId = TARGET_POINT_ID+1 ; linkId < m_graph.links.count ; linkId++)
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
    m_state.headings_count = 0;
    for(int i = 0; i < MAX_PATH_COUNT ; i++)
    {
        m_state.way[i] = 0;
        m_state.headings[i] = 0;
    }
    
    //m_info is always reset at computation time

    m_state.state = eGraphState_GS_IDLE;

    m_graph.links.list[0].target = TARGET_POINT_ID;
    m_graph.links.list[0].distance = 0;
    
    m_graph.links.list[1].source = START_POINT_ID;
    m_graph.links.list[1].distance = 0;
}

void Graph::setAllValid()
{
    for(int i = 0 ; i < m_graph.links.count ; i++)
    {
        m_state.valid[i] = true;
    }
}

NodeId Graph::setStartPoint(const Pose2D& source)
{
    //Update graph with source point position
    GraphNode* node = &m_graph.nodes.list[START_POINT_ID];
    node->x = source.x;
    node->y = source.y;
    m_state.startPoint.x = node->x;
    m_state.startPoint.y = node->y;
    m_state.startPoint.h = source.h;

    //compute entry point : the shortest graph point
    NodeId entry = getShortestNodeId(source, &m_graph.links.list[0].distance);
    LOG_INFO(String("    Entry point is : ") + (int)entry);

    //Update link list with start<->entry link
    m_graph.links.list[0].target = entry;
    m_state.valid[0] = true;
    
    return entry;
}


NodeId Graph::setTargetPoint(const Pose2D& target)
{
    //Update graph with source point position
    GraphNode* node = &m_graph.nodes.list[TARGET_POINT_ID];
    node->x = target.x;
    node->y = target.y;
    m_state.targetPoint.x = node->x;
    m_state.targetPoint.y = node->y;
    m_state.targetPoint.h = target.h;
    
    //compute entry point : the shortest graph point
    NodeId exit = getShortestNodeId(target, &m_graph.links.list[1].distance);
    LOG_INFO(String("    Exit point is : ") + (int)exit);
    
    //Update link list with start<->entry link
    m_graph.links.list[1].source = exit;
    m_state.valid[1] = true;
    
    return exit;
}

bool Graph::computeShortertPath(const Pose2D& source, const Pose2D& target, eDir sens)
{
    bool res = true;
    
    ASSERT_TEXT(m_state.state == eGraphState_GS_IDLE, "You should reset the graph before computing a new one");
    m_state.state = eGraphState_GS_COMPUTING;
    reset();
    
    DelayMs startCompute = millis();

    //Compute entry/exit point
    NodeId entry = setStartPoint(source);
    NodeId exit = setTargetPoint(target);

    //Compute path
    if(entry == exit )
    {
        res = true;
        setWayPoint(m_state.way_count++, START_POINT_ID);
        setWayPoint(m_state.way_count++, entry);
        setWayPoint(m_state.way_count++, TARGET_POINT_ID);

        m_state.state = eGraphState_GS_COMPUTED;
    }
    else
    {
        setWayPoint(m_state.way_count++, START_POINT_ID);
        res = computePathBetweenNodes(entry, exit);    
        setWayPoint(m_state.way_count++, TARGET_POINT_ID);
    }
    
    ASSERT(m_state.way_count <= MAX_PATH_COUNT);
    m_state.headings_count = m_state.way_count;

    //Display results
    DelayMs endCompute = millis();
    if(res)
        LOG_INFO(String("   --> computed in ") + String(endCompute - startCompute) + " ms.");

    return res;
}

bool Graph::computePathBetweenNodes(uint8_t idSource, uint8_t idTarget)
{
    ASSERT(idSource != idTarget);
    ASSERT_TEXT(m_state.state == eGraphState_GS_IDLE, "You have to reset the path before computing a new one.");
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
                uint16_t dist = m_info[i].distance
                        + m_graph.links.list[j].distance
                        + 200; //200 is a penalty to penalize manoeuvers TODO replace by motion duration calculation
                if( dist < m_info[connected_node].distance)
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
        //count items on path (one is already present before computation, and 2 are always found as we expect 2 different nodes)
    i = idTarget;
	uint8_t pathLength = 1; //there is at least the target point
    while(m_info[i].prev_node != idSource)
    {
        i = m_info[i].prev_node;
        ++pathLength;
    }
    ++pathLength;//count source
	
	//build path starting from path end
	j = pathLength-1;
	i = idTarget;
	setWayPoint(m_state.way_count + j , idTarget);
    while(m_info[i].prev_node != idSource)
    {
        i = m_info[i].prev_node;
        j--;
        setWayPoint(m_state.way_count + j, i);
    }
	setWayPoint(m_state.way_count, idSource);

    m_state.way_count+=pathLength;
    m_state.state = eGraphState_GS_COMPUTED;
    return true;
}

void Graph::optimizePath(eDir sens)
{
	ASSERT_TEXT(m_state.state == eGraphState_GS_COMPUTED, "You have to compute the path before optimizing it.");
	DelayMs startCompute = millis();

	//At this time there is at least 3 points in path (start - one point in graph - target)
	ASSERT(3 <= getWayPointNb());

	//If the 2 last waypoint are too close of each other, delete a waypoint
	if( getWayPoint(getWayPointNb()-2).distanceTo(getWayPoint(getWayPointNb()- 1)) <= PROXIMITY )
	{
		setWayPoint(getWayPointNb()-2, TARGET_POINT_ID);
		--m_state.way_count;
	}

	//At this there is at least 2 points in path (as one may have been remove in previous step)
	//When there is 2 way points there is nothing to do, it's the simpliest path start-target
	//If there is more waypoints points and the 2 first waypoints are close to each other, then
	//remove a waypoint (and so shift left all other waypoint)
	if(3 <= getWayPointNb() && getWayPoint(0).distanceTo(getWayPoint(1)) <= PROXIMITY)
	{
		for(uint8_t i = 1; i < getWayPointNb() ; ++i)
		{
			setWayPoint(i-1, getWayPointId(i));
		}
		--m_state.way_count;
	}

	//Initialize heading computation with last point heading on target heading
	m_state.headings[getWayPointNb()-1] = m_state.targetPoint.h;

	//Beginning with path end, compute each waypoint heading
    if(2 < getWayPointNb())
    {
		if( sens == eDir_BEST )
			sens = eDir_FORWARD;//TODO do the real job

		for( uint8_t i = getWayPointNb()-2 ; i != START_POINT_ID ; i--)
			m_state.headings[i] = headingToDir(getWayPoint(i).angleTo(getWayPoint(i+1)), sens);
		m_state.headings[START_POINT_ID] = m_state.startPoint.h;
    }

	// affichage debug
	for(uint8_t i=0; i < m_state.way_count; i++)
	{
		String s = String("    graph path : [" + String(i) + "] : id=" + int(m_state.way[i]-1) + " p=" + getWayPoint(i).toString());
		LOG_INFO(s);
	}

    //Display results
    DelayMs endCompute = millis();
    LOG_INFO(String("   --> optimized in ") + String(endCompute - startCompute) + " ms.");
}

NodeId Graph::getShortestNodeId(Point pos, uint16_t* distance)
{
    uint8_t i;
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

    if(distance)
        *distance = shortestDist;

    return shortestId;
}

NodeId Graph::getWayPointId(uint8_t rank) const
{
    ASSERT_TEXT(rank < m_state.way_count, "You requested a waypoint which is not in the list");
    ASSERT_TEXT(m_state.state != eGraphState_GS_IDLE, "You request a path without having computed it");
    //-1 because data is stocked as a string
    return m_state.way[rank]-1;
}

void Graph::setWayPoint(uint8_t rank, NodeId newId)
{
    ASSERT_TEXT(rank < MAX_PATH_COUNT, "Too many point in path");
    m_state.way[rank] = newId+1;
}

Pose2D Graph::getWayPoint(uint8_t rank) const
{
    Point waypoint = getNode(getWayPointId(rank));
    return Pose2D(waypoint.x, waypoint.y, degrees(m_state.headings[rank]));
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

