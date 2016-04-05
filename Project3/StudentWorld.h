#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#define random(x) (rand()%x)
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

using namespace std;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), dirt(64, vector<Dirt*>(64, nullptr))
	{
        
	}
    ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool deleteDirt(int i, int j);
    bool isDirt(int i, int j);
    
    int actorAtCoor(int x, int y, string actorID);
    
    void setActorDead(int k);
    void setActorPick(int k);
    void setActorLeave(int k);
    void setActorStun(int k);
    
    bool getActorLeave(int k);
    bool getActorFall(int k);
   
    int getFrackmanX() const;
    int getFrackmanY() const;
    void changeFrackmanHP(int x);
    
    

    void updateInv(int x, char c);
    
    void updateText();
    bool isBeenSet(int x ,int y);
    void newSonarWater();
    void throwGold(int x, int y);
    void activateSonar(int x, int y);
    void createNewSquirt(int x, int y, Actor::Direction dire);
    void addNewProtester();


private:
    string TextFormat(int score, int level, int life, int hp, int squirt, int gold, int sonar, int oilleft);
    
    int current_level_number;
    int oilleft;
    int gold;
    int water;
    int sonar;
    int numOfPro = 0;
    int ticksToNewPro = 0;
    vector<vector<Dirt*> > dirt;
    vector<Actor*> actor;
    Frackman *frackman;
};

#endif // STUDENTWORLD_H_
