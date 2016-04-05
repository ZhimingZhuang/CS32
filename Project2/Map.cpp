//
//  Map.cpp
//  Proj2
//
//  Created by ZhimingZhuang on 1/20/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "Map.h"

Map::Map(){
    head = nullptr;
    tail = nullptr;
    mapsize = 0;
}

Map::~Map(){
    KeyValue* p = head;
    while(p != nullptr){
        KeyValue* n = p->next;
        delete p;
        p = n;
    }
}

Map::Map(const Map& src){
    mapsize = src.mapsize;
    KeyValue* p = src.head;
    if(p == nullptr){
        head = nullptr;
    }else{
        
        KeyValue* q1;
        KeyValue* q2 = nullptr;
        
        while(p != nullptr){
            
            q1 = new KeyValue;
            q1->mapkey = p->mapkey;
            q1->mapvalue = p->mapvalue;
            q1->pre = q2;
            
            if(p == src.head)
                head = q1;
            
            if(q2 != nullptr)
                q2->next = q1;
            q2 = q1;
            
            p = p->next;
        }
        
        q2->next = nullptr;
        tail = q2;
    }
    
}

Map& Map::operator= (const Map &src){
    if(&src == this)
        return *this;
    
    KeyValue* s = head;
    while(s != nullptr){
        KeyValue* n = s->next;
        delete s;
        s = n;
    }
    
    mapsize = src.mapsize;
    
    KeyValue* p = src.head;
    if(p == nullptr){
        head = nullptr;
    }else{
        
        KeyValue* q1;
        KeyValue* q2 = nullptr;
        
        while(p != nullptr){
            
            q1 = new KeyValue;
            q1->mapkey = p->mapkey;
            q1->mapvalue = p->mapvalue;
            q1->pre = q2;
            
            if(p == src.head)
            head = q1;
            
            if(q2 != nullptr)
            q2->next = q1;
            q2 = q1;
            
            p = p->next;
        }
        
        q2->next = nullptr;
        tail = q2;
    }
    return *this;
}

bool Map::empty() const{
    if(mapsize == 0)
        return true;
    else
        return false;
}

int Map::size() const{
    return mapsize;
}

bool Map::insert(const KeyType& key, const ValueType& value){
    return doInsertOrUpdate(key, value, true /* insert */, false /* no update */);
}

bool Map::update(const KeyType& key, const ValueType& value){
    return doInsertOrUpdate(key, value, false /* no insert */, true /* update */);
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    return doInsertOrUpdate(key, value, true /* insert */, true /* update */);
}

bool Map::erase(const KeyType& key){
    KeyValue *cur = find(key);
    if(cur == nullptr){
        return false;
    }
    
    if(cur == head){
        if(cur->next != nullptr){
            head = cur->next;
            head->pre = nullptr;
            delete cur;
            mapsize--;
        }else{
            delete cur;
            head = nullptr;
            tail = nullptr;
            mapsize--;
        }
    }else if(cur == tail){
        tail = cur->pre;
        tail->next = nullptr;
        delete cur;
        mapsize--;
    }else{
        cur->pre->next = cur->next;
        cur->next->pre = cur->pre;
        delete cur;
        mapsize--;
    }

    return true;
}

bool Map::contains(const KeyType& key) const{
    return find(key) != nullptr;
}

bool Map::get(const KeyType& key, ValueType& value) const{
    KeyValue* cur = find(key);
    if(cur == nullptr)
        return false;
    value = cur->mapvalue;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const{
    if(i < 0 || i >= mapsize)
        return false;
    KeyValue* p = head;
    for(int index = 1; index <= i; index++){
        p = p->next;
    }
    
    key = p->mapkey;
    value = p->mapvalue;
    return true;
}

void Map::swap(Map& other){
    KeyValue* tmp = head;
    head = other.head;
    other.head = tmp;
    
    tmp = tail;
    tail = other.tail;
    other.tail = tmp;
    
    int tmpsize = mapsize;
    mapsize = other.mapsize;
    other.mapsize = tmpsize;
}

Map::KeyValue* Map::find(const KeyType& key) const{
    KeyValue* p = head;
    while(p != nullptr){
        if(p->mapkey == key)
            break;
        p = p->next;
    }
    return p;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,bool mayInsert, bool mayUpdate){
    KeyValue* p = find(key);
    if(p != nullptr){
        if(mayUpdate){
            p->mapvalue = value;
        }
        return mayUpdate;
    }
    if(!mayInsert)
        return false;
    //insert
    if(head == nullptr){
        KeyValue* cur = new KeyValue;
        cur->mapkey = key;
        cur->mapvalue = value;
        
        cur->next = nullptr;
        cur->pre = nullptr;
        head = cur;
        tail = cur;
        mapsize++;
    }else{
        KeyValue* cur = new KeyValue;
        cur->mapkey = key;
        cur->mapvalue = value;
        
        tail->next = cur;
        cur->pre = tail;
        cur->next = nullptr;
        tail = cur;
        mapsize++;
    }
    return true;

}

bool combine(const Map& m1, const Map& m2, Map& result){
    KeyType tmpkey;
    ValueType tmpvalue1, tmpvalue2;
    bool flag = true;
    
    result = m1;
    for(int i = 0; i < m2.size(); i++){
        m2.get(i, tmpkey, tmpvalue1);
        
        if(!result.get(tmpkey, tmpvalue2)){
            result.insert(tmpkey, tmpvalue1);
        }else{
            if(tmpvalue1 != tmpvalue2){
                result.erase(tmpkey);
                flag = false;
            }
        }
        
    }
    return flag;
}

void subtract(const Map& m1, const Map& m2, Map& result){
    KeyType tmpkey;
    ValueType tmpvalue;
    result = m1;
    
    for(int i = 0; i < result.size(); i++){
        result.get(i, tmpkey, tmpvalue);
        if(m2.contains(tmpkey)){
            result.erase(tmpkey);
            i--;
        }
    }
}




