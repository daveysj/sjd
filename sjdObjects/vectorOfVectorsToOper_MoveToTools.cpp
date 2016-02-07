#include "vectorOfVectorsToOper.h"

namespace sjd 
{
    void tableToOper(const std::vector<std::string> &columnHeadings, 
                     const std::vector<std::string> &rowHeadings, 
                     const std::vector<std::vector<double>> &data, 
                     OPER &xMatrix)
    {
        if (data.empty()) 
        {
            xMatrix.xltype = xltypeErr;
            xMatrix.val.err = xlerrNA;
            return;
        }
        size_t dataRows = data.size();
        size_t rows = dataRows;
        size_t rowOffset = 0;
        size_t dataColumns = data[0].size();
        for (size_t i = 0; i < rows; ++i) 
        {
            dataColumns = std::max(dataColumns, data[i].size());
        }
        size_t columns = dataColumns;
        size_t columnOffset = 0;
        if (columnHeadings.size() != 0) 
        {
            ++rows;
            rowOffset = 1;
        }
        if (rowHeadings.size() != 0) 
        {
            ++columns;
            columnOffset = 1;
        }

        xMatrix.val.array.rows = rows;
        xMatrix.val.array.columns = columns;
        xMatrix.val.array.lparray = new OPER[xMatrix.val.array.rows * xMatrix.val.array.columns]; 
        xMatrix.xltype = xltypeMulti | xlbitDLLFree;

        if (columnHeadings.size() != 0) 
        {
            for (size_t i=columnOffset; i<columns; ++i)
            {
                scalarToOper(columnHeadings[i-columnOffset], xMatrix.val.array.lparray[i]);
            }
        }

        if (rowHeadings.size() != 0) 
        {
            for (size_t i=rowOffset; i<rows; ++i)
            {
                scalarToOper(rowHeadings[i-rowOffset], xMatrix.val.array.lparray[i * columns]);
            }
        }

        for (size_t i=rowOffset; i<rows; ++i) 
        {
            for (size_t j=columnOffset; j<columns; ++j)
            {
                if (j < data[i-rowOffset].size()+columnOffset)
                {
                    scalarToOper(data[i-rowOffset][j-columnOffset], xMatrix.val.array.lparray[i * columns + j]);
                }
                else 
                {
                    scalarToOper(0.0, xMatrix.val.array.lparray[i * columns + j]);
                }
            }
        }

        if ((columnHeadings.size() != 0) && (rowHeadings.size() != 0))
        {
            scalarToOper("", xMatrix.val.array.lparray[0]);
        }
    }

}