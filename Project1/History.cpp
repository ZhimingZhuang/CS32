//
//  History.cpp
//  cs32
//
//  Created by ZhimingZhuang on 1/10/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols){
    edgerows = nRows;
    edgecols = nCols;
    gridrecord = new int*[MAXROWS];
    for(int i = 0; i < MAXROWS; i++){
        gridrecord[i] = new int [MAXCOLS];
    }
    
}

History::~History(){
    // Don't forget to delete the memory you create.
    for(int i = 0; i < MAXROWS; i++)
        delete [] gridrecord[i];
    delete []gridrecord;
}

bool History::record(int r, int c){
    if (r < 1  ||  r > edgerows  ||  c < 1  ||  c > edgecols)
        return false;
    else{
        gridrecord[r - 1][c - 1]++;
        return true;
    }
}

void History::display() const{
    
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    clearScreen();
    
    for (r = 0; r < edgerows; r++){
        for (c = 0; c < edgecols; c++){
            grid[r][c] = '.';
            if(gridrecord[r][c] > 0){
                if(gridrecord[r][c] <= 26){
                    grid[r][c] = 'A' + gridrecord[r][c] - 1;
                }else{
                    grid[r][c] = 'A' + 25;
                }
            }
            cout << grid[r][c];
        }
        cout << endl;
    }
    cout << endl;
}
