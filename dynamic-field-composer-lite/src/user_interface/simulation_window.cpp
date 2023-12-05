// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "./user_interface/simulation_window.h"

namespace dnf_composer
{
	namespace user_interface
	{
		SimulationWindow::SimulationWindow(const std::shared_ptr<Simulation>& simulation)
			: simulation(simulation)
		{
		}

		void SimulationWindow::render()
		{
			if (ImGui::Begin("Simulation control"))
			{
				renderStartSimulationButton();
				renderAddElement();
				renderSetInteraction();
				renderRemoveElement();
			}
			ImGui::End();
		}

		void SimulationWindow::renderStartSimulationButton() const
		{
			if (ImGui::Button("Start simulation"))
				simulation->init();
		}

		void SimulationWindow::renderAddElement()
		{
			if (ImGui::CollapsingHeader("Add element"))
			{
				if (ImGui::TreeNode("Element selector"))
				{
					for (const auto& pair : element::ElementLabelToString)
						renderElementProperties(pair);
					ImGui::TreePop();
				}
			}
		}

		void SimulationWindow::renderSetInteraction() const
		{
			if (ImGui::CollapsingHeader("Set interactions between elements"))
			{
				const int numberOfElementsInSimulation = simulation->getNumberOfElements();

				for (int i = 0; i < numberOfElementsInSimulation; i++)
				{
					const auto simulationElement = simulation->getElement(i);
					std::string targetElementId = simulationElement->getUniqueName();

					if (ImGui::TreeNode(targetElementId.c_str()))
					{
						static std::string selectedElementId{};
						static int currentElementIdx = 0;
						if (ImGui::BeginListBox("Select input element"))
						{
							for (int n = 0; n < numberOfElementsInSimulation; n++)
							{
								const auto element = simulation->getElement(n);
								std::string inputElementId = element->getUniqueName();
								const bool isSelected = (currentElementIdx == n);
								if (ImGui::Selectable(inputElementId.c_str(), isSelected))
								{
									selectedElementId = inputElementId;
									currentElementIdx = n;
								}

								if (isSelected)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndListBox();
						}

						if (ImGui::Button("Add", { 100.0f, 30.0f }))
						{
							auto input = simulation->getElement(selectedElementId);
							simulationElement->addInput(input);
						}
						ImGui::TreePop();
					}
				}
			}
		}

		void SimulationWindow::renderRemoveElement() const
		{
			if (ImGui::CollapsingHeader("Remove elements from simulation"))
			{
				int numberOfElementsInSimulation = simulation->getNumberOfElements();

				for (int i = 0; i < numberOfElementsInSimulation; i++)
				{
					const auto simulationElement = simulation->getElement(i);
					std::string elementId = simulationElement->getUniqueName();

					if (ImGui::TreeNode(elementId.c_str()))
					{
						if (ImGui::Button("Remove", { 100.0f, 30.0f }))
						{
							simulation->removeElement(elementId);
							numberOfElementsInSimulation--;
						}
						ImGui::TreePop();
					}
				}
			}
		}

		void SimulationWindow::renderElementProperties(const std::pair<int, std::string>& elementId)
		{
			if (ImGui::TreeNode(elementId.second.c_str()))
			{
				switch (elementId.first)
				{
				case element::ElementLabel::NEURAL_FIELD:
					//addElementNeuralField();
					break;
				case element::ElementLabel::GAUSS_STIMULUS:
					//addElementGaussStimulus();
					break;
				case element::ElementLabel::FIELD_COUPLING:
					//addElementFieldCoupling();
					break;
				case element::ElementLabel::GAUSS_KERNEL:
					//addElementGaussKernel();
					break;
				case element::ElementLabel::MEXICAN_HAT_KERNEL:
					//addElementMexicanHatKernel();
					break;
				case element::ElementLabel::NORMAL_NOISE:
					// normal noise to do
					break;
				default:
					std::cout << "There is a missing element in the TreeNode\n";
					std::cout << "ImGuiInterface::renderAddElementTreeNode.\n";
					break;
				}
				ImGui::TreePop();
			}
		}
	}
}
