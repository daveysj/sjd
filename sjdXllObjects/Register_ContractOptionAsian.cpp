#include <xlsdk/xlsdkdefines.hpp>

void registerContractsOptionAsian(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 21, &xDll,
            TempStrNoSize("\x19""createContractOptionAsian"),
            TempStrNoSize("\xD""CCPPPBCBPBCL#"),
            TempStrNoSize("\x19""createContractOptionAsian"),
            TempStrNoSize("\x4F""ObjectID,AveStart,AveEnd,SettDate,Strike,P/C,Volume,PremDate,Prem,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x7B""Constructs an Oil Average Rate Option (with averaging period) object which can be rolled forward in time and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"),              
            TempStrNoSize("\x19""Start of averaging period"),
            TempStrNoSize("\x17""End of averaging period"),
            TempStrNoSize("\x16""Option settlement date"),
            TempStrNoSize("\xD""Option strike"),
            TempStrNoSize("\xB""Put or call"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\xC""Premium Date"),
            TempStrNoSize("\xE""Premium Amount"),
            TempStrNoSize("\x1D""Buy or Sell underlying option"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

}


void unregisterContractsOptionAsian(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 21, &xDll,
            TempStrNoSize("\x19""createContractOptionAsian"),
            TempStrNoSize("\xD""CCPPPBCBPBCL#"),
            TempStrNoSize("\x19""createContractOptionAsian"),
            TempStrNoSize("\x4F""ObjectID,AveStart,AveEnd,SettDate,Strike,P/C,Volume,PremDate,Prem,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x7B""Constructs an Oil Average Rate Option (with averaging period) object which can be rolled forward in time and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"),              
            TempStrNoSize("\x19""Start of averaging period"),
            TempStrNoSize("\x17""End of averaging period"),
            TempStrNoSize("\x16""Option settlement date"),
            TempStrNoSize("\xD""Option strike"),
            TempStrNoSize("\xB""Put or call"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\xC""Premium Date"),
            TempStrNoSize("\xE""Premium Amount"),
            TempStrNoSize("\x1D""Buy or Sell underlying option"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x19""createContractOptionAsian"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

}
