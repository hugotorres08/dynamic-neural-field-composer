
#pragma once

#include "./user_interface/user_interface_window.h"

#include "./user_interface/logger_window.h"

#include "./elements/element_factory.h"

namespace dnf_composer
{
	namespace user_interface
	{
		class SimulationWindow : public UserInterfaceWindow
		{
		private:
			std::shared_ptr<Simulation> simulation;
		public:
			SimulationWindow(const std::shared_ptr<Simulation>& simulation);

			SimulationWindow(const SimulationWindow&) = delete;
			SimulationWindow& operator=(const SimulationWindow&) = delete;
			SimulationWindow(SimulationWindow&&) = delete;
			SimulationWindow& operator=(SimulationWindow&&) = delete;

			void render() override;
			~SimulationWindow() override = default;
		private:
			void renderStartSimulationButton() const;
			void renderAddElement();
			void renderSetInteraction() const;
			void renderRemoveElement() const;

			void renderElementProperties(const std::pair<int, std::string>& elementId);

			//// these functions need to die
			//// these need to be refactored - may lead to ODR violations
			//void addElementNeuralField()
			//{
			//	static char id[CHAR_SIZE] = "neural field u";
			//	ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			//	static double size = 100;
			//	ImGui::InputDouble("size", &size, 1.0f, 10.0f, "%.2f");
			//	static double tau = 20;
			//	ImGui::InputDouble("tau", &tau, 1.0f, 10.0f, "%.2f");
			//	static double sigmoidSteepness = 1;
			//	ImGui::InputDouble("sigmoid steepness", &sigmoidSteepness, 1.0f, 10.0f, "%.2f");
			//	static double restingLevel = -5.0f;
			//	ImGui::InputDouble("resting level", &restingLevel, 1.0f, 10.0f, "%.2f");

			//	if (ImGui::Button("Add", { 100.0f, 30.0f }))
			//	{
			//		user_interface::LoggerWindow::addLog(LogLevel::_ERROR, "added element wa");

			//		element::NeuralFieldParameters nfp = { tau, restingLevel };
			//		element::ActivationFunctionParameters afp = {element::ActivationFunctionType::Sigmoid, sigmoidSteepness, 0};
			//		std::shared_ptr<element::NeuralField> neuralField = std::make_shared<element::NeuralField>(id, size, nfp);
			//		simulation->addElement(neuralField);
			//	}
			//}
			//void addElementGaussStimulus()
			//{
			//	static char id[CHAR_SIZE] = "gauss stimulus a";
			//	ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			//	static double size = 100;
			//	ImGui::InputDouble("size", &size, 1.0f, 10.0f, "%.2f");
			//	static double sigma = 5;
			//	ImGui::InputDouble("sigma", &sigma, 1.0f, 10.0f, "%.2f");
			//	static double amplitude = 20;
			//	ImGui::InputDouble("amplitude", &amplitude, 1.0f, 10.0f, "%.2f");
			//	static double position = 50;
			//	ImGui::InputDouble("position", &position, 1.0f, 10.0f, "%.2f");


			//	if (ImGui::Button("Add", { 100.0f, 30.0f }))
			//	{
			//		//user_interface::LoggerWindow::addLog(LogLevel::_INFO, "added element inf");
			//		user_interface::LoggerWindow::addLog(LogLevel::_WARNING, "added element wa");
			//		//user_interface::LoggerWindow::addLog(LogLevel::_ERROR, "added element er");

			//		element::GaussStimulusParameters gsp = { sigma, amplitude, position };
			//		std::shared_ptr<element::GaussStimulus> gaussStimulus = std::make_shared<element::GaussStimulus>(id, size, gsp);
			//		simulation->addElement(gaussStimulus);
			//	}
			//}
			////void addElementFieldCoupling()
			////{
			////	static char id[CHAR_SIZE] = "coupling u - v";
			////	ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			////	static double size = 100;
			////	ImGui::InputDouble("size", &size, 1.0f, 10.0f, "%.2f");
			////	static double sigma = 5;
			////	ImGui::InputDouble("sigma", &sigma, 1.0f, 10.0f, "%.2f");

			////	/*if (ImGui::Button("Add", { 100.0f, 30.0f }))
			////	{
			////		FieldCouplingParameters fcp = { sigma };
			////		std::shared_ptr<FieldCoupling> gaussCoupling = std::make_shared<FieldCoupling>(id, size, size, fcp, LearningRule::DELTA_KROGH_HERTZ);
			////		simulation->addElement(gaussCoupling);
			////	}*/
			////}
			//void addElementGaussKernel()
			//{
			//	static char id[CHAR_SIZE] = "gauss kernel u -> u";
			//	ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			//	static double size = 100;
			//	ImGui::InputDouble("size", &size, 1.0f, 10.0f, "%.2f");
			//	static double sigma = 20;
			//	ImGui::InputDouble("sigma", &sigma, 1.0f, 10.0f, "%.2f");
			//	static double amplitude = 2;
			//	ImGui::InputDouble("amplitude", &amplitude, 1.0f, 10.0f, "%.2f");

			//	if (ImGui::Button("Add", { 100.0f, 30.0f }))
			//	{
			//		user_interface::LoggerWindow::addLog(LogLevel::_INFO, "added element gk");

			//		element::GaussKernelParameters gkp = { sigma, amplitude };
			//		std::shared_ptr<element::GaussKernel> gaussKernel = std::make_shared<element::GaussKernel>(id, size, gkp);
			//		simulation->addElement(gaussKernel);
			//	}
			//}
			////void addElementMexicanHatKernel()
			////{
			////	static char id[30] = "mexican hat kernel u -> u";
			////	ImGui::InputTextWithHint("id", "enter text here", id, IM_ARRAYSIZE(id));
			////	static double size = 100;
			////	ImGui::InputDouble("size", &size, 1.0f, 10.0f, "%.2f");
			////	static double sigmaExc = 5;
			////	ImGui::InputDouble("sigmaExc", &sigmaExc, 1.0f, 10.0f, "%.2f");
			////	static double amplitudeExc = 15;
			////	ImGui::InputDouble("amplitudeExc", &amplitudeExc, 1.0f, 10.0f, "%.2f");
			////	static double sigmaInh = 10;
			////	ImGui::InputDouble("sigmaInh", &sigmaInh, 1.0f, 10.0f, "%.2f");
			////	static double amplitudeInh = 15;
			////	ImGui::InputDouble("amplitudeInh", &amplitudeInh, 1.0f, 10.0f, "%.2f");
			////	static double amplitudeGlobal = 0;
			////	ImGui::InputDouble("global amplitude", &amplitudeGlobal, 1.0f, 10.0f, "%.2f");

			////	/*if (ImGui::Button("Add", { 100.0f, 30.0f }))
			////	{
			////		MexicanHatKernelParameters mhkp = { sigmaExc, amplitudeExc, sigmaInh, amplitudeInh, amplitudeGlobal };
			////		std::shared_ptr<MexicanHatKernel> mexicanHatKernel = std::make_shared<MexicanHatKernel>(id, size, mhkp);
			////		simulation->addElement(mexicanHatKernel);
			////	}*/
			////}
		};
	}
}