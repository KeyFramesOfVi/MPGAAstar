//
//  Edge.h
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//

#ifndef MPGAAstar_Edge_h
#define MPGAAstar_Edge_h

#include <vector>
#include <iostream>
using namespace std;

class Edge {
private:
    int from, to; // the integers that represent the IDs of the Nodes to associate a link
    bool usable; //true if the path is usable
    
protected:
    double cost, tempCost;
    
public:
    //Initial Constructor
    Edge(): from(-1), to(-1), cost(0), tempCost(0), usable(true){}
    //Constructor with parameters
    Edge(int n1, int n2, double c = 0): from(n1), to(n2), cost(c), usable(true){}
    // Checks if two edges are the same
    bool operator == (Edge e) {
        int eFrom = e.getFrom();
        int eTo = e.getTo();
        return (from == eFrom && to == eTo);
    }
    // Returns the total cost
    double getCost() { return cost + tempCost; }
    // Sets the cost
    void setCost(double c) { cost = c; }
    // Sets the temporary cost
    void setTempCost(double tc) { tempCost = tc; }
    // Returns the temporary cost
    double getTempCost() { return tempCost; } // Retrieves temporary cost
    // Returns the ID of the edge's first segment  node
    int getFrom() { return from; }
    // Returns the ID of the edge's second segment node
    int getTo() { return to; }
    // Toggles the use of the edge
    void setUsable(bool b) { usable = b; }
    // Returns boolean value
    bool isUsable() { return usable; }
    // Prints out edge information, DEBUG purposes
    void printEdge() const{
        cout << "<EDGE-From Node: " << from
        << " -To Node: " << to
        << " -Cost: " << cost
        << " -TempCost: " << tempCost << " > " << endl;
    }
};


#endif
