// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "user_interface/element_window.h"

#include "elements/field_coupling.h"
#include "elements/gauss_kernel.h"
#include "elements/lateral_interactions.h"
#include "elements/normal_noise.h"

namespace dnf_composer
{
	namespace user_interface
	{
		ElementWindow::ElementWindow(const std::shared_ptr<Simulation>& simulation)
			: simulation(simulation)
		{
		}

		void ElementWindow::render()
		{
			if (ImGui::Begin("Element control"))
			{
				renderModifyElementParameters();
			}
			ImGui::End();
		}

		void ElementWindow::renderModifyElementParameters() const
		{
			if (ImGui::CollapsingHeader("Modify element parameters"))
			{
				const int numberOfElementsInSimulation = simulation->getNumberOfElements();

				for (int i = 0; i < numberOfElementsInSimulation; i++)
				{
					auto simulationElement = simulation->getElement(i);
					switchElementToModify(simulationElement);
				}
			}
		}

		void ElementWindow::switchElementToModify(const std::shared_ptr<element::Element>& element)
		{
			const std::string elementId = element->getUniqueName();
			const element::ElementLabel label = element->getLabel();

			ImGui::Text("Element %s", elementId.c_str());

			switch (label)
			{
			case element::ElementLabel::NEURAL_FIELD:
				modifyElementNeuralField(element);
				break;
			case element::ElementLabel::GAUSS_STIMULUS:
				modifyElementGaussStimulus(element);
				break;
			case element::ElementLabel::FIELD_COUPLING:
				modifyElementFieldCoupling(element);
				break;
			case element::ElementLabel::GAUSS_KERNEL:
				modifyElementGaussKernel(element);
				break;
			case element::ElementLabel::MEXICAN_HAT_KERNEL:
				modifyElementMexicanHatKernel(element);
				break;
			case element::ElementLabel::NORMAL_NOISE:
				modifyElementNormalNoise(element);
				break;
			case element::ElementLabel::GAUSS_FIELD_COUPLING:
				modifyElementGaussFieldCoupling(element);
				break;
			case element::ElementLabel::LATERAL_INTERACTIONS:
				modifyElementLateralInteractions(element);
				break;
			default:
				log(tools::logger::LogLevel::ERROR, "There is a missing element in the TreeNode in simulation window.");
				break;
			}
		}

		void ElementWindow::modifyElementNeuralField(const std::shared_ptr<element::Element>& element)
		{
			const auto neuralField = std::dynamic_pointer_cast<element::NeuralField>(element);
			element::NeuralFieldParameters nfp = neuralField->getParameters();

			auto restingLevel = static_cast<float>(nfp.startingRestingLevel);

			const std::string label = "##" + element->getUniqueName() + "Resting level";
			ImGui::SliderFloat(label.c_str(), &restingLevel, -30, 0);
			ImGui::SameLine(); ImGui::Text("Resting level");

			static constexpr double epsilon = 1e-6;
			if (std::abs(restingLevel - static_cast<float>(nfp.startingRestingLevel)) > epsilon) 
			{
				nfp.startingRestingLevel = restingLevel;
				neuralField->setParameters(nfp);
			}
		}

		void ElementWindow::modifyElementGaussStimulus(const std::shared_ptr<element::Element>& element) 
		{
			const auto stimulus = std::dynamic_pointer_cast<element::GaussStimulus>(element);
			element::GaussStimulusParameters gsp = stimulus->getParameters();

			auto amplitude = static_cast<float>(gsp.amplitude);
			auto width = static_cast<float>(gsp.width);
			auto position = static_cast<float>(gsp.position);


			std::string label = "##" + element->getUniqueName() + "Amplitude";
			ImGui::SliderFloat(label.c_str(), &amplitude, 0, 30);
			ImGui::SameLine(); ImGui::Text("Amplitude");

			label = "##" + element->getUniqueName() + "Width";
			ImGui::SliderFloat(label.c_str(), &width, 0, 30);
			ImGui::SameLine(); ImGui::Text("Width");

			label = "##" + element->getUniqueName() + "Position";
			ImGui::SliderFloat(label.c_str(), &position, 0, static_cast<float>(stimulus->getElementCommonParameters().dimensionParameters.x_max));
			ImGui::SameLine(); ImGui::Text("Position");

			static constexpr double epsilon = 1e-6;
			if (std::abs(amplitude - static_cast<float>(gsp.amplitude)) > epsilon ||
				std::abs(width - static_cast<float>(gsp.width)) > epsilon ||
				std::abs(position - static_cast<float>(gsp.position)) > epsilon)
			{
				gsp.amplitude = amplitude;
				gsp.width = width;
				gsp.position = position;
				stimulus->setParameters(gsp);
			}
		}

