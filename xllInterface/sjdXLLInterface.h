#ifndef sjdXLLInterface_INCLUDED
#define sjdXLLInterface_INCLUDED

#include <sjd/maths/maths.h>
#include <sjd/OptionPricing/BlackScholesFormula.h>
#include "AsianOptionFormulaForExcel.h"
#include <sjd\RateSource\Volatility\VolatilitySurfaceDelta.h>
#include "xllServiceFunctions.h"



using namespace sjd;

xloper* __stdcall prGetXllExpiryDate();

xloper* __stdcall prInterpolate(char* interpolatorType, // Linear or Cubic                                
                                xl_array *xArray, 
                                xl_array *yArray, 
                                xl_array *pointsAtWhichToInterpolate, 
                                bool extrapolate = false);

xloper* __stdcall prVolFromDeltaSurface(char* type, double anchorDate, xl_array *surface, double toDate, double strike, double forward, bool extrapolate);

xloper* __stdcall prOptionValue(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor);

xloper* __stdcall prOptionDelta(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor);


xloper* __stdcall black76PremiumAndAnalyticGreeks(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor, 
                                                  double yearsToExpiry, double yearsToSettlement, bool returnAsRow = "ture", bool heading = "false");

xloper* __stdcall black76PremiumAndNumericGreeks(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor, 
                                                 double yearsToExpiry, double yearsToSettlement, bool returnAsRow = "ture", bool heading = "false", 
                                                 double changeInFwd = 0.0001, double changeInVol = 0.0001, 
                                                 double changeInNACCRate = 0.0001, double changeInTime = 0.0001);


xloper* __stdcall asianPremium(char* putOrCall, xl_array *fwds, int numRateSetFwdds, double X, xl_array *sds, double df);

xloper* __stdcall asianPremiumAndAnalyticGreeks(char* putOrCall, xl_array *forward, xl_array *yearFractionsToRateSet, double strike, 
                               double yearsToExpiry, double yearsToSettlement, xl_array *standardDeviation, 
                               double discountFactor, bool returnAsRow = "true", bool heading = "false");

xloper* __stdcall asianPremiumAndNumericGreeks(char* putOrCall, xl_array *forward,  xl_array *yearFractionsToRateSet, double strike, 
                               double yearsToExpiry, double yearsToSettlement, xl_array *standardDeviation, 
                               double discountFactor, bool returnAsRow = "true", bool heading = "false",
                               double changeInFwd = 0.0001, double changeInVol = 0.0001, 
                               double changeInNACCRate = 0.0001, double changeInTime = 0.0001);

xloper * __stdcall getIntersection(xl_array* firstDataSet, xl_array* secondDataSet);

#endif
