#include "user_interface/plot_control_window.h"

namespace dnf_composer
{
	namespace user_interface
	{
		PlotControlWindow::PlotControlWindow(const std::shared_ptr<Simulation>& simulation)
			:simulation(simulation)
		{}

		void PlotControlWindow::render()
		{
			/*if (ImGui::Begin("Plot control"))
			{
				if (ImGui::CollapsingHeader("Create new plot"))
				{
					static char title[CHAR_SIZE] = "This is a plot title";
					ImGui::InputTextWithHint("title", "enter text here", title, IM_ARRAYSIZE(title));

					static char x_label[CHAR_SIZE] = "x label";
					ImGui::InputTextWithHint("x_label", "enter text here", x_label, IM_ARRAYSIZE(x_label));

					static char y_label[CHAR_SIZE] = "y label";
					ImGui::InputTextWithHint("y_label", "enter text here", y_label, IM_ARRAYSIZE(y_label));

					static int x_min = 0;
					ImGui::InputInt("x_min", &x_min, 1.0, 10.0);

					static int x_max = 100;
					ImGui::InputInt("x_max", &x_max, 1.0, 10.0);

					static int y_min = -10;
					ImGui::InputInt("y_min", &y_min, 1.0, 10.0);

					static int y_max = 20;
					ImGui::InputInt("y_max", &y_max, 1.0, 10.0);

					static double d_x = 1.0;
					ImGui::InputDouble("d_x", &d_x, 0.05, 0.1);


					if (ImGui::Button("Add", { 100.0f, 30.0f }))
					{
						PlotParameters parameters;
						parameters.annotations = {title, x_label, y_label};
						parameters.dimensions = {x_min, x_max, y_min, y_max, d_x};
						std::shared_ptr<Simulation> simulation = plots[0].visualization->getAssociatedSimulationPtr();
						parameters.visualization = std::make_shared<Visualization>(simulation);
						createPlot(parameters);
					}
				}
			}
			ImGui::End();*/
		}


	}
}
