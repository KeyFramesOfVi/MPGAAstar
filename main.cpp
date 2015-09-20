

//
//  main.cpp
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//

#include <iostream>
#include "MPGAAstar.h"
#include <ctime>
const int DEFAULT_BUFFER_SIZE = 30;
int main(int argc, const char * argv[]) {
    std::clock_t timer;
    Map * dynamicMap, *unknownMap;
    unknownMap = new Map(602, 538, DEFAULT_BUFFER_SIZE);
    dynamicMap = new Map(602, 538, DEFAULT_BUFFER_SIZE);
    Graph * myGraph;
  
    myGraph = new Graph(unknownMap, 30);
    
    //Adds the walls after, so it learns initial costs from an "unknown" map
    dynamicMap->addWall("wall1", 0, 0, 602, 0);
    dynamicMap->addWall("wall2", 0, 0, 0, 538);
    dynamicMap->addWall("wall3", 602, 0, 602, 538);
    dynamicMap->addWall("wall4", 0, 538, 602, 538);
    dynamicMap->addWall("wall5", 205, 0, 205, 206);
    dynamicMap->addWall("wall6", 105, 206, 205, 206);
    dynamicMap->addWall("wall7", 102, 336, 205, 336);
    dynamicMap->addWall("wall8", 205, 336, 205, 538);
    dynamicMap->addWall("wall9", 422, 336, 422, 538);
    dynamicMap->addWall("wall10", 315, 336, 520, 336);
    dynamicMap->addWall("wall11", 315, 206, 422, 206);
    dynamicMap->addWall("wall12", 422, 0, 422, 206);
    dynamicMap->addVirtualWall("vwall6", 20, 0, 20, 60);
    dynamicMap->addVirtualWall("vwall7", 20, 0, 20, 60);
    dynamicMap->addVirtualWall("vwall10", 20, 0, 20, 60);
    dynamicMap->addVirtualWall("vwall11", 20, 0, 20, 60);
    dynamicMap->addVirtualWall("vwall6e", 20, 0, 20, 60);
    dynamicMap->addVirtualWall("vwall7e", 95, 326, 305, 346);
    dynamicMap->addVirtualWall("vw10e1", 305, 326, 305, 346);
    dynamicMap->addVirtualWall("vw10e2", 530, 326, 530, 346);
    dynamicMap->addVirtualWall("vw11e", 305, 196, 305, 216);
    myGraph->setMap(dynamicMap);
    Node start = myGraph->getNode(296);
    Node goal = myGraph->getNode(340);
    timer = std::clock();
    MPGAAstar tester(*myGraph,start,goal);
    double duration = ( std::clock() - timer ) / (double) CLOCKS_PER_SEC;
    cout << "time: " << duration << endl;
    return 0;
}
