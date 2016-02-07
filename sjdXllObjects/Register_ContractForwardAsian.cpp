#include <xlsdk/xlsdkdefines.hpp>

void registerContractsForwardAsian(const XLOPER &xDll) 
{

        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x1A""createContractForwardAsian"),
            TempStrNoSize("\xA""CCPPPBBCL#"),
            TempStrNoSize("\x1A""createContractForwardAsian"),
            TempStrNoSize("\x3D""ObjectID,AveStart,AveEnd,SettDate,Strike,Volume,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x26""Constructs an Oil Average Rate Forward"),
            TempStrNoSize("\x1A""id of object to be created"),  // parameter descriptions
            TempStrNoSize("\x19""Start of averaging period"),
            TempStrNoSize("\x17""End of averaging period"),
            TempStrNoSize("\xF""Settlement date"),
            TempStrNoSize("\x6""Strike"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\x1D""Buy or Sell at the fixed rate"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
}


void unregisterContractsForwardAsian(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x1A""createContractForwardAsian"),
            TempStrNoSize("\xA""CCPPPBBCL#"),
            TempStrNoSize("\x1A""createContractForwardAsian"),
            TempStrNoSize("\x3D""ObjectID,AveStart,AveEnd,SettDate,Strike,Volume,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x26""Constructs an Oil Average Rate Forward"),
            TempStrNoSize("\x1A""id of object to be created"),  // parameter descriptions
            TempStrNoSize("\x19""Start of averaging period"),
            TempStrNoSize("\x17""End of averaging period"),
            TempStrNoSize("\xF""Settlement date"),
            TempStrNoSize("\x6""Strike"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\x1D""Buy or Sell at the fixed rate"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1A""createContractForwardAsian"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
