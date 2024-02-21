#pragma once

#include <vector>
#include <cmath>
#include <complex>
#include <stdexcept>

#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

namespace dnf_composer
{
    namespace tools
    {
        namespace math
        {
            namespace fourier
            {
                using namespace std;

                typedef double Doub;
                typedef int Int;
                typedef complex<double> Complex;
                typedef vector<double> VecDoub;
                typedef vector<Complex> VecComplex;


                void four1(Doub* data, const Int n, const Int isign);

                void four1(VecDoub& data, const Int isign);

                void four1(VecComplex& data, const Int isign);

                void realft(VecDoub& data, const Int isign);

                void sinft(VecDoub& y);

                void cosft1(VecDoub& y);

                void cosft2(VecDoub& y, const Int isign);

            }
        }
    }

}
