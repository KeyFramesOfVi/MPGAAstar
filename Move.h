
//
//  Move.h
//  MPGAAstar
//
//  Created by Victor Cabrera on 8/18/15.
//  Copyright (c) 2015 Victor Cabrera. All rights reserved.
//


#ifndef MPGAAstar_Move_h
#define MPGAAstar_Move_h
#include <iostream>
using namespace std;

class Move {
public:
    Move(){
        x = 0; y = 0; theta = 0;
    }
    
    Move(double x, double y, double theta);
    
    double getTheta() const
    {
        return theta;
    }
    
    double getX() const
    {
        return x;
    }
    
    double getY() const
    {
        return y;
    }
    
    void print() {
        cout << "Move(" << x << "," << y << "," << theta << ")" ;
    }
    
private:
    double theta;
    double x;
    double y;
};

#endif /* MOVE_H_ */