#include <xlsdk/xlsdkdefines.hpp>

void registerContracts(const XLOPER &xDll) 
{

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x10""getContractValue"),
            TempStrNoSize("\x5""PCCP#"),
            TempStrNoSize("\x10""getContractValue"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x13""Values any Contract"),
            TempStrNoSize("\x17""id of Contract to value"),
            TempStrNoSize("\x26""Rates environment to use for valuation"),
            TempStrNoSize("\x07""Trigger"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x10""describeContract"),
            TempStrNoSize("\x5""PCCP#"),
            TempStrNoSize("\x10""describeContract"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x25""Returns a description of the contract"),
            TempStrNoSize("\x17""id of Contract to value"),
            TempStrNoSize("\x18""Rates environment to use"),
            TempStrNoSize("\x7""Trigger"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x13""createMultiContract"),
            TempStrNoSize("\x6""CCPPL#"),
            TempStrNoSize("\x13""createMultiContract"),
            TempStrNoSize("\x26""ObjectID,ContractIDs,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x43""Returns the ID of a contract that consists of a number of contracts"),
            TempStrNoSize("\x18""ID of the multi-contract"),
            TempStrNoSize("\x4D""A vector containing the IDs of all the contracts making up the multi-contract"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x14""createMultiContracts"),
            TempStrNoSize("\x6""PPPPL#"),
            TempStrNoSize("\x14""createMultiContracts"),
            TempStrNoSize("\x1F""Contracts,Key,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x43""Returns the IDs of multiple portfolios grouped according to the key"),
            TempStrNoSize("\x50""A vector containing the IDs of all the contracts making up the various portfolio"),
            TempStrNoSize("\x48""Determines the number of portfolios and groups the contracts accordingly"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
}


void unregisterContracts(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x10""getContractValue"),
            TempStrNoSize("\x5""PCCP#"),
            TempStrNoSize("\x10""getContractValue"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x13""Values any Contract"),
            TempStrNoSize("\x17""id of Contract to value"),
            TempStrNoSize("\x26""Rates environment to use for valuation"),
            TempStrNoSize("\x07""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x10""getContractValue"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x10""describeContract"),
            TempStrNoSize("\x5""PCCP#"),
            TempStrNoSize("\x10""describeContract"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x25""Returns a description of the contract"),
            TempStrNoSize("\x17""id of Contract to value"),
            TempStrNoSize("\x18""Rates environment to use"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x10""describeContract"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);


        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x16""getContractValueOnPath"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x16""getContractValueOnPath"),
            TempStrNoSize("\x21""ObjectID,Dates, Spot Path,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x3A""Returns the value of the contract for the given price path"),
            TempStrNoSize("\x1C""id of the contract to evolve"),
            TempStrNoSize("\x17""Dates for the valuation"),
            TempStrNoSize("\x1C""Spot rates for the valuation"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getContractValueOnPath"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x13""createMultiContract"),
            TempStrNoSize("\x6""CCPPL#"),
            TempStrNoSize("\x13""createMultiContract"),
            TempStrNoSize("\x26""ObjectID,ContractIDs,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x43""Returns the ID of a contract that consists of a number of contracts"),
            TempStrNoSize("\x18""ID of the multi-contract"),
            TempStrNoSize("\x4D""A vector containing the IDs of all the contracts making up the multi-contract"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x13""createMultiContract"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x14""createMultiContracts"),
            TempStrNoSize("\x6""PPPPL#"),
            TempStrNoSize("\x14""createMultiContracts"),
            TempStrNoSize("\x1F""Contracts,Key,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x43""Returns the IDs of multiple portfolios grouped according to the key"),
            TempStrNoSize("\x50""A vector containing the IDs of all the contracts making up the various portfolio"),
            TempStrNoSize("\x48""Determines the number of portfolios and groups the contracts accordingly"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x14""createMultiContracts"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