		void ElementWindow::modifyElementFieldCoupling(const std::shared_ptr<element::Element>& element) 
		{
			const auto fieldCoupling = std::dynamic_pointer_cast<element::FieldCoupling>(element);
			element::FieldCouplingParameters fcp = fieldCoupling->getParameters();

			auto scalar = static_cast<float>(fcp.scalar);

			const std::string label = "##" + element->getUniqueName() + "Scalar";
			ImGui::SliderFloat(label.c_str(), &scalar, 0, 2);
			ImGui::SameLine(); ImGui::Text("Scalar");

			static constexpr double epsilon = 1e-6;
			if (std::abs(scalar - static_cast<float>(fcp.scalar)) > epsilon)
			{
				fcp.scalar = scalar;
				fieldCoupling->setParameters(fcp);
			}

		}

		void ElementWindow::modifyElementGaussKernel(const std::shared_ptr<element::Element>& element) 
		{
			const auto kernel = std::dynamic_pointer_cast<element::GaussKernel>(element);
			element::GaussKernelParameters gkp = kernel->getParameters();

			auto amplitude = static_cast<float>(gkp.amplitude);
			auto width = static_cast<float>(gkp.width);

			std::string label = "##" + element->getUniqueName() + "Amplitude.";
			ImGui::SliderFloat(label.c_str(), &amplitude, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude.");

			label = "##" + element->getUniqueName() + "Width.";
			ImGui::SliderFloat(label.c_str(), &width, 0, 30);
			ImGui::SameLine(); ImGui::Text("Width");

			static constexpr double epsilon = 1e-6;
			if (std::abs(amplitude - static_cast<float>(gkp.amplitude)) > epsilon ||
				std::abs(width - static_cast<float>(gkp.width)) > epsilon)
			{
				gkp.amplitude = amplitude;
				gkp.width = width;
				kernel->setParameters(gkp);
			}
		}

		void ElementWindow::modifyElementMexicanHatKernel(const std::shared_ptr<element::Element>& element) 
		{
			const auto kernel = std::dynamic_pointer_cast<element::MexicanHatKernel>(element);
			element::MexicanHatKernelParameters mhkp = kernel->getParameters();

			auto amplitudeExc = static_cast<float>(mhkp.amplitudeExc);
			auto widthExc = static_cast<float>(mhkp.widthExc);
			auto amplitudeInh = static_cast<float>(mhkp.amplitudeInh);
			auto widthInh = static_cast<float>(mhkp.widthInh);

			std::string label = "##" + element->getUniqueName() + "Amplitude exc.";
			ImGui::SliderFloat(label.c_str(), &amplitudeExc, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude exc.");

			label = "##" + element->getUniqueName() + "Width exc.";
			ImGui::SliderFloat(label.c_str(), &widthExc, 0, 30);
			ImGui::SameLine(); ImGui::Text("Width exc.");

			label = "##" + element->getUniqueName() + "Amplitude inh.";
			ImGui::SliderFloat(label.c_str(), &amplitudeInh, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude inh.");

			label = "##" + element->getUniqueName() + "Width inh.";
			ImGui::SliderFloat(label.c_str(), &widthInh, 0, 30);
			ImGui::SameLine(); ImGui::Text("Width inh.");

			static constexpr double epsilon = 1e-6;
			if(std::abs(amplitudeExc - static_cast<float>(mhkp.amplitudeExc)) > epsilon ||
				std::abs(widthExc - static_cast<float>(mhkp.widthExc)) > epsilon ||
				std::abs(amplitudeInh - static_cast<float>(mhkp.amplitudeInh)) > epsilon ||
				std::abs(widthInh - static_cast<float>(mhkp.widthInh)) > epsilon)
			{
				mhkp.amplitudeExc = amplitudeExc;
				mhkp.widthExc = widthExc;
				mhkp.amplitudeInh = amplitudeInh;
				mhkp.widthInh = widthInh;

				kernel->setParameters(mhkp);
			}
		}

		void ElementWindow::modifyElementNormalNoise(const std::shared_ptr<element::Element>& element) 
		{
			const auto normalNoise = std::dynamic_pointer_cast<element::NormalNoise>(element);
			element::NormalNoiseParameters nnp = normalNoise->getParameters();

			auto amplitude = static_cast<float>(nnp.amplitude);

			const std::string label = "##" + element->getUniqueName() + "Amplitude";
			ImGui::SliderFloat(label.c_str(), &amplitude, 0, 5);
			ImGui::SameLine(); ImGui::Text("Amplitude");

			static constexpr double epsilon = 1e-6;
			if (std::abs(amplitude - static_cast<float>(nnp.amplitude)) > epsilon)
			{
				nnp.amplitude = amplitude;
				normalNoise->setParameters(nnp);
			}
		}

		void ElementWindow::modifyElementGaussFieldCoupling(const std::shared_ptr<element::Element>& element)
		{

			//                                                                                                                               
			//           ,--.                                                                                                                
			//         ,--.'|              ___                                                                                       ___     
			//     ,--,:  : |            ,--.'|_                ,---,                                                              ,--.'|_   
			//  ,`--.'`|  ' :   ,---.    |  | :,'             ,---.'|   ,---.        ,---,                                         |  | :,'  
			//  |   :  :  | |  '   ,'\   :  : ' :             |   | :  '   ,'\   ,-+-. /  |                                        :  : ' :  
			//  :   |   \ | : /   /   |.;__,'  /              |   | | /   /   | ,--.'|'   |   ,---.                .--,   ,---.  .;__,'  /   
			//  |   : '  '; |.   ; ,. :|  |   |             ,--.__| |.   ; ,. :|   |  ,"' |  /     \             /_ ./|  /     \ |  |   |    
			//  '   ' ;.    ;'   | |: ::__,'| :            /   ,'   |'   | |: :|   | /  | | /    /  |         , ' , ' : /    /  |:__,'| :    
			//  |   | | \   |'   | .; :  '  : |__         .   '  /  |'   | .; :|   | |  | |.    ' / |        /___/ \: |.    ' / |  '  : |__  
			//  '   : |  ; .'|   :    |  |  | '.'|        '   ; |:  ||   :    ||   | |  |/ '   ;   /|         .  \  ' |'   ;   /|  |  | '.'| 
			//  |   | '`--'   \   \  /   ;  :    ;        |   | '/  ' \   \  / |   | |--'  '   |  / |          \  ;   :'   |  / |  ;  :    ; 
			//  '   : |        `----'    |  ,   /         |   :    :|  `----'  |   |/      |   :    |           \  \  ;|   :    |  |  ,   /  
			//  ;   |.'                   ---`-'           \   \  /            '---'        \   \  /             :  \  \\   \  /    ---`-'   
			//  '---'                                       `----'                           `----'               \  ' ; `----'              
			//                                                                                                     `--`                      

		}

		void ElementWindow::modifyElementLateralInteractions(const std::shared_ptr<element::Element>& element)
		{
			const auto kernel = std::dynamic_pointer_cast<element::LateralInteractions>(element);
			element::LateralInteractionsParameters lip = kernel->getParameters();

			auto amplitudeExc = static_cast<float>(lip.amplitudeExc);
			auto widthExc = static_cast<float>(lip.widthExc);
			auto amplitudeInh = static_cast<float>(lip.amplitudeInh);
			auto widthInh = static_cast<float>(lip.widthInh);
			auto amplitudeGlobal = static_cast<float>(lip.amplitudeGlobal);

			std::string label = "##" + element->getUniqueName() + "Amplitude exc.";
			ImGui::SliderFloat(label.c_str(), &amplitudeExc, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude exc.");

			label = "##" + element->getUniqueName() + "Width exc.";
			ImGui::SliderFloat(label.c_str(), &widthExc, 0, 30);
			ImGui::SameLine(); ImGui::Text("Width exc.");

			label = "##" + element->getUniqueName() + "Amplitude inh.";
			ImGui::SliderFloat(label.c_str(), &amplitudeInh, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude inh.");

			label = "##" + element->getUniqueName() + "Width inh.";
			ImGui::SliderFloat(label.c_str(), &widthInh, 0, 30);
			ImGui::SameLine(); ImGui::Text("Width inh.");

			label = "##" + element->getUniqueName() + "Amplitude global";
			ImGui::SliderFloat(label.c_str(), &amplitudeGlobal, -5.0, 0.0);
			ImGui::SameLine(); ImGui::Text("Amplitude global");

			static constexpr double epsilon = 1e-6;
			if (std::abs(amplitudeExc - static_cast<float>(lip.amplitudeExc)) > epsilon ||
				std::abs(widthExc - static_cast<float>(lip.widthExc)) > epsilon ||
				std::abs(amplitudeInh - static_cast<float>(lip.amplitudeInh)) > epsilon ||
				std::abs(widthInh - static_cast<float>(lip.widthInh)) > epsilon ||
				std::abs(amplitudeGlobal - static_cast<float>(lip.amplitudeGlobal)) > epsilon)
			{
				lip.amplitudeExc = amplitudeExc;
				lip.widthExc = widthExc;
				lip.amplitudeInh = amplitudeInh;
				lip.widthInh = widthInh;
				lip.amplitudeGlobal = amplitudeGlobal;

				kernel->setParameters(lip);
			}
		}
	}


}
