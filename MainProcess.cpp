//
//  MainProcess.cpp
//  DCB
//
//  Created by Jiajun Chen on 31/05/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#include "MainProcess.hpp"

bool Process(string GIMfile, string IPPfilename, string p1p2file, string p1c1file){
    
    Ionex GIM;
    vector<IPPStruct> IPP;
    vector<IPPStruct> CorrectIPP;
    
    if (false == ReadGIMMap(GIMfile, GIM)){
        return false;
    }
    if (false == ReadIPPfile(IPPfilename, IPP)) {
        return false;
    }
    
    //read DCB file
    if (p1p2file == "") {
        cout << "No P1P2 DCB file" << endl;
    }
    else if (false == readDCBFile(p1p2file, P1P2SatDCB)) {
        cout << "Fail to read P1P2 DCB file: " << p1p2file << endl;;
    }
    
    if (p1c1file == "") {
        cout << "No P1C1 DCB file" << endl;
    }
    else if (false == readDCBFile(p1c1file, P1C1SatDCB)) {
        cout << "Fail to read P1C1 DCB file: " << p1c1file << endl;;
    }
    if (false == getC1P2DCB()) {
        cout << "No C1P2 DCB, use GIM DCB" << endl;
        C1P2SatDCB = GIMSatDCB;
    }
    
    //correct receiver DCB
    IPPStruct ipp_i;
    double vtec_GIMitp_i = 0.0;
    double rdcb_i = 0.0;
    double rdcb = 0.0;
    double std_rdcb = 0.0;
    vector<double> rdcb_v;
    vector<dcb_struct> rdcb_all;
    dcb_struct rdcb_temp;
    
    unsigned long countIPP = IPP.size();
    for (int i = 0; i < countIPP; i++){
        ipp_i = IPP[i];
        if (C1P2SatDCB.count(ipp_i.PRN) <= 0) {
            cout << "No DCB found for PRN" << ipp_i.PRN << endl;
            continue;
        }
        if (ipp_i.elevation < THRESHOLD_ELEVATION) {
            continue;
        }
        if (false == GIMInterpolation(GIM, ipp_i.ipp, ipp_i.calTime, vtec_GIMitp_i)){
            continue;
        }
        rdcb_i = CalculateDCB(vtec_GIMitp_i, ipp_i, C1P2SatDCB[ipp_i.PRN]);
        rdcb_v.push_back(rdcb_i);
        
        rdcb_temp.ctime = ipp_i.calTime;
        rdcb_temp.PrnNO = ipp_i.PRN;
        rdcb_temp.dcb = rdcb_i * 1e9 / (IONO_FACTOR * LIGHT_SPEED); //sdcb in ns
        rdcb_all.push_back(rdcb_temp);
    }
    rdcb = MeanFunc(rdcb_v);
    std_rdcb = StdFunc(rdcb_v);
    string siteName = IPPfilename.substr(IPPfilename.length()-14, 4);
    cout << "receiver "<< siteName <<" DCB: "<< rdcb*1e9/(IONO_FACTOR * LIGHT_SPEED)<< " ns" << endl;
    cout << "                          STD: " << std_rdcb << " ns" << endl;
    
    rdcb_temp.site = siteName;
    
    //correct IPP STEC
    for (int i = 0; i < countIPP; i++){
        ipp_i = IPP[i];
        if (C1P2SatDCB.count(ipp_i.PRN) <= 0) {
            continue;
        }
        ipp_i.stec = ipp_i.stec - rdcb - C1P2SatDCB[ipp_i.PRN] * 1e-9 * (IONO_FACTOR * LIGHT_SPEED);
        CorrectIPP.push_back(ipp_i);
    }
    
    //sort corrected TEC to IPP file structure
    vector<IPPfile> sortIPP;
    if (false == SortIPPfile(CorrectIPP, sortIPP)){
        return false;
    }
    
    //save to IPP file
    string outfilename = IPPfilename.substr(0,IPPfilename.length()-6) + "-crt" + IPPfilename.substr(IPPfilename.length()-6,6);
    if (false == WriteSortIPPfile(outfilename, sortIPP)){
        return false;
    }
    //save to ipp file
    string outfippilename = IPPfilename.substr(0,IPPfilename.length()-7)+"-crt.ipp";;
    if (false == WriteIPPfile(outfippilename, CorrectIPP)){
        return false;
    }
    
    //save DCB to file
    string outdcbfilename = IPPfilename.substr(0,IPPfilename.length()-3)+".DCB";
    if (false == WriteRDCB(outdcbfilename, rdcb_all)) {
        return false;
    }
    string outdcbfilename2 = IPPfilename.substr(0,IPPfilename.length()-3)+".dcb2";
    if (false==WriteRDCBall(outdcbfilename2, rdcb_all)){
        return false;
    }
    
    return true;
}
