#pragma once

#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/simulation.h"

namespace dnf_composer
{
	namespace user_interface
	{
		class NodeGraphWindow : public imgui_kit::UserInterfaceWindow
		{
		private:
			std::shared_ptr<Simulation> simulation;
			ImNodeEditor::Config config;
			ImNodeEditor::EditorContext* context;
		public:
			NodeGraphWindow(const std::shared_ptr<Simulation>& simulation);

			NodeGraphWindow(const NodeGraphWindow&) = delete;
			NodeGraphWindow& operator=(const NodeGraphWindow&) = delete;
			NodeGraphWindow(NodeGraphWindow&&) = delete;
			NodeGraphWindow& operator=(NodeGraphWindow&&) = delete;

			void render() override;
			~NodeGraphWindow() override = default;
		private:
			void renderElementNodes();
		};
	}
}