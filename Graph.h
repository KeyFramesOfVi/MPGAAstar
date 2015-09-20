//
//  Graph.h
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//

#ifndef __MPGAAstar__Graph__
#define __MPGAAstar__Graph__

#include "Node.h"
#include "Edge.h"
#include <vector>
#include <queue>
#include <math.h>
#include "Map.h"
#include <assert.h>

const int VIEW_BUFFER = 120;

class Graph {
private:
    vector<Node*> nodes; //Vector listing of all the Nodes
    vector<Edge*> edges; //Vector listing of all of the Edges
    
    int proximity; // proximity between 2 nodes ( in cm )
    
    Map * map ; //The Map drawn for this
    
    void generateNavGraph(); //Generates a Graph
    
    vector<int> obstacles;     // this keeps a list of nodes which the edges around them are temporarily disabled (not usable)
    
    vector<int> tempObstacles;     // this keeps a list of nodes which the edges around them are temporarily disabled (not usable)
    
    //Sets Obstacles in Map to true or false
    void toggleObstacle(bool b, int x1, int y1, int x2, int y2);
    
    //Sets Temporary Obstacles in Map to true or false
    void toggleTempObstacle(bool access, int x1, int y1, int x2, int y2);
    
    //True if an edge on the graph
    bool isEdge(Edge e);
    
    //Allows us to traverse the map up, down, left, or right a node. Will be used to create a sensor
    int vertical, horizontal;
    
public:
    //Constructor that takes a Map of any size, as well as a Buffer
    Graph(Map*, int);
    //Retrieves the Map in the Graph
    Map* getMap() { return map; }
    void setMap(Map* newMap) { map = nullptr; map = newMap; }
    //Returns proximity between two nodes
    int getProximity() const { return proximity; }
    //Returns nodes
    vector<Node*> getNodes() const { return nodes; }
    //Returns edges
    vector<Edge*> getEdges() const { return edges; }
    
    double updateCost(int, int);
    // this function is used to retreive node id that is at (x,y). Used during populating neigbors of nodes,
    // during construction of navGraph
    int getNodeID(int x, int y);
    
    int getVertical() const { return vertical; }
    int getHorizontal() const { return horizontal; }
    //Retrieves node using ID
    Node getNode(int n);
    //Retrieves Node pointer
    Node* getNodePtr(int i);
    //Retrieves edge using two IDs (from and to)
    Edge* getEdge(int n1, int n2);
    //! returns the neigbors of the node with index n. Calls directly Node::getNeighbors
    vector<int> getNeighbors(Node n);
    //! populates the graph with nodes and assigns their immediate neighbors
    void populateNodeNeighbors();
    //Returns true if a Node
    bool isNode(Node n);
    // Returns total number of states
    int numNodes() const { return (int)nodes.size(); }
    // Returns total number of edges
    int numEdges() const { return (int)edges.size(); }
    // Prints the graph
    void printGraph() ;
    // Retrieves Nodes in current region
    vector<Node*> getNodesInRegion( int x, int y, double r);
    vector<Node*> getNodesInRegion( int x1, int y1, int x2, int y2);
    
    //! sets all the nodes in the graph as accessible
    void clearGraph();
    
    //Adds an Obstacle
    void addObstacle(int x1, int y1, int x2, int y2);
    //Removes an Obstacle
    void removeObstacle(int x1, int y1, int x2, int y2);
    //Adds an Obstacle given the distance
    void addObstacle(int x, int y, double dist);
    //Removes an Obstacle given the distance
    void removeObstacle(int x, int y, double dist);
    //Removes all Obstales
    void removeAllObstacles();
    //Retrieves obstacles currently on the graph
    vector<int> getObstacles() { return obstacles; }
    //Adds a temporary Obstacle
    void addTempObstacle(int x1, int y1, int x2, int y2);
    //Removes a temporary Obstacle
    void removeTempObstacle(int x1, int y1, int x2, int y2);
    //Adds a temporary Obstacle given the distance
    void addTempObstacle(int x, int y, double dist);
    //Removes a temporary Obstacle given the distance
    void removeTempObstacle(int x, int y, double dist);
    //Removes all temporary Obstacles
    void removeAllTempObstacles();
    //Retrieves the list of temp obstacles
    vector<int> getTempObstacles() { return tempObstacles; }
    //Given two nodes, calculates the cost between them if there is an edge
    double calcEdgeCost(Node, Node);
    //Given two nodes, calculates the cost
    double calcCost(Node, Node);
};

#endif /* defined(__MPGAAstar__Graph__) */
