#pragma once

#include <vector>
#include <cmath>
#include <stdexcept>

#include "fourier.h"


// https://github.com/blackstonep/Numerical-Recipes/blob/master/convlv.h

#define SQR(a) ((a)*(a))

namespace dnf_composer
{
	namespace tools
	{
		namespace math
		{
			namespace numerical_recipes
			{

				using namespace std;

				typedef double Doub;
				typedef int Int;
				typedef vector<double> VecDoub;


				void convlv(const VecDoub& data, const VecDoub& respns, const Int isign, VecDoub& ans);

				void convlv_valid(const VecDoub& data, const VecDoub& respns, const Int isign, VecDoub& ans);
			}
		}
	}
}
