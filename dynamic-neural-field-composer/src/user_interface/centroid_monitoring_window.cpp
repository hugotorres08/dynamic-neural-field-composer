#include "./user_interface/centroid_monitoring_window.h"


namespace dnf_composer
{
	namespace user_interface
	{
		CentroidMonitoringWindow::CentroidMonitoringWindow(const std::shared_ptr<Simulation>& simulation)
			: simulation(simulation)
		{
		}

		void CentroidMonitoringWindow::render()
		{
			if(ImGui::Begin("Centroid monitoring"))
			{
				getNeuralFieldsAndRenderCentroids();
			}
			ImGui::End();
		}

		void CentroidMonitoringWindow::getNeuralFieldsAndRenderCentroids() const
		{
			const int numberOfElementsInSimulation = simulation->getNumberOfElements();

			for (int i = 0; i < numberOfElementsInSimulation; i++)
			{
				const auto simulationElement = simulation->getElement(i);
				const element::ElementLabel elementLabel = simulationElement->getLabel();

				if (elementLabel == element::NEURAL_FIELD)
				{
					const auto neuralField = std::dynamic_pointer_cast<dnf_composer::element::NeuralField>(simulationElement);
					const std::string elementName = neuralField->getUniqueName();
					const double centroid = neuralField->getCentroid();
					const bool stable = neuralField->isStable();

					ImGui::Text("%s centroid is %.2f.", elementName.c_str(), centroid);

					if (stable) {
						ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s is stable.", elementName.c_str());
					}
					else {
						ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s is not stable.", elementName.c_str());
					}
				}

			}
		}

	}
}
