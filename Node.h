//
//  Node.h
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//

#ifndef MPGAAstar_Node_h
#define MPGAAstar_Node_h

#include "Edge.h"
#include <iostream>
#include <vector>
using namespace std;

class Node {
protected:
    
    int id; //ID of the Node
    
    int x, y;
    
    bool inBuffer; // true if within a wall buffer ( too close to a wall )
    
    bool accessible; //false if the edges of this node are temporarily disabled.
    
    vector<int> neighbors; // this is the adjacency list of the node. Populated from Graph.
    
    vector<Edge*> nodeEdges; //vector of edges that goes out from this node ( or comes in if not digraph )
    
public:
    //Constructor with initial values
    Node(int i = INVALID_NODE_INDEX, int xInit = 0, int yInit = 0, bool buffInit = false)
    : id(i), x(xInit), y(yInit), inBuffer(buffInit), accessible(true) {}
    //Compares to see if two Nodes are the same
    bool operator == (const Node& n) const{
        return (this->id == n.getID() && this->x == n.getX() && this->y == n.getY() );
    }
    
    //Checks to see if two Nodes don't have the same ID
    bool operator != (const Node& n) const{
        return ( this->id != n.getID() );
    }
    
    //Checks to see if the ID of one Node is less than the other on the rhs
    bool operator<(const Node& n) const{
        return ( this->id < n.getID() );
    }
    
    //Sets the ID
    void setID(int i) { id = i; }
    //Returns ID
    int getID() const { return id; }
    //Sets x value of Node
    void setX(int x) { this->x = x; }
    //Returns x value of Node
    int getX() const { return x; }
    //Sets y value of Node
    void setY(int y) { this->y = y; }
    //Returns y value of Node
    int getY() const { return y; }
    //Checks if state is too close to a wall
    bool getInBuffer() const { return inBuffer; }
    void setInBuffer(bool currBuffer) { inBuffer = currBuffer; }
    //DEBUG: Prints contents of Node
    void printNode() const{
        cout << "<NODE: " << id << " :( " << x << " ," << y << ") >" << endl;
    }
    
    //DEBUG: Prints neighbors as well as their locations
    void printNeighbors() const{
        cout << "Node " << id << " neighbors: " ;
        for ( unsigned int i = 0; i < neighbors.size(); i++ ) {
            cout << neighbors.at(i) << "\t" ;
        }
        cout << endl;
    }
    //DEBUG: Prints edges of every neighbor, from this Node to that neighbor*/
    void printNodeEdges() const {
        cout << "Node " << id << " nodeEdges: " << endl;
        for ( unsigned int i = 0 ; i < nodeEdges.size(); i++ )
            nodeEdges.at(i)->printEdge();
    }
    bool isAccessible() { return accessible; }
    //Sets every Edge that the Node can traverse to usable
    void setAccessible(bool b) {
        accessible = b;
        vector<Edge*>::iterator iter;
        for( iter = nodeEdges.begin(); iter != nodeEdges.end(); iter++ )
            (*iter)->setUsable(b);
    }
    //Adds a Neighbor to the Node
    void addNeighbor(int i) { neighbors.push_back(i); }
    //Returns neighbors
    vector<int> getNeighbors() { return neighbors; }
    //Adds a NodeEdge
    void addNodeEdge(Edge* e) { nodeEdges.push_back(e); }
    //Returns Node Edges
    vector<Edge*>& getNodeEdges() { return nodeEdges; }
    //Returns all of the NodeEdges that are usable currently
    vector<Edge> getUsableNodeEdges() {
        vector<Edge> e;
        vector<Edge*>::iterator iter;
        for ( iter = nodeEdges.begin() ; iter != nodeEdges.end() ; iter++ )
            if ( (*iter)->isUsable() )
                e.push_back(*(*iter));
        return e;
    }
    //Returns the size of the neighbors*/
    int numNeighbors() const { return (int)neighbors.size(); }
    //Looks through every neighbor that has the same id as the Node n*/
    bool isNeighbor(Node n) {
        vector<int>::iterator iter;
        for( iter = neighbors.begin(); iter != neighbors.end(); iter++ )
            if ( *iter == n.getID() )
                return true;
        return false;
    }
    //Checks to see if the neighbor list is empty*/
    bool neighborEmpty(){ return ( neighbors.size() == 0 ); }
    //Retrieves cost from temp with the same id
    double getCostTo(int nid){
        double cost = 0;
        
        vector<Edge*>::iterator it;
        for(it = nodeEdges.begin(); it != nodeEdges.end(); it++){
            Edge* tmp = *it;
            if(tmp->getTo() == this->id && tmp->getFrom() == nid){
                cost = tmp->getCost();
            }
            else if(tmp->getFrom() == this->id && tmp->getTo() == nid)
            {
                cost = tmp->getCost();
            }
        }
        return cost;
    }
    static const int INVALID_NODE_INDEX = -1;
};

#endif
