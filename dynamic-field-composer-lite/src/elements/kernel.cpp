// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/kernel.h"

namespace dnf_composer
{
	namespace element
	{
		Kernel::Kernel()
		{
			circular = true;
			normalized = false;
			kernelRange = { 0, 0 };
		}
	}
}
