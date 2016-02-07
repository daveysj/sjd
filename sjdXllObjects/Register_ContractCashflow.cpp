#include <xlsdk/xlsdkdefines.hpp>

void registerContractsCashflow(const XLOPER &xDll) 
{
    
        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x1B""createContractFixedCashFlow"),
            TempStrNoSize("\x7""CCPBCL#"),
            TempStrNoSize("\x1B""createContractFixedCashFlow"),
            TempStrNoSize("\x22""ObjectID,Date,Amount,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2F""Constructs Fixed Cashflow object returns its id"),
            TempStrNoSize("\x1A""ID of object to be created"),
            TempStrNoSize("\x0D""Cashflow date"),
            TempStrNoSize("\x6""Amount"),
            TempStrNoSize("\x34""Buy = Receive or Sell = pay the underlying commodity"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
}


void unregisterContractsCashflow(const XLOPER &xDll) 
{
    
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x1B""createContractFixedCashFlow"),
            TempStrNoSize("\x7""CCPBCL#"),
            TempStrNoSize("\x1B""createContractFixedCashFlow"),
            TempStrNoSize("\x22""ObjectID,Date,Amount,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2F""Constructs Fixed Cashflow object returns its id"),
            TempStrNoSize("\x1A""ID of object to be created"),
            TempStrNoSize("\x0D""Cashflow date"),
            TempStrNoSize("\x6""Amount"),
            TempStrNoSize("\x34""Buy = Receive or Sell = pay the underlying commodity"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1B""createContractFixedCashFlow"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
