//
//  ReceiverDCB.cpp
//  DCB
//
//  Created by Jiajun Chen on 31/05/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#include "ReceiverDCB.hpp"

double CalculateDCB(double& GIMVTEC, IPPStruct& ipp, double& SDCB){
    double GIMSTEC = GIMVTEC / cos(Dre2Rad * ipp.zenith);
    double IPPSTEC = ipp.stec;
    //STEC caused by receiver DCB, unit: TECU
    double RDCB = (IPPSTEC - GIMSTEC) - SDCB * 1e-9 * (IONO_FACTOR * LIGHT_SPEED);
    
    return RDCB;
}
