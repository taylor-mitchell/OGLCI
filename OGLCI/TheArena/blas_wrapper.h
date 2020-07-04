#pragma once
// Simple placeholder code for BLAS calls - replace with calls to a real BLAS library

#include <vector>

namespace robertbridson {

    namespace BLAS {

        // dot products ==============================================================

        inline double dot(std::vector<float>& x, std::vector<float>& y)
        {
            //return cblas_ddot((int)x.size(), &x[0], 1, &y[0], 1); 

            double sum = 0;
            for (int i = 0; i < x.size(); ++i)
                sum += x[i] * y[i];
            return sum;
        }

        // inf-norm (maximum absolute value: index of max returned) ==================

        inline int index_abs_max(std::vector<float>& x)
        {
            //return cblas_idamax((int)x.size(), &x[0], 1); 
            int maxind = 0;
            double maxvalue = 0;
            for (int i = 0; i < x.size(); ++i) {
                if (fabs(x[i]) > maxvalue) {
                    maxvalue = fabs(x[i]);
                    maxind = i;
                }
            }
            return maxind;
        }

        // inf-norm (maximum absolute value) =========================================
        // technically not part of BLAS, but useful

        inline double abs_max(std::vector<float>& x)
        {
            return fabs(x[index_abs_max(x)]);
        }

        // saxpy (y=alpha*x+y) =======================================================

        inline void add_scaled(float alpha, std::vector<float>& x, std::vector<float>& y)
        {
            //cblas_daxpy((int)x.size(), alpha, &x[0], 1, &y[0], 1); 
            for (int i = 0; i < x.size(); ++i)
                y[i] += alpha * x[i];
        }
    }
}