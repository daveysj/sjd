#include "xllServiceFunctions.h"


#include <fstream>
#include "math.h"
#include <string>
#include <cctype>

using namespace std;

#define __cplusplus__strings__

//the expiry date
const int ExpiryDay = 10;
const int ExpiryMonth = 3;
const int ExpiryYear = 2015;

//TO DISABLE, SET mustProtect to FALSE
const bool mustProtect = false;

bool isSetToExpire() {
    return mustProtect;
}

bool hasExpired()
{
   if (isSetToExpire())
   {
      //get the system time
      SYSTEMTIME st;
      GetSystemTime(&st);

      if ((ExpiryYear < st.wYear) || ((ExpiryYear == st.wYear) && (ExpiryMonth < st.wMonth)) || ((ExpiryYear == st.wYear) && (ExpiryMonth == st.wMonth) && (ExpiryDay < st.wDay)) )
      {
         return true;
      }
   }
   return false;
}

int GetExpiryDay()
{
   return ExpiryDay;
}

int GetExpiryMonth()
{
   return ExpiryMonth;
}

int GetExpiryYear()
{
   return ExpiryYear;
}

vector<double> constructVector(xl_array* xlArray, bool &successful, string &errorMessage)
{
    successful = true;
    errorMessage = "No error";
    WORD xlArray_rows, xlArray_columns;
    cpp_xloper cppXloper(xlArray);
    cppXloper.GetArraySize(xlArray_rows, xlArray_columns);
    if (xlArray_columns != 1) {
        successful = false;
        errorMessage = "input not a column vector";        
        return vector<double>(1,0);
    }
    double rate;
    vector<double> result;
    for (size_t i = 0; i < xlArray_rows; ++i) {
        cppXloper.GetArrayElement(i,0,rate);
        result.push_back(rate);
    }
    return result;
}

void extractDataFromSurface(double anchorDate, xl_array* surfaceInput, vector<double> &columnHeadings, vector<double> &rowHeadings, vector<vector<double>> &data, bool &successful, string &errorMessage)
{
    successful = true;
    errorMessage = "No error";

    WORD xlArray_rows, xlArray_columns;
    cpp_xloper cppXloper(surfaceInput);
    cppXloper.GetArraySize(xlArray_rows, xlArray_columns);
    if ((xlArray_columns < 2) || (xlArray_rows < 2)) {
        successful = false;
        errorMessage = "Input surface must contain at least 2 rows and 2 columns";        
        columnHeadings = vector<double>(1,0);
        rowHeadings = vector<double>(1,0);
        data.clear();
        data.push_back(columnHeadings);
        return;
    }
    double rate;
    rowHeadings.clear();
    for (size_t i = 1; i < xlArray_rows; ++i) {
        cppXloper.GetArrayElement(i,0,rate);
        rowHeadings.push_back(rate);
    }

    columnHeadings = vector<double>(xlArray_columns - 1,0);
    rowHeadings = vector<double>(xlArray_rows - 1,0);
    data.clear();
    for (size_t i = 1; i < xlArray_rows; ++i) 
        data.push_back(columnHeadings);

    for (size_t i = 1; i < xlArray_rows; ++i) {
        if (!cppXloper.GetArrayElement(i,0,rate)) {
            successful = false;
            errorMessage = "At least one row heading is not a numeric value";        
            return;
        }
        rowHeadings[i-1] = (rate);
    }
    for (size_t i = 1; i < xlArray_columns; ++i) {
        if (!cppXloper.GetArrayElement(0,i,rate)) {
            successful = false;
            errorMessage = "At least one column heading is not a numeric value";        
            return;
        }
        columnHeadings[i-1] = (rate - anchorDate) / 365;
    }
    for (size_t i = 1; i < xlArray_rows; ++i) {
        for (size_t j = 1; j < xlArray_columns; ++j) {
            if (!cppXloper.GetArrayElement(i,j,rate)) {
                successful = false;
                errorMessage = "At least one data point is not a numeric value";        
                return;
            }
            data[i-1][j-1] = (rate);
        }
    }
}


xloper* returnXloperOnError(std::string errorMessage)
{
    cpp_xloper outputMatrix(1, 1);
   outputMatrix.SetArrayElement(0,0,(char*) errorMessage.c_str());
   return outputMatrix.ExtractXloper(false);
}

bool getOptionType(char *putOrCall, sjd::PutCall &type, string &errorMessage)
{
    string optionType(putOrCall);
    boost::to_lower(optionType);
    if ((optionType.compare("c") == 0) || (optionType.compare("call") == 0))
        type = sjd::CALL;
    else if ((optionType.compare("p") == 0) || (optionType.compare("put") == 0))
        type = sjd::PUT;
    else {
        errorMessage = "Option type must be either \"P\" or \"C\"";
        return false;
    }
    return true;
}   

void formatOutput(vector<double> data, bool outputAsRow, cpp_xloper &formattedOutput)
{
   WORD COLUMNS_IN_RETURN_ARRAY, LENGHT_OF_RETURN_ARRAY;
    if (!outputAsRow) {
        LENGHT_OF_RETURN_ARRAY = data.size();
        COLUMNS_IN_RETURN_ARRAY = 1;
    }
    else {
        COLUMNS_IN_RETURN_ARRAY = data.size();
        LENGHT_OF_RETURN_ARRAY = 1;        
    }
    formattedOutput.SetTypeMulti(LENGHT_OF_RETURN_ARRAY, COLUMNS_IN_RETURN_ARRAY);
    if (!outputAsRow) {
        for (size_t i = 0; i < data.size(); ++i)
            formattedOutput.SetArrayElement(i, 0, data[i]);        
    }
    else {
        for (size_t i = 0; i < data.size(); ++i)
            formattedOutput.SetArrayElement(0, i, data[i]);        
    }
}


void formatOutput(vector<string> heading, vector<double> data, bool outputAsRow, cpp_xloper &formattedOutput)
{
    if (heading.size() != data.size()) {
        formattedOutput.SetTypeMulti(1,1);
        formattedOutput.SetArrayElement(0,0,"Unable to format output. Heading and data have different dimensions");
    }
   WORD COLUMNS_IN_RETURN_ARRAY, LENGHT_OF_RETURN_ARRAY;
    if (!outputAsRow) {
        LENGHT_OF_RETURN_ARRAY = data.size();
        COLUMNS_IN_RETURN_ARRAY = 2;
    }
    else {
        LENGHT_OF_RETURN_ARRAY = 2;        
        COLUMNS_IN_RETURN_ARRAY = data.size();
    }
    formattedOutput.SetTypeMulti(LENGHT_OF_RETURN_ARRAY, COLUMNS_IN_RETURN_ARRAY);
    if (!outputAsRow) {
        for (size_t i = 0; i < data.size(); ++i) {
            const char* headingAsChar = ((heading[i]).c_str());
            formattedOutput.SetArrayElement(i, 0, (char*) headingAsChar);        
            formattedOutput.SetArrayElement(i, 1, data[i]);        
        }
    }
    else {
        for (size_t i = 0; i < data.size(); ++i) {
            const char* headingAsChar = ((heading[i]).c_str());
            formattedOutput.SetArrayElement(0, i, (char*) headingAsChar);        
            formattedOutput.SetArrayElement(1, i, data[i]);        
        }
    }
}

