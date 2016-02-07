#include <xlsdk/xlsdkdefines.hpp>


void registerDiscountCurves(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x19""createDiscountFactorCurve"),
            TempStrNoSize("\x08""CCPPPPL#"),              
            TempStrNoSize("\x19""createDiscountFactorCurve"),
            TempStrNoSize("\x36""ObjectID,Anchor Date,Dates,Disc. Fact,Interp,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x42""Construct an object of class DiscountFactorCurve and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\x10""Discount Factors"),
            TempStrNoSize("\x24""Interpolation Type (LINEAR or CUBIC)"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x12""createDRSByBumping"),
            TempStrNoSize("\x09""CCCBPPPL#"),              
            TempStrNoSize("\x12""createDRSByBumping"),
            TempStrNoSize("\x45""OutputID,InputCurve,Spread,Day Count,Compounding,Frequency,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x41""Construct an object of class DiscountRateSource and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x1D""id of base curve to be bumped"),
            TempStrNoSize("\x14""Interest Rate Spread"),
            TempStrNoSize("\x25""a/360, a/365, Actual/365 (Fixed), etc"),
            TempStrNoSize("\x1E""Simple, Compounded, Continuous"),
            TempStrNoSize("\x56""NoFrequency, Once, Annual, Semiannual, EveryFourthMonth, Quarterly, Bimonthly, Monthly"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x07""rollDRS"),
            TempStrNoSize("\x6""CCPCL#"),             
            TempStrNoSize("\x07""rollDRS"),
            TempStrNoSize("\x29""OutputObjectID,To Date,InputDRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Role a DRS deterministically to a forward date and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x25""Anchor date for the new, rolled curve"),
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));


        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x11""getDiscountFactor"),
            TempStrNoSize("\x05""PCPP#"),              // parameter codes
            TempStrNoSize("\x11""getDiscountFactor"),
            TempStrNoSize("\x16""ObjectID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x35""Returns the interpolated discount factor from a curve"),
            TempStrNoSize("\x34""ID of discount factor curve to use for interpolation"),
            TempStrNoSize("\x5""Dates"),
            TempStrNoSize("\x7""Trigger"));           

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\xF""convertDFToRate"),
            TempStrNoSize("\x08""PBPPPPP#"),              // parameter codes
            TempStrNoSize("\xF""convertDFToRate"),
            TempStrNoSize("\x33""DF,FromDate,ToDate,DayCounter,Compounding,Frequency"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x33""Returns an interest rate in the required convention"),
            TempStrNoSize("\x27""Input discount factor (between 0 and 1)"),
            TempStrNoSize("\x9""From date"),
            TempStrNoSize("\x7""To date"),
            TempStrNoSize("\x25""a/360, a/365, Actual/365 (Fixed), etc"),
            TempStrNoSize("\x1E""Simple, Compounded, Continuous"),
            TempStrNoSize("\x56""NoFrequency, Once, Annual, Semiannual, EveryFourthMonth, Quarterly, Bimonthly, Monthly"));           

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\xF""convertRateToDF"),
            TempStrNoSize("\x08""PBPPPPP#"),              // parameter codes
            TempStrNoSize("\xF""convertRateToDF"),
            TempStrNoSize("\x35""Rate,FromDate,ToDate,DayCounter,Compounding,Frequency"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x19""Returns a discount factor"),
            TempStrNoSize("\x1F""Interest Rate (between 0 and 1)"),
            TempStrNoSize("\x9""From date"),
            TempStrNoSize("\x7""To date"),
            TempStrNoSize("\x25""a/360, a/365, Actual/365 (Fixed), etc"),
            TempStrNoSize("\x1E""Simple, Compounded, Continuous"),
            TempStrNoSize("\x56""NoFrequency, Once, Annual, Semiannual, EveryFourthMonth, Quarterly, Bimonthly, Monthly"));           
}

