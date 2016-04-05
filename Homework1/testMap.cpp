//
//  main.cpp
//  hw1
//
//  Created by ZhimingZhuang on 1/14/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map m;  // maps strings to doubles
    assert(m.empty());
    ValueType v = -1234.5;
    assert( !m.get("abc", v)  &&  v == -1234.5); // v unchanged by get failure
    
    m.insert("xyz", 9876.5);
    m.insertOrUpdate("abc", 1111.1);
    m.update("abc", 222);
    assert(m.get("abc", v) && v == 222);
    
    m.insertOrUpdate("abc", 333);
    assert(m.get("abc", v) && v == 333);
    
    assert(m.contains("xyz"));
    m.erase("abc");
    assert(m.size() == 1);
    KeyType k = "hello";
    assert(m.get(0, k, v)  &&  k == "xyz"  &&  v == 9876.5);
    
    Map m1;
    m1.insert("Fred", 2.956);
    Map m2;
    m2.insert("Ethel", 3.538);
    m2.insert("Lucy", 2.956);
    m1.swap(m2);
    assert(m1.size() == 2  &&  m1.contains("Ethel")  &&  m1.contains("Lucy")  &&
           m2.size() == 1  &&  m2.contains("Fred"));
}

