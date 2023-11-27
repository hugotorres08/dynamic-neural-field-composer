#pragma once

#include "element.h"
#include <array>

class Kernel : public Element
{
protected:
	bool circular;
	bool normalized;
	std::array<int, 2> kernelRange;
	std::vector<int> extIndex;
public:
	Kernel();
    virtual ~Kernel() override = default;
};