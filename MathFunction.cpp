//
//  MathFunction.cpp
//  DCB
//
//  Created by Jiajun Chen on 01/06/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#include "MathFunction.hpp"

double MeanFunc(vector<double>& input){
    //double sum = accumulate(std::begin(input), std::end(input), 0.0);
    double sum = SUM(input);
    double mean =  sum / input.size(); //均值
    return mean;
}

double StdFunc(vector<double>& input){
    //double sum = accumulate(std::begin(input), std::end(input), 0.0);
    double sum = SUM(input);
    double mean =  sum / input.size(); //均值
    
    unsigned long count = input.size();
    double accum  = 0.0;
    int count_4times = int (count - count % 4);
    int i = 0;
    double accum_t  = 0.0;
    for (i = 0; i < count_4times; i+=4){
        accum_t = (input[i]-mean)*(input[i]-mean) + (input[i+1]-mean)*(input[i+1]-mean)
                  + (input[i+2]-mean)*(input[i+2]-mean) + (input[i+3]-mean)*(input[i+3]-mean);
        accum += accum_t;
    }
    for (i = count_4times; i < count; i++) {
        accum += (input[i]-mean)*(input[i]-mean);
    }
    
    double stdev = sqrt(accum/(input.size()-1)); //方差
    return stdev;
}

double SUM(vector<double>& indata){
    double sum = 0.0;
    double sum_t = 0.0;
    unsigned long count = indata.size();
    int count_4times = int (count - count % 4);
    int i = 0;
    for (i = 0; i < count_4times; i+=4){
        sum_t = indata[i] + indata[i+1] + indata[i+2] + indata[i+3];
        sum += sum_t;
    }
    for (i = count_4times; i < count; i++) {
        sum += indata[i];
    }
    return sum;
}
