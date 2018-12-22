//
//  Process.cpp
//  DCB
//
//  Created by Jiajun Chen on 31/05/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#include "GIM.hpp"



bool ReadGIMMap(string ionexfile, Ionex& GIMmap)
{
    Ionex_header header;
    vector<GIM_tec_epoch> vtec;
    
    ifstream infile;
    infile.open(ionexfile.c_str());
    
    //infile.open(v_ObsFileName.at(obsnum).c_str());
    if (!infile.is_open())
    {
        cout<<"Fail to open "<<ionexfile<<endl;
        return false;
    }
    string Line;
    //header
    int nLat = 0; int nLon = 0;
    double Iono_shell_height = 0.0;
    double exponent = 0.1;
    GIMmap.header = header;
    
    //Satellite DCB
    int PRN = 0;
    double sdcb = 0.0;
    
    while (getline(infile, Line))
    {
        if (Line.find("INTERVAL") != string::npos)
        {
            header.Interval = atof(Line.substr(2,4).c_str());
            continue;
        }
        if (Line.find("# OF MAPS IN FILE") != string::npos)
        {
            header.MAP_NUM = atoi(Line.substr(4,3).c_str());
            continue;
        }
        if (Line.find("LAT1 / LAT2 / DLAT") != string::npos)
        {
            header.LAT.push_back(atof(Line.substr(3,5).c_str())); //LAT1
            header.LAT.push_back(atof(Line.substr(9,5).c_str())); //LAT2
            header.LAT.push_back(atof(Line.substr(15,5).c_str())); //dLAT
            nLat = floor((header.LAT[1]-header.LAT[0])/header.LAT[2] +0.5)+1;
            header.nLat = nLat;
            continue;
        }
        if (Line.find("LON1 / LON2 / DLON") != string::npos)
        {
            header.LON.push_back(atof(Line.substr(2,6).c_str())); //LON1
            header.LON.push_back(atof(Line.substr(8,6).c_str())); //LON2
            header.LON.push_back(atof(Line.substr(15,5).c_str())); //dLON
            nLon = floor((header.LON[1]-header.LON[0])/header.LON[2] +0.5)+1;
            header.nLon = nLon;
            continue;
        }
        if (Line.find("HGT1 / HGT2 / DHGT") != string::npos) {
            Iono_shell_height = atof(Line.substr(0,8).c_str());
        }
        if (Line.find("EXPONENT") != string::npos)
        {
            header.Exponent = atoi(Line.substr(4,2).c_str());
            exponent = pow(10, header.Exponent );
            continue;
        }
        if (Line.find("DIFFERENTIAL CODE BIASES")!= string::npos)
        {
            while(getline(infile,Line))
            {
                if (Line.find("PRN / BIAS / RMS")!= string::npos && Line.substr(3,1) == "G")
                { 
                    PRN = atoi(Line.substr(4,2).c_str());
                    sdcb = atof(Line.substr(9,7).c_str());
                    GIMSatDCB[PRN] = sdcb;
                    continue;
                }
                if (Line.find("END OF HEADER")!= string::npos)
                {
                    break;
                }
            }
        }
        if (Line.find("END OF HEADER") != string::npos)
        {
            break;
        }
    }
    //TEC MAPS
    GIM_tec_epoch temp_epoch;
    GIM_tec_lat temp_lat;
    
    while (getline(infile,Line))
    {
        if (Line.find("START OF TEC MAP") != string::npos)
        {
            continue;
        }
        if (Line.find("START OF RMS MAP") != string::npos)
        {
            break;
        }
        //each epoch
        if (Line.find("EPOCH OF CURRENT MAP") != string::npos)
        {
            temp_epoch.ctime.year = atoi(Line.substr(2,4).c_str());
            temp_epoch.ctime.month = atoi(Line.substr(10,2).c_str());
            temp_epoch.ctime.day = atoi(Line.substr(16,2).c_str());
            temp_epoch.ctime.hour = atoi(Line.substr(22,2).c_str());
            temp_epoch.ctime.minute = atoi(Line.substr(28,2).c_str());
            temp_epoch.ctime.second = atof(Line.substr(33,7).c_str());
            temp_epoch.TimeNO = CalToNO(temp_epoch.ctime);
            continue;
        }
        //each lat
        for (int i = 0; i < header.nLat; i++)
        {
            temp_lat.vtec.resize(nLon);
            if (Line.find("LAT/LON1/LON2/DLON/H") != string::npos)
            {
                temp_lat.Lat = atof(Line.substr(3,5).c_str());
                if (nLon <= 16)
                {
                    for (int j = 0; j < nLon; j++)
                    {
                        infile >> temp_lat.vtec[j];
                        temp_lat.vtec[j] *= exponent;
                    }
                    getline(infile, Line);
                }
                if (nLon > 26 && nLon <= 32)
                {
                    for (int j = 0; j < 16; j++)
                    {
                        infile >> temp_lat.vtec[j];
                        temp_lat.vtec[j] *= exponent;
                    }
                    getline(infile,Line);
                    for (int j = 16; j < nLon; j++)
                    {
                        infile >> temp_lat.vtec[j];
                        temp_lat.vtec[j] *= exponent;
                    }
                    getline(infile,Line);
                }
                if (nLon > 32 && nLon <= 48)
                {
                    for (int j = 0; j < 16; j++)
                    {
                        infile >> temp_lat.vtec[j];
                        temp_lat.vtec[j] *= exponent;
                    }
                    getline(infile,Line);
                    for (int j = 16; j < 32; j++)
                    {
                        infile >> temp_lat.vtec[j];
                        temp_lat.vtec[j] *= exponent;
                    }
                    getline(infile,Line);
                    for (int j = 32; j < nLon; j++)
                    {
                        infile >> temp_lat.vtec[j];
                        temp_lat.vtec[j] *= exponent;
                    }
                    getline(infile,Line);
                }
                
                //////
                if (nLon > 64 && nLon <= 80)
                {
                    for (int m = 0; m < 4; m++)
                    {
                        for (int j = 16*m; j < 16*(m+1); j++)
                        {
                            infile >> temp_lat.vtec[j];
                            temp_lat.vtec[j] *= exponent;
                        }
                        getline(infile,Line);
                    }
                    
                    for (int j = 64; j < nLon; j++)
                    {
                        infile >> temp_lat.vtec[j];
                        temp_lat.vtec[j] *= exponent;
                    }
                    getline(infile,Line);
                }
                if (temp_lat.vtec.size() == 0)
                {
                    cout << "Error in reading this epoch!" << endl;
                    continue;
                }
                temp_epoch.vtec.push_back(temp_lat);
                temp_lat.vtec.clear();
                vector<double>().swap(temp_lat.vtec);
            }
            getline(infile,Line);
            
            //for zhangrui GIM
            if (Line.size() < 5)
            {
                getline(infile,Line);
            }
    
        }
        vtec.push_back(temp_epoch);
        vector<GIM_tec_lat>().swap(temp_epoch.vtec);
    }
    
    infile.close();
    
    vtec[header.MAP_NUM-1].TimeNO = vtec[header.MAP_NUM-2].TimeNO + int(header.Interval/30.0); //only when the last arc is the first of next day
    GIMmap.header = header;
    GIMmap.vtec = vtec;
    
    cout << "Ionex file read successfully!" << endl;
    return true;
}

