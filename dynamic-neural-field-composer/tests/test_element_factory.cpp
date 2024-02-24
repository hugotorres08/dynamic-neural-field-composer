#include <catch2/catch_test_macros.hpp>

#include "elements/element_factory.h"

using namespace dnf_composer::element;

// Neural Field
TEST_CASE("ElementFactory - Creation of Neural Field")
{
    ElementFactory elementFactory;
    const NeuralFieldParameters nfp{ 20.0, 0.5, SigmoidFunction{0.0, 10.0}}; // Example parameters
    const ElementCommonParameters ecp{ "neural_field_1", 100 };

    auto neuralField = elementFactory.createElement(ElementLabel::NEURAL_FIELD, ecp, nfp);
    REQUIRE(neuralField != nullptr);
    REQUIRE(neuralField->getLabel() == ElementLabel::NEURAL_FIELD);
}

// Gauss Stimulus
TEST_CASE("ElementFactory - Creation of Gauss Stimulus")
{
    ElementFactory elementFactory;
    const GaussStimulusParameters gsp{ 1.0, 2.0 }; // Example parameters
    const ElementCommonParameters ecp{ "gauss_stimulus_1", 50 };

    auto gaussStimulus = elementFactory.createElement(ElementLabel::GAUSS_STIMULUS, ecp, gsp);
    REQUIRE(gaussStimulus != nullptr);
    REQUIRE(gaussStimulus->getLabel() == ElementLabel::GAUSS_STIMULUS);
}

// Gauss Kernel
TEST_CASE("ElementFactory - Creation of Gauss Kernel")
{
    ElementFactory elementFactory;
    const GaussKernelParameters gkp{ 0.8, 1.5 }; // Example parameters
    const ElementCommonParameters ecp{ "gauss_kernel_1", 200 };

    auto gaussKernel = elementFactory.createElement(ElementLabel::GAUSS_KERNEL, ecp, gkp);
    REQUIRE(gaussKernel != nullptr);
    REQUIRE(gaussKernel->getLabel() == ElementLabel::GAUSS_KERNEL);
}

// Mexican Hat Kernel
TEST_CASE("ElementFactory - Creation of Mexican Hat Kernel")
{
    ElementFactory elementFactory;
    const ElementCommonParameters ecp{ "mexican_hat_kernel_1", 150 };
    const MexicanHatKernelParameters mhkp{ 0.5, 1.0, 0.3, 1.2 }; // Example parameters

    auto mexicanHatKernel = elementFactory.createElement(ElementLabel::MEXICAN_HAT_KERNEL, ecp, mhkp);
    REQUIRE(mexicanHatKernel != nullptr);
    REQUIRE(mexicanHatKernel->getLabel() == ElementLabel::MEXICAN_HAT_KERNEL);
}

// Normal Noise
TEST_CASE("ElementFactory - Creation of Normal Noise")
{
    ElementFactory elementFactory;
    const NormalNoiseParameters nnp{ 0.2 }; // Example parameter
    const ElementCommonParameters ecp{ "normal_noise_1", 300 };

    auto normalNoise = elementFactory.createElement(ElementLabel::NORMAL_NOISE, ecp, nnp);
    REQUIRE(normalNoise != nullptr);
    REQUIRE(normalNoise->getLabel() == ElementLabel::NORMAL_NOISE);
}

// Field Coupling
TEST_CASE("ElementFactory - Creation of Field Coupling")
{
    ElementFactory elementFactory;
    const FieldCouplingParameters fcp{100, 0.3, 0.2, dnf_composer::LearningRule::OJA}; 
    const ElementCommonParameters ecp{ "field_coupling_1", 180 };

    auto fieldCoupling = elementFactory.createElement(ElementLabel::FIELD_COUPLING, ecp, fcp);
    REQUIRE(fieldCoupling != nullptr);
    REQUIRE(fieldCoupling->getLabel() == ElementLabel::FIELD_COUPLING);
}

// Lateral Interactions 
TEST_CASE("ElementFactory - Creation of Lateral Interactions")
{
    ElementFactory elementFactory;
    const LateralInteractionsParameters lip{ 0.5, 0.8, 0.3, 0.6, -0.1 };
    const ElementCommonParameters ecp{ "lateral_interactions_1", 120 };

    auto lateralInteractions = elementFactory.createElement(ElementLabel::LATERAL_INTERACTIONS, ecp, lip);
    REQUIRE(lateralInteractions != nullptr);
    REQUIRE(lateralInteractions->getLabel() == ElementLabel::LATERAL_INTERACTIONS);
}