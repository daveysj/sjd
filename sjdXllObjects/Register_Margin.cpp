#include <xlsdk/xlsdkdefines.hpp>


void registerMargin(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x12""createMarginSimple"),
            TempStrNoSize("\xB""CCPLBLBBBL#"),            
            TempStrNoSize("\x12""createMarginSimple"),
            TempStrNoSize("\x5E""ObjectID,AnchorDate,UseMyThresh,MyThreshold,UseCptyThresh,CptyThreshold,MTA,Rounding,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x3B""Construct an object of class SimpleMargin and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\xA""AnchorDate"),
            TempStrNoSize("\x3E""False if we are not margining the counterparty, true otherwise"),
            TempStrNoSize("\x3E""Threshold above which the counterpart must post margin with us"),
            TempStrNoSize("\x46""False if we are not being margined by the counterparty, true otherwise"),
            TempStrNoSize("\x3E""Threshold above which we must post margin with the counterpart"),
            TempStrNoSize("\x17""Minimum transfer amount"),
            TempStrNoSize("\x8""Rounding"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\xD""getMarginCall"),
            TempStrNoSize("\x06""PCBBP#"),              
            TempStrNoSize("\xD""getMarginCall"),
            TempStrNoSize("\x1C""ObjectID,Balance,MtM,Trigger"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x34""Calculates a margin call for a given opening balance"),
            TempStrNoSize("\x13""ID of margin object"),
            TempStrNoSize("\x71""The opening balance for the margin account. This does not change the state of the object outside of this function"),
            TempStrNoSize("\x20""The Mark-to-Market of the trades"),
            TempStrNoSize("\x7""Trigger"));           

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x10""setMarginBalance"),
            TempStrNoSize("\x5""PCBP#"),
            TempStrNoSize("\x10""setMarginBalance"),
            TempStrNoSize("\x18""MarginID,Balance,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x2E""Sets the current balance in the margin account"),
            TempStrNoSize("\x17""ID of the Margin Object"),
            TempStrNoSize("\x7""Balance"),
            TempStrNoSize("\x8""Trigger"));
}

void unregisterMargin(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x12""createMarginSimple"),
            TempStrNoSize("\xB""CCPLBLBBBL#"),            
            TempStrNoSize("\x12""createMarginSimple"),
            TempStrNoSize("\x5E""ObjectID,AnchorDate,UseMyThresh,MyThreshold,UseCptyThresh,CptyThreshold,MTA,Rounding,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x3B""Construct an object of class SimpleMargin and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\xA""AnchorDate"),
            TempStrNoSize("\x3E""False if we are not margining the counterparty, true otherwise"),
            TempStrNoSize("\x3E""Threshold above which the counterpart must post margin with us"),
            TempStrNoSize("\x46""False if we are not being margined by the counterparty, true otherwise"),
            TempStrNoSize("\x3E""Threshold above which we must post margin with the counterpart"),
            TempStrNoSize("\x17""Minimum transfer amount"),
            TempStrNoSize("\x8""Rounding"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x12""createMarginSimple"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\xD""getMarginCall"),
            TempStrNoSize("\x06""PCBBP#"),              
            TempStrNoSize("\xD""getMarginCall"),
            TempStrNoSize("\x1C""ObjectID,Balance,MtM,Trigger"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x34""Calculates a margin call for a given opening balance"),
            TempStrNoSize("\x13""ID of margin object"),
            TempStrNoSize("\x71""The opening balance for the margin account. This does not change the state of the object outside of this function"),
            TempStrNoSize("\x20""The Mark-to-Market of the trades"),
            TempStrNoSize("\x7""Trigger"));           

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xD""getMarginCall"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x10""setMarginBalance"),
            TempStrNoSize("\x5""PCBP#"),
            TempStrNoSize("\x10""setMarginBalance"),
            TempStrNoSize("\x18""MarginID,Balance,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x2E""Sets the current balance in the margin account"),
            TempStrNoSize("\x17""ID of the Margin Object"),
            TempStrNoSize("\x7""Balance"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x10""setMarginBalance"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