bool ReadIPPfile(string filename, vector<IPPStruct>& ippSite){
    ifstream infile;
    infile.open(filename.c_str());
    
    //infile.open(v_ObsFileName.at(obsnum).c_str());
    if (!infile.is_open())
    {
        cout<<"Fail to open "<<filename<<endl;
        return false;
    }
    string Line;
    //header
    double Iono_shell_height = 0.0;
    while (getline(infile,Line))
    {
        if (Line.find("# HEIGHT OF SHELL") != string::npos) {
            Iono_shell_height = atof(Line.substr(0,8).c_str());
        }
        if (Line.find("END OF HEADER") != string::npos)
        {
            break;
        }
    }
    //data
    int temp_satnum = 0;
    IPPStruct temp_oneTec;
    TimeCalendar temp_ctime;
  
    while (getline(infile,Line))
    {
        if(Line.size() > 17 && (Line.substr(0,4) != " " && Line.substr(4,1) == " " && Line.substr(5,2) != " " \
                                && Line.substr(7,1) == " " && Line.substr(8,2) != " " && Line.substr(10,1) == " " && Line.substr(11,2) != " " \
                                && Line.substr(13,1) == " " && Line.substr(14,2) != " "))
        {
            temp_ctime.year = atoi(Line.substr(0,4).c_str());
            temp_ctime.month = atoi(Line.substr(5,2).c_str());
            temp_ctime.day = atoi(Line.substr(8,2).c_str());
            temp_ctime.hour = atoi(Line.substr(11,2).c_str());
            temp_ctime.minute = atoi(Line.substr(14,2).c_str());
            temp_ctime.second = atof(Line.substr(21,4).c_str());
            temp_satnum = atoi(Line.substr(30,2).c_str());
        }
        for (int i = 0; i < temp_satnum; i++)
        {
            getline(infile,Line);
            if (Line.size() < 5) //empty line
            {
                continue;
            }
            
            temp_oneTec.PRN = atoi(Line.substr(3,2).c_str());
            temp_oneTec.stec = atof(Line.substr(60,7).c_str());
            //IPP
            temp_oneTec.ipp.B = atof(Line.substr(7,12).c_str());
            temp_oneTec.ipp.L = atof(Line.substr(19,12).c_str());
            temp_oneTec.ipp.H = Iono_shell_height;
            temp_oneTec.zenith = atof(Line.substr(51,7).c_str());
            //path direction
            temp_oneTec.azimuth = atof(Line.substr(42,7).c_str());
            temp_oneTec.elevation = atof(Line.substr(33,7).c_str());
            temp_oneTec.calTime = temp_ctime;
            
            ippSite.push_back(temp_oneTec);
        }
    }
    infile.close();
    return true;
}

