#include "tools/convlv.h"


// https://github.com/blackstonep/Numerical-Recipes/blob/master/convlv.h

namespace tools
{
    namespace math
    {
        namespace numerical_recipes
        {
            void convlv(const VecDoub& data, const VecDoub& respns, const Int isign, VecDoub& ans) {
                Int i, no2, n = data.size(), m = respns.size();
                Doub mag2, tmp;
                VecDoub temp(n);
                temp[0] = respns[0];
                for (i = 1; i < (m + 1) / 2; i++) {
                    temp[i] = respns[i];
                    temp[n - i] = respns[m - i];
                }
                for (i = (m + 1) / 2; i < n - (m - 1) / 2; i++)
                    temp[i] = 0.0;
                ans = data;
                fourier::realft(ans, 1);
                fourier::realft(temp, 1);
                no2 = n >> 1;
                if (isign == 1) {
                    for (i = 2; i < n; i += 2) {
                        tmp = ans[i];
                        ans[i] = (ans[i] * temp[i] - ans[i + 1] * temp[i + 1]) / no2;
                        ans[i + 1] = (ans[i + 1] * temp[i] + tmp * temp[i + 1]) / no2;
                    }
                    ans[0] = ans[0] * temp[0] / no2;
                    ans[1] = ans[1] * temp[1] / no2;
                }
                else if (isign == -1) {
                    for (i = 2; i < n; i += 2) {
                        mag2 = SQR(temp[i]) + SQR(temp[i + 1]);
                        if (mag2 == 0.0)
                            throw runtime_error("Deconvolving at response zero in convlv.");
                        tmp = ans[i];
                        ans[i] = (ans[i] * temp[i] + ans[i + 1] * temp[i + 1]) / mag2 / no2;
                        ans[i + 1] = (ans[i + 1] * temp[i] - tmp * temp[i + 1]) / mag2 / no2;
                    }
                    if (temp[0] == 0.0 || temp[1] == 0.0)
                        throw runtime_error("Deconvolving at response zero in convlv.");
                    ans[0] = ans[0] / temp[0] / no2;
                    ans[1] = ans[1] / temp[1] / no2;
                }
                else {
                    throw runtime_error("No meaning for isign in convlv.");
                }
                fourier::realft(ans, -1);
            }

            // this is not working!
            // do not use this function yet!
            void convlv_valid(const VecDoub& data, const VecDoub& respns, const Int isign, VecDoub& ans) {
                Int i, no2, n = data.size(), m = respns.size();
                Doub mag2, tmp;

                int resultSize = m - n + 1;
                // Create a vector to hold the result of convolution
                VecDoub result(resultSize);
                VecDoub temp(resultSize);

                temp[0] = respns[0];
                for (i = 1; i < (m + 1) / 2; i++) {
                    temp[i] = respns[i];
                    temp[n - i] = respns[m - i];
                }
                for (i = (m + 1) / 2; i < n - (m - 1) / 2; i++)
                    temp[i] = 0.0;
                ans = data;
                fourier::realft(ans, 1);
                fourier::realft(temp, 1);
                no2 = n >> 1;
                if (isign == 1) {
                    for (i = 2; i < n; i += 2) {
                        tmp = ans[i];
                        ans[i] = (ans[i] * temp[i] - ans[i + 1] * temp[i + 1]) / no2;
                        ans[i + 1] = (ans[i + 1] * temp[i] + tmp * temp[i + 1]) / no2;
                    }
                    ans[0] = ans[0] * temp[0] / no2;
                    ans[1] = ans[1] * temp[1] / no2;
                }
                else if (isign == -1) {
                    for (i = 2; i < n; i += 2) {
                        mag2 = SQR(temp[i]) + SQR(temp[i + 1]);
                        if (mag2 == 0.0)
                            throw runtime_error("Deconvolving at response zero in convlv.");
                        tmp = ans[i];
                        ans[i] = (ans[i] * temp[i] + ans[i + 1] * temp[i + 1]) / mag2 / no2;
                        ans[i + 1] = (ans[i + 1] * temp[i] - tmp * temp[i + 1]) / mag2 / no2;
                    }
                    if (temp[0] == 0.0 || temp[1] == 0.0)
                        throw runtime_error("Deconvolving at response zero in convlv.");
                    ans[0] = ans[0] / temp[0] / no2;
                    ans[1] = ans[1] / temp[1] / no2;
                }
                else {
                    throw runtime_error("No meaning for isign in convlv.");
                }
                fourier::realft(ans, -1);
            }
        }
    }
}
