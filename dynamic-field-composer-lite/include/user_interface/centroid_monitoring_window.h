#pragma once

#include "./user_interface/user_interface_window.h"
#include "simulation/simulation.h"

namespace dnf_composer
{
	namespace user_interface
	{
		class CentroidMonitoringWindow : public UserInterfaceWindow
		{
		private:
			std::shared_ptr<Simulation> simulation;
		public:
			CentroidMonitoringWindow(const std::shared_ptr<Simulation>& simulation);

			CentroidMonitoringWindow(const CentroidMonitoringWindow&) = delete;
			CentroidMonitoringWindow& operator=(const CentroidMonitoringWindow&) = delete;
			CentroidMonitoringWindow(CentroidMonitoringWindow&&) = delete;
			CentroidMonitoringWindow& operator=(CentroidMonitoringWindow&&) = delete;

			void render() override;
			~CentroidMonitoringWindow() override = default;
		};
	}
}