#include "user_interface/2dplot_window.h"


namespace dnf_composer
{
	namespace user_interface
	{
		PlotWindow2D::PlotWindow2D(const std::shared_ptr<Simulation>& simulation)
			: simulation(simulation)
		{
		}

		void PlotWindow2D::render()
		{
            bool open = true;

            if (ImGui::Begin("Heatmap", &open, ImGuiWindowFlags_NoCollapse))
            {
                renderPlottableElements();
            }
            ImGui::End();

            if (!open)
            {
                log(tools::logger::LogLevel::INFO, "Heatmap window closed.");
            }
		}

		void PlotWindow2D::renderPlottableElements() 
		{
            std::shared_ptr<element::GaussFieldCoupling> coupling;

            for(const auto& element : simulation->getElements())
            {
	            if (element->getLabel() == element::GAUSS_FIELD_COUPLING)
	            {
	            	coupling = std::dynamic_pointer_cast<element::GaussFieldCoupling>(element);
				}
			}

            const std::vector<double> flattened_matrix = coupling->getComponent("kernel");

            static float scale_min = 0;
            static float scale_max = 2.0f;

            static ImPlotColormap map = ImPlotColormap_Jet;
            if (ImPlot::ColormapButton(ImPlot::GetColormapName(map), ImVec2(225, 0), map)) {
                map = (map + 1) % ImPlot::GetColormapCount();
                // We bust the color cache of our plots so that item colors will
                // resample the new colormap in the event that they have already
                // been created. See documentation in implot.h.
                ImPlot::BustColorCache("##Heatmap1");
                ImPlot::BustColorCache("##Heatmap2");
            }

            ImGui::SameLine();
            ImGui::LabelText("##Colormap Index", "%s", "Change Colormap");
            ImGui::SetNextItemWidth(225);
            ImGui::DragFloatRange2("Min / Max", &scale_min, &scale_max, 0.01f, -20, 20);

            static ImPlotHeatmapFlags hm_flags = 0;

            ImGui::CheckboxFlags("Column Major", (unsigned int*)&hm_flags, ImPlotHeatmapFlags_ColMajor);

            static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;

            ImPlot::PushColormap(map);

            if (ImPlot::BeginPlot("##Heatmap1", ImVec2(225, 225), ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText)) {
                //ImPlot::SetupAxes(nullptr, nullptr, axes_flags, axes_flags);
                ImPlot::PlotHeatmap("Heatmap Data", flattened_matrix.data(), 100, 100,
                    scale_min, scale_max, nullptr,
                    ImPlotPoint(0, 0), ImPlotPoint(100, 100), hm_flags);
                ImPlot::EndPlot();
            }
            ImGui::SameLine();
            ImPlot::ColormapScale("##HeatScale", scale_min, scale_max, ImVec2(60, 225));

            ImPlot::PopColormap();
        }
	}
}
