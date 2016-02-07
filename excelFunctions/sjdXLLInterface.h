#ifndef sjdXLLInterface_INCLUDED
#define sjdXLLInterface_INCLUDED

#include "./xll/xloper.h"
#include "./xll/xl_array.h"
#include "./xll/cpp_xloper.h"
#include "./xll/xllAddIn.h"


xloper* __stdcall interpolateXLL(char* interpolatorType, xl_array *xArray, xl_array *yArray, xl_array *pointsAtWhichToInterpolate, bool extrapolate = false);

xloper* returnXloperOnError(std::string errorMessage);

#endif
