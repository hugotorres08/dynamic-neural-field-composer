#include "user_interface/node_graph_window.h"


namespace dnf_composer
{
	namespace user_interface
	{
		NodeGraphWindow::NodeGraphWindow(const std::shared_ptr<Simulation>& simulation)
			: simulation(simulation)
		{
			config.SettingsFile = "imnode-window.json";
			context = ImNodeEditor::CreateEditor();
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
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255)); // Set text color to white
				ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
				ImNodeEditor::Begin("My Node ImNodeEditor");
					renderElementNodes();
				ImNodeEditor::End();
				ImNodeEditor::SetCurrentEditor(nullptr);
				ImGui::PopStyleColor();
			}
			ImGui::End();
		}

		void NodeGraphWindow::renderElementNodes()
		{
			for (const auto& element : simulation->getElements())
			{
				ImNodeEditor::BeginNode(element->getUniqueIdentifier());
				renderElementNode(element);
				ImNodeEditor::EndNode();
			}
			for (const auto& element : simulation->getElements())
			{
				renderElementNodeConnections(element);
			}
		}

		void NodeGraphWindow::renderElementNode(const std::shared_ptr<element::Element>& element)
		{
			renderElementNodeHeader(element);
			renderElementCommonParameters(element);
			//constexpr ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
			//const std::string header = "##Parameters " + std::to_string(element->getUniqueIdentifier()) ;
			//const bool isOpen = ImGui::TreeNodeEx(header.c_str(), headerFlags);
			//if (isOpen)
			renderElementSpecificParameters(element);
			renderElementPins(element);
		}

		void NodeGraphWindow::renderElementNodeHeader(const std::shared_ptr<element::Element>& element)
		{
			static constexpr float rounding = 2.0f;
			static constexpr ImU32 headerBgColor = IM_COL32(65, 60, 65, 255);
			static constexpr ImU32 headerTextColor = IM_COL32(255, 255, 255, 255);
			ImNodeEditor::PushStyleColor(ImNodeEditor::StyleColor_NodeBg, imgui_kit::colours::DarkGray);
			ImNodeEditor::PushStyleColor(ImNodeEditor::StyleColor_NodeBorder, imgui_kit::colours::LightGray);
			//ImNodeEditor::PushStyleColor(ImNodeEditor::StyleColor_PinRect, ImColor(60, 180, 255, 150));
			//ImNodeEditor::PushStyleColor(ImNodeEditor::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));
			//ImNodeEditor::PushStyleVar(ImNodeEditor::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
			ImNodeEditor::PushStyleVar(ImNodeEditor::StyleVar_NodeRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			//ImNodeEditor::PushStyleVar(ImNodeEditor::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
			//ImNodeEditor::PushStyleVar(ImNodeEditor::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
			//ImNodeEditor::PushStyleVar(ImNodeEditor::StyleVar_LinkStrength, 0.0f);
			//ImNodeEditor::PushStyleVar(ImNodeEditor::StyleVar_PinBorderWidth, 1.0f);
			//ImNodeEditor::PushStyleVar(ImNodeEditor::StyleVar_PinRadius, 5.0f);
			const ImVec2 nodePos = ImNodeEditor::GetNodePosition(element->getUniqueIdentifier());
			const ImVec2 nodeSize = ImNodeEditor::GetNodeSize(element->getUniqueIdentifier());

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			const ImVec2 titleBarPos = ImVec2(nodePos.x + 1.5f, nodePos.y + 1.0f);
			const ImVec2 titleBarSize = ImVec2(nodeSize.x -2.5f, ImGui::GetTextLineHeightWithSpacing());

			// Draw title bar background
			draw_list->AddRectFilled(titleBarPos, 
				ImVec2(titleBarPos.x + titleBarSize.x, titleBarPos.y + titleBarSize.y),
				headerBgColor, rounding);
			// Draw title text
			const element::ElementCommonParameters parameters = element->getElementCommonParameters();
			const std::string name = parameters.identifiers.uniqueName;
			draw_list->AddText(ImVec2(titleBarPos.x + 3.0f, titleBarPos.y), headerTextColor, name.c_str());
			// Offset the position of the remaining node content to avoid overlapping with the title bar
			ImGui::Dummy(ImVec2(0, 15));
		}

		void NodeGraphWindow::renderElementCommonParameters(const std::shared_ptr<element::Element>& element)
		{
			const element::ElementCommonParameters parameters = element->getElementCommonParameters();
			const std::string name = "Name: " + parameters.identifiers.uniqueName;
			ImGui::TextUnformatted(name.c_str());

			const std::string type = "Type: " + element::ElementLabelToString.find(parameters.identifiers.label)->second;
			ImGui::Text(type.c_str());
			ImGui::Text("Size: %d", parameters.dimensionParameters.size);
			ImGui::Text("Step size: %.2f", parameters.dimensionParameters.d_x);
		}

		void NodeGraphWindow::renderElementSpecificParameters(const std::shared_ptr<element::Element>& element)
		{
			switch (element->getElementCommonParameters().identifiers.label)
			{
			case element::ElementLabel::NEURAL_FIELD:
			{
				const auto neuralField = std::dynamic_pointer_cast<element::NeuralField>(element);
				const element::NeuralFieldParameters parameters = neuralField->getParameters();
				ImGui::Text("Resting level: %.2f", parameters.startingRestingLevel);
				ImGui::Text("Tau: %.2f", parameters.tau);
			}
			break;
			case element::ElementLabel::NORMAL_NOISE:
			{
				const auto normalNoise = std::dynamic_pointer_cast<element::NormalNoise>(element);
				const element::NormalNoiseParameters parameters = normalNoise->getParameters();
				ImGui::Text("Amplitude: %.2f", parameters.amplitude);
			}
			break;
			case element::ElementLabel::GAUSS_KERNEL:
			{
				const auto gaussKernel = std::dynamic_pointer_cast<element::GaussKernel>(element);
				const element::GaussKernelParameters parameters = gaussKernel->getParameters();
				ImGui::Text("Width: %.2f", parameters.width);
				ImGui::Text("Amplitude: %.2f", parameters.amplitude);
			}
			break;
			case element::ElementLabel::GAUSS_STIMULUS:
			{
				const auto gaussStimulus = std::dynamic_pointer_cast<element::GaussStimulus>(element);
				const element::GaussStimulusParameters parameters = gaussStimulus->getParameters();
				ImGui::Text("Amplitude: %.2f", parameters.amplitude);
				ImGui::Text("Center: %.2f", parameters.position);
				ImGui::Text("Width: %.2f", parameters.width);
				ImGui::Text("Circular: %s", parameters.circular ? "true" : "false");
				ImGui::Text("Normalized: %s", parameters.normalized ? "true" : "false");
			}
			break;
			case element::ElementLabel::MEXICAN_HAT_KERNEL:
			{
				const auto mexicanHatKernel = std::dynamic_pointer_cast<element::MexicanHatKernel>(element);
				const element::MexicanHatKernelParameters parameters = mexicanHatKernel->getParameters();
				ImGui::Text("Amplitude exc: %.2f", parameters.amplitudeExc);
				ImGui::Text("Amplitude inh: %.2f", parameters.amplitudeInh);
				ImGui::Text("Width exc: %.2f", parameters.widthExc);
				ImGui::Text("Width inh: %.2f", parameters.widthInh);
				ImGui::Text("Circular: %s", parameters.circular ? "true" : "false");
				ImGui::Text("Normalized: %s", parameters.normalized ? "true" : "false");
			}
			break;
			case element::ElementLabel::GAUSS_FIELD_COUPLING:
			{
				const auto gfc = std::dynamic_pointer_cast<element::GaussFieldCoupling>(element);
				const element::GaussFieldCouplingParameters parameters = gfc->getParameters();

				ImGui::Text("Normalized: %s", parameters.normalized ? "true" : "false");
				ImGui::Text("Circular: %s", parameters.circular ? "true" : "false");

				ImGui::Text("Couplings:");
				for (const auto& coupling : parameters.couplings)
				{
					ImGui::Text("x_i: %.2f", coupling.x_i); ImGui::SameLine();
					ImGui::Text("x_j: %.2f", coupling.x_j); ImGui::SameLine();
					ImGui::Text("A: %.2f", coupling.amplitude); ImGui::SameLine();
					ImGui::Text("W: %.2f", coupling.width);
				}
			}
			break;
			default:
				tools::logger::log(tools::logger::LogLevel::ERROR, "Element label not recognized at node graph.");
				break;
			}
		}

		void NodeGraphWindow::renderElementPins(const std::shared_ptr<element::Element>& element)
		{
			// Begin an input pin for the node with a unique identifier
			ImNodeEditor::BeginPin(startingInputPinId + element->getUniqueIdentifier(), ImNodeEditor::PinKind::Input);
			// Align the input pin to the left
			ImNodeEditor::PinPivotAlignment(ImVec2(0.0f, 0.5f));
			ImGui::Text("Input");
			ImNodeEditor::EndPin();

			// Make sure the next element is on the same line
			ImGui::SameLine();

			// Add some space to separate the input from the output visually
			ImGui::Dummy(ImVec2(100.0f, 0.0f)); ImGui::SameLine();  // Adjust 200.0f as necessary for spacing

			// Begin an output pin for the node with a unique identifier
			ImNodeEditor::BeginPin(startingOutputPinId + element->getUniqueIdentifier(), ImNodeEditor::PinKind::Output);
			// Align the output pin to the right
			ImNodeEditor::PinPivotAlignment(ImVec2(1.0f, 0.5f));
			ImGui::Text("Output");
			ImNodeEditor::EndPin();


		}

		void NodeGraphWindow::renderElementNodeConnections(const std::shared_ptr<element::Element>& element)
		{
			static constexpr float thickness = 3.0f;
			for (const auto& connection : element->getInputs())
			{
				const std::string linkId = std::to_string(element->getUniqueIdentifier())
					+ std::to_string(connection->getUniqueIdentifier());
				const size_t link = stoull(linkId) + startingLinkId;
				ImNodeEditor::Link(link,
					connection->getUniqueIdentifier() + startingOutputPinId, 
					element->getUniqueIdentifier() + startingInputPinId,
					imgui_kit::colours::White, thickness);
			}
		}
	}
}
