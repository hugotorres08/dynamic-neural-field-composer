#pragma once

#include "element.h"
#include <array>

namespace dnf_composer
{
	namespace element
	{
		class Kernel : public Element
		{
		protected:
			bool circular;
			bool normalized;
			std::array<int, 2> kernelRange;
			std::vector<int> extIndex;
		public:
			Kernel();
			~Kernel() override = default;
		};
	}

}

