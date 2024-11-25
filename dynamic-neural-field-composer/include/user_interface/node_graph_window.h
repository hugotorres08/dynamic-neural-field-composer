#pragma once

#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/simulation.h"
#include "elements/gauss_kernel.h"
#include "elements/gauss_stimulus.h"
#include "elements/neural_field.h"
#include "elements/normal_noise.h"
#include "elements/mexican_hat_kernel.h"
#include "elements/gauss_field_coupling.h"

namespace dnf_composer
{
	namespace user_interface
	{
		inline ImVec4 getNodeColorForElementType(element::ElementLabel label)
		{
			switch (label) {
			case element::ElementLabel::NEURAL_FIELD:
				return imgui_kit::colours::PastelBlue;
			case element::ElementLabel::NORMAL_NOISE:
				return imgui_kit::colours::PastelGreen;
			case element::ElementLabel::GAUSS_KERNEL:
				return imgui_kit::colours::PastelOrange;
			case element::ElementLabel::GAUSS_STIMULUS:
				return imgui_kit::colours::PastelPink;
			case element::ElementLabel::MEXICAN_HAT_KERNEL:
				return imgui_kit::colours::PastelViolet;
			case element::ElementLabel::GAUSS_FIELD_COUPLING:
				return imgui_kit::colours::PastelYellow;
			case element::ElementLabel::FIELD_COUPLING:
				return imgui_kit::colours::Beige;
			default:
				return imgui_kit::colours::Blue;
			}
		}


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
			void setNodeStyle(const std::shared_ptr<element::Element>& element);
			void renderElementNode(const std::shared_ptr<element::Element>& element);
			void renderElementNodeHeader(const std::shared_ptr<element::Element>& element);
			void renderElementCommonParameters(const std::shared_ptr<element::Element>& element);
			void renderElementSpecificParameters(const std::shared_ptr<element::Element>& element);
			void renderElementPins(const std::shared_ptr<element::Element>& element);
			void renderElementNodeConnections(const std::shared_ptr<element::Element>& element);
		};
	}
}