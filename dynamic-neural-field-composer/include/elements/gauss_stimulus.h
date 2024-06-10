#pragma once

#include <iostream>

#include "tools/math.h"
#include "element.h"


namespace dnf_composer
{
	namespace element
	{
		struct GaussStimulusParameters : ElementSpecificParameters
		{

			GaussStimulusParameters(double width = 5.0, double amplitude = 15.0, 
				double position = 0.0, bool circular = true, bool normalized = false)
				: width(width), amplitude(amplitude), position(position),
					circular(circular), normalized(normalized)
			{}

			double width;
			double amplitude;
			double position;
			bool circular;
			bool normalized;

			bool operator==(const GaussStimulusParameters& other) const
			{
				constexpr double epsilon = 1e-6;

				return std::abs(width - other.width) < epsilon &&
					std::abs(position - other.position) < epsilon &&
					std::abs(amplitude - other.amplitude) < epsilon &&
					circular == other.circular &&
					normalized == other.normalized;
			}
		};

		class GaussStimulus : public Element
		{
		private:
			GaussStimulusParameters parameters;
		public:
			GaussStimulus(const ElementCommonParameters& elementCommonParameters,
				const GaussStimulusParameters& parameters);

			void init() override;
			void step(double t, double deltaT) override;
			void printParameters() override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const GaussStimulusParameters& parameters);
			GaussStimulusParameters getParameters() const;
		};
	}
}
