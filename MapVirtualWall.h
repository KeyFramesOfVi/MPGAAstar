//
//  MapVirtualWall.h
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//

#ifndef __MPGAAstar__MapVirtualWall__
#define __MPGAAstar__MapVirtualWall__
#include "Utils.h"
#include "MapWallBuffer.h"
#include "Position.h"
#include <vector>

class MapVirtualWall {
public:
    MapVirtualWall() {}
    
    MapVirtualWall(string i, int bSize, double fx, double fy, double sx, double sy);
    
    string getId() const { return id; }
    
    double getX0() const { return x0; }
    double getX1() const { return x1; }
    double getY0() const { return y0; }
    double getY1() const { return y1; }
    
    void setX0(double x) { x0 = x; }
    void setY0(double y) { y0 = y; }
    void setX1(double x) { x1 = x; }
    void setY1(double y) { y1 = y; }
    
    // Wall Buffer functions
    void initBuffers();
    vector<MapWallBuffer> getWallBuffers(){ return buffers; }
    
    double getP1x() const { return p1x; }
    double getP1y() const { return p1y; }
    double getP2x() const { return p2x; }
    double getP2y() const { return p2y; }
    double getP3x() const { return p3x; }
    double getP3y() const { return p3y; }
    double getP4x() const { return p4x; }
    double getP4y() const { return p4y; }
    
    
private:
    double x0, y0, x1, y1;
    string id;
    int bufferSize;
    vector<MapWallBuffer> buffers;
    int p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
    
    void assignSortedEndPoints(double, double, double, double);
};

#endif /* defined(__MPGAAstar__MapVirtualWall__) */
