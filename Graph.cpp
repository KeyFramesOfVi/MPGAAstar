//
//  Graph.cpp
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//

#include "Graph.h"

Graph::Graph(Map * m, int p = 30): proximity(p), map(m) {
    
    // If p is given, but 0
    if ( p == 0 )
        this->proximity = 30;
    
    generateNavGraph();
}

/*
 * return a node with the given index or return one with invalid_index if not found.
 */
Node Graph::getNode(int n){
    return (*(nodes.at(n)));
}
/*
 * returns pointer of the Node/returns address
 */
Node* Graph::getNodePtr(int n){
    return nodes.at(n);
}

/*
 * return an edge with the given indexes of nodes or return one with invalid indexes if not found
 */
Edge* Graph::getEdge(int n1, int n2) {
    vector<Edge*>::iterator eiter;
    Node nd1 = getNode(n1);
    Node nd2 = getNode(n2);
    if (! (nd1.getID() == Node::INVALID_NODE_INDEX || nd2.getID() == Node::INVALID_NODE_INDEX )){
        for( eiter = edges.begin(); eiter != edges.end(); eiter++ ){
            if ( n1 == (*eiter)->getFrom() && n2 == (*eiter)->getTo() )
                return (*eiter);
            if ( n1 == (*eiter)->getTo() && n2 == (*eiter)->getFrom() )
                return (*eiter);
        }
    }
    Edge * e0 = new Edge(Node::INVALID_NODE_INDEX, Node::INVALID_NODE_INDEX);
    return e0;
}

/*
 * returns true if the node n is in the Graph::nodes used by PathPlanner to check if the source/target is a valid node in Graph
 */
bool Graph::isNode(Node n) {
    vector<Node*>::iterator iter;
    for( iter = nodes.begin(); iter != nodes.end(); iter++ )
        if ( (*(*iter)) == n )
            return true;
    return false;
}

/*
 * returns true if the edge exists in the Node::nodeEdges of "From" node
 */
bool Graph::isEdge(Edge e) {
    vector<Edge*> edges = getNode(e.getFrom()).getNodeEdges();
    vector<Edge*>::iterator iter;
    for ( iter = edges.begin() ; iter != edges.end() ; iter++ ) {
        if ( e == (*(*iter)) )
            return true;
        Edge en((*iter)->getTo(), (*iter)->getFrom(), (*iter)->getCost());
        if ( en == e )
            return true;
    }
    return false;
}

/*
 * Generates Graph that will be worked with
 */
void Graph::generateNavGraph() {
    int l = map->getLength();
    int h = map->getHeight();
    
    int xExcess = l % proximity;
    int yExcess = h % proximity;
    
    if ( xExcess == 0 )
        xExcess = proximity;
    if ( yExcess == 0 )
        yExcess = proximity;
    
    // create nodes
    int index = 0;
    for( int x = xExcess/2; x < l; x += proximity ){
        for( int y = yExcess/2; y < h; y += proximity ){
            bool inBuf = false;
            if ( map->isPointInBuffer(x,y) )
                inBuf = true;
            Node * n = new Node(index, x, y, inBuf);
            nodes.push_back(n);
            index++;
        }
    }
    
    //Creates ints that traverse the graph vertically and horizontally, will be used to emulate sensors
    vertical = ceil(static_cast<double>(h) / proximity);
    horizontal = 1;
    
    // update node neighbors
    populateNodeNeighbors();
    
    // create edges
    vector<Node*>::iterator iter;
    for( iter = nodes.begin(); iter != nodes.end(); iter++ ){
        
        vector<int> nbrs = getNeighbors(*(*iter));
        vector<int>::iterator it;
        
        for( it = nbrs.begin(); it != nbrs.end(); it++ ){
            Edge * e = new Edge( (*iter)->getID(), getNode(*it).getID() );
            
            if ( !isEdge((*e)) ) {
                
                double distCost = Utils::get_euclidian_distance((*iter)->getX(), (*iter)->getY(),
                                                                getNode(*it).getX(), getNode(*it).getY());
                int multiplier = 1;
                if ( (*iter)->getInBuffer() )
                    multiplier += 2;
                if ( getNode(*it).getInBuffer() )
                    multiplier += 2;
                if ( (*iter)->getX() == getNode(*it).getX() || (*iter)->getY() == getNode(*it).getY() ){
                    e->setCost(multiplier * distCost);
                }
                else{
                    e->setCost(multiplier * distCost);
                }
                
                edges.push_back(e);
                
                (*iter)->addNodeEdge(e);
                nodes.at(*it)->addNodeEdge(e);
            }
        }
    }
}

/*
 *  Updates the cost of edges as the graph learns the map
 */
