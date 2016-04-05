//
//  main.cpp
//  hw1
//
//  Created by ZhimingZhuang on 1/14/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = {"a", "b", "c", "d", "e", "f"};
    ValueType v  = 123;
    
    // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v));
    Map e,f;
    e = f = b;
    for (int n = 0; n < 5; n++)
        assert(e.contains(k[n]));
    // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k[5], v));
    assert(!e.insert(k[5], v));
    // When two Maps' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(k[5], v)  &&  b.insert(k[5], v));
    for (int n = 0; n < 5; n++)
        assert(a.contains(k[n]));
}

