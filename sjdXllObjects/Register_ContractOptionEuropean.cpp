#include <xlsdk/xlsdkdefines.hpp>

void registerContractsOptionEuropean(const XLOPER &xDll) 
{
    Excel(xlfRegister, 0, 20, &xDll,
            TempStrNoSize("\x14""createContractOption"),
            TempStrNoSize("\xC""CCPPBCBPBCL#"),
            TempStrNoSize("\x14""createContractOption"),
            TempStrNoSize("\x46""ObjectID,MatDat,SettDate,Strike,P/C,Volume,PremDate,Prem,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x34""Constructs a Gold European Option and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"),  // parameter descriptions
            TempStrNoSize("\x14""Option maturity date"),
            TempStrNoSize("\x16""Option settlement date"),
            TempStrNoSize("\xD""Option strike"),
            TempStrNoSize("\xB""Put or call"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\xC""Premium Date"),
            TempStrNoSize("\xE""Premium Amount"),
            TempStrNoSize("\x1D""Buy or Sell underlying option"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
}


void unregisterContractsOptionEuropean(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

    Excel(xlfRegister, 0, 20, &xDll,
            TempStrNoSize("\x14""createContractOption"),
            TempStrNoSize("\xC""CCPPBCBPBCL#"),
            TempStrNoSize("\x14""createContractOption"),
            TempStrNoSize("\x46""ObjectID,MatDat,SettDate,Strike,P/C,Volume,PremDate,Prem,B/S,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x34""Constructs a Gold European Option and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"),  // parameter descriptions
            TempStrNoSize("\x14""Option maturity date"),
            TempStrNoSize("\x16""Option settlement date"),
            TempStrNoSize("\xD""Option strike"),
            TempStrNoSize("\xB""Put or call"),
            TempStrNoSize("\x6""Volume"),
            TempStrNoSize("\xC""Premium Date"),
            TempStrNoSize("\xE""Premium Amount"),
            TempStrNoSize("\x1D""Buy or Sell underlying option"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x14""createContractOption"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

}
