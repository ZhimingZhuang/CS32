//
//  testWeightMap.cpp
//  hw1
//
//  Created by ZhimingZhuang on 1/17/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

//
//  main.cpp
//  hw1
//
//  Created by ZhimingZhuang on 1/14/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "WeightMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    WeightMap wm;
    wm.enroll("Jimmy", 145);
    assert(!wm.enroll("Jimmy", 145));
    wm.enroll("Jay", 155);
    
    ValueType startWeight;
    startWeight = wm.weight("Jimmy");
    assert(startWeight == 145);
    assert(!wm.adjustWeight("John", 20));
    assert(!wm.adjustWeight("Jay", -220));
    
    wm.adjustWeight("Jay", -20);
    startWeight = wm.weight("Jay");
    assert(startWeight == 135);
    assert(wm.size() == 2);
    wm.print();
}

