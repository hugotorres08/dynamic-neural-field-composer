#include <catch2/catch_test_macros.hpp>

#include "elements/element_factory.h"

using namespace dnf_composer::element;

TEST_CASE("ElementFactory - Creation of Neural Field")
{
    ElementFactory elementFactory;
    ElementParameters params;
    params.nfp.tau = 20.0;
    params.nfp.startingRestingLevel = 0.5;

    auto neuralField = elementFactory.create(ElementLabel::NEURAL_FIELD, "neural_field_1", 100, params);

    REQUIRE(neuralField != nullptr);
    REQUIRE(neuralField->getLabel() == ElementLabel::NEURAL_FIELD);
}

TEST_CASE("ElementFactory - Creation of Gauss Stimulus")
{
    ElementFactory elementFactory;
    ElementParameters params;
    params.gsp.amplitude = 1.0;
    params.gsp.sigma = 2.0;

    auto gaussStimulus = elementFactory.create(ElementLabel::GAUSS_STIMULUS, "gauss_stimulus_1", 50, params);

    REQUIRE(gaussStimulus != nullptr);
    REQUIRE(gaussStimulus->getLabel() == ElementLabel::GAUSS_STIMULUS);
}

TEST_CASE("ElementFactory - Creation of Gauss Kernel")
{
    ElementFactory elementFactory;
    ElementParameters params;
    params.gkp.amplitude = 0.8;
    params.gkp.sigma = 1.5;

    auto gaussKernel = elementFactory.create(ElementLabel::GAUSS_KERNEL, "gauss_kernel_1", 200, params);

    REQUIRE(gaussKernel != nullptr);
    REQUIRE(gaussKernel->getLabel() == ElementLabel::GAUSS_KERNEL);
}

TEST_CASE("ElementFactory - Creation of Mexican Hat Kernel")
{
    ElementFactory elementFactory;
    ElementParameters params;
    params.mhkp.amplitudeExc = 0.5;
    params.mhkp.sigmaExc = 1.0;
    params.mhkp.amplitudeInh = 0.3;
    params.mhkp.sigmaInh = 1.2;

    auto mexicanHatKernel = elementFactory.create(ElementLabel::MEXICAN_HAT_KERNEL, "mexican_hat_kernel_1", 150, params);

    REQUIRE(mexicanHatKernel != nullptr);
    REQUIRE(mexicanHatKernel->getLabel() == ElementLabel::MEXICAN_HAT_KERNEL);
}

TEST_CASE("ElementFactory - Creation of Normal Noise")
{
    ElementFactory elementFactory;
    ElementParameters params;
    params.nnp.amplitude = 0.2;

    auto normalNoise = elementFactory.create(ElementLabel::NORMAL_NOISE, "normal_noise_1", 300, params);

    REQUIRE(normalNoise != nullptr);
    REQUIRE(normalNoise->getLabel() == ElementLabel::NORMAL_NOISE);
}

TEST_CASE("ElementFactory - Creation of Gauss Field Coupling")
{
    ElementFactory elementFactory;
    ElementParameters params;
    params.gfcp.sigma = 10.0;
    params.gfcp.inputFieldSize = 100;

    auto gaussFieldCoupling = elementFactory.create(ElementLabel::GAUSS_FIELD_COUPLING, "gauss_field_coupling_1", 250, params);

    REQUIRE(gaussFieldCoupling != nullptr);
    REQUIRE(gaussFieldCoupling->getLabel() == ElementLabel::GAUSS_FIELD_COUPLING);
}

TEST_CASE("ElementFactory - Creation of Field Coupling")
{
    ElementFactory elementFactory;
    ElementParameters params;
    params.fcp.inputFieldSize = 200;

    auto fieldCoupling = elementFactory.create(ElementLabel::FIELD_COUPLING, "field_coupling_1", 180, params);

    REQUIRE(fieldCoupling != nullptr);
    REQUIRE(fieldCoupling->getLabel() == ElementLabel::FIELD_COUPLING);
}

TEST_CASE("ElementFactory - Unsupported Element Type")
{
    ElementFactory elementFactory;
    const ElementParameters params;

    auto unsupportedElement = elementFactory.create(static_cast<ElementLabel>(100), "unsupported_element", 100, params);

    REQUIRE(unsupportedElement == nullptr);
}
