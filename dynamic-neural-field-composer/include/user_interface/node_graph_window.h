#pragma once

#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/simulation.h"
#include "elements/gauss_kernel.h"
#include "elements/gauss_stimulus.h"
#include "elements/neural_field.h"
#include "elements/normal_noise.h"
#include "elements/mexican_hat_kernel.h"
#include "elements/gauss_field_coupling.h"
#include "elements/field_coupling.h"
#include "widgets.h"

namespace dnf_composer
{
	namespace user_interface
	{
		inline ImU32 getHeaderColorForElementType(element::ElementLabel label)
		{
			switch (label) {
			case element::ElementLabel::NEURAL_FIELD:
				return IM_COL32(86, 128, 191, 255); // Blue
			case element::ElementLabel::NORMAL_NOISE:
				return IM_COL32(223, 148, 84, 255);  // Orange
			case element::ElementLabel::GAUSS_KERNEL:
				return IM_COL32(191, 63, 63, 255);   // Warm Red
			case element::ElementLabel::GAUSS_STIMULUS:
				return IM_COL32(127, 191, 127, 255); // Green
			case element::ElementLabel::MEXICAN_HAT_KERNEL:
				return IM_COL32(154, 121, 191, 255); // Purple
			case element::ElementLabel::GAUSS_FIELD_COUPLING:
				return IM_COL32(165, 102, 71, 255);  // Brown
			case element::ElementLabel::FIELD_COUPLING:
				return IM_COL32(212, 192, 121, 255); // Light Gold
			default:
				return IM_COL32(127, 127, 127, 255); // Neutral gray
			}
		}

		/* Deep color scale
		IM_COL32(95, 192, 208, 255) Light Teal
		IM_COL32(212, 192, 121, 255) Light Gold	 
		IM_COL32(200, 200, 200, 255) Light Gray	
		IM_COL32(236, 149, 192, 255) Light Pink
		IM_COL32(165, 102, 71, 255) Brown
		IM_COL32(154, 121, 191, 255) Purple
		IM_COL32(191, 63, 63, 255) Warm Red
		IM_COL32(127, 191, 127, 255) Green
		IM_COL32(223, 148, 84, 255) Orange
		IM_COL32(86, 128, 191, 255 Blue
		*/

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
			void handleInteractions();
			void handlePinInteractions();
			void handleLinkInteractions();
		};
	}
}