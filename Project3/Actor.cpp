#include "Actor.h"
#include "StudentWorld.h"
#include <queue>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// Class Dirt
Dirt::Dirt(StudentWorld *world, int startX, int startY)
: Actor(world, IID_DIRT, startX, startY, right, 0.25, 3){
}

void Dirt::doSomething(){}


// Class Boulder
Boulder::Boulder(StudentWorld *world, int startX, int startY)
: Actor(world, IID_BOULDER, startX, startY, down, 1.0, 1){
    setName("Boulder");
}

bool Boulder::canFall(int x, int y){
    if(y - 1 < 0)
        return false;
    for(int j = x; j < x + 4; j++)
        if(getWorld()->isDirt(y-1, j))
            return false;
    for(int j = x - 3; j <= x + 3; j++){
        if(getWorld()->actorAtCoor(j, y-4, "Boulder") != -1)
            return false;
    }
    return true;
}

void Boulder::doSomething(){
    if(getStatus())
        return;
    if(stableState){
        if(!canFall(getX(), getY())) return;
        stableState = false;
        waitState = true;
    }else if(waitState){
        timeCount++;
        if(timeCount == 31){
            timeCount = 0;
            waitState = false;
            fallState = true;
            getWorld()->playSound(SOUND_FALLING_ROCK);
        }
    }else if(fallState){
        if(canFall(getX(), getY())){
            moveTo(getX(), getY()-1);
            for(int i = getY()-3; i <= getY()+3; i++){
                for(int j = getX()-3; j <= getX()+3; j++){
                    int dist = pow(getX() - j, 2) + pow(getY() - i, 2);
                    if(dist <= 9){
                        bool leaveState = true;
                        int m = getWorld()->actorAtCoor(j, i, "Protester");
                        if(m != -1)
                            leaveState= getWorld()->getActorLeave(m);
                        
                        if(m != -1 && !leaveState){
                            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
                            getWorld()->setActorLeave(m);
                            getWorld()->increaseScore(500);
                        }
                        leaveState = true;
                            m = getWorld()->actorAtCoor(j, i, "Hardcore");
                        if(m != -1)
                            leaveState = getWorld()->getActorLeave(m);
                        if(m != -1 && !leaveState){
                            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
                            getWorld()->setActorLeave(m);
                            getWorld()->increaseScore(500);
                        }
                    }
                }
            }
            
            int x = getWorld()->getFrackmanX();
            int y = getWorld()->getFrackmanY();
            double dist = pow((getX() - x), 2) + pow((getY()-1 - y),2);
            if(dist <= 9){
                getWorld()->changeFrackmanHP(-10);
            }
            
        }
        else{
            setVisible(false);
            setDead();
        }
    }
    
}

bool Boulder::getFallState() const{
    return fallState;
}
// Class Oil
Oil::Oil(StudentWorld *world, int startX, int startY)
: Actor(world, IID_BARREL, startX, startY, right, 1.0, 2){
    setName("Oil");
    setVisible(false);
}

void Oil::doSomething(){
    if(getStatus()) return;
    int fx = getWorld()->getFrackmanX();
    int fy = getWorld()->getFrackmanY();
    double dist = pow(fx-getX(), 2) + pow(fy-getY(), 2);
    if(!isVisible() && dist <= 16){
        setVisible(true);
        return;
    }else if(dist <= 9){
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->updateInv(-1, 'o');
    }
}

// Class Gold Nugget
Gold::Gold(StudentWorld *world, int startX, int startY)
: Actor(world, IID_GOLD, startX, startY, right, 1.0, 2){
    setName("Gold");
    setVisible(true);
    temporary = true;
    for(int j = startX; j < startX + 4; j++){
        for(int i = startY; i < startY + 4; i++){
            if(world->isDirt(i, j)){
                setVisible(false);
                temporary = false;
                break;
            }
        }
    }  
}

