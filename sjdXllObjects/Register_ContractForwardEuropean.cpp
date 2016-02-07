#include <xlsdk/xlsdkdefines.hpp>

void registerContractsForwardEuropean(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x15""createContractForward"),
            TempStrNoSize("\x9""CCPPBBCL#"),
            TempStrNoSize("\x15""createContractForward"),
            TempStrNoSize("\x38""ObjectID,RateSetDat,SettDate,Strike,Volume,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x34""Constructs an European Forward object returns its id"),
            TempStrNoSize("\x1B""ID of forward to be created"),
            TempStrNoSize("\xD""Rate Set Date"),
            TempStrNoSize("\xF""Settlement Date"),
            TempStrNoSize("\x6""Strike"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\x20""Buy or Sell underlying commodity"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
}


void unregisterContractsForwardEuropean(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x15""createContractForward"),
            TempStrNoSize("\x9""CCPPBBCL#"),
            TempStrNoSize("\x15""createContractForward"),
            TempStrNoSize("\x38""ObjectID,RateSetDat,SettDate,Strike,Volume,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x34""Constructs an European Forward object returns its id"),
            TempStrNoSize("\x1B""ID of forward to be created"),
            TempStrNoSize("\xD""Rate Set Date"),
            TempStrNoSize("\xF""Settlement Date"),
            TempStrNoSize("\x6""Strike"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\x20""Buy or Sell underlying commodity"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xE""createContractForward"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
