#pragma once

#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/simulation.h"
#include "elements/gauss_kernel.h"
#include "elements/gauss_stimulus.h"
#include "elements/lateral_interactions.h"
#include "elements/neural_field.h"
#include "elements/normal_noise.h"

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
			static constexpr uint16_t startingInputPinId = 1000;
			static constexpr uint16_t startingOutputPinId = 2000;
			static constexpr uint16_t startingLinkId = 3000;
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
			void renderElementNode(const std::shared_ptr<element::Element>& element);
			void renderElementNodeHeader(const std::shared_ptr<element::Element>& element);
			void renderElementCommonParameters(const std::shared_ptr<element::Element>& element);
			void renderElementSpecificParameters(const std::shared_ptr<element::Element>& element);
			void renderElementPins(const std::shared_ptr<element::Element>& element);
			void renderElementNodeConnections(const std::shared_ptr<element::Element>& element);
		};
	}
}