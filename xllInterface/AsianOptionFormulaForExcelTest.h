#ifndef asianoption_test_maths
#define asianoption_test_maths

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/math/special_functions/fpclassify.hpp> // isnan
#include <sjd\sjdTest\TestingParameters.h>
#include "AsianOptionFormula.h"

/*======================================================================================
AsianOptionInputParameters
A simple class to group inputs for Asian Option. This makes testing a lot neater
=======================================================================================*/
class AsianOptionInputParameters
{
public:
    AsianOptionInputParameters(vector<double> yearFractionsToRateSets,
                                vector<double> fixingAndForwardRates, 
                                double strike, 
                                double yearsToExpiry, 
                                double yearsToSettlement, 
                                vector<double> standardDeviation, 
                                double discountFactorToSettlement);

    vector<double> time, fwds, sds;
    double strike, yte, yts, df; 
    size_t numFwdsRateSet;
};

class AsianOptionTest {
  public:
    static void testErrorMessages();
    static void testErrorMessagesWithSpecificData(AsianOptionInputParameters inputParamters);
    static void testGreeks();
    static void testGreeksWithSpecificData(AsianOptionInputParameters inputParamters);
    static void testSpecialCases();
    static void testSpecialCasesWithSpecificData(AsianOptionInputParameters inputParamters);



    static boost::unit_test_framework::test_suite* suite();

private:
    static AsianOptionInputParameters getAsianInputsBeforeAveragingStarts();
    static AsianOptionInputParameters getAsianInputsDuringAveraging(); // soon after averaging starts
    static AsianOptionInputParameters getAsianInputsJustBeforeSettlementWithHighAverage(); // relative to strike
    static AsianOptionInputParameters getAsianInputsJustBeforeSettlementWithLowAverage(); // realtive to strike
};

#endif
