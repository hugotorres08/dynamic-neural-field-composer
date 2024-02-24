#pragma once

#include "simulation/simulation.h"
#include "user_interface/user_interface_window.h"
#include "elements/gauss_stimulus.h"
#include "elements/neural_field.h"


namespace dnf_composer
{
	namespace user_interface
	{
		class ElementWindow : public UserInterfaceWindow
		{
		private:
			std::shared_ptr<Simulation> simulation;
		public:
			ElementWindow(const std::shared_ptr<Simulation>& simulation);

			ElementWindow(const ElementWindow&) = delete;
			ElementWindow& operator=(const ElementWindow&) = delete;
			ElementWindow(ElementWindow&&) = delete;
			ElementWindow& operator=(ElementWindow&&) = delete;

			void render() override;
			~ElementWindow() override = default;
		private:
			void renderModifyElementParameters() const;
			static void switchElementToModify(const std::shared_ptr<element::Element>& element);
			// unfortunately, these functions need to be here like this
			//                .=-.-.   _ __    
			//    .-.,.---.  /==/_ /.-`.' ,`.  
			//   /==/  `   \|==|, |/==/, -   \ 
			//  |==|-, .=., |==|  |==| _ .=. | 
			//  |==|   '='  /==|- |==| , '=',| 
			//  |==|- ,   .'|==| ,|==|-  '..'  
			//  |==|_  . ,'.|==|- |==|,  |     
			//  /==/  /\ ,  )==/. /==/ - |     
			//  `--`-`--`--'`--`-``--`---'     
			static void modifyElementNeuralField(const std::shared_ptr<element::Element>& element) ;
			static void modifyElementGaussStimulus(const std::shared_ptr<element::Element>& element);
			static void modifyElementFieldCoupling(const std::shared_ptr<element::Element>& element);
			static void modifyElementGaussKernel(const std::shared_ptr<element::Element>& element);
			static void modifyElementMexicanHatKernel(const std::shared_ptr<element::Element>& element);
			static void modifyElementNormalNoise(const std::shared_ptr<element::Element>& element);
			static void modifyElementGaussFieldCoupling(const std::shared_ptr<element::Element>& element);
			static void modifyElementLateralInteractions(const std::shared_ptr<element::Element>& element);

		};
	}
}