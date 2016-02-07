#include "sjdXLLInterface.h"

#include <boost/lexical_cast.hpp> // to convert numbers to strings

string expiredXllMsg = "The xll that contains this function has exipred. Please call Steve Davey on +27 83 408 7656 to get an updated dll";
    
xloper* __stdcall prGetXllExpiryDate()
{
    string expiryString;
    if (isSetToExpire()) {
        expiryString = "This xll is set to expire on " + boost::lexical_cast<string>(GetExpiryDay()) + 
            "/" + boost::lexical_cast<string>(GetExpiryMonth()) + "/" + boost::lexical_cast<string>(GetExpiryYear()); 
    }
    else {
        expiryString = "This xll does not expire";
    }
    cpp_xloper outputMatrix(1, 1);
	outputMatrix.SetArrayElement(0,0,(char*) expiryString.c_str());
	return outputMatrix.ExtractXloper(false);
}

xloper* __stdcall prInterpolate(char* type, xl_array *xArray, xl_array *yArray, xl_array *pointsAtWhichToInterpolate, bool extrapolate)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    bool successful = false;
    string errorMessage = "";
    vector<double> xVector = constructVector(xArray, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);
    vector<double> yVector = constructVector(yArray, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);
    vector<double> interpolationVector = constructVector(pointsAtWhichToInterpolate, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);

    ArrayInterpolator *arrayInterpolator;

    string interpolatorType(type);
    boost::to_lower(interpolatorType);
    if (interpolatorType.compare("linear") == 0) {
        arrayInterpolator = new LinearArrayInterpolator(xVector, yVector, extrapolate);
    }
    else if (interpolatorType.compare("cubic") == 0) {
        arrayInterpolator = new CubicSplineInterpolator(xVector, yVector, extrapolate);
    }
    else 
        return returnXloperOnError("Interpolator type \"" + interpolatorType + "\" not yet implemented, please use \"linear\" or \"cubic\"");

    if (!arrayInterpolator->isInRange(interpolationVector))
        return returnXloperOnError("Points to which to interpolate are not in the required range");

    vector<double> interpolatedValues = arrayInterpolator->getRate(interpolationVector);
    delete arrayInterpolator;

    return returnXloper<double>(interpolatedValues);
}

xloper* __stdcall prVolFromDeltaSurface(char* type, double anchorDate, xl_array *surface, double toDate, double strike, double forward, bool extrapolate)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    if ((forward < 1e-14) || (strike < 1e-14))
        return returnXloperOnError("All numeric inputs to this function must be strictly positive");

    vector<double> columnHeadings, rowHeadings;
    vector<vector<double>> data;
    bool successful = false;
    string errorMessage = "";
    extractDataFromSurface(anchorDate, surface, columnHeadings, rowHeadings, data, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);

    SimpleDeltaSurface deltaSurface(columnHeadings, rowHeadings, data, extrapolate, type);
    if (!deltaSurface.isOK())
        return returnXloperOnError(deltaSurface.getErrorMessage());
    

    double moneyness = (strike - forward) / forward;
    double time = (toDate - anchorDate) / 365;
    if (!deltaSurface.isInMoneynessRange(time, moneyness))
        return returnXloperOnError("Point to interpolate is outside of the surface range and extrapolation is set to false");

    double vol = deltaSurface.getVolatilityForMoneyness(time, moneyness);

    cpp_xloper outputObject(1,1);
    outputObject.SetArrayElement(0, 0, vol);
    return outputObject.ExtractXloper(false);
}

xloper* __stdcall prOptionValue(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    if ((forward < 1e-14) || (strike < 1e-14) || (standardDeviation < 1e-14) || (discountFactor < 1e-14))
        return returnXloperOnError("All numeric inputs to this function must be strictly positive");

    string errorMessage = "";
    PutCall type;
    if(!getOptionType(putOrCall, type, errorMessage))
        return returnXloperOnError(errorMessage);
    double optionPremium;
    if (type == CALL) {
        if (standardDeviation < 1e-14)
            optionPremium = max(forward - strike, 0.0) * discountFactor;
        else
            optionPremium = EuropeanCallBlack76::getPremium(forward, strike, standardDeviation, discountFactor);
    }
    else {
        if (abs(standardDeviation) < 1e-14)
            optionPremium = max(strike - forward, 0.0) * discountFactor;
        else
            optionPremium = EuropeanPutBlack76::getPremium(forward, strike, standardDeviation, discountFactor);
    }

    cpp_xloper outputObject(1,1);
    outputObject.SetArrayElement(0, 0, optionPremium);
    return outputObject.ExtractXloper(false);
}

