#include "./user_interface/centroid_monitoring_window.h"

#include "elements/neural_field.h"

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
					ImGui::Text("%s centroid is %2f.", elementName.c_str(), centroid);
				}
			}
		}
	}
}
