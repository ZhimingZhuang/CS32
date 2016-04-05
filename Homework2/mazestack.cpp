//
//  mazestack.cpp
//  hw2
//
//  Created by ZhimingZhuang on 1/31/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <stack>
#include <iostream>
using namespace std;

class Coord {
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    stack<Coord> stk;
    Coord start(sr,sc);
    maze[sr][sc] = 'X';
    stk.push(start);
    
    
    while(!stk.empty()){
        Coord cur = stk.top();
        stk.pop();
        
        int r = cur.r();
        int c = cur.c();
        cout<<r<<"  "<<c<<endl;
        
        if(r == er && c == ec)
            return true;
        
        if(r-1 >= 0 && maze[r-1][c] == '.'){
            maze[r-1][c] = 'X';
            Coord tmp(r-1, c);
            stk.push(tmp);
        }
        
        
        if(c+1 < nCols && maze[r][c+1] == '.'){
            maze[r][c+1] = 'X';
            Coord tmp(r, c+1);
            stk.push(tmp);
        }
        
        
        if(r+1 < nRows && maze[r+1][c] == '.'){
            maze[r+1][c] = 'X';
            Coord tmp(r+1, c);
            stk.push(tmp);
        }
        
        
        if(c-1 >= 0 && maze[r][c-1] == '.'){
            maze[r][c-1] = 'X';
            Coord tmp(r, c-1);
            stk.push(tmp);
        }
    }
    return false;
    
}
