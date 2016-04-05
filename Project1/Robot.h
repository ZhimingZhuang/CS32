//
//  Robot.h
//  cs32
//
//  Created by ZhimingZhuang on 1/10/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#ifndef __cs32__Robot__
#define __cs32__Robot__

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Robot declaration.

class Robot
{
    public:
    // Constructor
    Robot(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool getAttacked(int dir);
    
    private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
};

#endif /* defined(__cs32__Robot__) */
