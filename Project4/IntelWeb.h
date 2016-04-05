//
//  IntelWeb.h
//  proj4
//
//  Created by ZhimingZhuang on 3/6/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include <string>
#include <vector>
#include <map>


bool operator<(const InteractionTuple& l, const InteractionTuple& r);

class IntelWeb
{
public:
    IntelWeb();
    ~IntelWeb();
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    bool openExisting(const std::string& filePrefix);
    void close();
    bool ingest(const std::string& telemetryFile);
    unsigned int crawl(const std::vector<std::string>& indicators,
                       unsigned int minPrevalenceToBeGood,
                       std::vector<std::string>& badEntitiesFound,
                       std::vector<InteractionTuple>& interactions
                       );
    bool purge(const std::string& entity);
private:
    unsigned int frequency(const std::string& item);
    
    DiskMultiMap fromMap;
    DiskMultiMap toMap;
    DiskMultiMap preMap;
    std::map<std::string, unsigned int> prevalence;
    // Your private member declarations will go here
};

#endif // INTELWEB_H_

