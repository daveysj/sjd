#include <xlsdk/xlsdkdefines.hpp>


void registerFixingCurves(const XLOPER &xDll) {
        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x16""createDailyFixingCurve"),
            TempStrNoSize("\x06""CCPPL#"),
            TempStrNoSize("\x16""createDailyFixingCurve"),
            TempStrNoSize("\x1E""ObjectID,Dates,Rates,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x40""Construct an object of class DailyFixingCurve and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\xE""Rate Set Rates"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x18""createMonthlyFixingCurve"),
            TempStrNoSize("\x06""CCPPL#"),
            TempStrNoSize("\x18""createMonthlyFixingCurve"),
            TempStrNoSize("\x1E""ObjectID,Dates,Rates,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x42""Construct an object of class MonthlyFixingCurve and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\xE""Rate Set Rates"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x6""getFix"),
            TempStrNoSize("\x05""PCPP#"),           
            TempStrNoSize("\x6""getFix"),
            TempStrNoSize("\x16""ObjectID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x2A""Returns the rate set for the relevant date"),
            TempStrNoSize("\x33""ID of the FixingRateSource to use for interpolation"),
            TempStrNoSize("\x7""Trigger"));  

}

void unregisterFixingCurves(const XLOPER &xDll) {
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.


        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x16""createDailyFixingCurve"),
            TempStrNoSize("\x06""CCPPL#"),
            TempStrNoSize("\x16""createDailyFixingCurve"),
            TempStrNoSize("\x1E""ObjectID,Dates,Rates,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x40""Construct an object of class DailyFixingCurve and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\xE""Rate Set Rates"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""createDailyFixingCurve"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x18""createMonthlyFixingCurve"),
            TempStrNoSize("\x06""CCPPL#"),
            TempStrNoSize("\x18""createMonthlyFixingCurve"),
            TempStrNoSize("\x1E""ObjectID,Dates,Rates,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x42""Construct an object of class MonthlyFixingCurve and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\xE""Rate Set Rates"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x18""createMonthlyFixingCurve"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x6""getFix"),
            TempStrNoSize("\x05""PCPP#"),           
            TempStrNoSize("\x6""getFix"),
            TempStrNoSize("\x16""ObjectID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x2A""Returns the rate set for the relevant date"),
            TempStrNoSize("\x33""ID of the FixingRateSource to use for interpolation"),
            TempStrNoSize("\x7""Trigger"));  

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x6""getFix"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

    
}