double Graph::updateCost(int from, int to){
    Node current = getNode(from);
    Node neighbor = getNode(to);
    if ( map->isPointInBuffer(current.getX(),current.getY())){
        current.setInBuffer(true);
    }
    if ( map->isPointInBuffer(neighbor.getX(), neighbor.getY())){
        neighbor.setInBuffer(true);
    }
    Edge * e = getEdge(from, to);
    
    double dist_cost = Utils::get_euclidian_distance(current.getX(), current.getY(), neighbor.getX(), neighbor.getY());
    int multiplier = 1;
    if (current.getInBuffer()){
        multiplier += 2;
    }
    if (neighbor.getInBuffer()){
        multiplier += 2;
    }
    if (current.getX() == neighbor.getX() || current.getY() == neighbor.getY())
        e->setCost(multiplier * dist_cost);
    else
        e->setCost(multiplier * dist_cost);
    
    return (multiplier * dist_cost);
}

/*
 * Used to pick the nodes in a circular area to disable edges
 */
vector<Node*> Graph::getNodesInRegion( int x, int y, double dist ) {
    vector<Node*> nodesInRegion;
    vector<Node*>::iterator iter;
    for(iter = nodes.begin(); iter != nodes.end(); iter++ ){
        int iterx = (*iter)->getX();
        int itery = (*iter)->getY();
        if( iterx - dist <= x )
            if( iterx + dist >= x )
                if( itery - dist <= y )
                    if( itery + dist >= y )
                        if ( Utils::get_euclidian_distance( x, y, iterx, itery) <= dist ) {
                            nodesInRegion.push_back(*iter);
                        }
    }
    return nodesInRegion;
}

vector<Node*> Graph::getNodesInRegion( int x1, int y1, int x2, int y2 ){
    vector<Node*> nodesInRegion;
    vector<Node*>::iterator iter;
    for ( iter = nodes.begin(); iter != nodes.end(); iter++ ) {
        int iterx = (*iter)->getX();
        int itery = (*iter)->getY();
        if ( iterx >= x1 && iterx <= x2 && itery >= y2 && itery <= y1 )
            nodesInRegion.push_back(*iter);
    }
    return nodesInRegion;
}

void Graph::clearGraph() {
    vector<Node*>::iterator itr;
    for(itr = nodes.begin(); itr != nodes.end(); itr++) {
        (*itr)->setAccessible(true);
    }
}
/*
 * Adds obstacle by making the accessible false
 */
void Graph::addObstacle(int x1, int y1, int x2, int y2){
    toggleObstacle(false, x1, y1, x2, y2);
}
/*
 *Removes obstacle by making accessible true
 */
void Graph::removeObstacle(int x1, int y1, int x2, int y2){
    toggleObstacle(true, x1, y1, x2, y2);
}
/*
 * Sets the accessible, given the bool
 */
void Graph::toggleObstacle(bool access, int x1, int y1, int x2, int y2){
    assert( x1 <= x2 && y1 >= y2 );
    
    int buffer = ceil(proximity / 2);
    vector<Node*> roi_nodes = getNodesInRegion( (x1 - buffer), (y1 + buffer),
                                               (x2 + buffer), (y2 - buffer) );
    
    vector<Node*>::iterator iter;
    for ( iter = roi_nodes.begin(); iter != roi_nodes.end(); iter++ ) {
        (*iter)->setAccessible(access);
        
        if ( !access ) {
            obstacles.push_back((*iter)->getID()) ;
        }
        else {
            vector<int>::iterator o_iter;
            for( o_iter = obstacles.begin(); o_iter != obstacles.end(); o_iter++ ){
                if( (*o_iter) == (*iter)->getID() ) {
                    obstacles.erase(o_iter);
                    break;
                }
            }
        }
    }
}

void Graph::addObstacle(int x, int y, double dist) {
    vector<Node*> roiNodes = getNodesInRegion(x,y,dist);
    vector<Node*>::iterator iter;
    for ( iter = roiNodes.begin(); iter != roiNodes.end(); iter++ ) {
        obstacles.push_back((*iter)->getID());
        (*iter)->setAccessible(false);
    }
}

void Graph::removeObstacle(int x, int y, double dist) {
    vector<Node*> roiNodes = getNodesInRegion(x,y,dist);
    vector<Node*>::iterator iter;
    for(iter = roiNodes.begin(); iter != roiNodes.end(); iter++) {
        (*iter)->setAccessible(true);
        
        vector<int>::iterator it;
        for(it = obstacles.begin(); it != obstacles.end(); it++) {
            if((*it) == (*iter)->getID()) {
                obstacles.erase(it);
                break;
            }
        }
    }
}

void Graph::removeAllObstacles(){
    vector<int>::iterator o_iter;
    for( o_iter = obstacles.begin(); o_iter != obstacles.end(); o_iter++ ){
        getNodePtr(*o_iter)->setAccessible(true);
    }
    obstacles.clear();
}

void Graph::addTempObstacle(int x1, int y1, int x2, int y2){
    toggleTempObstacle(false, x1, y1, x2, y2);
}

void Graph::removeTempObstacle(int x1, int y1, int x2, int y2){
    toggleTempObstacle(true, x1, y1, x2, y2);
}

