//
//  DCB.cpp
//  TEC_Process
//
//  Created by Jiajun Chen on 15/11/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#include "DCB.hpp"

using namespace std;


dcb_struct::dcb_struct(){
    int PrnNO = 0;
    string site="";
    double dcb = 0.0;
}

//read CODE DCB
bool readDCBFile(std::string filename, map<int,double>& satDcb){
    ifstream infile;
    infile.open(filename.c_str());
    
    //infile.open(v_ObsFileName.at(obsnum).c_str());
    if (!infile.is_open())
    {
        cout<<"Fail to open "<<filename<<endl;
        return false;
    }
    string Line;
    int PRN = 0;
    double sdcb = 0.0;
    while(getline(infile,Line)){
        if (Line.find("PRN / STATION NAME")!= string::npos){
            getline(infile, Line);
            while (getline(infile, Line)) {
                if (Line.size() < 36) {
                    continue;
                }
                if(Line.substr(0,1) == "G" && Line.substr(1,2) !=""){
                    PRN = atoi(Line.substr(1,2).c_str());
                    sdcb = atof(Line.substr(26,9).c_str());
                    satDcb[PRN] = sdcb;
                }
                else if(Line.substr(0,1) == "R" && Line.substr(1,2) !=""){
                    break;
                }
                else{
                    cout << Line << endl;
                    continue;
                }
            }
            break;
        }
    }
    infile.close();
    if (satDcb.size() == 0) {
        return false;
    }
    return true;
}

bool getC1P2DCB(){
    if (P1P2SatDCB.size()==0 || P1C1SatDCB.size()==0) {
        return false;
    }
    for (int i = 1; i < 33; ++i) {
        if (P1C1SatDCB.count(i) > 0 && P1P2SatDCB.count(i) > 0) {
            C1P2SatDCB[i] = -(P1C1SatDCB[i] - P1P2SatDCB[i]);
        }
    }
    if (C1P2SatDCB.size() == 0) {
        return false;
    }
    return true;
}

double CalculateDCB(double& GIMVTEC, IPPStruct& ipp, double& SDCB){
    double GIMSTEC = GIMVTEC / cos(Dre2Rad * ipp.zenith);
    double IPPSTEC = ipp.stec;
    //STEC caused by receiver DCB, unit: TECU
    double RDCB = (IPPSTEC - GIMSTEC) - SDCB * 1e-9 * (IONO_FACTOR * LIGHT_SPEED);
    
    return RDCB;
}

bool WriteRDCB(string filename, vector<dcb_struct>& dcb){
    //sort data
    dcb_struct dcb_one;
    int prn_t = 0;
    //sorted data
    vector< vector<dcb_struct> > dcb_sort;
    //dcb_sort.reserve(GPSPRNNUM);
    dcb_sort.resize(GPSPRNNUM);
    
    unsigned long count = dcb.size();
    for (int i = 0; i < count; ++i) {
        prn_t = dcb[i].PrnNO;
        dcb_sort[prn_t].push_back(dcb[i]);
    }
    //write to file
    ofstream fout(filename.c_str(),ios::out);
    fout.setf(ios_base::adjustfield,ios_base::left);
    fout<<"PRN    TIME           DCB(ns)"<<endl;
    //////////////////////////////////////////////////////////////////////////
    fout.setf(ios_base::adjustfield,ios_base::right);
    fout.setf(ios_base::fixed,ios_base::floatfield);
    
    unsigned long dcb_prn_size = 0;
    for (int i = 0; i < GPSPRNNUM; ++i) {
        dcb_prn_size = dcb_sort[i].size();
        if (dcb_prn_size==0) {
            continue;
        }
        for (int j = 0; j < dcb_prn_size; ++j) {
            dcb_one = dcb_sort[i][j];
            fout << "PRN" << setw(2) << setfill('0')
            << dcb_one.PrnNO << "  " 
            << setw(2) << dcb_one.ctime.hour << ":"
            << setw(2) << dcb_one.ctime.minute<< ":"
            << setw(5) << setprecision(2) << dcb_one.ctime.second << setfill(' ')
            << setw(11) << setprecision(4) << dcb_one.dcb << endl;
        }
        fout << endl;
    }
    fout.close();
    
    return true;
}

bool WriteRDCBall(string filename, vector<dcb_struct>& dcb){
    unsigned long count = dcb.size();
    ofstream fout(filename.c_str(),ios::out);
    fout.setf(ios_base::adjustfield,ios_base::left);
    fout<<"EPOCH    PRN           DCB(ns)"<<endl;
    //////////////////////////////////////////////////////////////////////////
    fout.setf(ios_base::adjustfield,ios_base::right);
    fout.setf(ios_base::fixed,ios_base::floatfield);
    
    dcb_struct dcb_one;
    int epochNum = 0;
    TimeCalendar start = dcb[0].ctime;
    start.hour = 0;
    start.minute = 0;
    start.second = 0;
    double refAllseconds = ti_getAllSecond(start);
    double tempAllseconds = 0.0;
    for (int i = 0; i < count; ++i) {
        dcb_one = dcb[i];
        tempAllseconds = ti_getAllSecond(dcb_one.ctime);
        epochNum = int((tempAllseconds - refAllseconds + 0.0001) / INTERVAL);
        
        //calculate epoch number;
        fout << setw(2) << setfill('0') << epochNum << "    "
        << "PRN"  << setw(2) << dcb_one.PrnNO << "  "
        << setfill(' ') << setw(11) << setprecision(4) << dcb_one.dcb << endl;
        
    }
    
    return true;
}
