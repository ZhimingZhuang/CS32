//
//  IntelWeb.cpp
//  proj4
//
//  Created by ZhimingZhuang on 3/6/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include "IntelWeb.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <queue>
using namespace std;

IntelWeb::IntelWeb(){


}

IntelWeb::~IntelWeb(){
    fromMap.close();
    toMap.close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems){
    fromMap.close();
    toMap.close();

    bool createSuccess = false;
    string filenameFrom = filePrefix + "-from-hash-table.dat";
    string filenameTo = filePrefix + "-to-hash-table.dat";
    string filenamePre = filePrefix + "-pre-hash-table.dat";
    float loadfactor = 0.55;
    unsigned int numOfBuckets = maxDataItems/loadfactor;
    if(fromMap.createNew(filenameFrom, numOfBuckets) && toMap.createNew(filenameTo, numOfBuckets) && preMap.createNew(filenamePre, numOfBuckets))
        createSuccess = true;
    return createSuccess;
}

bool IntelWeb::openExisting(const std::string& filePrefix){
    fromMap.close();
    toMap.close();
    
    string filenameFrom = filePrefix + "-from-hash-table.dat";
    string filenameTo = filePrefix + "-to-hash-table.dat";
    string filenamePre = filePrefix + "-pre-hash-table.dat";
    bool openSuccess = false;
    if(fromMap.openExisting(filenameFrom) && toMap.openExisting(filenameTo) && preMap.openExisting(filenamePre))
        openSuccess = true;
    return true;
}

void IntelWeb::close(){
    fromMap.close();
    toMap.close();
    preMap.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile){
    ifstream inf(telemetryFile);
    if(!inf)
        return false;
    string line;
    while(getline(inf, line)){
        istringstream iss(line);
        string context, key, value;
        if(!(iss >> context >> key >> value))
            continue;
        fromMap.insert(key, value, context);
        toMap.insert(value, key, context);
        prevalence[key]++;
        prevalence[value]++;
    }
    map<string, unsigned int>::iterator it = prevalence.begin();
    
    
    for(; it != prevalence.end(); it++){
        unsigned int tmp = 0;
        DiskMultiMap::Iterator it_preMap = preMap.search(it->first);
        while(it_preMap.isValid()){
            if((*it_preMap).key == it->first){
                tmp = atoi((*it_preMap).value.c_str());
                break;
            }
            ++it_preMap;
        }
        if(it_preMap.isValid())
            preMap.erase((*it_preMap).key, (*it_preMap).value, (*it_preMap).context);
        
        preMap.insert(it->first, to_string(tmp + it->second), "");
        
    }
    return true;

}
bool operator<(const InteractionTuple& l, const InteractionTuple& r){
    if(l.context != r.context)
        return l.context < r.context;
    else if(l.from != r.from)
        return l.from < r.from;
    else
        return l.to < r.to;

}

