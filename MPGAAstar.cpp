//
//  MPGAAstar.cpp
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//

#include "MPGAAstar.h"

double MPGAAstar::small_cost = 1;
double MPGAAstar::diag_cost = sqrt(2 * small_cost * small_cost);
int MPGAAstar::counter = 0;

double MPGAAstar::euclidian_h(_VNode* a, _VNode* b){
    return sqrt( (a->x - b->x) * (a->x - b->x) +
                (a->y - b->y) * (a->y - b->y) );
}

double MPGAAstar::octile_h(_VNode* a, _VNode* b){
    double dx, dy;
    dx = abs(a->x - b->x);
    dy = abs(a->y - b->y);
    return min(dx,dy) * diag_cost + (max(dx,dy) - min(dx, dy)) * small_cost;
}

MPGAAstar::MPGAAstar(Graph *g){
    this->graph = g;
    this->path.clear();
}

MPGAAstar::MPGAAstar(Graph g, Node &startNode, Node &goalNode){
    this->graph = &g;
    this->path.clear();
    if(createPath(startNode.getID(), goalNode.getID()))
        printPath();
    else
        cout << "Goal is not reachable." << endl;
}

/*
 * Initializes state for the first time if it hasn't been initialized with respect
 * to the counter. Then updates the VNode's last search with the current counter to keep
 * it updated.
 */
void MPGAAstar::initializeState(_VNode* s){
    if (s->search != counter)
        s->g = INT_MAX;
    s->search = counter;
}

/*
 * push update is an algorithm that checks if the robot's current state is in the open list.
 * If it is, it replaces that state with a state that updates the cost, if the new cost is smaller.
 * If it isn't, it simply is added into the open list.
 */
void MPGAAstar::push_update_f(priority_queue<_VNode*, vector<_VNode*>, _Compare> &pq, _VNode* n)
{
    list<_VNode*> l; // Hold all node pointers temporarily
    bool found = false;
    
    while(!pq.empty())
    {
        _VNode* tmp = pq.top();
        pq.pop();
        if(tmp->id == n->id)
        {
            found = true;
            if(tmp->f < n->f)
                pq.push(tmp);
            else
                pq.push(n);
            break;
        }
        
        l.push_back(tmp);
    }
    while(!l.empty())
    {
        pq.push(l.front());
        l.pop_front();
    }
    if(!found)
        pq.push(n);
    
    return;
}

void MPGAAstar::push_update_h(priority_queue<_VNode*, vector<_VNode*>, _Compare> &pq, _VNode* n)
{
    list<_VNode*> l; // Hold all node pointers temporarily
    bool found = false;
    
    while(!pq.empty())
    {
        _VNode* tmp = pq.top();
        pq.pop();
        if(tmp->id == n->id)
        {
            found = true;
            if(tmp->h < n->h)
                pq.push(tmp);
            else
                pq.push(n);
            break;
        }
        
        l.push_back(tmp);
    }
    while(!l.empty())
    {
        pq.push(l.front());
        l.pop_front();
    }
    if(!found)
        pq.push(n);
    
    return;
}


/*
 * Returns true if left hand side has a lower f value than right hand side
 */
bool MPGAAstar::_Compare::operator() (_VNode* lhs, _VNode* rhs){
    if(lhs->priority > rhs->priority)
        return true;
    if(lhs->priority < rhs->priority)
        return false;
    if(lhs->f > rhs->f)
        return true;
    if(lhs->g > rhs->g)
        return true;
    if(lhs->h > rhs->h)
        return true;
    return false;
}

/*
 * Classic A* Algorithm, computes optimal path from the start to the goal
 */
int MPGAAstar::Astar(){
    initializeState(start);
    start->g = 0;
    start->h = octile_h(start,goal);
    start->f = start->priority = (start->g + start->h);
    //Makes the open list empty
    open = priority_queue<_VNode*, vector<_VNode*>, _Compare>();
    open.push(start);
    closed.empty();
    while (!open.empty()){
        _VNode* current = open.top(); open.pop(); // Get and remove the top of the open list
        if(goalCondition(current)){
            return current->id;
        }
        closed.push_back(current);
        vector<int> neighbors = graph->getNode(current->id).getNeighbors();
        for (uint i = 0; i < neighbors.size(); i++){
            _VNode* curr_neighbor = vnodes[(graph->getNode(neighbors[i])).getID()];
            initializeState(curr_neighbor);
            if(curr_neighbor->g > current->g + graph->getNode(current->id).getCostTo(curr_neighbor->id)){
                curr_neighbor->g = current->g + graph->getNode(current->id).getCostTo(curr_neighbor->id);
                curr_neighbor->h = octile_h(curr_neighbor, goal);
                curr_neighbor->f = curr_neighbor->priority = (curr_neighbor->g + curr_neighbor->h);
                curr_neighbor->parent = current;
                push_update_f(open, curr_neighbor);
            }
        }
    }
    return Node::INVALID_NODE_INDEX;
}

