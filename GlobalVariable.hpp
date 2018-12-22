//
//  GlobalVariable.hpp
//  DCB
//
//  Created by Jiajun Chen on 01/06/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#ifndef GlobalVariable_hpp
#define GlobalVariable_hpp

#include <stdio.h>
#include <map>
#include <string>

using namespace std;

extern double THRESHOLD_ELEVATION;

extern map<int,double> GIMSatDCB;
extern map<int,double> P1P2SatDCB;
extern map<int,double> P1C1SatDCB;
extern map<int,double> C1P2SatDCB;
#endif /* GlobalVariable_hpp */