unsigned int IntelWeb::frequency(const std::string& item){
    DiskMultiMap::Iterator it_preMap = preMap.search(item);
    while(it_preMap.isValid()){
        if((*it_preMap).key == item)
            return atoi((*it_preMap).value.c_str());
        ++it_preMap;
    }
    return 0;
    
}
unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators,
                   unsigned int minPrevalenceToBeGood,
                   std::vector<std::string>& badEntitiesFound,
                   std::vector<InteractionTuple>& interactions){
    
    badEntitiesFound.clear();
    interactions.clear();
    
    set<string> uniqueEntries;
    set<InteractionTuple> uniqueInteractions;
    
    queue<string> que;
    for(int i = 0; i < indicators.size(); i++)
        que.push(indicators[i]);
    
    while(!que.empty()){
        string tmpBad = que.front();
        que.pop();
        
        if(uniqueEntries.find(tmpBad) == uniqueEntries.end()){
            
            DiskMultiMap::Iterator it_fromMap = fromMap.search(tmpBad);
            DiskMultiMap::Iterator it_toMap = toMap.search(tmpBad);
            if((it_fromMap.isValid() || it_toMap.isValid()) && prevalence[tmpBad] < minPrevalenceToBeGood)
                uniqueEntries.insert(tmpBad);
            
            // Search in fromMap
            while(it_fromMap.isValid()){
                if((*it_fromMap).key == tmpBad){
                    string fromBadEntries = (*it_fromMap).value;
                    InteractionTuple fromBadInt;
                    fromBadInt.from = (*it_fromMap).key;
                    fromBadInt.to = fromBadEntries;
                    fromBadInt.context = (*it_fromMap).context;
                    uniqueInteractions.insert(fromBadInt);
                    
                    
                    if(frequency(fromBadEntries) < minPrevalenceToBeGood)
                        que.push(fromBadEntries);
                }
                ++it_fromMap;
            }
            
            // Search in toMap
            while(it_toMap.isValid()){
                if((*it_toMap).key == tmpBad){
                    string toBadEntries = (*it_toMap).value;
                    InteractionTuple toBadInf;
                    toBadInf.from = toBadEntries;
                    toBadInf.to = (*it_toMap).key;
                    toBadInf.context = (*it_toMap).context;
                    uniqueInteractions.insert(toBadInf);
                    
                    if(frequency(toBadEntries) < minPrevalenceToBeGood)
                        que.push(toBadEntries);
                
                }
                ++it_toMap;
            
            }
            
        }
    }
    set<string>::iterator it_entries = uniqueEntries.begin();
    for(;it_entries != uniqueEntries.end(); it_entries++)
        badEntitiesFound.push_back((*it_entries));
    
    set<InteractionTuple>::iterator it_int = uniqueInteractions.begin();
    for(;it_int != uniqueInteractions.end(); it_int++)
        interactions.push_back((*it_int));
    
    unsigned int sizeOfBadEntries = (unsigned int)badEntitiesFound.size();
    
    return sizeOfBadEntries;
}

bool IntelWeb::purge(const std::string& entity){
    bool ispurge = false;
    vector<MultiMapTuple> foundInTo;
    vector<MultiMapTuple> foundInFrom;
    vector<MultiMapTuple> foundInPre;
    map<string, unsigned int> Pre;
    
    DiskMultiMap::Iterator it_fromMap = fromMap.search(entity);
    
    while(it_fromMap.isValid()){
        ispurge = true;
        if((*it_fromMap).key == entity){
            foundInTo.push_back((*it_fromMap));
            
        }
        ++it_fromMap;
    }
    
    DiskMultiMap::Iterator it_toMap = toMap.search(entity);
    while(it_toMap.isValid()){
        ispurge = true;
        if((*it_toMap).key == entity){
            foundInFrom.push_back((*it_toMap));
            Pre[(*it_toMap).key]++;
        }
        ++it_toMap;
    }

    
    for(int i = 0; i < foundInFrom.size(); i++){
        Pre[foundInFrom[i].key]++;
        Pre[foundInFrom[i].value]++;
        fromMap.erase(foundInFrom[i].value, foundInFrom[i].key, foundInFrom[i].context);
        toMap.erase(foundInFrom[i].key, foundInFrom[i].value, foundInFrom[i].context);
    }
    for(int i = 0; i < foundInTo.size(); i++){
        Pre[foundInTo[i].key]++;
        Pre[foundInTo[i].value]++;
        toMap.erase(foundInTo[i].value, foundInTo[i].key, foundInTo[i].context);
        fromMap.erase(foundInTo[i].key, foundInTo[i].value, foundInTo[i].context);
    }
    
    map<string, unsigned int>::iterator it = Pre.begin();
    
    
    for(; it != Pre.end(); it++){
        DiskMultiMap::Iterator it_preMap = preMap.search(it->first);
        int tmp = 0;
        while(it_preMap.isValid()){
            if((*it_preMap).key == it->first){
                tmp = atoi((*it_preMap).value.c_str());
                break;
            }
            ++it_preMap;
        }
        if(it_preMap.isValid())
            preMap.erase((*it_preMap).key, (*it_preMap).value, (*it_preMap).context);
        int newPre = tmp - it->second;
        if(newPre > 0)
            preMap.insert(it->first, to_string(newPre), "");
    }
    
    return ispurge;
}