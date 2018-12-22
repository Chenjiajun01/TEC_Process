//
//  SubpartTEC.cpp
//  DCB
//
//  Created by Jiajun Chen on 01/06/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#include "SubpartTEC.hpp"

bool GetPartTEC(string filename, SubTECParameter& parameter, vector<IPPStruct> IPP_piece)
{
    vector<IPPStruct> allIPP;
    vector<IPPfile> allSortIPP;
    if (false == ReadIPPfile(filename, allIPP)) {
        return false;
    }
    if (false == SortIPPfile(allIPP, allSortIPP)) {
        return false;
    }
    
    double t_allsecond = 0.0;
    
    unsigned long ippEpochNum = allSortIPP.size();
    for (int i = 0 ; i < ippEpochNum; i++){
        t_allsecond = ti_getAllSecond(allSortIPP[i].epoch);
        
    }
    
    
    
    
    
    return true;
}
