#include "simulation/simulation.h"
#include "visualization/visualization.h"

#include "elements/element_factory.h"

int main(int argc, char* argv[])
{
	try
	{
		using namespace dnf_composer;
		using namespace dnf_composer::element;

		const std::shared_ptr<Simulation> simulation = 
			std::make_shared<Simulation>("test", 1.0, 0.0, 0.0);

		Visualization visualization(simulation);

		ElementFactory elementFactory;

		const auto elem1 = elementFactory.createElement(NEURAL_FIELD, ElementCommonParameters(), NeuralFieldParameters());
		const auto elem2 = elementFactory.createElement(GAUSS_STIMULUS, ElementCommonParameters(), GaussStimulusParameters());
		const auto elem3 = elementFactory.createElement(GAUSS_KERNEL, ElementCommonParameters(), GaussKernelParameters());

		simulation->addElement(elem1);
		simulation->addElement(elem2);
		simulation->addElement(elem3);
		simulation->init();

		simulation->step();

		visualization.plot({ { elem1->getUniqueName(), "output"}, {elem2->getUniqueName(), "output"}});

		visualization.plot(elem3->getUniqueName(), "output");

		PlotDimensions plotDimensions{ 1,2,3,4,1.0 };
		PlotAnnotations plotAnnotations{ "x", "y", "title" };
		PlotParameters plotParameters{ plotDimensions, plotAnnotations };

		visualization.plot(plotParameters, { { elem1->getUniqueName(), "output" } });
		visualization.plot(plotParameters, elem3->getUniqueName(), "output" );

		visualization.plot(0, { { elem1->getUniqueName(), "output" } });
		//visualization.plot(5, { { elem2->getUniqueName(), "output" } }); // raises exception
		//visualization.plot(1, elem2->getUniqueName(), "output");
		visualization.plot(1, { { elem1->getUniqueName(), "output" },  { elem1->getUniqueName(), "activation" }, { elem1->getUniqueName(), "input" } });
		visualization.plot(1, elem1->getUniqueName(), "output");
		visualization.plot(1, elem2->getUniqueName(), "output");


		visualization.changePlotParameters(0, plotParameters);
		visualization.removePlot(0);

		visualization.removePlottingDataFromPlot(1, { elem2->getUniqueName(), "output" });
		visualization.removePlottingDataFromPlot(1, { elem1->getUniqueName(), "activation" });
		visualization.removePlottingDataFromPlot(1, { elem1->getUniqueName(), "output" });
		visualization.removePlottingDataFromPlot(1, { elem3->getUniqueName(), "input" });


		visualization.removeAllPlots();

		return 0;
	}
	catch (const dnf_composer::Exception& ex)
	{
		const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". ";
		log(dnf_composer::tools::logger::LogLevel::FATAL, errorMessage, dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Unknown exception occurred. ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
}