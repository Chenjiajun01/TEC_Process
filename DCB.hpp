//
//  DCB.hpp
//  TEC_Process
//
//  Created by Jiajun Chen on 15/11/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#ifndef DCB_hpp
#define DCB_hpp

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include "GlobalVariable.hpp"
#include "TimeSystem.h"
#include "GIM.hpp"


class dcb_struct{
public:
    string site;
    int PrnNO;
    TimeCalendar ctime;
    double dcb;
public:
    dcb_struct();
};



bool readDCBFile(std::string filename, map<int,double>& sdcb);

//calculate C1P2 satellite DCB using C1P1 P1P2 DCB
bool getC1P2DCB();

//calculate receiver DCB in TECU
double CalculateDCB(double& GIMVTEC, IPPStruct& IPPSTEC, double& SDCB);

//save dcb sorted by PRN number for each satellite to file
bool WriteRDCB(string filename, vector<dcb_struct>& dcb);

//save dcb sorted by time for each satellite to file
bool WriteRDCBall(string filename, vector<dcb_struct>& dcb);


#endif /* DCB_hpp */
