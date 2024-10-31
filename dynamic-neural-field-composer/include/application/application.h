#pragma once

#include <type_traits>
#if defined(_WIN32)
#include <imgui-platform-kit/user_interface.h>
#elif defined(__linux__)
#endif

#include "exceptions/exception.h"
#include "simulation/simulation.h"

namespace dnf_composer
{
	// Base template: assumes T does not have a constructor that takes Simulation*
	template<typename T, typename = void>
	struct has_simulation_constructor : std::false_type {};

	// Specialization for types that take a Simulation*
	template<typename T>
	struct has_simulation_constructor<T, std::void_t<decltype(T(std::declval<std::shared_ptr<Simulation>>()))>> : std::true_type {};

	struct ApplicationParameters
	{
		imgui_kit::UserInterfaceParameters uiParameters;
		bool uiActive;

		ApplicationParameters(bool activateUserInterface = true);
		ApplicationParameters(imgui_kit::UserInterfaceParameters userInterfaceParameters);
	};

	class Application
	{
	private:
		std::shared_ptr<Simulation> simulation;
		std::shared_ptr<imgui_kit::UserInterface> ui;
		ApplicationParameters parameters;
	public:
		Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface = true);
		Application(const std::shared_ptr<Simulation>& simulation, ApplicationParameters uiParams);

		void init() const;
		void step() const;
		void close() const;

		template<typename WindowType, typename... Args, std::enable_if_t<!has_simulation_constructor<WindowType>::value, int> = 0>
		void addWindow(Args&&... args) const {
			// For window types that do not require a Simulation* argument
			ui->addWindow<WindowType>(std::forward<Args>(args)...);
		}

		template<typename WindowType, typename... Args, std::enable_if_t<has_simulation_constructor<WindowType>::value, int> = 0>
		void addWindow(Args&&... args) const {
			// For window types that require a Simulation* argument
			ui->addWindow<WindowType>(simulation, std::forward<Args>(args)...);
		}

		void setActivateUserInterfaceAs(bool activateUI);

		bool hasUIBeenClosed() const;
		bool isUIActive() const;

		~Application() = default;
	};
}

