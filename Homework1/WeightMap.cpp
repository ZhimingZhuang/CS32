//
//  WeightMap.cpp
//  hw1
//
//  Created by ZhimingZhuang on 1/16/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "WeightMap.h"
#include <iostream>

WeightMap::WeightMap(){

}

bool WeightMap::enroll(KeyType name, ValueType startWeight){
    if(startWeight < 0 || map_weight.contains(name))
        return false;
    else{
        if(map_weight.insert(name, startWeight))
            return true;
        else
            return false;
    }
}

double WeightMap::weight(KeyType name) const{
    ValueType startWeight;
    if(map_weight.get(name, startWeight))
        return startWeight;
    else
        return -1;
}

bool WeightMap::adjustWeight(KeyType name, ValueType amt){
    double totalWeight;
    if (map_weight.get(name, totalWeight)){
        totalWeight = totalWeight + amt;
        if(totalWeight < 0)
            return false;
        else{
            if(map_weight.update(name, totalWeight))
                return true;
            else
                return false;
        }
    }else{
        return false;
    }
}

int WeightMap::size() const{
    return map_weight.size();
}

void WeightMap::print() const{
    KeyType name;
    ValueType startWeight;
    for(int i = 0; i < map_weight.size(); i++){
        if(map_weight.get(i, name, startWeight))
            std::cout<<name<<" "<<startWeight<<std::endl;
    }
}

