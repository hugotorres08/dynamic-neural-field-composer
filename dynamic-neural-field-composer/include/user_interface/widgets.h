#pragma once

#include <imgui.h>
#include <implot.h>

#include "visualization/plot.h"
#include "tools/math.h"

namespace dnf_composer
{
	namespace user_interface
	{
		namespace widgets
		{
			void renderHelpMarker(const char* desc);
			void renderLinePlot(const Plot& plot, bool whereDimensionsChangedByUser);
			void renderHeatmap(const Plot& plot);
		}
	}
}