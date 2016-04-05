//
//  landmark.cpp
//  hw3
//
//  Created by ZhimingZhuang on 2/4/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

class Landmark{
public:
    virtual string color() const = 0;
    virtual string icon() const = 0;
    string name() const{
        return myname;
    }
    
    Landmark(string name){
        myname = name;
    }
    virtual ~Landmark(){
    }
    
private:
    string myname;
};


class Hotel:public Landmark{
public:
    Hotel(string name):Landmark(name){
        myname = name;
    }
    
    virtual ~Hotel(){
        cout<<"Destroying the hotel "<<myname<<endl;
    }
    
    virtual string color() const{
        return "yellow";
    }
    virtual string icon() const{
        return "bed";
    }
    
    
private:
    string myname;
};

class Restaurant:public Landmark{
public:
    Restaurant(string name, int capacity):Landmark(name){
        myname = name;
        mycapacity = capacity;
    }
    
    virtual ~Restaurant(){
        cout<<"Destroying the restaurant "<<myname<<endl;
    }
    
    virtual string color() const{
        return "yellow";
    }
    virtual string icon() const{
        if (mycapacity < 40) {
            return "small knife/fork";
        }else{
            return "large knife/fork";
        }
    }
    
    
private:
    string myname;
    int mycapacity;
};

class Hospital:public Landmark{
public:
    Hospital(string name):Landmark(name){
        myname = name;
    }
    
    virtual ~Hospital(){
        cout<<"Destroying the hospital "<<myname<<endl;
    }
    
    virtual string color() const{
        return "blue";
    }
    virtual string icon() const{
        return "H";
    }
    
    
private:
    string myname;
};

