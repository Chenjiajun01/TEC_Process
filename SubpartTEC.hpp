//
//  SubpartTEC.hpp
//  DCB
//
//  Created by Jiajun Chen on 01/06/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#ifndef SubpartTEC_hpp
#define SubpartTEC_hpp

#include <stdio.h>
#include "GIM.hpp"
#include "TimeSystem.h"

struct SubTECParameter{
    TimeCalendar centreTime;
    vector<double> latRange; //Lat0/Lat1/dLat
    vector<double> lonRange;
    vector<double> azimuthRange;
    vector<double> elevationRange;
};



//read a piece of DCB corrected IPP
bool ReadIPPfile(string filename, SubTECParameter& parameter, vector<IPPStruct> IPP_piece);

#endif /* SubpartTEC_hpp */
