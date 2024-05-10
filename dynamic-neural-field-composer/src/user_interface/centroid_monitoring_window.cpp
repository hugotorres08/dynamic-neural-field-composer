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
					const double lowestActivation = neuralField->getLowestActivation();
					const double highestActivation = neuralField->getHighestActivation();
					std::vector<element::NeuralFieldBump> bumps = neuralField->getBumps();

					ImGui::Text("%s centroid is %.2f.", elementName.c_str(), centroid);

					if (stable) {
						ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s is stable.", elementName.c_str());
					}
					else {
						ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s is not stable.", elementName.c_str());
					}

					ImGui::Text("%s lowest activation is %.2f.", elementName.c_str(), lowestActivation);
					ImGui::Text("%s highest activation is %.2f.", elementName.c_str(), highestActivation);
					ImGui::Text("%s has %d bumps.", elementName.c_str(), bumps.size());
					for (int j = 0; j < bumps.size(); j++)
					{
						const element::NeuralFieldBump bump = bumps[j];
						const double startPosition = bump.startPosition;
						const double endPosition = bump.endPosition;
						const double bumpAmplitude = bump.amplitude;
						const double bumpWidth = bump.width;
						const double bumpPosition = bump.centroid;
						ImGui::Text("%s bump %d: start position %.2f, end position %.2f, amplitude %.2f, width %.2f, position %.2f.", elementName.c_str(), j, startPosition, endPosition, bumpAmplitude, bumpWidth, bumpPosition);
					}
				}

			}
		}

	}
}