bool GIMInterpolation(Ionex& GIM, CoorGeo& point, TimeCalendar& ctime, double& intpVtec){
    //interpolation time
    double second = 0.0;
    second = ti_getAllSecond(ctime);
    
    //Get the 8 GIM points
    TimeCalendar startCtime, endCtime;
    double startSecond = 0.0;
    double endSecond = 0.0;
    startCtime = GIM.vtec[0].ctime;
    endCtime = GIM.vtec.back().ctime;
    startSecond = ti_getAllSecond(startCtime);
    endSecond = ti_getAllSecond(endCtime);
    
    //check if out of range
    if (second < startSecond || second > endSecond) {
        cout << "Interpolation time out of range!" << endl;
        return false;
    }
    
    double T_interval = GIM.header.Interval;
    int ti = 0;
    
    ti = floor((second - startSecond) / T_interval + 0.00001);
    double dt0 = second - (ti * T_interval + startSecond); // t - Ti
    double dt1 = (ti + 1) * T_interval + startSecond - second; //Ti+1 - t
    double Lr = point.L + dt0 / 3600; //Lrotate = L + (t - Ti)
    double Lrp1 = point.L - dt1 / 3600;
    
    double Bmin = 0.0;
    double Bmax = 0.0;
    double dB = 0.0;
    double q = 0.0;
    int Bi = 0;
    Bmin = GIM.header.LAT[0];
    Bmax = GIM.header.LAT[1];
    dB = GIM.header.LAT[2];
    //check the range
    if (point.B < Bmax || point.B > Bmin) {
        cout << "Interpolation LAT out of range!" << endl;
        return false;
    }
    Bi = floor((point.B - Bmin) / dB + 0.00001);
    q = (point.B - ((Bi+1) * dB + Bmin)) / fabs(dB);
    
    double Lmin = 0.0;
    double Lmax = 0.0;
    double dL = 0.0;
    double p = 0.0;
    int Li = 0;
    Lmin = GIM.header.LON[0];
    Lmax = GIM.header.LON[1];
    dL = GIM.header.LON[2];
    if (point.L < Lmin || point.L > Lmax) {
        cout << "Interpolation LON out of range!" << endl;
        return false;
    }
    Li = floor((Lr - Lmin) / dL + 0.00001);
    p = (Lr - (Li * dL + Lmin)) / dL;
    //Li = floor((point.L - Lmin) / dL + 0.00001);
    //p = (point.L - (Li * dL + Lmin)) / dL;
    
    //spatial interpolation
    interpolate vtec_GIM_i;
    double intpVtec_i = 0.0;
    vtec_GIM_i.E00 = GIM.vtec[ti].vtec[Bi+1].vtec[Li];
    vtec_GIM_i.E01 = GIM.vtec[ti].vtec[Bi].vtec[Li];
    vtec_GIM_i.E10 = GIM.vtec[ti].vtec[Bi+1].vtec[Li+1];
    vtec_GIM_i.E11 = GIM.vtec[ti].vtec[Bi].vtec[Li+1];
    intpVtec_i = (1 - p) * (1 - q) * vtec_GIM_i.E00 + p * (1 - q) * vtec_GIM_i.E10 + q * (1-p) * vtec_GIM_i.E01 + p * q * vtec_GIM_i.E11;
    
    //for piece point
    if (dt0 == 0) {
        intpVtec = (dt1 / T_interval) * intpVtec_i;
        return true;
    }
    
    Li = floor((Lrp1 - Lmin) / dL + 0.00001);
    p = (Lrp1 - (Li * dL + Lmin)) / dL;
    interpolate vtec_GIM_ip1;
    double intpVtec_ip1 = 0.0;
    vtec_GIM_ip1.E00 = GIM.vtec[ti+1].vtec[Bi+1].vtec[Li];
    vtec_GIM_ip1.E01 = GIM.vtec[ti+1].vtec[Bi].vtec[Li];
    vtec_GIM_ip1.E10 = GIM.vtec[ti+1].vtec[Bi+1].vtec[Li+1];
    vtec_GIM_ip1.E11 = GIM.vtec[ti+1].vtec[Bi].vtec[Li+1];
    intpVtec_ip1 = (1 - p) * (1 - q) * vtec_GIM_ip1.E00 + p * (1 - q) * vtec_GIM_ip1.E10 + q * (1-p) * vtec_GIM_ip1.E01 + p * q * vtec_GIM_ip1.E11;
    
    //temporal interpolation
    //double dt0 = second - (ti * T_interval + startSecond); // t - Ti
    //double dt1 = (ti + 1) * T_interval + startSecond - second; //Ti+1 - t
    intpVtec = (dt1 / T_interval) * intpVtec_i + (dt0 / T_interval) * intpVtec_ip1;
    
    return true;
}


