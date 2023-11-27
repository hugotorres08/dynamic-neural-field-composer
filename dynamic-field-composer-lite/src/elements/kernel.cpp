// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/kernel.h"

Kernel::Kernel()
{
	circular = true;
	normalized = true;
	kernelRange = { 0, 0 };
}
