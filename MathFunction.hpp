//
//  MathFunction.hpp
//  DCB
//
//  Created by Jiajun Chen on 01/06/2018.
//  Copyright © 2018 Jiajun Chen. All rights reserved.
//

#ifndef MathFunction_hpp
#define MathFunction_hpp

#include <stdio.h>
#include <vector>
#include <numeric>
#include <cmath>
using namespace std;

struct statistic{
    double mean;
    double median;
    double max;
    double min;
    double sum;
    double variance;
public:
    statistic(){
        mean = 0.0;
        median = 0.0;
        max = 0.0;
        min = 0.0;
        sum = 0.0;
        variance = 0.0;
    }
};

double MeanFunc(vector<double>& input);
double StdFunc(vector<double>& input);
double SUM(vector<double>& indata);

#endif /* MathFunction_hpp */
