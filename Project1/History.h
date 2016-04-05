//
//  History.h
//  cs32
//
//  Created by ZhimingZhuang on 1/10/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#ifndef __cs32__History__
#define __cs32__History__

class Arena;

class History
{
    public:
    History(int nRows, int nCols);
    ~History();
    bool record(int r, int c);
    void display() const;
   
    
    private:
    int edgerows;
    int edgecols;
    int **gridrecord;
};


#endif /* defined(__cs32__History__) */
