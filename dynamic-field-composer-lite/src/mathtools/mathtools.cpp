// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "mathtools/mathtools.h"

std::array<int, 2> mathtools::computeKernelRange(double sigma, int cutOfFactor, int fieldSize, bool circular)
{
	int par1 = std::ceil(sigma * cutOfFactor);
	int fieldSizeMinus1 = (fieldSize - 1);
	if (circular)
	{

		double aux = ((double)fieldSize - 1) / 2;
		int par2 = std::floor(aux);
		int par3 = std::ceil(aux);
		return std::min(std::array<int, 2>{par1, par1}, std::array<int, 2>{par2, par3});
	}
	else {
		return std::min(std::array<int, 2>{par1, par1}, std::array<int, 2>{fieldSizeMinus1, fieldSizeMinus1});
	}
}

std::vector<int> mathtools::createExtendedIndex(int fieldSize, const std::array<int, 2>& kernelRange)
{

	const int startingValue = fieldSize - kernelRange[1] + 1;
	const int initialVectorSize = fieldSize - startingValue + 1;
	std::vector<int> initialVector(initialVectorSize);
	std::iota(initialVector.begin(), initialVector.end(), startingValue);
	std::vector<int> secondVector(fieldSize);
	std::iota(secondVector.begin(), secondVector.end(), 1);
	std::vector<int> thirdVector(kernelRange[0]);
	std::iota(thirdVector.begin(), thirdVector.end(), 1);

	std::vector<int> extendedVector;
	std::vector<int>::iterator it = extendedVector.begin();

	extendedVector.insert(it, initialVector.begin(), initialVector.end());
	it = extendedVector.begin() + initialVector.size();
	extendedVector.insert(it, secondVector.begin(), secondVector.end());
	it = extendedVector.begin() + initialVector.size() + secondVector.size();
	extendedVector.insert(it, thirdVector.begin(), thirdVector.end());

	return extendedVector;
}

std::vector<double> mathtools::generateNormalVector(int size)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> dist(0, 1);

	std::vector<double> vec(size);
	for (int i = 0; i < size; ++i)
	{
		vec[i] = dist(gen);
	}

	return vec;
}

int mathtools::countNumOfLinesInFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (file.is_open()) {
		int lineCount = 0;
		std::string line;
		while (std::getline(file, line)) {
			lineCount++;
		}
		file.close();
		return lineCount;
	}
	else {
		std::cerr << "Failed to open the file " << filename << std::endl;
		return -1; // Return -1 to indicate an error
	}
}