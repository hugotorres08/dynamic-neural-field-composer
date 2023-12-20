#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "elements/neural_field.h"
#include "elements/activation_function.h"

TEST_CASE("NeuralField class tests", "[neural_field]")
{
    std::string id = "nf";
    int size = 10;
    double tau = 1.0;
    double sigmoidSteepness = 2.0;
    double startingRestingLevel = -1.5;
    const dnf_composer::element::SigmoidFunction sigmoid{ sigmoidSteepness, 0 };
    dnf_composer::element::NeuralFieldParameters nfp{ tau, startingRestingLevel, sigmoid};

	SECTION("NeuralField constructor and getParameters() method")
	{
        dnf_composer::element::NeuralField nf({ id, size }, nfp);

        REQUIRE(nf.getLabel() == dnf_composer::element::ElementLabel::NEURAL_FIELD);
        REQUIRE(nf.getUniqueName() == id);
        REQUIRE(nf.getSize() == size);
        //REQUIRE(nf.getParameters() == nfp);

        REQUIRE(static_cast<int>(nf.getComponent("input").size()) == size);
        REQUIRE(static_cast<int>(nf.getComponent("activation").size()) == size);
        REQUIRE(static_cast<int>(nf.getComponent("output").size()) == size);
        REQUIRE(static_cast<int>(nf.getComponent("resting level").size()) == size);
	}

    SECTION("init() method")
    {
        dnf_composer::element::NeuralField nf({ id, size }, nfp);

        nf.init();

        REQUIRE(nf.getComponent("input").size() == size);
        for (const auto& value : nf.getComponent("input"))
            REQUIRE(Catch::Approx(value) == 0.0);


        REQUIRE(nf.getComponent("activation").size() == size);
        for (const auto& value : nf.getComponent("activation"))
            REQUIRE(Catch::Approx(value) == startingRestingLevel);

        REQUIRE(nf.getComponent("output").size() == size);

        REQUIRE(nf.getComponent("resting level").size() == size);
        for (const auto& value : nf.getComponent("resting level"))
            REQUIRE(Catch::Approx(value) == startingRestingLevel);
    }

    SECTION("step() close() methods") {
        dnf_composer::element::NeuralField nf({ id, size }, nfp);

        // Run a single step
        nf.step(0, 1);
        nf.close();
        //invoked without exceptions
    }

    SECTION("setParameters() method")
    {
        dnf_composer::element::NeuralField nf({ id, size }, nfp);
        double newTau = 1.5;
        double newStartingRestingLevel = -1;

	    dnf_composer::element::NeuralFieldParameters new_nf{ newTau, newStartingRestingLevel, sigmoid };

        nf.setParameters(new_nf);

        // Check if parameters were updated
        //REQUIRE(nf.getParameters() == new_nf);

    }

}