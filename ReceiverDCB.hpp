//
//  ReceiverDCB.hpp
//  DCB
//
//  Created by Jiajun Chen on 31/05/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#ifndef ReceiverDCB_hpp
#define ReceiverDCB_hpp

#include <stdio.h>
#include "GIM.hpp"

double CalculateDCB(double& GIMVTEC, IPPStruct& IPPSTEC, double& SDCB);
//bool FilterDCB(vector<double> rdcb_day, string method);

#endif /* ReceiverDCB_hpp */