xloper* __stdcall prOptionDelta(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    if ((forward < 1e-14) || (strike < 1e-14) || (standardDeviation < 1e-14) || (discountFactor < 1e-14))
        return returnXloperOnError("All numeric inputs to this function must be strictly positive");

    string errorMessage = "";
    PutCall type;
    if(!getOptionType(putOrCall, type, errorMessage))
        return returnXloperOnError(errorMessage);
    EuropeanOptionBlack76* option;
    double optionDelta;
    if (type == CALL) {        
        if (standardDeviation < 1e-14) {
            if (forward >= strike) 
                optionDelta = 1;
            else 
                optionDelta = 0;
        }
        else {
            option = new EuropeanCallBlack76(forward, strike, 0, 0, standardDeviation, discountFactor);
            optionDelta = option->getDeltaAnalytic();
            delete option;
        }
    }
    else {
        if (abs(standardDeviation) < 1e-14) {
            if (forward <= strike) 
                optionDelta = 1.0;
            else 
                optionDelta = 0.0;
        }
        else {
            option = new EuropeanPutBlack76(forward, strike, 0, 0, standardDeviation, discountFactor);
            optionDelta = option->getDeltaAnalytic();
            delete option;
        }
    }
    cpp_xloper outputObject(1,1);
    outputObject.SetArrayElement(0, 0, optionDelta);
    return outputObject.ExtractXloper(false);
}


xloper* __stdcall black76PremiumAndAnalyticGreeks(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor, 
                                          double yearsToExpiry, double yearsToSettlement, bool returnAsRow, bool heading)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    if ((forward <= 0) || (strike <= 0) || (standardDeviation <= 0) || (discountFactor <=0) || (yearsToExpiry <= 0) || (yearsToSettlement <= 0) )
        return returnXloperOnError("All numeric inputs to this function must be strictly positive");
    string errorMessage = "";
    PutCall type;
    if(!getOptionType(putOrCall, type, errorMessage))
        return returnXloperOnError(errorMessage);
    EuropeanOptionBlack76* option;
    if (type == CALL)
        option = new EuropeanCallBlack76(forward, strike, yearsToExpiry, yearsToSettlement, standardDeviation, discountFactor);
    else 
        option = new EuropeanPutBlack76(forward, strike, yearsToExpiry, yearsToSettlement ,standardDeviation, discountFactor);
    if (!option->isOK()) {
        errorMessage = option->getErrorMessage();
        delete option;
        return returnXloperOnError(errorMessage);
    }

    BlackScholesGreeks greeks;
    greeks = option->getOptionPriceAndGreeksAnalytic();

    vector<double> data = greeks.getAllGreeks();
    cpp_xloper outputObject;
    if (heading) {
        vector<string> headings = greeks.getAllGreeksDescription();
        formatOutput(headings, data, returnAsRow, outputObject);
    }
    else {
        formatOutput(data, returnAsRow, outputObject);
    }
    delete option;
    return outputObject.ExtractXloper(false);
}

