#include <vector>
#include <complex>
#include <random>
#include <chrono>
#include <math.h>

#include "lttb.hpp"

#pragma once

using namespace std;

namespace processing::utils
{
    struct ExamplePoint {
        float x;
        float y;
    };

    using PointLttb = LargestTriangleThreeBuckets<ExamplePoint, float, &ExamplePoint::x, &ExamplePoint::y>;

    unsigned int GetRandomUInt();

    struct DataPoint {
        float x;
        float y;
    };

    vector<float> SplineInterpolation(vector<DataPoint> data, float x);

    template <typename T>
    T VectorMean(std::vector<T> X);

    template <typename T>
    std::vector<T> VectorMultiply(std::vector<T> X, std::vector<T> Y);

    template <typename T1, typename T2>
    class LinReg
    {
        private:
        void EstimateCoef(std::vector<T1> indep_var,std::vector<T1> dep_var);
        T2 B_0;
        T2 B_1;
        
        public:        
        void Fit(std::vector<std::vector<T1>> dataset);
        void GetCoeff(T2 &b_0, T2 &b_1);
        T1 Predict(const T1 & test_data)
        {
            return B_0 + (B_1 * test_data); 
        }
    };

}