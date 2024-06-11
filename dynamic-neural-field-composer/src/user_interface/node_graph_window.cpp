#include "user_interface/node_graph_window.h"

#include "elements/gauss_kernel.h"
#include "elements/gauss_stimulus.h"
#include "elements/lateral_interactions.h"
#include "elements/neural_field.h"
#include "elements/normal_noise.h"

namespace dnf_composer
{
	namespace user_interface
	{
		NodeGraphWindow::NodeGraphWindow(const std::shared_ptr<Simulation>& simulation)
			: simulation(simulation)
		{
			config.SettingsFile = "dnf_composer_node_graph_window.json";
			context = ImNodeEditor::CreateEditor(&config);
		}

		void NodeGraphWindow::render()
		{
			if (ImGui::Begin("Node Graph Window", nullptr,
				ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_NoScrollWithMouse
			))
			{
				ImNodeEditor::SetCurrentEditor(context);
				const auto& io = ImGui::GetIO();
				ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
				ImNodeEditor::Begin("My Node ImNodeEditor");
					renderElementNodes();
				ImNodeEditor::End();
				ImNodeEditor::SetCurrentEditor(nullptr);
			}
			ImGui::End();
		}

		void NodeGraphWindow::renderElementNodes()
		{
			for (const auto& element : simulation->getElements())
			{
				ImNodeEditor::BeginNode(element->getUniqueIdentifier());
				renderElementNode(element);
				renderElementNodeConnections(element);
				ImNodeEditor::EndNode();
			}
		}

		void NodeGraphWindow::renderElementNode(const std::shared_ptr<element::Element>& element)
		{
			renderElementCommonParameters(element);
			renderElementSpecificParameters(element);
		}

		void NodeGraphWindow::renderElementCommonParameters(const std::shared_ptr<element::Element>& element)
		{
			const element::ElementCommonParameters parameters = element->getElementCommonParameters();
			const std::string name = "Name: " + parameters.identifiers.uniqueName;
			ImGui::Text(name.c_str());
			const std::string type = "Type: " + element::ElementLabelToString.find(parameters.identifiers.label)->second;
			ImGui::Text(type.c_str());
			const std::string size = "Size: " + std::to_string(parameters.dimensionParameters.size);
			ImGui::Text(size.c_str());
			const std::string step_size = "Step size: " + std::to_string(parameters.dimensionParameters.d_x);
			ImGui::Text(step_size.c_str());
		}

		void NodeGraphWindow::renderElementSpecificParameters(const std::shared_ptr<element::Element>& element)
		{
			switch (element->getElementCommonParameters().identifiers.label)
			{
			case element::ElementLabel::NEURAL_FIELD:
				{
					const auto neuralField = std::dynamic_pointer_cast<element::NeuralField>(element);
					const element::NeuralFieldParameters parameters = neuralField->getParameters();
					const std::string resting_level = "Resting level: " + std::to_string(parameters.startingRestingLevel);
					const std::string tau = "Tau: " + std::to_string(parameters.tau);
					ImGui::Text(resting_level.c_str());
					ImGui::Text(tau.c_str());
				}
					break;
			case element::ElementLabel::NORMAL_NOISE:
				{
					const auto normalNoise = std::dynamic_pointer_cast<element::NormalNoise>(element);
					const element::NormalNoiseParameters parameters = normalNoise->getParameters();
					const std::string amplitude = "amplitude: " + std::to_string(parameters.amplitude);
					ImGui::Text(amplitude.c_str());
				}
				break;
			case element::ElementLabel::GAUSS_KERNEL:
				{
					const auto gaussKernel = std::dynamic_pointer_cast<element::GaussKernel>(element);
					const element::GaussKernelParameters parameters = gaussKernel->getParameters();
					const std::string width = "Width: " + std::to_string(parameters.width);
					const std::string amplitude = "Amplitude: " + std::to_string(parameters.amplitude);
					ImGui::Text(width.c_str());
					ImGui::Text(amplitude.c_str());
				}
				break;
			case element::ElementLabel::GAUSS_STIMULUS:
				{
					const auto gaussStimulus = std::dynamic_pointer_cast<element::GaussStimulus>(element);
					const element::GaussStimulusParameters parameters = gaussStimulus->getParameters();
					const std::string amplitude = "Amplitude: " + std::to_string(parameters.amplitude);
					const std::string center_x = "Center: " + std::to_string(parameters.position);
					const std::string width = "Width: " + std::to_string(parameters.width);
					const std::string circular = "Circular: " + std::to_string(parameters.circular);
					const std::string normalized = "Normalized: " + std::to_string(parameters.normalized);
					ImGui::Text(amplitude.c_str());
					ImGui::Text(center_x.c_str());
					ImGui::Text(width.c_str());
					ImGui::Text(circular.c_str());
					ImGui::Text(normalized.c_str());
				}
				break;
			case element::ElementLabel::LATERAL_INTERACTIONS:
				{
					const auto lateralInteractions = std::dynamic_pointer_cast<element::LateralInteractions>(element);
					const element::LateralInteractionsParameters parameters = lateralInteractions->getParameters();
					const std::string amplitude_exc = "Amplitude exc: " + std::to_string(parameters.amplitudeExc);
					const std::string amplitude_inh = "Amplitude inh: " + std::to_string(parameters.amplitudeInh);
					const std::string width_exc = "Width exc: " + std::to_string(parameters.widthExc);
					const std::string width_inh = "Width inh: " + std::to_string(parameters.widthInh);
					const std::string amplitude_global = "Amplitude global: " + std::to_string(parameters.amplitudeGlobal);
					const std::string circular = "Circular: " + std::to_string(parameters.circular);
					const std::string normalized = "Normalized: " + std::to_string(parameters.normalized);
					ImGui::Text(amplitude_exc.c_str());
					ImGui::Text(amplitude_inh.c_str());
					ImGui::Text(width_exc.c_str());
					ImGui::Text(width_inh.c_str());
					ImGui::Text(amplitude_global.c_str());
					ImGui::Text(circular.c_str());
					ImGui::Text(normalized.c_str());
				}
				break;
			case element::ElementLabel::MEXICAN_HAT_KERNEL:
				{
					const auto mexicanHatKernel = std::dynamic_pointer_cast<element::MexicanHatKernel>(element);
					const element::MexicanHatKernelParameters parameters = mexicanHatKernel->getParameters();
					const std::string amplitude_exc = "Amplitude exc: " + std::to_string(parameters.amplitudeExc);
					const std::string amplitude_inh = "Amplitude inh: " + std::to_string(parameters.amplitudeInh);
					const std::string width_exc = "Width exc: " + std::to_string(parameters.widthExc);
					const std::string width_inh = "Width inh: " + std::to_string(parameters.widthInh);
					const std::string circular = "Circular: " + std::to_string(parameters.circular);
					const std::string normalized = "Normalized: " + std::to_string(parameters.normalized);
					ImGui::Text(amplitude_exc.c_str());
					ImGui::Text(amplitude_inh.c_str());
					ImGui::Text(width_exc.c_str());
					ImGui::Text(width_inh.c_str());
					ImGui::Text(circular.c_str());
					ImGui::Text(normalized.c_str());
				}
				break;
			default:
				tools::logger::log(tools::logger::LogLevel::ERROR, "Element label not recognized at node graph.");
				break;
			}
		}

		void NodeGraphWindow::renderElementNodeConnections(const std::shared_ptr<element::Element>& element)
		{

		}

	}
}
