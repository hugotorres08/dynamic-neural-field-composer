// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "application/application.h"


namespace dnf_composer
{
	Application::Application(const std::shared_ptr<Simulation>& simulation, const std::shared_ptr<Visualization>& visualization)
		: simulation(simulation ? simulation : std::make_shared<Simulation>("default", 1.0, 0.0, 0.0)),
		visualization(visualization ? visualization : std::make_shared<Visualization>(this->simulation)),
		guiActive(true)
	{
		if (this->visualization->getSimulation() != this->simulation)
			throw Exception(ErrorCode::APP_VIS_SIM_MISMATCH);
		setGUIParameters();
	}

	void Application::init() const
	{
		simulation->init();
		gui->initialize();
		log(tools::logger::LogLevel::INFO, "Application initialized successfully.");

		// [Put this elsewhere] Enable Keyboard Controls
		auto io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// [Put this elsewhere] Using the light style with borders
		ImGui::StyleColorsLight();
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameBorderSize = 1.0f;
		style.FrameRounding = 6.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.7f;
		style.Colors[ImGuiCol_FrameBg].w = 0.8f;
	}

	void Application::step() const
	{
		simulation->step();
		if (guiActive)
			gui->render();
	}

	void Application::close() const
	{
		simulation->close();
		if (guiActive)
			gui->shutdown();
		log(tools::logger::LogLevel::INFO, "Application closed successfully.");
	}

	void Application::toggleGUI()
	{
		guiActive = !guiActive;
		log(tools::logger::LogLevel::INFO,std::string("GUI is ") + (guiActive ? "enabled." : "disabled."));
	}

	bool Application::hasGUIBeenClosed() const
	{
		if (guiActive)
			return gui->isShutdownRequested();
		return false;
	}

	bool Application::isGUIActive() const
	{
		return guiActive;
	}

	void Application::setGUIParameters()
	{
		using namespace imgui_kit;
		const WindowParameters winParams{ "Dynamic Neural Field Composer" };
		const FontParameters fontParams{ std::string(PROJECT_DIR) + "/resources/fonts/Lexend-Light.ttf", 15 };

		//const StyleParameters styleParams{ ImVec4(0.3f, 0.3f, 0.3f, 0.6f), colours::White };
		constexpr ImVec4 themeColor = ImVec4(0.55f, 0.80f, 0.85f, 0.85f);
		const StyleParameters styleParams{ themeColor, colours::White };

		const IconParameters iconParams{ std::string(PROJECT_DIR) + "/resources/icons/icon.ico" };
		const BackgroundImageParameters bgParams{ std::string(PROJECT_DIR) + "/resources/images/background.png", ImageFitType::ZOOM_TO_FIT };
		const UserInterfaceParameters guiParameters{ winParams, fontParams, styleParams, iconParams, bgParams };
		gui = std::make_shared<UserInterface>(guiParameters);
		log(tools::logger::LogLevel::INFO, "GUI parameters set successfully.");
	}
}
