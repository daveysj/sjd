#include <xlsdk/xlsdkdefines.hpp>

void registerContractStressor(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x16""createMarketRiskReport"),
            TempStrNoSize("\x04""PCC#"),    
            TempStrNoSize("\x16""createMarketRiskReport"),
            TempStrNoSize("\x21""Contract,Rates"),
            TempStrNoSize("\x01""1"),   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x51""Combines a Contract with a Rates Environment in order to run a market risk report"),
            TempStrNoSize("\x18""ID of contract to stress"),
            TempStrNoSize("\x26""Rates environment to use for valuation"));
}


void unregisterContractStressor(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x16""createMarketRiskReport"),
            TempStrNoSize("\x04""PCC#"),    
            TempStrNoSize("\x16""createMarketRiskReport"),
            TempStrNoSize("\x21""Contract,Rates"),
            TempStrNoSize("\x01""1"),   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x51""Combines a Contract with a Rates Environment in order to run a market risk report"),
            TempStrNoSize("\x18""ID of contract to stress"),
            TempStrNoSize("\x26""Rates environment to use for valuation"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x18""createMarketRateStresses"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
