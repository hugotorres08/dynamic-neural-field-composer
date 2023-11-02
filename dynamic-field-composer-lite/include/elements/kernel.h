#pragma once

#include "element.h"
#include <array>

class Kernel : public Element
{
protected:
	bool circular;
	bool normalized;
	std::array<uint32_t, 2> kernelRange;
	std::vector<uint32_t> extIndex;
public:
	Kernel();
    virtual ~Kernel() override = default;
};