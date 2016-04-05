#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

// Base Class Actor
class Actor : public GraphObject{
public:
    Actor(StudentWorld *world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    
    virtual ~Actor() = default;
    virtual void doSomething() = 0;
    bool getStatus() const;
    string getName() const;
    void setName(string ID);
    void setDead();
    
    virtual void afterPickGold(){}
    virtual void setLeaveState(){}
    virtual bool getLeaveState() const{return false;}
    virtual bool getFallState() const{return false;}
    virtual void setStunState(){}
    StudentWorld * getWorld() const;
    
private:
    StudentWorld *my_world;
    string my_ID;
    bool isDead;
};

inline Actor::Actor(StudentWorld *world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
: GraphObject(imageID, startX, startY, dir, size, depth){
    
    setVisible(true);
    my_world = world;
    isDead = false;
    
}
inline void Actor::setName(string ID){
    my_ID = ID;
}

inline string Actor::getName() const{
    return my_ID;
}
inline bool Actor::getStatus() const{
    return isDead;
}

inline void Actor::setDead(){
    isDead = true;
}

inline StudentWorld *Actor::getWorld() const{
    return my_world;
}



// Class Dirt
class Dirt : public Actor{
public:
    Dirt(StudentWorld *world, int startX, int startY);
    virtual ~Dirt() = default;
    virtual void doSomething();
};
// Class Boulder
class Boulder : public Actor{
public:
    Boulder(StudentWorld *world, int startX, int startY);
    virtual ~Boulder() = default;
    virtual void doSomething();
    virtual bool getFallState() const;
private:
    bool canFall(int x, int y);
    bool stableState = true;
    bool waitState = false;
    bool fallState = false;
    int timeCount = 0;
};

// Class Oil
class Oil : public Actor{
public:
    Oil(StudentWorld *world, int startX, int startY);
    virtual ~Oil() = default;
    virtual void doSomething();
};

// Class Gold Nugget
class Gold : public Actor{
public:
    Gold(StudentWorld *world, int startX, int startY);
    virtual ~Gold() = default;
    virtual void doSomething();
public:
    bool temporary;
    int timecount = 0;
};


// Class Sonar Kit
class Sonar : public Actor{
public:
    Sonar(StudentWorld *world);
    virtual ~Sonar() = default;
    virtual void doSomething();
private:
    int timecount = 0;
};

// Class Water Pool
class Water : public Actor{
public:
    Water(StudentWorld *world, int startX, int startY);
    virtual ~Water() = default;
    virtual void doSomething();
private:
    int timecount = 0;
};
// Class DynamicActor
class DynamicActor : public Actor{
public:
    DynamicActor(int HP, StudentWorld *world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~DynamicActor() = default;
    virtual void doSomething() = 0;
    bool noBlockOnDire(int x, int y, Direction dire);
    bool moveInField(int x, int y);
    int getHP() const;
    
    virtual void playDeadSound(){}
    virtual void playAnnoySound(){}
    virtual void givePointsOnDeath(){}
    void changeHP(int x);
    
    virtual void setLeaveState(){}
    virtual bool getLeaveState() const{return false;}
    
    
private:
    int my_HP;
};

// Class Squirt
class Squirt : public DynamicActor{
public:
    Squirt(StudentWorld *world, int startX, int startY, Direction dire);
    virtual ~Squirt() = default;
    virtual void doSomething();
};

// Class Protester
class Protester : public DynamicActor{
public:
    Protester(StudentWorld *world, int HP, int ID);
    virtual ~Protester()= default;
    virtual void doSomething();
    virtual void afterPickGold();
    virtual void playDeadSound();
    virtual void playAnnoySound();
    virtual void givePointsOnDeath();
    void doWhenNotRest();
    bool doneWhenRest();
    int pathToDes(int x, int y, int fx, int fy);
    
    struct Node{
        int x,y;
    };
    Node kthNodeToDes(int k);
    
    void setTicks();
    
    virtual void setStunState();
    virtual void setLeaveState();
    virtual bool getLeaveState() const;

private:
    void leaveOilField();
    bool leaveState = false;
    bool stunState = false;
    bool annoyState = false;
    int stunCount = 0;
    int ticksWait;
    int current_level_number;
    int shoutCount = 16;
    int numToMove;
    int perpendCount = 0;
    bool haveCalcExit = false;
    
    bool visited[64][64] = {false};
    Node recordPre[64][64];
    vector<Node> path;
    int stepsToExit = 0;
    
    void checkSquirt();
    virtual bool phoneSearch(){return false;}
    bool isVisited(int x, int y, Direction dire);
    void setVisited(int x, int y, Direction dire);
    bool isFaceFrackman(int fx, int fy);
    bool sameLineLargerFour(int fx, int fy);
    bool canMoveToMan(int fx, int fy);
    Direction changeDirection(int fx, int fy);
    bool isPerpendTurn(Direction before, Direction after);
    Direction isIntersect(Direction dire);
    Direction getIntersectDire(Direction cur, Direction a, Direction b);

};

// Class Hardcore
class Hardcore: public Protester{
public:
    Hardcore(StudentWorld*world, int HP, int ID);
    virtual ~Hardcore() = default;
    virtual void doSomething();
    virtual void givePointsOnDeath();
private:
    virtual bool phoneSearch();
    int M;
};
// Class Frackman
class Frackman : public DynamicActor{
public:
    Frackman(StudentWorld *world);
    virtual ~Frackman() = default;
    virtual void doSomething();
    virtual void playDeadSound();
    
private:
    bool canCreateSquirt(int x, int y, Direction dire);
    bool cleanDirt(int x, int y, Direction dir);
    bool canMoveOnto(int x, int y, Direction dir);
};

#endif // ACTOR_H_
