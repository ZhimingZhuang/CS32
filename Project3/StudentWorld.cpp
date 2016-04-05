#include "StudentWorld.h"
#include <string>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::init(){
    
    // Set the value of private members;
    current_level_number = getLevel();
    oilleft = min(2 + current_level_number, 20);
    gold = 0;
    water = 5;
    sonar = 1;
    numOfPro = 0;
    
    // Initiate dirt
    for(int i = 0; i < 60; i++){
        for(int j = 0; j < 64; j++){
            if(i >= 4 && i <= 59 && j >= 30 && j <= 33)
                continue;
            dirt[i][j] = new Dirt(this, j, i);
        }
    }
    // Initiate player
    frackman = new Frackman(this);
    
    // Initiate boulder
    int num_boulder = min(current_level_number / 2 + 2, 6);
    
    for(int k = 0; k < num_boulder; k++){
        int x, y;
        y = random(37) + 20;
        x = random(61);
        while((x >= 27 && x <= 33) || isBeenSet(x, y)){
            x = random(61);
        }
        for(int j = x; j < x + 4; j++)
            for(int i = y; i < y + 4; i++)
                if(dirt[i][j] != nullptr)
                    deleteDirt(i, j);
        Boulder* tmpboulder = new Boulder(this, x, y);
        actor.push_back(tmpboulder);
    }
    
    // Initiate oil
    int num_oil = min(2 + current_level_number, 20);
    for(int k = 0; k < num_oil; k++){
        int x, y;
        y = random(37) + 20;
        x = random(61);
        while((x >= 27 && x <= 33) || isBeenSet(x, y)){
            x = random(61);
        }
        
        Oil* tmpoil = new Oil(this, x, y);
        actor.push_back(tmpoil);
    }
    
    // Initiate gold nugget
    int num_gold = max(5 - current_level_number / 2, 2);
    for(int k = 0; k < num_gold; k++){
        int x, y;
        y = random(37) + 20;
        x = random(61);
        while((x >= 27 && x <= 33) || isBeenSet(x, y)){
            x = random(61);
        }
        
        Gold* tmpgold = new Gold(this, x, y);
        actor.push_back(tmpgold);
    }
    
    
    // Initiate sonar and water
    newSonarWater();
    
    // Initiate protester
    addNewProtester();
    ticksToNewPro = 0;
    
    // UpdateText;
    updateText();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    updateText();

    
    // Create New object in current tick
    newSonarWater();
    ticksToNewPro++;
    
    if(ticksToNewPro > max(25, 200 - current_level_number)){
        ticksToNewPro =0;
        int  maxNumPro = 2 + current_level_number*1.5;
        maxNumPro = min(15, maxNumPro);
        
        if(numOfPro < maxNumPro)
            addNewProtester();
        
        
    }
    
    
    for(int i = 0; i < actor.size(); i++){
        actor[i]->doSomething();
        if(frackman->getStatus()){
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    frackman->doSomething();
    
    
    if(frackman->getStatus()){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if(oilleft == 0){
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    for(int i = 0; i < actor.size(); i++){
        if(actor[i]->getStatus() == true){
            if(actor[i]->getName() == "Protester" || actor[i]->getName() == "Hardcore")
                numOfPro--;
            delete actor[i];
            actor.erase(actor.begin()+i);
            i--;
        }
    }
    

    return GWSTATUS_CONTINUE_GAME;

}
void StudentWorld::cleanUp(){
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 64; j++){
            if(dirt[j][i] != nullptr){
                delete dirt[j][i];
                dirt[j][i] = nullptr;
            }
        }
    }
    for(int i = 0; i < actor.size(); i++){
        delete actor[i];
    }
    actor.clear();
    
    delete frackman;
    frackman = nullptr;
}
StudentWorld::~StudentWorld(){
    cleanUp();
}

bool StudentWorld::deleteDirt(int i, int j){
    if(dirt[i][j] != nullptr){
        dirt[i][j]->setVisible(false);
        delete dirt[i][j];
        dirt[i][j] = nullptr;
        return true;
    }
    return false;
}

bool StudentWorld::isDirt(int i, int j){
    if (dirt[i][j] != nullptr)
        return true;
    else
        return false;
}

int StudentWorld::actorAtCoor(int x, int y, string actorID){
    for(int k = 0; k < actor.size(); k++){
        string tmp = actor[k]->getName();
        if(tmp == actorID && actor[k]->getX() == x && actor[k]->getY() == y)
            return k;
    }
    return -1;
}




void StudentWorld::setActorDead(int k){
    actor[k]->setDead();
}
void StudentWorld::setActorPick(int k){
    actor[k]->afterPickGold();
}
int StudentWorld::getFrackmanX() const{
    return frackman->getX();
}

int StudentWorld::getFrackmanY() const{
    return frackman->getY();
}
void StudentWorld::changeFrackmanHP(int x){
    frackman->changeHP(x);
    if(frackman->getHP() <= 0){
        frackman->setDead();
    }
}



void StudentWorld::updateText(){
    int score = getScore();
    int life = getLives();
    int level = getLevel();
    int hp = frackman->getHP() * 10;
    
    string s = TextFormat(score, level, life, hp, water, gold, sonar, oilleft);
    
    setGameStatText(s);
    
}


void StudentWorld::updateInv(int x, char c){
    switch (c) {
        case 's':
            sonar += x;
            if(sonar <0) sonar = 0;
            break;
        case 'o':
            oilleft += x;
            break;
        case 'g':
            gold += x;
            if(gold <0) gold = 0;
            break;
        case 'w':
            water += x;
            if(water <0) water = 0;
            break;
            
        default:
            break;
    }
}

string StudentWorld::TextFormat(int score, int level, int life, int hp, int water, int gold, int sonar, int oilleft){
    int size;
    string s_score = to_string(score);
    size = int(s_score.size());
    for(int i = 0; i < 6 - size; i++)
        s_score = "0" + s_score;
    s_score = " " + s_score;
    string s_level = " " + to_string(level);
    string s_life = " " + to_string(life);
    string s_hp = " " + to_string(hp) + "%";
    string s_water = " " + to_string(water);
    string s_gold = " " + to_string(gold);
    string s_sonar = " " + to_string(sonar);
    string s_oilleft = " " + to_string(oilleft);
    
    string s = "Src:" + s_score + "  " + "Lvl:" + s_level + "  " + "Lives:" + s_life + "  " + "Hlth:" + s_hp + "  " + "Water:" + s_water + "  " + "Gld:" + s_gold + "  " + "Sonar:" + s_sonar + "  " + "Oil Left:" + s_oilleft;
    return s;
}

bool StudentWorld::isBeenSet(int x ,int y){
    for(int i = 0; i < actor.size(); i++){
        int ax = actor[i]->getX();
        int ay = actor[i]->getY();
        double dist = pow(x-ax, 2) + pow(y-ay, 2);
        if(dist <= 36)
            return true;
    }
    return false;
}


void StudentWorld::newSonarWater(){
    int chance = current_level_number * 25 + 300;
    if(random(chance) == 1){
        int subchance = 5;
        if(random(subchance) == 1){
            Sonar* tmpsonar = new Sonar(this);
            actor.push_back(tmpsonar);
        }else{
            int x, y;
            bool stop = false;
            while(!stop){
                stop = true;
                
                y = random(61);
                x = random(61);
                while(isBeenSet(x, y)){
                    y = random(61);
                    x = random(61);
                }
                
                for(int i = y; i < y+4; i++){
                    for(int j = x; j < x+4; j++){
                        if(isDirt(i, j)){
                            stop = false;
                            break;
                        }
                    }
                }
            }
            
            Water* tmpwater = new Water(this, x, y);
            actor.push_back(tmpwater);
        }
        
    }
}
void StudentWorld::setActorLeave(int k){
    actor[k]->setLeaveState();
}

bool StudentWorld::getActorLeave(int k){
    return actor[k]->getLeaveState();
}

void StudentWorld::setActorStun(int k){
    actor[k]->setStunState();
}
bool StudentWorld::getActorFall(int k){
    return actor[k]->getFallState();
}
void StudentWorld::throwGold(int x, int y){
    if(gold > 0){
        Gold* tmpgold = new Gold(this, x, y);
        actor.push_back(tmpgold);
        gold--;
    }
}

void StudentWorld::activateSonar(int x, int y){
    if(sonar > 0){
        playSound(SOUND_SONAR);
        for(int i = 0; i < actor.size(); i++){
            if(actor[i]->getName() == "Oil" || actor[i]->getName() == "Gold"){
                int ax = actor[i]->getX();
                int ay = actor[i]->getY();
                double dist = pow(ax - x, 2) + pow(ay - y, 2);
                if(dist <= 144)
                    actor[i]->setVisible(true);
            }
        }
        sonar--;
    }


}

void StudentWorld::createNewSquirt(int x, int y, Actor::Direction dire){
    if(water > 0){
        playSound(SOUND_PLAYER_SQUIRT);
        switch (dire) {
            case Actor::left:
                x = x - 4;
                break;
            case Actor::right:
                x = x + 4;
                break;
            case Actor::down:
                y = y - 4;
                break;
            case Actor::up:
                y = y + 4;
                break;
            default:
                break;
        }
        Squirt * tmpsquirt = new Squirt(this, x, y, dire);
        actor.push_back(tmpsquirt);
        water--;
    }
}

void StudentWorld:: addNewProtester(){
    int change = min(90, current_level_number * 10 + 30);
    int p = 1 + random(100);
    if(p <= change){
        Protester *tmphard = new Hardcore(this, 20, IID_HARD_CORE_PROTESTER);
        actor.push_back(tmphard);
    }else{
        Protester *tmppro = new Protester(this, 5, IID_PROTESTER);
        actor.push_back(tmppro);
    }
    numOfPro++;

}
