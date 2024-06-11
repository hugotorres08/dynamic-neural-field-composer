#include "user_interface/node_graph_window.h"

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
				ImGui::Text(element->getUniqueName().c_str());
				ImGui::Text(element::ElementLabelToString.find(element->getLabel())->second.c_str());

				ImNodeEditor::EndNode();
			}
		}


	}
}