void Gold::doSomething(){
    if(getStatus()) return;
    if(!temporary){
        int fx = getWorld()->getFrackmanX();
        int fy = getWorld()->getFrackmanY();
        double dist = pow(fx-getX(), 2) + pow(fy-getY(), 2);
        if(!isVisible() && dist <= 16){
            setVisible(true);
            return;
        }else if(dist <= 9){
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getWorld()->updateInv(1, 'g');
        }
    }else{
        timecount++;
        
        for(int x = getX()-3; x <= getX() + 3; x++){
            for(int y = getY()-3; y <= getY() + 3; y++){
                if(x < 0 || x > 60 || y < 0 || y > 60) continue;
                double dist = pow(x-getX(), 2) + pow(y-getY(), 2);
                if(dist <= 9){
                    int p1 = getWorld()->actorAtCoor(x, y, "Protester");
                    int p2 = getWorld()->actorAtCoor(x, y, "Hardcore");
                    if(p1 == -1 && p2 == -1) continue;
                    else if(p1 != -1 && p2 == -1){
                        getWorld()->setActorPick(p1);
                        getWorld()->setActorLeave(p1);
                        getWorld()->increaseScore(25);
                        setDead();
                    }else if(p1 == -1 && p2 != -1){
                        getWorld()->setActorPick(p2);
                        getWorld()->setActorStun(p2);
                        getWorld()->increaseScore(50);
                        setDead();
                    }else{
                        int r = random(2);
                        switch (r) {
                            case 0:
                                getWorld()->setActorPick(p1);
                                getWorld()->setActorLeave(p1);
                                getWorld()->increaseScore(25);
                                setDead();
                                break;
                            case 1:
                                getWorld()->setActorPick(p2);
                                getWorld()->setActorStun(p2);
                                getWorld()->increaseScore(50);
                                setDead();
                                break;
                            default:
                                break;
                        }
                    
                    }
                }
            }
        }
        if(timecount > 100){
            setVisible(false);
            setDead();
        }
    
    
    }
}

// Class Sonar
Sonar::Sonar(StudentWorld *world)
: Actor(world, IID_SONAR, 0, 60, right, 1.0, 2){
    setName("Sonar");
}
void Sonar::doSomething(){
    if(getStatus()) return;
    int current_level_number = getWorld()->getLevel();
    int T = min(100, 300 - 10*current_level_number);
    timecount++;
    if(timecount > T){
        timecount = 0;
        setDead();
        return;
    }
    int fx = getWorld()->getFrackmanX();
    int fy = getWorld()->getFrackmanY();
    double dist = pow(fx-getX(), 2) + pow(fy-getY(), 2);
    if(dist <= 9){
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        getWorld()->updateInv(2, 's');
    }

}

// Class Water Pool
Water::Water(StudentWorld *world, int startX, int startY)
: Actor(world, IID_WATER_POOL, startX, startY, right, 1.0, 2){
    setName("Water");
}

void Water::doSomething(){
    if(getStatus()) return;
    int current_level_number = getWorld()->getLevel();
    int T = min(100, 300 - 10*current_level_number);
    timecount++;
    if(timecount > T){
        timecount = 0;
        setDead();
        return;
    }
    int fx = getWorld()->getFrackmanX();
    int fy = getWorld()->getFrackmanY();
    double dist = pow(fx-getX(), 2) + pow(fy-getY(), 2);
    if(dist <= 9){
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(100);
        getWorld()->updateInv(5, 'w');
    }
    
}

