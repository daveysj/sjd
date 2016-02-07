#include <xlsdk/xlsdkdefines.hpp>


void registerVolatilityCurves(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x0E""createVolCurve"),// function code name
            TempStrNoSize("\x0A""CCPPPPPLL#"),              // parameter codes
            TempStrNoSize("\x0E""createVolCurve"),// function display name
            TempStrNoSize("\x44""ObjectID,Anchor Date,Dates,Vols,DayCount,Interpolation,CMA,Permanent"), // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2E""Construct a Volatility Curve and return its id"),             // function description            
            TempStrNoSize("\x1A""id of object to be created"),  // parameter descriptions
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\x11""Volatility inputs"),
            TempStrNoSize("\x2E""Day count convention - typically A/365 (Fixed)"),
            TempStrNoSize("\x24""Interpolation Type (LINEAR or CUBIC)"),
            TempStrNoSize("\x3D""True = Calendar Month Average Inputs, False = European Inputs"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x10""createVolSurface"),
            TempStrNoSize("\x0B""CCPPPPPPLL#"),              
            TempStrNoSize("\x10""createVolSurface"),
            TempStrNoSize("\x53""ObjectID,AnchorDate,Dates,Dimension,VolData,Interpolation,SurfaceType,CMA,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),            
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x29""Construct a vol surface and return its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\x4D""moneyness or (put) delta inputs. Put Deltas are between 0 and 100 not 0 and 1"),
            TempStrNoSize("\x32""Matrix of input volatility (entered as 0.1 NOT 10)"),
            TempStrNoSize("\x2F""Interpolation type - either BILINEAR or BICUBIC"),
            TempStrNoSize("\x2A""Dimension type - either MONEYNESS or DELTA"),
            TempStrNoSize("\x2F""TRUE = Calendar Month Average, False = European"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\xD""getVolatility"), // function code name
            TempStrNoSize("\x06""PCPLP#"),              // parameter codes
            TempStrNoSize("\xD""getVolatility"),// function display name
            TempStrNoSize("\x1B""ObjectID,Date,Stdev,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x5C""Returns the volatility or standard deviation to specific dates from a volatility rate source"),
            TempStrNoSize("\x35""ID of volatility rate source to use for interpolation"),
            TempStrNoSize("\xB""Single Date"),
            TempStrNoSize("\x19""Return Standard Deviation (TRUE) or Volatility (FALSE)"),
            TempStrNoSize("\x7""Trigger"));            // function category

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x16""getVolatilityForStrike"),
            TempStrNoSize("\x08""PCPBBLP#"),              
            TempStrNoSize("\x16""getVolatilityForStrike"),
            TempStrNoSize("\x26""ObjectID,Date,Strike,Fwd,Stdev,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x69""Returns the volatility or standard deviation to specific date / strike pair from a volatility rate source"),
            TempStrNoSize("\x35""ID of volatility rate source to use for interpolation"),
            TempStrNoSize("\x10""Single Date Only"),
            TempStrNoSize("\x06""Strike"),
            TempStrNoSize("\x07""Forward"),
            TempStrNoSize("\x19""Return Standard Deviation (TRUE) or Volatility (FALSE)"),
            TempStrNoSize("\x07""Trigger"));            // function category

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x12""createVRSByBumping"),
            TempStrNoSize("\x09""CCCBL#"),              
            TempStrNoSize("\x12""createVRSByBumping"),
            TempStrNoSize("\x24""OutputID,InputCurve,Spread,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x43""Construct an object of class VolatiltiyRateSource and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x1D""id of base curve to be bumped"),
            TempStrNoSize("\x0A""Vol Spread"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x07""rollVRS"),
            TempStrNoSize("\x6""CCPCL#"),             
            TempStrNoSize("\x07""rollVRS"),
            TempStrNoSize("\x29""OutputObjectID,To Date,InputVRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Role a VRS deterministically to a forward date and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x25""Anchor date for the new, rolled curve"),
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

}

void unregisterVolatilityCurves(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x0E""createVolCurve"),// function code name
            TempStrNoSize("\x0A""CCPPPPPLL#"),              // parameter codes
            TempStrNoSize("\x0E""createVolCurve"),// function display name
            TempStrNoSize("\x44""ObjectID,Anchor Date,Dates,Vols,DayCount,Interpolation,CMA,Permanent"), // comma-delimited list of parameters
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2E""Construct a Volatility Curve and return its id"),             // function description            
            TempStrNoSize("\x1A""id of object to be created"),  // parameter descriptions
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\x11""Volatility inputs"),
            TempStrNoSize("\x2E""Day count convention - typically A/365 (Fixed)"),
            TempStrNoSize("\x24""Interpolation Type (LINEAR or CUBIC)"),
            TempStrNoSize("\x3D""True = Calendar Month Average Inputs, False = European Inputs"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x0E""createVolCurve"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x10""createVolSurface"),
            TempStrNoSize("\x0B""CCPPPPPPLL#"),              
            TempStrNoSize("\x10""createVolSurface"),
            TempStrNoSize("\x53""ObjectID,AnchorDate,Dates,Dimension,VolData,Interpolation,SurfaceType,CMA,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),            
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x29""Construct a vol surface and return its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\x4D""moneyness or (put) delta inputs. Put Deltas are between 0 and 100 not 0 and 1"),
            TempStrNoSize("\x32""Matrix of input volatility (entered as 0.1 NOT 10)"),
            TempStrNoSize("\x2F""Interpolation type - either BILINEAR or BICUBIC"),
            TempStrNoSize("\x2A""Dimension type - either MONEYNESS or DELTA"),
            TempStrNoSize("\x2F""TRUE = Calendar Month Average, False = European"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x10""createVolSurface"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\xD""getVolatility"), // function code name
            TempStrNoSize("\x06""PCPLP#"),              // parameter codes
            TempStrNoSize("\xD""getVolatility"),// function display name
            TempStrNoSize("\x1B""ObjectID,Date,Stdev,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x5C""Returns the volatility or standard deviation to specific dates from a volatility rate source"),
            TempStrNoSize("\x35""ID of volatility rate source to use for interpolation"),
            TempStrNoSize("\xB""Single Date"),
            TempStrNoSize("\x19""Return Standard Deviation (TRUE) or Volatility (FALSE)"),
            TempStrNoSize("\x7""Trigger"));            // function category

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xD""getVolatility"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x16""getVolatilityForStrike"),
            TempStrNoSize("\x08""PCPBBLP#"),              
            TempStrNoSize("\x16""getVolatilityForStrike"),
            TempStrNoSize("\x26""ObjectID,Date,Strike,Fwd,Stdev,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x69""Returns the volatility or standard deviation to specific date / strike pair from a volatility rate source"),
            TempStrNoSize("\x35""ID of volatility rate source to use for interpolation"),
            TempStrNoSize("\x10""Single Date Only"),
            TempStrNoSize("\x06""Strike"),
            TempStrNoSize("\x07""Forward"),
            TempStrNoSize("\x19""Return Standard Deviation (TRUE) or Volatility (FALSE)"),
            TempStrNoSize("\x07""Trigger"));            // function category

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getVolatilityForStrike"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
    
        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x12""createVRSByBumping"),
            TempStrNoSize("\x09""CCCBL#"),              
            TempStrNoSize("\x12""createVRSByBumping"),
            TempStrNoSize("\x24""OutputID,InputCurve,Spread,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x43""Construct an object of class VolatiltiyRateSource and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x1D""id of base curve to be bumped"),
            TempStrNoSize("\x0A""Vol Spread"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x12""createVRSByBumping"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x07""rollVRS"),
            TempStrNoSize("\x6""CCPCL#"),             
            TempStrNoSize("\x07""rollVRS"),
            TempStrNoSize("\x29""OutputObjectID,To Date,InputVRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Role a VRS deterministically to a forward date and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x25""Anchor date for the new, rolled curve"),
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x07""rollVRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
