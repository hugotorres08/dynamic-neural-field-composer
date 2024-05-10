#pragma once

#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/simulation.h"
#include "elements/neural_field.h"

namespace dnf_composer
{
	namespace user_interface
	{
		class FieldMetricsWindow : public imgui_kit::UserInterfaceWindow
		{
		private:
			std::shared_ptr<Simulation> simulation;
		public:
			FieldMetricsWindow(const std::shared_ptr<Simulation>& simulation);

			FieldMetricsWindow(const FieldMetricsWindow&) = delete;
			FieldMetricsWindow& operator=(const FieldMetricsWindow&) = delete;
			FieldMetricsWindow(FieldMetricsWindow&&) = delete;
			FieldMetricsWindow& operator=(FieldMetricsWindow&&) = delete;

			void render() override;
			~FieldMetricsWindow() override = default;
		private:
			void getNeuralFieldsAndRenderCentroids() const;
			static void renderNeuralFieldDetails(const std::shared_ptr<element::NeuralField>& neuralField);
		};
	}
}