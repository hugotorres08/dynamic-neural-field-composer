#include <catch2/catch_test_macros.hpp>

#include "elements/element.h"
#include "elements/neural_field.h"

// Helper function to create a sample Element object for testing
std::shared_ptr<dnf_composer::element::NeuralField> createSampleNeuralField(const std::string& elementId, uint8_t size = 1)
{
    const dnf_composer::element::SigmoidFunction sigmoidFunction{ 1, 0 };
	dnf_composer::element::NeuralFieldParameters nfp{ 1, -5, sigmoidFunction };
    return std::make_shared<dnf_composer::element::NeuralField>(dnf_composer::element::ElementCommonParameters{ elementId, size }, nfp);
}

// Mock class for testing Element
class MockElement : public dnf_composer::element::Element
{
public:
    MockElement() : Element(dnf_composer::element::ElementCommonParameters{ "MockElement", 100 }) {}
    MockElement(const dnf_composer::element::ElementCommonParameters& parameters) : Element(parameters) {}

    void init() override {}
    void step(double t, double deltaT) override {}
    void close() override {}
    std::shared_ptr<Element> clone() const override {
        return std::make_shared<MockElement>(*this);
    }
    void printParameters() override {}
};


TEST_CASE("Element class tests", "[element]")
{
    SECTION("Initialization and Basic Information")
    {
        auto element = std::make_shared<MockElement>();

        // Check initial state
        REQUIRE(element->getUniqueName() == "MockElement");
        REQUIRE(element->getLabel() == dnf_composer::element::ElementLabel::UNINITIALIZED);
        REQUIRE(element->getSize() == 100);
    }
    SECTION("addInput() hasInput() methods")
    {
        // Create a mock Element object for testing
        auto element = createSampleNeuralField("Element1");

        // Add input
        auto inputElement = createSampleNeuralField("inputElement");
        element->addInput(inputElement, "output");

        // Add nullptr element, should not throw exception
        element->addInput(nullptr, "component");
        REQUIRE(element->getInputs().size() == 1);

        // Check if input was added
        REQUIRE(element->hasInput("inputElement", "output") == true);
        REQUIRE(element->hasInput("inputElement", "input") == false);
        REQUIRE(element->hasInput("nonexistentElement", "output") == false);

        // Add input with same unique identifier, nothing should happen
        const int numberOfInputsBeforeAttemptedAddInput = static_cast<int>(element->getInputs().size());
        element->addInput(inputElement, "output");
    	int numberOfInputsAfterAttemptedAddInput = static_cast<int>(element->getInputs().size());
        REQUIRE(numberOfInputsBeforeAttemptedAddInput == numberOfInputsAfterAttemptedAddInput);

        // Add input with different sizes, nothing should happen
        auto inputElement2 = createSampleNeuralField("inputElement2", 2);
        numberOfInputsAfterAttemptedAddInput = static_cast<int>(element->getInputs().size());
        REQUIRE(numberOfInputsBeforeAttemptedAddInput == numberOfInputsAfterAttemptedAddInput);
    }
    SECTION("removeInput() method")
    {
        // Create a mock Element object for testing
        auto element = createSampleNeuralField("Element1");

        // Add input
        auto inputElement = createSampleNeuralField("inputElement");
        REQUIRE(inputElement->getUniqueName() == "inputElement");
        element->addInput(inputElement, "output");

        // Remove input
        element->removeInput("inputElement");

        // Check if input was removed
        REQUIRE(element->hasInput("inputElement", "output") == false);
    }
    SECTION("updateInput() getComponent() getComponentPtr() methods")
    {
        uint8_t size = 5;
        std::vector<double> expectedInput(size, 0.0);
          
        // Create a mock Element object for testing
        auto element = createSampleNeuralField("Element1", size);

        // Add input
        auto inputElement1 = createSampleNeuralField("inputElement", size);
        element->addInput(inputElement1, "output");

        // Add another input
        auto inputElement2 = createSampleNeuralField("inputElement2", size);
        element->addInput(inputElement2, "output");

        // Update input
        element->updateInput();

        // Check if input was updated
        std::vector<double> actualInput = element->getComponent("input");
        REQUIRE(actualInput == expectedInput);
        std::vector<double> actualInputPtr = *element->getComponentPtr("input");
        REQUIRE(actualInputPtr == expectedInput);

    }
    SECTION("getUniqueIdentifier() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        // Check unique identifier
        REQUIRE(element->getUniqueName() == "MockElement");
    }
    SECTION("getLabel() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        // Check element label
        REQUIRE(element->getLabel() == dnf_composer::element::ElementLabel::UNINITIALIZED);
    }
    SECTION("getSize() method")
    {
        // Create a mock Element object for testing
        auto element = std::make_shared<MockElement>();
        REQUIRE(element->getSize() == 100);
    }
}