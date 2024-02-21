#pragma once

#include <chrono>
#include <string>
#include <iostream>

namespace dnf_composer
{
	namespace tools
	{
		namespace profiling
		{
			// to use the timer class:
			// void your_function()
			// {
			//      {
			//          timer t;
			//          const int a = 1 + 5;
			//          std::cout << "My integer is: " << a << std::endl;
			//		}
			// }

			class Timer
			{
			public:
				Timer(std::string sig = "something that takes time");
				~Timer();
			private:
				void stop() const;
				std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
				std::string sig;
			};
		}
	}
}

