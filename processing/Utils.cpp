#include "Utils.h"

namespace processing::utils
{
    unsigned int GetRandomUInt()
    {
        std::default_random_engine gen;
        gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<float> dist(0, 10000000);
        return static_cast<unsigned int>(dist(gen));
    }

    

    vector<float> SplineInterpolation(vector<DataPoint> data, float x) 
    {
        int n = data.size();
        vector<float> a(n);
        vector<float> b(n);
        vector<float> d(n);
        vector<float> h(n);

        for (int i = 0; i < n; i++) 
        {
            a[i] = data[i].y;
        }

        for (int i = 0; i < n - 1; i++) 
        {
            h[i] = data[i + 1].x - data[i].x;
        }

        vector<float> alpha(n);
        alpha[0] = 0;
        for (int i = 1; i < n - 1; i++) 
        {
            alpha[i] = static_cast<float>(3) * (a[i + 1] - a[i]) / h[i] - static_cast<float>(3) * (a[i] - a[i - 1]) / h[i - 1];

        }

        vector<float> c(n + 1);
        vector<float> l(n + 1);
        vector<float> mu(n + 1);
        vector<float> z(n + 1);
        l[0] = 1;
        mu[0] = 0;
        z[0] = 0;

        for (int i = 1; i < n; i++) 
        {
            l[i] = static_cast<float>(2) * (data[i + 1].x - data[i - 1].x) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }

        l[n] = 1;
        z[n] = 0;
        c[n] = 0;

        for (int j = n - 1; j >= 0; j--) 
        {
            c[j] = z[j] - mu[j] * c[j + 1];
            b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + static_cast<float>(2) * c[j]) / static_cast<float>(3);
            d[j] = (c[j + 1] - c[j]) / (static_cast<float>(3) * h[j]);
        }

        int i = 0;
        while (data[i + 1].x <= x) 
        {
            i++;
        }

        float t = x - data[i].x;
        return { a[i] + b[i] * t + c[i] * t * t + d[i] * t * t * t };
    }


    template <typename T>
    T VectorMean(std::vector<T> X)
    {
        T acc = 0;
        for (auto x : X)
        {
            acc += x;
        }
        return acc/static_cast<T>(X.size());        
    }

    template <typename T>
    std::vector<T> VectorMultiply(std::vector<T> X, std::vector<T> Y)
    {
        std::vector<T> res(X.size());
        for (size_t i = 0; i < X.size(); i++)
        {
            res[i] = X[i]*Y[i];
        }
        return res;
    }

    template <typename T1, typename T2>
    void LinReg<T1,T2>::EstimateCoef(std::vector<T1> indep_var,std::vector<T1> dep_var )
    {        
        T2 X_mean = VectorMean<T2>(indep_var);
        T2 Y_mean = VectorMean<T2>(dep_var);
        T2 SS_xy = 0;
        T2 SS_xx = 0;
        int n = indep_var.size();
        {
            std::vector<T1> temp;
            temp  = VectorMultiply(indep_var, dep_var);
            SS_xy = std::accumulate(temp.begin(), temp.end(), 0);
            SS_xy = SS_xy - n * X_mean * Y_mean;
        }
        {
            std::vector<T1> temp;
            temp = VectorMultiply(indep_var, indep_var);
            SS_xx = std::accumulate(temp.begin(), temp.end(), 0);
            SS_xx = SS_xx - n * X_mean * X_mean;
        }
        /*std::cout<< "SS_xy : " << SS_xy <<std::endl;
        std::cout<< "SS_xx : " << SS_xx <<std::endl;
        std::cout<< "X_mean : " << X_mean <<std::endl;
        std::cout<< "Y_mean : " << Y_mean <<std::endl;*/
        B_1 =  SS_xy / SS_xx;
        B_0 =  Y_mean - B_1 * X_mean ;
    }
    
    template <typename T1, typename T2>
    void LinReg<T1,T2>::GetCoeff(T2 &b_0, T2 &b_1)
    {
        b_0 = B_0;
        b_1 = B_1;
    }

    template <typename T1, typename T2>
    void LinReg<T1,T2>::Fit(std::vector<std::vector<T1>> dataset)
    {
        EstimateCoef(dataset[0],dataset[1]);
    }

    template class LinReg<float, float>;
    template class LinReg<double, double>;
}
