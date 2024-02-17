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
				log(LogLevel::ERROR, "There is a missing element in the TreeNode in simulation window.\n");
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

			nfp.startingRestingLevel = restingLevel;

			neuralField->setParameters(nfp);
		}

		void ElementWindow::modifyElementGaussStimulus(const std::shared_ptr<element::Element>& element) 
		{
			const auto stimulus = std::dynamic_pointer_cast<element::GaussStimulus>(element);
			element::GaussStimulusParameters gsp = stimulus->getParameters();

			auto amplitude = static_cast<float>(gsp.amplitude);
			auto sigma = static_cast<float>(gsp.sigma);
			auto position = static_cast<float>(gsp.position);


			std::string label = "##" + element->getUniqueName() + "Amplitude";
			ImGui::SliderFloat(label.c_str(), &amplitude, 0, 30);
			ImGui::SameLine(); ImGui::Text("Amplitude");

			label = "##" + element->getUniqueName() + "Sigma";
			ImGui::SliderFloat(label.c_str(), &sigma, 0, 30);
			ImGui::SameLine(); ImGui::Text("Sigma");

			label = "##" + element->getUniqueName() + "Position";
			ImGui::SliderFloat(label.c_str(), &position, 0, static_cast<float>(stimulus->getElementCommonParameters().dimensionParameters.size));
			ImGui::SameLine(); ImGui::Text("Position");

			gsp.amplitude = amplitude;
			gsp.sigma = sigma;
			gsp.position = position;

			stimulus->setParameters(gsp);

		}

		void ElementWindow::modifyElementFieldCoupling(const std::shared_ptr<element::Element>& element) 
		{
			const auto fieldCoupling = std::dynamic_pointer_cast<element::FieldCoupling>(element);
			element::FieldCouplingParameters fcp = fieldCoupling->getParameters();

			auto scalar = static_cast<float>(fcp.scalar);

			const std::string label = "##" + element->getUniqueName() + "Scalar";
			ImGui::SliderFloat(label.c_str(), &scalar, 0, 2);
			ImGui::SameLine(); ImGui::Text("Scalar");

			fcp.scalar = scalar;

			fieldCoupling->setParameters(fcp);
		}

		void ElementWindow::modifyElementGaussKernel(const std::shared_ptr<element::Element>& element) 
		{
			const auto kernel = std::dynamic_pointer_cast<element::GaussKernel>(element);
			element::GaussKernelParameters gkp = kernel->getParameters();

			auto amplitude = static_cast<float>(gkp.amplitude);
			auto sigma = static_cast<float>(gkp.sigma);

			std::string label = "##" + element->getUniqueName() + "Amplitude.";
			ImGui::SliderFloat(label.c_str(), &amplitude, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude.");

			label = "##" + element->getUniqueName() + "Sigma.";
			ImGui::SliderFloat(label.c_str(), &sigma, 0, 30);
			ImGui::SameLine(); ImGui::Text("Sigma");

			gkp.amplitude = amplitude;
			gkp.sigma = sigma;

			kernel->setParameters(gkp);
		}

		void ElementWindow::modifyElementMexicanHatKernel(const std::shared_ptr<element::Element>& element) 
		{
			const auto kernel = std::dynamic_pointer_cast<element::MexicanHatKernel>(element);
			element::MexicanHatKernelParameters mhkp = kernel->getParameters();

			auto amplitudeExc = static_cast<float>(mhkp.amplitudeExc);
			auto sigmaExc = static_cast<float>(mhkp.sigmaExc);
			auto amplitudeInh = static_cast<float>(mhkp.amplitudeInh);
			auto sigmaInh = static_cast<float>(mhkp.sigmaInh);

			std::string label = "##" + element->getUniqueName() + "Amplitude exc.";
			ImGui::SliderFloat(label.c_str(), &amplitudeExc, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude exc.");

			label = "##" + element->getUniqueName() + "Sigma exc.";
			ImGui::SliderFloat(label.c_str(), &sigmaExc, 0, 30);
			ImGui::SameLine(); ImGui::Text("Sigma exc.");

			label = "##" + element->getUniqueName() + "Amplitude inh.";
			ImGui::SliderFloat(label.c_str(), &amplitudeInh, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude inh.");

			label = "##" + element->getUniqueName() + "Sigma inh.";
			ImGui::SliderFloat(label.c_str(), &sigmaInh, 0, 30);
			ImGui::SameLine(); ImGui::Text("Sigma inh.");

			mhkp.amplitudeExc = amplitudeExc;
			mhkp.sigmaExc = sigmaExc;
			mhkp.amplitudeInh = amplitudeInh;
			mhkp.sigmaInh = sigmaInh;

			kernel->setParameters(mhkp);
		}

		void ElementWindow::modifyElementNormalNoise(const std::shared_ptr<element::Element>& element) 
		{
			const auto normalNoise = std::dynamic_pointer_cast<element::NormalNoise>(element);
			element::NormalNoiseParameters nnp = normalNoise->getParameters();

			auto amplitude = static_cast<float>(nnp.amplitude);

			const std::string label = "##" + element->getUniqueName() + "Amplitude";
			ImGui::SliderFloat(label.c_str(), &amplitude, 0, 5);
			ImGui::SameLine(); ImGui::Text("Amplitude");

			nnp.amplitude = amplitude;

			normalNoise->setParameters(nnp);
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
			auto sigmaExc = static_cast<float>(lip.sigmaExc);
			auto amplitudeInh = static_cast<float>(lip.amplitudeInh);
			auto sigmaInh = static_cast<float>(lip.sigmaInh);
			auto amplitudeGlobal = static_cast<float>(lip.amplitudeGlobal);

			std::string label = "##" + element->getUniqueName() + "Amplitude exc.";
			ImGui::SliderFloat(label.c_str(), &amplitudeExc, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude exc.");

			label = "##" + element->getUniqueName() + "Sigma exc.";
			ImGui::SliderFloat(label.c_str(), &sigmaExc, 0, 30);
			ImGui::SameLine(); ImGui::Text("Sigma exc.");

			label = "##" + element->getUniqueName() + "Amplitude inh.";
			ImGui::SliderFloat(label.c_str(), &amplitudeInh, 0, 100);
			ImGui::SameLine(); ImGui::Text("Amplitude inh.");

			label = "##" + element->getUniqueName() + "Sigma inh.";
			ImGui::SliderFloat(label.c_str(), &sigmaInh, 0, 30);
			ImGui::SameLine(); ImGui::Text("Sigma inh.");

			label = "##" + element->getUniqueName() + "Amplitude global";
			ImGui::SliderFloat(label.c_str(), &amplitudeGlobal, -5.0, 0.0);
			ImGui::SameLine(); ImGui::Text("Amplitude global");

			lip.amplitudeExc = amplitudeExc;
			lip.sigmaExc = sigmaExc;
			lip.amplitudeInh = amplitudeInh;
			lip.sigmaInh = sigmaInh;
			lip.amplitudeGlobal = amplitudeGlobal;

			kernel->setParameters(lip);
		}
	}


}