xloper* __stdcall black76PremiumAndNumericGreeks(char* putOrCall, double forward, double strike, double standardDeviation, double discountFactor, 
                                                 double yearsToExpiry, double yearsToSettlement, bool returnAsRow, bool heading, 
                                                 double changeInFwd, double changeInVol, double changeInNACCRate, double changeInTime)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    if ((forward <= 0) || (strike <= 0) || (standardDeviation <= 0) || (discountFactor <=0) || (yearsToExpiry <= 0) || (yearsToSettlement <= 0) )
        return returnXloperOnError("All numeric inputs to this function must be strictly positive");
    string errorMessage = "";
    PutCall type;
    if(!getOptionType(putOrCall, type, errorMessage))
        return returnXloperOnError(errorMessage);
    EuropeanOptionBlack76* option;
    if (type == CALL)
        option = new EuropeanCallBlack76(forward, strike, yearsToExpiry, yearsToSettlement, standardDeviation, discountFactor);
    else 
        option = new EuropeanPutBlack76(forward, strike, yearsToExpiry, yearsToSettlement ,standardDeviation, discountFactor);
    if (!option->isOK()) {
        errorMessage = option->getErrorMessage();
        delete option;
        return returnXloperOnError(errorMessage);
    }

    BlackScholesGreeks greeks;
    greeks = option->getOptionPriceAndGreeksNumeric(changeInFwd, changeInVol, changeInTime, changeInNACCRate);

    vector<double> data = greeks.getAllGreeks();
    cpp_xloper outputObject;
    if (heading) {
        vector<string> headings = greeks.getAllGreeksDescription();
        formatOutput(headings, data, returnAsRow, outputObject);
    }
    else {
        formatOutput(data, returnAsRow, outputObject);
    }
    delete option;
    return outputObject.ExtractXloper(false);
}

xloper* __stdcall asianPremium(char* putOrCall, xl_array *fwds, int numRateSetFwds, double X, xl_array *sds, double df)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    // Using static call in AverageRate class so we MUST do some sanity checking
    if (numRateSetFwds < 0)
        return returnXloperOnError("Number of rate set forwards must be non-negative");
    if (X <= 0)
        return returnXloperOnError("Strike must be positive");
    if (df <= 0)
        return returnXloperOnError("Discount Factor must be positive");
    bool successful;
    string errorMessage = "";
    vector<double> fwdsVector = constructVector(fwds, successful, errorMessage);
    size_t N = fwdsVector.size();
    if (!successful)
        return returnXloperOnError(errorMessage);
    vector<double> sdVector = constructVector(sds, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);
    if (N != sdVector.size())
        return returnXloperOnError("Number standard deviations does not equal the number of forwards");

    PutCall type;
    if(!getOptionType(putOrCall, type, errorMessage))
        return returnXloperOnError(errorMessage);
    double optionPremium;
    if (type == CALL)
        optionPremium = AverageRateCall::getPremium(fwdsVector, numRateSetFwds, X, sdVector, df);
    else 
        optionPremium = AverageRatePut::getPremium(fwdsVector, numRateSetFwds, X, sdVector, df);

    cpp_xloper outputObject(1,1);
    outputObject.SetArrayElement(0, 0, optionPremium);
    return outputObject.ExtractXloper(false);
}

xloper* __stdcall asianPremiumAndAnalyticGreeks(char* putOrCall, xl_array *fwds, xl_array *time, double X, 
                                               xl_array *sds, double df, double yte, double yts, 
                                               bool returnAsRow, bool heading)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    bool successful;
    string errorMessage;
    vector<double> fwdsVector = constructVector(fwds, successful, errorMessage);
    size_t N = fwdsVector.size();
    if (!successful)
        return returnXloperOnError(errorMessage);
    vector<double> sdVector = constructVector(sds, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);
    if (N != sdVector.size())
        return returnXloperOnError("Number of standard deviations does not equal the number of forwards");
    vector<double> timeVector = constructVector(time, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);
    if (N != timeVector.size())
        return returnXloperOnError("Number of times does not equal the number of forwards");


    PutCall type;
    if(!getOptionType(putOrCall, type, errorMessage))
        return returnXloperOnError(errorMessage);
    AverageRateOption* option;
    if (type == CALL)
        option = new AverageRateCall(timeVector, fwdsVector, X, yte, yts, sdVector, df);
    else 
        option = new AverageRatePut(timeVector, fwdsVector, X, yte, yts, sdVector, df);
    if (!option->isOK()) {
        errorMessage = option->getErrorMessage();
        delete option;
        return returnXloperOnError(errorMessage);
    }
    BlackScholesGreeks greeks;
    greeks = option->getOptionPriceAndGreeksAnalytic();
    vector<double> data = greeks.getAllGreeks();
    cpp_xloper outputObject;
    if (heading) {
        vector<string> headings = greeks.getAllGreeksDescription();
        formatOutput(headings, data, returnAsRow, outputObject);
    }
    else {
        formatOutput(data, returnAsRow, outputObject);
    }
    delete option;
    return outputObject.ExtractXloper(false);
}

