//
//  DiskMultiMap.h
//  proj4
//
//  Created by ZhimingZhuang on 3/5/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator();
        // You may add additional constructors
        Iterator(const std::string& filename, const BinaryFile::Offset addr);
        Iterator(const Iterator& other);
        ~Iterator();
        
        bool isValid() const;
        Iterator& operator++();
        Iterator& operator=(const Iterator & src);
        MultiMapTuple operator*();
        
    private:
        // Your private member declarations will go here
        BinaryFile bf;
        BinaryFile::Offset pos;
        bool valid;
        string itfilename;
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    // Your private member declarations will go here
    unsigned int hashFun(const std::string& s);
    bool matchData(const char* nodedata, const char* data);
    
    struct DiskNode {
        char key[120+1];
        char value[120+1];
        char context[120+1];
    
        BinaryFile::Offset pos;
        BinaryFile::Offset next;    // instead of a DiskNode *
    };
    std::string myfilename;
    BinaryFile bf;

};

#endif // DISKMULTIMAP_H_
