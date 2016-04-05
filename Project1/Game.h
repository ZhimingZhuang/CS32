//
//  Game.h
//  cs32
//
//  Created by ZhimingZhuang on 1/10/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#ifndef __cs32__Game__
#define __cs32__Game__

class Arena;
class Player;

class Game
{
    public:
    // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();
    
    // Mutators
    void play();
    
    private:
    Arena* m_arena;
};

#endif /* defined(__cs32__Game__) */
