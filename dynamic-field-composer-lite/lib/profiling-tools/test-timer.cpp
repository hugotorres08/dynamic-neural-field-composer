#include "timer.h"

int main()
{
	{
		Timer t{"A for cycle for 10000 iterations."};
		for (int i = 0; i < 10000; i++);
	}

	{
		Timer t{ "A std::cout and std::endl for 10000 iterations." };
		std::cout << std::endl;
	}

}