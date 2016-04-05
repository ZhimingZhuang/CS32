//
//  Arena.h
//  cs32
//
//  Created by ZhimingZhuang on 1/10/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#ifndef __cs32__Arena__
#define __cs32__Arena__
#include "globals.h"
#include "History.h"

class Player;
class Robot;
class History;

class Arena
{
    public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     nRobotsAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    History& history();
    
    // Mutators
    bool   addRobot(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackRobotAt(int r, int c, int dir);
    bool   moveRobots();
    
    private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;
    History his;
};

#endif /* defined(__cs32__Arena__) */