/*
 * Builds the path backwards using parent Nodes until it reaches the start
 */
void MPGAAstar::buildPath(int curr){
    _VNode* s = vnodes[curr];
    while(s != start){
        s->parent->next = s;
        s = s->parent;
    }
}

/*
 * Updates heuristic values of states
 */
void MPGAAstar::insertState(_VNode* curr, _VNode* next, priority_queue<_VNode*, vector<_VNode*>, _Compare> & pq){
    if(curr->h > graph->getNode(curr->id).getCostTo(next->id) + next->h){
        curr->h = curr->priority = (graph->getNode(curr->id).getCostTo(next->id) + next->h);
        curr->next = nullptr;
        curr->support = next;
        push_update_h(pq, curr);
    }
}
bool MPGAAstar::Observe(_VNode* s){
    const int SENSOR_RANGE = VISION_SENSOR / graph->getProximity();
    vector<pair<int,int>> decreased_edges;
    _VNode* current;
    double new_cost, old_cost;
    bool cost_change = false;
    for (int y = 0; y <= SENSOR_RANGE; y++){
        for (int x = 0; x <= SENSOR_RANGE; x++){
            if (vnodes[s->id+(y*graph->getVertical())+(x*graph->getHorizontal())] != nullptr){
                current = vnodes[s->id+(y*graph->getVertical())+(x*graph->getHorizontal())];
                vector<int> curr_neighbors = graph->getNode(current->id).getNeighbors();
                vector<int>::iterator iter;
                for(iter = curr_neighbors.begin(); iter != curr_neighbors.end(); iter++){
                    //if(!graph->getMap()->isPathObstructed(current->x, current->y, graph->getNode(*iter).getX(), graph->getNode(*iter).getY())){
                        old_cost = graph->getEdge(current->id,graph->getNode(*iter).getID())->getCost();
                        new_cost = graph->updateCost(current->id,graph->getNode(*iter).getID());
                        if(new_cost < old_cost){
                            decreased_edges.push_back(std::make_pair(current->id,graph->getNode(*iter).getID()));
                            cost_change = true;
                        }
                        else if(old_cost < new_cost)
                            cost_change = true;
                    //}
                }
            }
        }
    }
    if(decreased_edges.size() != 0){
        s->next = nullptr;
        reestablishConsistency(decreased_edges);
    }
    return cost_change;
}

void MPGAAstar::reestablishConsistency(vector<pair<int,int>> dec_edges){
    while(!open.empty())
        open.pop();
    for(int i = 0; i < dec_edges.size();i++){
        insertState(vnodes[dec_edges[i].first], vnodes[dec_edges[i].second], open);
    }
    while(!open.empty()){
        _VNode* current = open.top(); open.pop();
        if(current->support->next != nullptr)
            current->next = current->support;
        vector<int> neighbors = graph->getNode(current->id).getNeighbors();
        vector<int>::iterator iter;
        for(iter = neighbors.begin(); iter != neighbors.end(); iter++){
            insertState(current,vnodes[graph->getNode(*iter).getID()],open);
        }
    }
}

void MPGAAstar::initializeVNodes(){
    for(int i = 0; i < graph->numNodes(); i++){
        vnodes.push_back(new _VNode(graph->getNode(i)));
    }
}
bool MPGAAstar::createPath(int start_id, int goal_id){
    counter = 0;
    initializeVNodes();
    start = vnodes[start_id];
    goal = vnodes[goal_id];
    bool change = false;
    for(int i = 0; i < vnodes.size(); i++){
        vnodes[i]->h = octile_h(start,goal);
    }
    Observe(start);
    while(start != goal){
        counter++;
        int temp = Astar();
        if (temp == Node::INVALID_NODE_INDEX)
            return false;
        _VNode* s = vnodes[temp];
        for(int i = 0; i < closed.size(); i++){
            closed[i]->h = s->g + s->h - closed[i]->g; //heuristic update
        }
        buildPath(s->id);
        while(!change && (start != goal)){
            _VNode* t = start;
            path.push_back(start->id);
            start = start->next;
            t->next = nullptr;
            //Move Agent?
            change = Observe(start);
        }
        change = false;
    }
    return true;
}

void MPGAAstar::printPath(){
    list<int>::iterator iter;
    for (iter = path.begin(); iter != path.end(); iter++){
        graph->getNode(*iter).printNode();
        cout << endl;
    }
}