void unregisterDiscountCurves(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x19""createDiscountFactorCurve"),
            TempStrNoSize("\x08""CCPPPPL#"),              
            TempStrNoSize("\x19""createDiscountFactorCurve"),
            TempStrNoSize("\x36""ObjectID,Anchor Date,Dates,Disc. Fact,Interp,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x42""Construct an object of class DiscountFactorCurve and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\x10""Discount Factors"),
            TempStrNoSize("\x24""Interpolation Type (LINEAR or CUBIC)"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x19""createDiscountFactorCurve"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 17, &xDll,
            TempStrNoSize("\x12""createDRSByBumping"),
            TempStrNoSize("\x09""CCCBPPPL#"),              
            TempStrNoSize("\x12""createDRSByBumping"),
            TempStrNoSize("\x45""OutputID,InputCurve,Spread,Day Count,Compounding,Frequency,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x41""Construct an object of class DiscountRateSource and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x1D""id of base curve to be bumped"),
            TempStrNoSize("\x14""Interest Rate Spread"),
            TempStrNoSize("\x25""a/360, a/365, Actual/365 (Fixed), etc"),
            TempStrNoSize("\x1E""Simple, Compounded, Continuous"),
            TempStrNoSize("\x56""NoFrequency, Once, Annual, Semiannual, EveryFourthMonth, Quarterly, Bimonthly, Monthly"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x12""createDRSByBumping"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);


        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x07""rollDRS"),
            TempStrNoSize("\x6""CCPCL#"),             
            TempStrNoSize("\x07""rollDRS"),
            TempStrNoSize("\x29""OutputObjectID,To Date,InputDRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Role a DRS deterministically to a forward date and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x25""Anchor date for the new, rolled curve"),
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x07""rollDRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x11""getDiscountFactor"),
            TempStrNoSize("\x05""PCPP#"),              // parameter codes
            TempStrNoSize("\x11""getDiscountFactor"),
            TempStrNoSize("\x16""ObjectID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x35""Returns the interpolated discount factor from a curve"),
            TempStrNoSize("\x34""ID of discount factor curve to use for interpolation"),
            TempStrNoSize("\x5""Dates"),
            TempStrNoSize("\x7""Trigger"));           

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x11""getDiscountFactor"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\xF""convertDFToRate"),
            TempStrNoSize("\x08""PBPPPPP#"),              // parameter codes
            TempStrNoSize("\xF""convertDFToRate"),
            TempStrNoSize("\x33""DF,FromDate,ToDate,DayCounter,Compounding,Frequency"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x33""Returns an interest rate in the required convention"),
            TempStrNoSize("\x27""Input discount factor (between 0 and 1)"),
            TempStrNoSize("\x9""From date"),
            TempStrNoSize("\x7""To date"),
            TempStrNoSize("\x25""a/360, a/365, Actual/365 (Fixed), etc"),
            TempStrNoSize("\x1E""Simple, Compounded, Continuous"),
            TempStrNoSize("\x56""NoFrequency, Once, Annual, Semiannual, EveryFourthMonth, Quarterly, Bimonthly, Monthly"));           

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xF""convertDFToRate"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\xF""convertRateToDF"),
            TempStrNoSize("\x08""PBPPPPP#"),              // parameter codes
            TempStrNoSize("\xF""convertRateToDF"),
            TempStrNoSize("\x35""Rate,FromDate,ToDate,DayCounter,Compounding,Frequency"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x19""Returns a discount factor"),
            TempStrNoSize("\x1F""Interest Rate (between 0 and 1)"),
            TempStrNoSize("\x9""From date"),
            TempStrNoSize("\x7""To date"),
            TempStrNoSize("\x25""a/360, a/365, Actual/365 (Fixed), etc"),
            TempStrNoSize("\x1E""Simple, Compounded, Continuous"),
            TempStrNoSize("\x56""NoFrequency, Once, Annual, Semiannual, EveryFourthMonth, Quarterly, Bimonthly, Monthly"));           

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xF""convertRateToDF"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

}
