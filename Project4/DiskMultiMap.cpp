//
//  DiskMultiMap.cpp
//  proj4
//
//  Created by ZhimingZhuang on 3/5/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//
//

#include "DiskMultiMap.h"
#include <functional>
#include <string>

DiskMultiMap::DiskMultiMap(){
}


DiskMultiMap::~DiskMultiMap(){
    close();
}


bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets){
    close();
    
    bool createSuccess = bf.createNew(filename);
    if (!createSuccess)
        return false;
    
    myfilename = filename;
    
    unsigned int diskSize = 0;
    BinaryFile::Offset headDel = 0;
    bf.write(numBuckets, 0);
    bf.write(diskSize, sizeof(numBuckets));
    bf.write(headDel, sizeof(numBuckets) + sizeof(diskSize));
    
    BinaryFile::Offset bucketAddr = sizeof(numBuckets) + sizeof(diskSize) + sizeof(headDel);
    BinaryFile::Offset tmp = 0;
    
    for(int i = 0; i < numBuckets; i++){
        bucketAddr += i*sizeof(BinaryFile::Offset);
        bf.write(tmp, bucketAddr);
    }
    return true;
}

bool DiskMultiMap::openExisting(const std::string& filename){
    close();
    
    bool openSuccess = bf.openExisting(filename);
    if(!openSuccess)
        return false;
    
    myfilename = filename;
    
    return true;
}

void DiskMultiMap::close(){
    bf.close();
}


unsigned int DiskMultiMap::hashFun(const std::string& s){
    unsigned int numOfBuckets;
    bf.read(numOfBuckets, 0);
    std::hash<std::string> hash_fn;
    return hash_fn(s) % numOfBuckets;

}
bool DiskMultiMap::matchData(const char* nodedata, const char* data){
    int i = 0;
    while(nodedata[i] != '\0'){
        if(data[i] != nodedata[i])
            return false;
        i++;
    }
    return true;
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context){
    if(key.size() > 120 || value.size() > 120 || context.size() > 120)
        return false;
    unsigned int n = hashFun(key);
    unsigned int numOfBuckets;
    bf.read(numOfBuckets, 0);
    
    BinaryFile::Offset nodeStart = sizeof(unsigned int) + sizeof(unsigned int) + sizeof(BinaryFile::Offset) + numOfBuckets * sizeof(BinaryFile::Offset);
    unsigned int diskSize;
    bf.read(diskSize, sizeof(unsigned int));
    BinaryFile::Offset headDel;
    bf.read(headDel, sizeof(unsigned int) + sizeof(unsigned int));
    
    BinaryFile::Offset bucketHead;
    BinaryFile::Offset bucketAddr = sizeof(unsigned int) + sizeof(unsigned int) + sizeof(BinaryFile::Offset) + n * sizeof(BinaryFile::Offset);
    
    bf.read(bucketHead, bucketAddr);
    
    BinaryFile::Offset prehead = bucketHead;
    
    if(headDel == 0){
        bucketHead = nodeStart + diskSize*sizeof(DiskNode);
    }else{

        BinaryFile::Offset cur = headDel;
        DiskNode tmp;
        bf.read(tmp, headDel);
        headDel = tmp.next;
        bf.write(headDel, sizeof(unsigned int) + sizeof(unsigned int));
        bucketHead = cur;
    }
    bf.write(bucketHead, bucketAddr);
    
    DiskNode newNode;
    newNode.next = prehead;
    newNode.pos = bucketHead;
    memcpy(newNode.key, key.c_str(), sizeof(char)*(key.size()+1));
    memcpy(newNode.value, value.c_str(), sizeof(char)*(value.size()+1));
    memcpy(newNode.context, context.c_str(), sizeof(char)*(context.size()+1));
    bf.write(newNode, bucketHead);
    
    diskSize++;
    bf.write(diskSize, sizeof(unsigned int));
    return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key){
    unsigned int n = hashFun(key);
    BinaryFile::Offset bucketAddr = sizeof(unsigned int) + sizeof(unsigned int) + sizeof(BinaryFile::Offset) + n * sizeof(BinaryFile::Offset);
    BinaryFile::Offset bucketHead;
    bf.read(bucketHead, bucketAddr);
    Iterator it(myfilename, bucketHead);
    return it;
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context){
    unsigned int n = hashFun(key);
    BinaryFile::Offset bucketAddr = sizeof(unsigned int) + sizeof(unsigned int) + sizeof(BinaryFile::Offset) + n * sizeof(BinaryFile::Offset);
    BinaryFile::Offset bucketHead, headDel;
    bf.read(bucketHead, bucketAddr);
    
    if(bucketHead == 0)
        return 0;
    
    int eraseSize = 0;
    unsigned int diskSize;
    bf.read(diskSize, sizeof(unsigned int));
    bf.read(headDel, sizeof(unsigned int) + sizeof(unsigned int));
    
    DiskNode cur;
    bf.read(cur, bucketHead);
    
    // if we have to delete the first node of the bucket which means that we need to bucketHead
    while(matchData(cur.value, value.c_str()) && matchData(cur.context, context.c_str())){
        BinaryFile::Offset newBucketHead = cur.next;
        bf.write(newBucketHead, bucketAddr);
        
        if(headDel == 0)
            cur.next = 0;
        else
            cur.next = headDel;
        
        bf.write(cur, bucketHead);
        headDel = bucketHead;
        bf.write(headDel, sizeof(unsigned int) + sizeof(unsigned int));
        diskSize--;
        eraseSize++;
        if(newBucketHead == 0){
            bf.write(diskSize, sizeof(unsigned int));
            return eraseSize;
        
        }
        
        DiskNode next;
        bf.read(next, newBucketHead);
        cur = next;
    }

    DiskNode pre = cur;
    bf.read(cur, pre.next);
    
    while(pre.next != 0){
        if(matchData(cur.value, value.c_str()) && matchData(cur.context, context.c_str())){
            pre.next = cur.next;
            bf.write(pre, pre.pos);
            
            if(headDel == 0)
                cur.next = 0;
            else
                cur.next = headDel;
            
            bf.write(cur, cur.pos);
            headDel = cur.pos;
            bf.write(headDel, sizeof(unsigned int) + sizeof(unsigned int));
            
            bf.read(cur, pre.next);
            diskSize--;
            eraseSize++;
        }else{
            DiskNode q;
            bf.read(q, cur.next);
            pre = cur;
            cur = q;
        }
    }
    bf.write(diskSize, sizeof(unsigned int));
    
    return eraseSize;
}