bool WriteIPPfile(string filename, vector<IPPStruct>& ipp){
    //checkName=fileDirecName.substr(0,fileDirecName.length()-4)+"-smt.ipp";
    
    ofstream fout(filename.c_str(),ios::out);
    fout.setf(ios_base::adjustfield,ios_base::left);
    fout<<"TIME    PRN   IPPLAT       IPPLON      ELEVATION AZIMUTH  ZENITH   STEC   VTEC"<<endl;
    //////////////////////////////////////////////////////////////////////////
    //fout.setf(ios_base::adjustfield,ios_base::right);
    fout.setf(ios_base::fixed,ios_base::floatfield);
    
    unsigned long count = ipp.size();
    for (int i = 0; i < count; i++){
        //time: hour.second
        fout << setw(2) << setfill('0') << ipp[i].calTime.hour << "." << setw(4) << setprecision(0) << ipp[i].calTime.minute * 60 + ipp[i].calTime.second \
        <<" " <<"PRN"<<setw(2)<<ipp[i].PRN
        <<" "<<setw(12)<<setprecision(7)<<setfill(' ')<<ipp[i].ipp.B
        <<" "<<setw(12)<<setprecision(7)<<ipp[i].ipp.L
        <<" "<<setw(8)<<setprecision(4)<<ipp[i].elevation
        <<" "<<setw(8)<<setprecision(3)<<ipp[i].azimuth
        <<" "<<setw(8)<<setprecision(4)<<ipp[i].zenith
        <<" "<<setw(6)<<setprecision(2)<<ipp[i].stec //DCB corrected smooth STEC
        <<" "<<setw(6)<<setprecision(2)<<ipp[i].stec*cos(ipp[i].zenith*Dre2Rad)// VTEC
        <<endl;
    }
    fout.close();
    
    return true;
}

