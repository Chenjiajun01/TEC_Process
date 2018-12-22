//
//  main.cpp
//  DCB
//
//  Created by Jiajun Chen on 31/05/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#include <iostream>
#include "MainProcess.hpp"

int main(int argc, const char * argv[]) {
    
    if(argc==1)  //parameter should be: GIMfile IPPfile DCBdir
    {
        cout<<"Function: Correct receiver DCB of STEC from IPPfile" << endl;
        cout<<"Usage:    TEC_Process GIMfile IPPfile P1P2DCBfile P1C1DCBfile" << endl;
        //cout<<"Inputs:   GIMfile IPPfile" << endl;
        //cout<<"Output:   " << endl;
        
    }
    else{
        string GIMfile = argv[1];
        string IPPfile = argv[2];
        string p1p2file = "";
        string p1c1file = "";
        if (argc == 5) {
            p1p2file = argv[3];
            p1c1file = argv[4];
        }
        
        //DCB correct and output TEC
        Process(GIMfile, IPPfile, p1p2file, p1c1file);
    }
     
    
    //string GIMfile = "/Users/jchen/Documents/Projects/Ionosphere/calif_gps/222/CODG2220.06I";
    //string IPPfile = "/Users/jchen/Documents/Projects/Ionosphere/calif_gps/222/wwmt2220.06IPP";
    //Process(GIMfile, IPPfile);
    
    return 0;
}
