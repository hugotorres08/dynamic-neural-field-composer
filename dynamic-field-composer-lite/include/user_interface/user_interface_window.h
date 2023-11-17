#pragma once


#include <memory>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#include "implot.h"
#include "implot_internal.h"


#include "./simulation/visualization.h"

#define CHAR_SIZE 50

class UserInterfaceWindow
{
public:
	UserInterfaceWindow();
	virtual void render() = 0;
	virtual ~UserInterfaceWindow() = default;
};