/********************** Iterator **********************/
DiskMultiMap::Iterator::Iterator(const std::string& filename, const BinaryFile::Offset addr){

    itfilename = filename;
    pos = addr;
    if(addr != 0)
        valid = true;
    else
        valid = false;
    
    bf.close();
    bf.openExisting(itfilename);
}

DiskMultiMap::Iterator::Iterator(){

}

DiskMultiMap::Iterator::Iterator(const Iterator& other){
    pos = other.pos;
    itfilename = other.itfilename;
    valid = other.valid;
    
    bf.close();
    bf.openExisting(itfilename);

}

DiskMultiMap::Iterator::~Iterator(){
    bf.close();
}

bool DiskMultiMap::Iterator::isValid() const{
    return valid;
}

DiskMultiMap::Iterator&  DiskMultiMap::Iterator::operator++(){

    if(valid){
        DiskNode tmp;
        bf.read(tmp, pos);
        pos = tmp.next;
        if(pos == 0)
            valid = false;
    
    }
    return (*this);
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator=(const Iterator & src){
    if(&src == this)
        return (*this);
    this->itfilename = src.itfilename;
    this->valid = src.valid;
    this->pos = src.pos;
    this->bf.close();
    this->bf.openExisting(this->itfilename);
    return (*this);
}
MultiMapTuple DiskMultiMap::Iterator::operator*(){
    
    MultiMapTuple x;
    DiskNode tmp;
    bf.read(tmp, pos);
    x.key = tmp.key;
    x.value = tmp.value;
    x.context = tmp.context;
    return x;
}
