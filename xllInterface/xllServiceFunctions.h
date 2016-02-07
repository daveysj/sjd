#ifndef sjdXLLServiceFunctions_INCLUDED
#define sjdXLLServiceFunctions_INCLUDED

#include <ql\auto_link.hpp>

#include <vector>

#include "./xll/xloper.h"
#include "./xll/xl_array.h"
#include "./xll/cpp_xloper.h"
#include "./xll/xllAddIn.h"
#include <sjd/OptionPricing/OptionsFormula.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaces.h>

using namespace std;

bool isSetToExpire();
bool hasExpired();
int GetExpiryDay();
int GetExpiryMonth();
int GetExpiryYear();


xloper* returnXloperOnError(string errorMessage);

vector<double> constructVector(xl_array* column, bool &successful, string &errorMessage);

void extractDataFromSurface(double anchorDate, xl_array* surfaceInput, vector<double> &columnHeadings, vector<double> &rowHeadings, vector<vector<double>> &data, bool &successful, string &errorMessage);

template <typename T>
xloper* returnXloper(vector<T> outputVector)
{
    WORD output_rows = (WORD) outputVector.size();
    WORD output_columns = 1;
   cpp_xloper outputMatrix(output_rows, output_columns);
    for (size_t i = 0; i < output_rows; ++i) {
        double rate = outputVector[i];
        outputMatrix.SetArrayElement(i,0,rate);
    }
    return outputMatrix.ExtractXloper(false);
}

bool getOptionType(char *putOrCAll, sjd::PutCall &type, string &errorMessage);

// used when there is an option to return data to excel as a row or a column
void formatOutput(vector<double> data, bool outputAsRow, cpp_xloper &formattedOutput);
// used when there is an option to return a heading row data to excel as a row or a column 
void formatOutput(vector<string> heading, vector<double> data, bool outputAsRow, cpp_xloper &formattedOutput);

#endif