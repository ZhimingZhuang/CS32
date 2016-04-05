//
//  Map.cpp
//  hw1
//
//  Created by ZhimingZhuang on 1/14/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "Map.h"

Map::Map(){
    mapsize = 0;
}

bool Map::empty() const{
    if (mapsize == 0)
        return true;
    else
        return false;
}


int Map::size() const{
    return mapsize;
}

bool Map::insert(const KeyType& key, const ValueType& value){
    if(mapsize >= DEFAULT_MAX_ITEMS)
        return false;
    else{
        int i;
        for(i = 0; i < mapsize; i++)
            if(mymap[i].mapkey == key)    break;
        
        if(i == mapsize){
            mymap[i].mapkey = key;
            mymap[i].mapvalue = value;
            mapsize++;
            return true;
        }else{
            return false;
        }
    }

}

bool Map::update(const KeyType& key, const ValueType& value){
    int i;
    for(i = 0; i < mapsize; i++)
        if(mymap[i].mapkey == key)    break;
    
    if(i < mapsize){
        mymap[i].mapvalue = value;
        return true;
    }else{
        return false;
    }
}


bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    int i;
    for(i = 0; i < mapsize; i++)
        if(mymap[i].mapkey == key)    break;
    if(i < mapsize){
        mymap[i].mapvalue = value;
        return true;
    }else if(i < DEFAULT_MAX_ITEMS){
        mymap[i].mapkey = key;
        mymap[i].mapvalue = value;
        mapsize++;
        return true;
    }else{
        return false;
    }
}

bool Map::erase(const KeyType& key){
    int i;
    for(i = 0; i < mapsize; i++)
        if(mymap[i].mapkey == key)    break;
    if(i < mapsize){
        for(int j = i+1; j < mapsize; j++){
            mymap[j-1].mapkey = mymap[j].mapkey;
            mymap[j-1].mapvalue = mymap[j].mapvalue;
        }
        mapsize--;
        return true;
    }else{
        return false;
    }
}


bool Map::contains(const KeyType& key) const{
    int i;
    for(i = 0; i < mapsize; i++)
        if(mymap[i].mapkey == key)    break;
    if(i < mapsize)
        return true;
    else
        return false;
}

bool Map::get(const KeyType& key, ValueType& value) const{
    int i;
    for(i = 0; i < mapsize; i++)
        if(mymap[i].mapkey == key)    break;
    if(i < mapsize){
        value = mymap[i].mapvalue;
        return true;
    }else{
        return false;
    }
}

bool Map::get(int i, KeyType& key, ValueType& value) const{
    if(i >= 0 && i < mapsize){
        key = mymap[i].mapkey;
        value = mymap[i].mapvalue;
        return true;
    }else{
        return false;
    }
}


void Map::swap(Map& other){
    int maxcount = std::max(mapsize,other.mapsize);
    KeyType tmpkey;
    ValueType tmpvalue;
    int tmpsize;
    for(int i = 0 ; i < maxcount; i++){
        tmpkey = mymap[i].mapkey;
        mymap[i].mapkey = other.mymap[i].mapkey;
        other.mymap[i].mapkey = tmpkey;
        
        tmpvalue = mymap[i].mapvalue;
        mymap[i].mapvalue = other.mymap[i].mapvalue;
        other.mymap[i].mapvalue = tmpvalue;
    }
    tmpsize = mapsize;
    mapsize = other.mapsize;
    other.mapsize = tmpsize;
}