void Graph::toggleTempObstacle(bool access, int x1, int y1, int x2, int y2){
    assert( x1 <= x2 && y1 >= y2 );
    
    int buffer = ceil(proximity / 2);
    vector<Node*> roi_nodes = getNodesInRegion( (x1 - buffer), (y1 + buffer),
                                               (x2 + buffer), (y2 - buffer) );
    
    vector<Node*>::iterator iter;
    for ( iter = roi_nodes.begin(); iter != roi_nodes.end(); iter++ ) {
        
        vector<Edge*> nodeEdges = (*iter)->getNodeEdges();
        vector<Edge*>::iterator e_itr;
        
        if ( !access ) {
            for ( e_itr = nodeEdges.begin(); e_itr != nodeEdges.end(); e_itr++ ) {
                (*e_itr)->setTempCost(2 * (*e_itr)->getCost());
            }
            tempObstacles.push_back((*iter)->getID()) ;
        }
        else {
            for ( e_itr = nodeEdges.begin(); e_itr != nodeEdges.end(); e_itr++ ) {
                (*e_itr)->setTempCost(0);
            }
            vector<int>::iterator o_iter;
            for( o_iter = tempObstacles.begin(); o_iter != tempObstacles.end(); o_iter++ ){
                if( (*o_iter) == (*iter)->getID() ) {
                    tempObstacles.erase(o_iter);
                    break;
                }
            }
        }
    }
}

void Graph::addTempObstacle(int x, int y, double dist){
    vector<Node*> roiNodes = getNodesInRegion(x,y,dist);
    vector<Node*>::iterator iter;
    for ( iter = roiNodes.begin(); iter != roiNodes.end(); iter++ ) {
        
        vector<Edge*> nodeEdges = (*iter)->getNodeEdges();
        vector<Edge*>::iterator e_itr;
        for ( e_itr = nodeEdges.begin(); e_itr != nodeEdges.end(); e_itr++ ) {
            (*e_itr)->setTempCost(2 * (*e_itr)->getCost());
        }
        
        tempObstacles.push_back((*iter)->getID());
    }
}

void Graph::removeTempObstacle(int x, int y, double dist){
    vector<Node*> roiNodes = getNodesInRegion(x,y,dist);
    vector<Node*>::iterator iter;
    for ( iter = roiNodes.begin(); iter != roiNodes.end(); iter++ ) {
        
        vector<Edge*> nodeEdges = (*iter)->getNodeEdges();
        vector<Edge*>::iterator e_itr;
        for ( e_itr = nodeEdges.begin(); e_itr != nodeEdges.end(); e_itr++ ) {
            (*e_itr)->setTempCost(0);
        }
        
        vector<int>::iterator o_iter;
        for( o_iter = tempObstacles.begin(); o_iter != tempObstacles.end(); o_iter++ ){
            if( (*o_iter) == (*iter)->getID() ) {
                tempObstacles.erase(o_iter);
                break;
            }
        }
    }
}

void Graph::removeAllTempObstacles(){
    vector<int>::iterator o_iter;
    for( o_iter = tempObstacles.begin(); o_iter != tempObstacles.end(); o_iter++ ){
        vector<Edge*> nodeEdges = getNode(*o_iter).getNodeEdges();
        vector<Edge*>::iterator e_itr;
        for ( e_itr = nodeEdges.begin(); e_itr != nodeEdges.end(); e_itr++ ) {
            (*e_itr)->setTempCost(0);
        }
    }
    tempObstacles.clear();
}

vector<int> Graph::getNeighbors(Node n){
    return n.getNeighbors();
}

void Graph::populateNodeNeighbors(){
    vector<Node*>::iterator iter;
    for ( iter = nodes.begin(); iter != nodes.end(); iter++ ) {
        for( int x = (*iter)->getX()-proximity; x <= (*iter)->getX()+proximity; x += proximity )
            for( int y = (*iter)->getY()-proximity; y <= (*iter)->getY()+proximity; y += proximity )
                if( ! ( x == (*iter)->getX() && y == (*iter)->getY() ) )
                    if ( map->isWithinBorders(x, y) &&  !map->isPathObstructed((*iter)->getX(), (*iter)->getY(), x, y) ){
                        (*iter)->addNeighbor(getNodeID(x,y));
                    }
    }
}

int Graph::getNodeID(int x, int y) {
    vector<Node*>::iterator iter;
    for( iter = nodes.begin(); iter != nodes.end(); iter++ ){
        if ( (*iter)->getX() == x && (*iter)->getY() == y ) {
            return (*iter)->getID();
        }
    }
    return -1 ;
}

double Graph::calcCost(Node n1, Node n2){
    return Utils::get_euclidian_distance(n1.getX(), n1.getY(), n2.getX(), n2.getY());
}

void Graph::printGraph() {
    cout << "Printing graph..." << endl ;
    vector<Edge*>::iterator iter;
    for( iter = edges.begin(); iter != edges.end(); iter++ )
        (*iter)->printEdge();
    cout << "Total number of nodes: " << numNodes() << ", number of edges: " << numEdges() << endl;
}

