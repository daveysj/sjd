#ifndef SJD_OBJ_VectorOfVectors
#define SJD_OBJ_VectorOfVectors
//#pragma once

#include <qlxl/conversions/matrixtooper.hpp>
#include <ohxl/conversions/scalartooper.hpp>
#include <oh/exception.hpp>
#include <algorithm>

using namespace ObjectHandler;

namespace sjd 
{
    void tableToOper(const std::vector<std::string> &columnHeadings, 
                     const std::vector<std::string> &rowHeadings, 
                     const std::vector<std::vector<double>> &data, 
                     OPER &xMatrix);
}

#endif