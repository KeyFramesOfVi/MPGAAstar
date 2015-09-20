//
//  MPGAAstar.h
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//
#ifndef ____MPGAAstar__
#define ____MPGAAstar__

#include <iostream>
#include "Graph.h"
#include <queue>
#include <list>
#include <cmath>
#include <climits>

const int VISION_SENSOR = 120;


class MPGAAstar {
private:
    class _Compare; // prototype
    class _VNode; //prototype
    Graph *graph;
    _VNode *start, *goal;
    vector<_VNode*> vnodes; //Holds every node as a virtual node
    
    vector<_VNode*> closed;
    
    // Private funcs
    double euclidian_h(_VNode*, _VNode*);
    double octile_h(_VNode*, _VNode*);
    // Update open list
    void push_update_f(priority_queue<_VNode*, vector<_VNode*>, _Compare> &, _VNode*);
    void push_update_h(priority_queue<_VNode*, vector<_VNode*>, _Compare> &, _VNode*);
    //Constructs a path from arg to start by tracing backwards until it finds a _VNode with a null 'parent' pointer
    void buildPath(int);
    
    //Some statics since we don't want to compute these on the fly
    static double small_cost; // Min c(s,s')
    static double diag_cost; // sqrt(2*c(s,s')^2)
    static int counter;
    
    // Virtual Node Struct: hold MPGAAstar specific node data
    class _VNode{
    public:
        bool accessible;    // coped from the Node
        int id,             // copied from the Node
        x, y,             // copied from the Node
        search;           // returns number of the last search that Node was generated, 0 if never
        double g,           // distance from start
        h,                // heuristic value
        f,                  // g + h
        priority;                  // current priority to compare for open listzß
        _VNode* parent;       // best predecessor to move from, used to built the path
        _VNode* next;
        _VNode* support;
        
        // Make a _VNode from a Node
        _VNode(){
            id = x = y = g = priority = h = search = 0;
            this->accessible = false;
            parent = nullptr;
            next = nullptr;
        }
        // Make a _VNode from a Node
        _VNode(Node n){
            this->id = n.getID();
            this->x = n.getX();
            this->y = n.getY();
            this->accessible = n.isAccessible();
            g = priority = search = 0;
            parent = nullptr;
            next = nullptr;
        }
    };

    class _Compare{
        //Returns true if first node has a smaller f value
    public:
        bool operator() (MPGAAstar::_VNode*, MPGAAstar::_VNode*);
    };
    priority_queue<_VNode*, vector<_VNode*>, _Compare> open;
    
    
public:
    list<int> path;
    
    MPGAAstar(Graph*);
    MPGAAstar(Graph, Node&, Node&);
    
    // Search the graph for a path and return true if found
    int Astar();
    
    //Wrappers
    bool goalCondition(_VNode* current) {
        while((current->next  != nullptr) && (current->h == current->next->h + graph->getNode(current->id).getCostTo(current->next->id)))
            current = current->next;
        return ( current->id == goal->id );
    }
    list<int> getPathToTarget() { return path; }
    void initializeState(_VNode*);
    void initializeVNodes();
    void insertState(_VNode*, _VNode*, priority_queue<_VNode*, vector<_VNode*>, _Compare> &);
    bool Observe(_VNode*);
    void reestablishConsistency(vector<pair<int,int>>);
    bool createPath(int, int);
    void printPath();
    //Embedded comparator class
};

#endif /* defined(____MPGAAstar__) */