xloper* __stdcall asianPremiumAndNumericGreeks(char* putOrCall, xl_array *fwds, xl_array *time, double X, 
                                       xl_array *sds, double df, double yte, double yts, 
                                       bool returnAsRow, bool heading,
                                       double changeInFwd, double changeInVol, double changeInNACCRate, double changeInTime)
{
    if (hasExpired())
        return returnXloperOnError(expiredXllMsg);

    bool successful;
    string errorMessage;
    vector<double> fwdsVector = constructVector(fwds, successful, errorMessage);
    size_t N = fwdsVector.size();
    if (!successful)
        return returnXloperOnError(errorMessage);
    vector<double> sdVector = constructVector(sds, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);
    if (N != sdVector.size())
        return returnXloperOnError("Number of standard deviations does not equal the number of forwards");
    vector<double> timeVector = constructVector(time, successful, errorMessage);
    if (!successful)
        return returnXloperOnError(errorMessage);
    if (N != timeVector.size())
        return returnXloperOnError("Number of times does not equal the number of forwards");

    PutCall type;
    if(!getOptionType(putOrCall, type, errorMessage))
        return returnXloperOnError(errorMessage);
    AverageRateOption* option;
    if (type == CALL)
        option = new AverageRateCall(timeVector, fwdsVector, X, yte, yts, sdVector, df);
    else 
        option = new AverageRatePut(timeVector, fwdsVector, X, yte, yts, sdVector, df);
    if (!option->isOK()) {
        errorMessage = option->getErrorMessage();
        delete option;
        return returnXloperOnError(errorMessage);
    }

    BlackScholesGreeks greeks;
    greeks = option->getOptionPriceAndGreeksNumeric(changeInFwd, changeInVol, changeInTime, changeInNACCRate);

    vector<double> data = greeks.getAllGreeks();
    cpp_xloper outputObject;
    if (heading) {
        vector<string> headings = greeks.getAllGreeksDescription();
        formatOutput(headings, data, returnAsRow, outputObject);
    }
    else {
        formatOutput(data, returnAsRow, outputObject);
    }
    delete option;
    return outputObject.ExtractXloper(false);
}

xloper * __stdcall getIntersection(xl_array* firstDataSet, xl_array* secondDataSet)
{

	WORD COLUMNS = 2;
	WORD COLUMNS_IN_RETURN_ARRAY = 3;

	WORD len1 = firstDataSet->rows;
	WORD len2 = secondDataSet->rows;
	WORD len = min(len1, len2);

	cpp_xloper unionArray((WORD)len, (WORD)3);

	if(!unionArray.IsType(xltypeMulti))
		return NULL;
	
	WORD i = 0; // counter for the data set 1
	WORD j = 0; // counter for the data set 2
	WORD k = 0; // counter for the output array

	double date1 = firstDataSet->array[i * COLUMNS];
	double date2 = secondDataSet->array[j * COLUMNS];
	while ((i < len1) && (j < len2) && (k < len)) 
   {
		while ((date1 != date2) && (i < len1) && (j < len2)) 
      {
			while ((date1 < date2) && (i < len1)) 
         {
				++i;
				date1 = firstDataSet->array[i * COLUMNS];
			}
			while ((date1 > date2) && (j < len2)) 
         {
				++j;
				date2 = secondDataSet->array[j * COLUMNS];
			}
		}
		if (date1 == date2) 
      {
			unionArray.SetArrayElement(k * COLUMNS_IN_RETURN_ARRAY, date1);
			double tmp1 = firstDataSet->array[i * COLUMNS + 1];
			unionArray.SetArrayElement(k * COLUMNS_IN_RETURN_ARRAY + 1, 
                                    firstDataSet->array[i * COLUMNS + 1]);
			double tmp2 = secondDataSet->array[j * COLUMNS + 1];
			unionArray.SetArrayElement(k * COLUMNS_IN_RETURN_ARRAY + 2, 
                                    secondDataSet->array[j * COLUMNS + 1]);

			++k; ++j; ++i;
			date1 = firstDataSet->array[i * COLUMNS];
			date2 = secondDataSet->array[j * COLUMNS];
		}
	}
	return unionArray.ExtractXloper(false);
}