bool WriteSortIPPfile(string filename, vector<IPPfile>& ipp){
    ofstream fout(filename.c_str(),ios::out);
    fout.setf(ios_base::adjustfield,ios_base::left);
    //fout.setf(ios_base::fixed,ios_base::floatfield);
    
    fout<<setw(60)<<"1.0 IONOSPHERE PIECE POINT INFO FILE"<<"# FILE VERSION"<<endl;
    fout<<setw(60)<<"IONPREPROCESS V1.5  BY GPS Research Center of WHU"<<"# SOFTWARE INFO"<<endl;
    fout<<setw(5)<<"P2 & P1_CA"<<setw(53)<<" ARE USED IN IPP STEC CALCULATION"<<"# STEC CAL INFO"<<endl;
    fout<<setw(60)<<"PRN IPPLAT IPPLON ELEVATION AZIMUTH ZENITH STEC VTEC"<<"# DATA TYPES"<<endl;
    
    fout<<setw(60)<<"One Way Carrier Phase leveled to Code"<<"# COMMENT"<<endl;
    
    fout<<setw(20)<<setprecision(12)<<A_wgs<<setw(40)<<" "<<"# R OF EARTH"<<endl;
    fout<<setw(10)<<Shell_H<<setw(50)<<" "<<"# HEIGHT OF SHELL"<<endl;
    //
    double Muti_Ele45=0.0;
    fout<<setw(10)<<setprecision(3)<<Muti_Ele45<<setw(50)<<" "<<"#DELTA OF MUTIPATH AT ELEVATION 45"<<endl;
    fout<<setw(60)<<" "<<"END OF HEADER"<<endl;
    //////////////////////////////////////////////////////////////////////////
    fout.setf(ios_base::adjustfield,ios_base::right);

    for (int i=0;i<ipp.size();i++)
    {
        if (ipp[i].PRN.size()==0) continue;
        fout.setf(ios_base::fixed,ios_base::floatfield);
        fout<<setw(2)<<setfill('0')<<ipp[i].epoch.year
        <<" "<<setw(2)<<setfill('0')<<ipp[i].epoch.month
        <<" "<<setw(2)<<setfill('0')<<ipp[i].epoch.day
        <<" "<<setw(2)<<setfill('0')<<ipp[i].epoch.hour
        <<" "<<setw(2)<<setfill('0')<<ipp[i].epoch.minute
        <<" "<<setw(11)<<setfill(' ')<<setprecision(4)<<ipp[i].epoch.second
        <<"  "<<setw(2)<<setfill('0')<<ipp[i].PRN.size()<<" ";
        for (int j=0;j<ipp[i].PRN.size();j++)
        {
            fout<<setw(2)<<setfill('0')<<ipp[i].PRN[j]<<" ";
        }
        fout<<endl;
        for (int j=0;j<ipp[i].PRN.size();j++)
        {
            fout.setf(ios_base::fixed,ios_base::floatfield);
            fout<<"PRN"<<setw(2)<<setfill('0')<<ipp[i].ippPiece[j].PRN
            <<" "<<setw(12)<<setprecision(7)<<setfill(' ')<<ipp[i].ippPiece[j].ipp.B
            <<" "<<setw(12)<<setprecision(7)<<ipp[i].ippPiece[j].ipp.L
            <<" "<<setw(8)<<setprecision(4)<<ipp[i].ippPiece[j].elevation
            <<" "<<setw(8)<<setprecision(3)<<ipp[i].ippPiece[j].azimuth
            <<" "<<setw(8)<<setprecision(4)<<ipp[i].ippPiece[j].zenith
            <<" "<<setw(8)<<setprecision(2)<<ipp[i].ippPiece[j].stec
            <<" "<<setw(8)<<setprecision(3)<<ipp[i].ippPiece[j].stec*cos(ipp[i].ippPiece[j].zenith*Dre2Rad)<<endl;
        }
    }
    fout.close();
    return true;
}



bool SortIPPfile(vector<IPPStruct>& ipp, vector<IPPfile>& sortipp){
    
    double t0 = ti_getAllSecond(ipp[0].calTime);
    double t = 0.0;
    TimeCalendar ct;
    unsigned long count = ipp.size();
    int j = 0;
    
    vector<int> prn_epoch;
    int epochNum = 0;
    
    for (int i = 0; i < count; i++){
        ct = ipp[i].calTime;
        t = ti_getAllSecond(ct);
        
        if (t > t0 || i == count-1) {
            epochNum += 1;
            IPPfile ippfile_epoch;
            
            vector<IPPStruct> ipp_epoch;
            ipp_epoch.assign(ipp.begin()+j, ipp.begin()+i);
            //copy(ipp.begin()+j, ipp.begin()+i-1, ipp_epoch.begin());
            ippfile_epoch.epochNUM = epochNum;
            ippfile_epoch.ippPiece = ipp_epoch;
            ippfile_epoch.PRN = prn_epoch;
            ippfile_epoch.epoch = ipp[i-1].calTime;
            
            sortipp.push_back(ippfile_epoch);
            
            //clear prn_epoch
            vector<int>().swap(prn_epoch);
            prn_epoch.clear();
            
            //vector<IPPStruct> ipp_epoch;
            prn_epoch.push_back(ipp[i].PRN);
            j = i;
            t0 = t;
            continue;
        }
        if (t == t0) {
            prn_epoch.push_back(ipp[i].PRN);
        }
    }
    return true;
}
