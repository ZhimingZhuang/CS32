//
//  Map.cpp
//  hw1
//
//  Created by ZhimingZhuang on 1/14/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "newMap.h"
#include <cstdlib>

Map::Map(int maxsize){
    if(maxsize < 0)
        exit(0);
    mymap = new KeyValue[maxsize];
    mapsize = 0;
    maxitems = maxsize;
}

Map::Map(const Map &mp){
    mapsize = mp.mapsize;
    maxitems = mp.maxitems;
    mymap = new KeyValue[maxitems];
    for(int i = 0; i < maxitems; i++){
        mymap[i].mapkey = mp.mymap[i].mapkey;
        mymap[i].mapvalue = mp.mymap[i].mapvalue;
    }
}

Map& Map::operator= (const Map &src){
    if(&src == this)
        return *this;
    delete [] mymap;
    mapsize = src.mapsize;
    maxitems = src.maxitems;
    mymap = new KeyValue[maxitems];
    for(int i = 0; i < maxitems; i++){
        mymap[i].mapkey = src.mymap[i].mapkey;
        mymap[i].mapvalue = src.mymap[i].mapvalue;
    }
    return *this;
}

Map::~Map(){
    delete [] mymap;
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
    if(mapsize >= maxitems)
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
    }else if(i < maxitems){
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
    KeyValue *tmpmap = mymap;
    mymap = other.mymap;
    other.mymap = tmpmap;
    
    int tmpmapsize = mapsize;
    mapsize = other.mapsize;
    other.mapsize = tmpmapsize;
    
    int tmpmaxitems = maxitems;
    maxitems = other.maxitems;
    other.maxitems = tmpmaxitems;
    
}