// Class DynamicActor
DynamicActor::DynamicActor(int HP, StudentWorld *world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
: Actor(world, imageID, startX, startY, dir, size, depth){
    my_HP = HP;
}
void DynamicActor::changeHP(int x){
    my_HP += x;
    if(my_HP <= 0){
        setLeaveState();
        playDeadSound();
        givePointsOnDeath();
        
    }else if(x < 0){
        playAnnoySound();
    }
}

int DynamicActor::getHP() const{
    return my_HP;
}

bool DynamicActor::moveInField(int x, int y){
    if(x >= 0 && x <= 63 && y >=0 && y <= 63)
        return true;
    else
        return false;
}

bool DynamicActor::noBlockOnDire(int x, int y, Direction dire){
    
    switch (dire) {
        case left:
            for(int i = y; i < y + 4; i++){
                if(x - 1 < 0 || getWorld()->isDirt(i, x-1))
                    return false;
            }
            
            for(int i = y - 3; i <= y + 3; i++){
                if(i < 0 || i > 56) continue;
                int k = getWorld()->actorAtCoor(x - 4, i, "Boulder");
                if(x - 4 >=0 && k != -1 && !getWorld()->getActorFall(k))
                    return false;
            }
            break;
        case right:
            for(int i = y; i < y + 4; i++){
                if(x + 4 > 63 || getWorld()->isDirt(i, x+4))
                    return false;
            }
            
            for(int i = y - 3; i <= y + 3; i++){
                if(i < 0 || i > 56) continue;
                int k = getWorld()->actorAtCoor(x + 4, i, "Boulder");
                if(x + 4 <= 60 && k != -1 && !getWorld()->getActorFall(k))
                    return false;
            }
            break;
        case down:
            for(int j = x; j < x + 4; j++){
                if(y - 1 < 0 || getWorld()->isDirt(y-1, j))
                    return false;
            }
            
            for(int j = x - 3; j <= x + 3; j++){
                if(j < 0 || j > 60) continue;
                int k = getWorld()->actorAtCoor(j, y-4, "Boulder");
                if(y - 4 >= 0 && k != -1 && !getWorld()->getActorFall(k))
                    return false;
            }
            break;
        case up:
            for(int j = x; j < x + 4; j++){
                if(y + 4 > 63 || getWorld()->isDirt(y+4, j))
                    return false;
            }
            for(int j = x - 3; j <= x + 3; j++){
                if(j < 0 || j > 60) continue;
                int k = getWorld()->actorAtCoor(j, y+4, "Boulder");
                if (y + 4 <= 60 &&  k!= -1 && !getWorld()->getActorFall(k))
                    return false;
            }
            break;
        default:
            break;
    }
    return true;
    
}

// Class Squirt
Squirt::Squirt(StudentWorld *world, int startX, int startY, Direction dire)
: DynamicActor(4, world, IID_WATER_SPURT, startX, startY, dire, 1.0, 0){
    setName("Squirt");
}
void Squirt::doSomething(){
    if(getStatus()) return;
    if(getHP() <= 0 || !noBlockOnDire(getX(), getY(), getDirection()))
        setDead();
    else{
        switch (getDirection()) {
            case left:
                moveTo(getX()-1, getY());
                changeHP(-1);
                break;
            case right:
                moveTo(getX()+1, getY());
                changeHP(-1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                changeHP(-1);
                break;
            case up:
                moveTo(getX(), getY()+1);
                changeHP(-1);
                break;
            default:
                break;
        }
        
    }

}


// Class Frackman
Frackman::Frackman(StudentWorld *world)
: DynamicActor(10, world, IID_PLAYER, 30, 60, right, 1.0, 0){
    setName("Frackman");
}

void Frackman::doSomething(){
    if(getStatus()) return;
    int key;
    if(getWorld()->getKey(key)){
        switch (key) {
            case KEY_PRESS_LEFT:
                if(getDirection() != left)
                    setDirection(left);
                else if(canMoveOnto(getX()-1, getY(), left)){
                        moveTo(getX()-1, getY());
                        setDirection(left);
                        if(cleanDirt(getX(), getY(), left))
                            getWorld()->playSound(SOUND_DIG);
                }
                break;
            case KEY_PRESS_RIGHT:
                if(getDirection() != right)
                    setDirection(right);
                else if(canMoveOnto(getX()+4, getY(), right)){
                        moveTo(getX()+1, getY());
                        setDirection(right);
                        if(cleanDirt(getX()+3, getY(), right))
                            getWorld()->playSound(SOUND_DIG);
                }
                break;
            case KEY_PRESS_DOWN:
                if(getDirection() != down)
                    setDirection(down);
                else if(canMoveOnto(getX(), getY()-1, down)){
                        moveTo(getX(), getY()-1);
                        setDirection(down);
                        if(cleanDirt(getX(), getY(), down))
                            getWorld()->playSound(SOUND_DIG);
                }
                break;
            case KEY_PRESS_UP:
                if(getDirection() != up)
                    setDirection(up);
                else if(canMoveOnto(getX(), getY()+4, up)){
                        moveTo(getX(), getY()+1);
                        setDirection(up);
                        if(cleanDirt(getX(), getY()+3, up))
                            getWorld()->playSound(SOUND_DIG);
                }
                break;
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_TAB:
                getWorld()->throwGold(getX(), getY());
                break;
            case 'z':
            case 'Z':
                getWorld()->activateSonar(getX(), getY());
                break;
            case KEY_PRESS_SPACE:
                if(canCreateSquirt(getX(), getY(), getDirection()))
                    getWorld()->createNewSquirt(getX(), getY(), getDirection());
                else
                    getWorld()->updateInv(-1, 'w');
            default:
                break;
        }
    
    }
}

bool Frackman::cleanDirt(int x, int y, Direction dir){
    bool doclean = false;
    switch (dir) {
        case left:
            if(x >= 0){
                for(int i = y; i < y + 4; i++)
                    if(getWorld()->deleteDirt(i, x))
                        doclean = true;
            }
            break;
        case right:
            if(x <= 63){
                for(int i = y; i < y + 4; i++)
                    if(getWorld()->deleteDirt(i, x))
                       doclean = true;
            }
            break;
        case down:
            if(y >= 0){
                for(int j = x; j < x + 4; j++)
                    if(getWorld()->deleteDirt(y, j))
                        doclean = true;
            }
            break;
        case up:
            if(y <= 63){
                for(int j = x; j < x + 4; j++)
                   if(getWorld()->deleteDirt(y, j))
                       doclean = true;
            }
        default:
            break;
    }
    return doclean;
}

bool Frackman::canCreateSquirt(int x, int y, Direction dire){
    switch (dire) {
        case left:
            if(noBlockOnDire(x, y, left) && noBlockOnDire(x-1, y, left) && noBlockOnDire(x-2, y, left) && noBlockOnDire(x-3, y, left))
                return true;
            break;
        case right:
            if(noBlockOnDire(x+3, y, right) && noBlockOnDire(x+4, y, right) && noBlockOnDire(x+5, y, right) && noBlockOnDire(x+6, y, right))
                return true;
            break;
        case down:
            if(noBlockOnDire(x, y, down) && noBlockOnDire(x, y-1, down) && noBlockOnDire(x, y-2, down) && noBlockOnDire(x, y-3, down))
                return true;
            break;
        case up:
            if (noBlockOnDire(x, y+3, up) && noBlockOnDire(x, y+4, up) && noBlockOnDire(x, y+5, up) && noBlockOnDire(x, y+6, up)) {
                return true;
            }
        default:
            break;
    }
    return false;

}

bool Frackman::canMoveOnto(int x, int y, Direction dir){
    if(moveInField(x, y)){
        
        switch (dir){
            case left:
                x = x + 2;
                y = y + 2;
                break;
            case right:
                x = x - 1;
                y = y + 2;
                break;
            case down:
                x = x + 2;
                y = y + 2;
                break;
            case up:
                x = x + 2;
                y = y - 1;
                break;
            default:
                break;
        }
        for(int j = x - 3; j <= x + 3; j++){
            for(int i = y - 3; i <= y + 3; i++){
                if(!(j >= 2 && j <= 62 && i >= 22 && i <= 58))
                    continue;
                double dist = dist = pow(x-j, 2) + pow(y-i, 2);
                if(dist <= 9){
                    if(getWorld()->actorAtCoor(j-2, i-2, "Boulder") != -1)
                        return false;
                }
            }
        }
        return true;
        
    }
    return false;
}

void Frackman::playDeadSound(){
    getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
}


// Class Protester
Protester::Protester(StudentWorld *world, int HP, int ID)
: DynamicActor(HP, world, ID, 60, 60, left, 1.0, 0){
        setName("Protester");
        current_level_number = world->getLevel();
        ticksWait = max(0, 3 - current_level_number/4);
        numToMove = 8 + random(53);
}

void Protester::doSomething(){
    if(getStatus()) return;
    
    if(!doneWhenRest()){
        doWhenNotRest();
    }
}

bool Protester::doneWhenRest(){
    bool runInthisFun = false;
    int stunMax = min(50, 100 - current_level_number * 10);
    
    if(ticksWait > 0){
        runInthisFun = true;
        ticksWait--;
    }else if(leaveState){
        runInthisFun = true;
        if(!haveCalcExit){
            pathToDes(getX(), getY(), 60, 60);
            haveCalcExit = true;
        }
        
        if(getX() == 60 && getY() == 60)
            setDead();
        else{
            leaveOilField();
        }
    }else if(stunState){
        runInthisFun = true;
        checkSquirt();
        stunCount++;
        perpendCount++;
        if(stunCount > stunMax){
            stunState = false;
            stunCount = 0;
        }
    }else if(annoyState){
        runInthisFun = true;
        stunCount++;
        perpendCount++;
        annoyState = false;
        stunState = true;
    }
    return runInthisFun;
}
void Protester::checkSquirt(){
    for(int x = getX()-3; x <= getX()+3; x++)
        for(int y = getY()-3; y <= getY()+3; y++){
            if(x < 0 || x > 60 || y < 0 || y > 60) continue;
            double dist = pow(x-getX(), 2) + pow(y-getY(), 2);
            if(dist <= 9){
                int sq = getWorld()->actorAtCoor(x, y, "Squirt");
                if(sq != -1){
                    annoyState = true;
                    changeHP(-2);
                    getWorld()->setActorDead(sq);
                }
            }
        }
}



void Protester::doWhenNotRest(){
    
    // Check whether there is a squirt
    checkSquirt();

    shoutCount++;
    if(shoutCount > 16) shoutCount = 16;
    
    perpendCount++;
    if(perpendCount > 201) perpendCount = 201;
    
    int fx = getWorld()->getFrackmanX();
    int fy = getWorld()->getFrackmanY();

    double dist = pow(fx-getX(), 2) + pow(fy-getY(), 2);
    if(dist <= 16 && isFaceFrackman(fx, fy) && shoutCount > 15){
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        shoutCount = 0;
        getWorld()->changeFrackmanHP(-2);
        stunState = true;
        return;
    }else if(phoneSearch()){
        
    }else if(sameLineLargerFour(fx, fy) && canMoveToMan(fx, fy)){
        Direction before = getDirection();
        Direction dire = changeDirection(fx, fy);
        
        if(isPerpendTurn(before, dire))
            perpendCount = 0;
        
        switch (dire) {
            case left:
                moveTo(getX()-1, getY());
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            default:
                break;
        }
        ticksWait = max(0, 3 - current_level_number/4);
    }else if(numToMove <= 0){
        Direction before = getDirection();
        do{
            int r = random(4);
            switch (r) {
                case 0:
                    setDirection(left);
                    break;
                case 1:
                    setDirection(right);
                    break;
                case 2:
                    setDirection(down);
                    break;
                case 3:
                    setDirection(up);
                    break;
                default:
                    break;
            }
        }while(!noBlockOnDire(getX(), getY(), getDirection()));
        
        Direction after = getDirection();
        if(isPerpendTurn(before, after))
            perpendCount = 0;
        numToMove = 8 + random(53);
        
    }else if(numToMove > 0){
        if(perpendCount > 200){
            Direction cur = isIntersect(getDirection());
            if(cur != getDirection()){
                setDirection(cur);
                perpendCount = 0;
            }
        }
        
        Direction dire = getDirection();
        switch (dire) {
            case left:
                if(noBlockOnDire(getX(), getY(), getDirection()))
                    moveTo(getX()-1, getY());
                else{
                    numToMove = 0;
                    return;
                }
                break;
            case right:
                if(noBlockOnDire(getX(), getY(), getDirection()))
                    moveTo(getX()+1, getY());
                else{
                    numToMove = 0;
                    return;
                }
                break;
            case up:
                if(noBlockOnDire(getX(), getY(), getDirection()))
                    moveTo(getX(), getY()+1);
                else{
                    numToMove = 0;
                    return;
                }
                break;
            case down:
                if(noBlockOnDire(getX(), getY(), getDirection()))
                    moveTo(getX(), getY()-1);
                else{
                    numToMove = 0;
                    return;
                }
                break;
            default:
                break;
        }
        numToMove--;
        ticksWait = max(0, 3 - current_level_number/4);
    }
}
bool Protester::isFaceFrackman(int fx, int fy){
    Direction dir = getDirection();
    switch (dir) {
        case left:
            if(fx < getX())
                return true;
            break;
        case right:
            if(fx > getX())
                return true;
            break;
        case up:
            if(fy > getY())
                return true;
            break;
        case down:
            if(fy < getY())
                return true;
        default:
            break;
    }
    return false;
}

bool Protester::sameLineLargerFour(int fx, int fy){
    if(fx - getX() == 0 || fy - getY() == 0){
        double dist = pow(fx-getX(), 2) + pow(fy-getY(), 2);
        if(dist > 16)
            return true;
        else
            return false;
    }
    else
        return false;
}

void Protester::playDeadSound(){
    getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
}
void Protester::playAnnoySound(){
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
}
void Protester::givePointsOnDeath(){
    getWorld()->increaseScore(100);
}

void Protester::setStunState(){
    stunState = true;
}

bool Protester::canMoveToMan(int fx, int fy){
    if(fx - getX() == 0){
        int Ymax = max(getY(), fy);
        int Ymin = min(getY(), fy);
        
        for(int x = getX(); x < getX() + 4; x++){
            for(int y = Ymin + 4; y < Ymax; y++){
                if(getWorld()->isDirt(y, x))
                    return false;
            }
        }
        for(int x = getX()-3; x < getX() + 4; x++){
            for(int y = Ymin + 4; y <= Ymax - 4; y++){
                if(getWorld()->actorAtCoor(x, y, "Boulder") != -1)
                    return false;
            }
        }
        return true;
    }else if(fy - getY() == 0){
        int Xmax = max(getX(), fx);
        int Xmin = min(getX(), fx);
        for(int x = Xmin + 4; x < Xmax; x++){
            for(int y = getY(); y < getY() + 4; y++){
                if(getWorld()->isDirt(y, x))
                    return false;
            }
        }
        
        for(int x = Xmin + 4; x <= Xmax - 4; x++){
            for(int y = getY() - 3; y < getY() + 4; y++){
                if(getWorld()->actorAtCoor(x, y, "Boulder") != -1)
                    return false;
            }
        }
        return true;
    }
    return false;
}
void Protester::setLeaveState(){
    leaveState = true;
}
bool Protester::getLeaveState() const{
    return leaveState;
}
Actor::Direction Protester::changeDirection(int fx, int fy){
    if(getX() - fx == 0){
        if(getY() < fy){
            setDirection(up);
            return up;
        }else{
            setDirection(down);
            return down;
        }
    }else if(getY() - fy == 0){
        if(getX() < fx){
            setDirection(right);
            return right;
        }else{
            setDirection(left);
            return left;
        }
    }
    return getDirection();
}

bool Protester::isPerpendTurn(Direction before, Direction after){
    switch (before) {
        case left:
        case right:
            if(after == down || after == up)
                return true;
            break;
        case up:
        case down:
            if(after == left || after == right)
                return true;
            break;
        default:
            break;
    }
    return false;
}

Actor::Direction Protester::isIntersect(Direction dire){
    switch (dire) {
        case left:
            return getIntersectDire(left, up, down);
            break;
        case right:
            return getIntersectDire(right, up, down);
            break;
        case up:
            return getIntersectDire(up, left, right);
            break;
        case down:
            return getIntersectDire(down, left, right);
            break;
        default:
            break;
    }
    return dire;
}

Actor::Direction Protester::getIntersectDire(Direction cur, Direction a, Direction b){
    int i = random(2);
    if(noBlockOnDire(getX(), getY(), a) && !(noBlockOnDire(getX(), getY(), b))){
        return a;
    }else if(!noBlockOnDire(getX(), getY(), a) && noBlockOnDire(getX(), getY(), b)){
        return b;
    }else if(noBlockOnDire(getX(), getY(), a) && noBlockOnDire(getX(), getY(), b)){
        if(i == 0)
            return a;
        else
            return b;
    }else{
        return cur;
    }
}

void Protester::afterPickGold(){
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    stunState = true;
    
}

int Protester::pathToDes(int fx, int fy, int x, int y){
    queue<int> xque;
    queue<int> yque;
    path.clear();
    xque.push(x);
    yque.push(y);
    bool find = false;

    while(!xque.empty()){
        int curx = xque.front();
        int cury = yque.front();
        xque.pop();
        yque.pop();
        
        if(noBlockOnDire(curx, cury, left) && !isVisited(curx, cury, left)){
            xque.push(curx-1);
            yque.push(cury);
            recordPre[cury][curx-1].x = curx;
            recordPre[cury][curx-1].y = cury;
            if(curx-1 == fx && cury == fy) {find = true; break;}
            else setVisited(curx, cury, left);
        }
        if(noBlockOnDire(curx, cury, right) && !isVisited(curx, cury, right)){
            xque.push(curx+1);
            yque.push(cury);
            recordPre[cury][curx+1].x = curx;
            recordPre[cury][curx+1].y = cury;
            if(curx+1 == fx && cury == fy) {find = true; break;}
            else setVisited(curx, cury, right);
        }
        if(noBlockOnDire(curx, cury, down) && !isVisited(curx, cury, down)){
            xque.push(curx);
            yque.push(cury-1);
            recordPre[cury-1][curx].x = curx;
            recordPre[cury-1][curx].y = cury;
            if(curx == fx && cury-1 == fy) {find = true; break;}
            else setVisited(curx, cury, down);
        }
        if(noBlockOnDire(curx, cury, up) && !isVisited(curx, cury, up)){
            xque.push(curx);
            yque.push(cury+1);
            recordPre[cury+1][curx].x = curx;
            recordPre[cury+1][curx].y = cury;
            if(curx == fx && cury+1 == fy) {find = true; break;}
            else setVisited(curx, cury, up);
        }
        
    }
    
    for(int i = 0; i < 64; i++)
        for(int j = 0; j < 64; j++)
            visited[i][j] = false;
    
    if(!find) return -1;
    int i = fy, j = fx, m, n;
    while(i != y || j != x){
        Node tmp;
        m = i;
        n = j;
        i = recordPre[m][n].y;
        j = recordPre[m][n].x;
        tmp.x = j;
        tmp.y = i;
        path.push_back(tmp);
    }
    return int(path.size());
}

void Protester::setVisited(int x, int y, Direction dire){
    if(x < 0 || x > 64 || y < 0 || y > 64) return;
    switch (dire) {
        case left:
                visited[y][x-1] = true;
            break;
        case right:
                visited[y][x+1] = true;
            break;
        case down:
                visited[y-1][x] = true;
            break;
        case up:
                visited[y+1][x] = true;
            break;
        default:
            break;
    }
}

bool Protester::isVisited(int x, int y, Direction dire){
    switch (dire) {
        case left:
            if(visited[y][x-1]) return true;
            break;
        case right:
            if(visited[y][x+1]) return true;
            break;
        case down:
            if(visited[y-1][x]) return true;
            break;
        case up:
            if(visited[y+1][x]) return true;
            break;
        default:
            break;
    }
    return false;

}

Protester::Node Protester::kthNodeToDes(int k){
    return path[k];
}

void Protester::setTicks(){
    ticksWait = max(0, 3 - current_level_number/4);
}


void Protester::leaveOilField(){
    int x = getX();
    int y = getY();
    int nextx = kthNodeToDes(stepsToExit).x;
    int nexty = kthNodeToDes(stepsToExit).y;
    if(nextx < x && nexty == y){
        setDirection(left);
    }else if(nextx > x && nexty == y){
        setDirection(right);
    }else if(nextx == x && nexty < y){
        setDirection(down);
    }else if(nextx == x && nexty > y){
        setDirection(up);
    }
    stepsToExit++;
    moveTo(nextx, nexty);
    ticksWait = max(0, 3 - current_level_number/4);
}
// Class Hardcore
Hardcore::Hardcore(StudentWorld*world, int HP, int ID)
: Protester(world, HP, ID){
    setName("Hardcore");
    M = 16 + world->getLevel() * 2;
}

void Hardcore::doSomething(){
    if(getStatus()) return;
    
    if(!doneWhenRest()){
        doWhenNotRest();
    }
}

bool Hardcore::phoneSearch(){
    int fx = getWorld()->getFrackmanX();
    int fy = getWorld()->getFrackmanY();
    int x = getX();
    int y = getY();
    double distance =sqrt(pow(x - fx, 2) + pow(y - fy, 2));
    if(distance > M) return false;

    
    int dist = pathToDes(x, y, fx, fy);
    if(dist != -1 && dist <= M){
        int nextx = kthNodeToDes(0).x;
        int nexty = kthNodeToDes(0).y;
        if(nextx < x && nexty == y){
            setDirection(left);
        }else if(nextx > x && nexty == y){
            setDirection(right);
        }else if(nextx == x && nexty < y){
            setDirection(down);
        }else if(nextx == x && nexty > y){
            setDirection(up);
        }
        moveTo(nextx, nexty);
        setTicks();
        return true;
    }else
        return false;
    return false;
}
void Hardcore::givePointsOnDeath(){
    getWorld()->increaseScore(250);
}
