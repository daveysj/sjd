#include "sjdXLLInterface.h"

xloper* __stdcall interpolateXLL(char* interpolatorType, xl_array *xArray, xl_array *yArray, xl_array *pointsAtWhichToInterpolate, bool extrapolate)
{
    return returnXloperOnError("Not yet implemented");
}


xloper* returnXloperOnError(std::string errorMessage)
{
    cpp_xloper outputMatrix(1, 1);
	outputMatrix.SetArrayElement(0,0,(char*) errorMessage.c_str());
	return outputMatrix.ExtractXloper(false);
}
