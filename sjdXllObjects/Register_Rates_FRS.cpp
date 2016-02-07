#include <xlsdk/xlsdkdefines.hpp>


void registerFuturesCurves(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 20, &xDll,
            TempStrNoSize("\x12""createForwardCurve"),
            TempStrNoSize("\x0C""CCPPPPLLCLL#"),
            TempStrNoSize("\x12""createForwardCurve"),
            TempStrNoSize("\x4E""ObjectID,Anchor Date,Dates,Futures,Interp,CMA,Stochastic,VRS,SpotVol,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x2C""Constructs a ForwardCurve and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\xE""Futures inputs"),
            TempStrNoSize("\x24""Interpolation Type (LINEAR or CUBIC)"),
            TempStrNoSize("\x39""True = Calendar Month Average Inputs, False = Spot Inputs"),
            TempStrNoSize("\x48""True = Curve Rolls Stochastically, False = Curve Rolls deterministically"),
            TempStrNoSize("\x40""Volatility rate source to bump forward curve if it is stochastic"),
            TempStrNoSize("\x43""Ture = Roll using spot and Fwd Fwd vols, False = roll using fwd vol"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x14""createForwardCurveFX"),
            TempStrNoSize("\x0B""CCPBPCCLCL#"),
            TempStrNoSize("\x14""createForwardCurveFX"),
            TempStrNoSize("\x51""ObjectID,AnchorDate,Spot,SpotDate,DomesticDRS,ForeignDRS,Stochastic,VRS,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x4E""Constructs a forward curve using a spot rate and an interest rate differential"),
            TempStrNoSize("\x1A""ID of object to be created"), 
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\x09""Spot Rate"),
            TempStrNoSize("\x25""Spot Date (must be after anchor date)"),
            TempStrNoSize("\x20""Domestic Discounting Rate Source"),
            TempStrNoSize("\x1F""Foreign Discounting Rate Source"),
            TempStrNoSize("\x48""True = Curve Rolls Stochastically, False = Curve Rolls deterministically"),
            TempStrNoSize("\x40""Volatility rate source to bump forward curve if it is stochastic"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\xA""getForward"),
            TempStrNoSize("\x05""PCPP#"),           
            TempStrNoSize("\xA""getForward"),
            TempStrNoSize("\x16""ObjectID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x64""Returns the interpolated future for the relevant date"),
            TempStrNoSize("\x34""ID of the ForwardRateSource to use for interpolation"),
            TempStrNoSize("\x05""Dates"),  
            TempStrNoSize("\x07""Trigger"));  

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x12""createFRSByBumping"),
            TempStrNoSize("\x06""CCCBL#"),              
            TempStrNoSize("\x12""createFRSByBumping"),
            TempStrNoSize("\x22""OutputID,InputFRS,Spread,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x09""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Construct an object of class ForwardRateSource and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x1D""id of base curve to be bumped"),
            TempStrNoSize("\x06""Spread"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x07""rollFRS"),
            TempStrNoSize("\x06""CCPCL#"),             
            TempStrNoSize("\x07""rollFRS"),
            TempStrNoSize("\x29""OutputObjectID,To Date,InputFRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Role a FRS deterministically to a forward date and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x25""Anchor date for the new, rolled curve"),
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x11""rollFRSStochastic"),
            TempStrNoSize("\x7""PCCPPL#"),             
            TempStrNoSize("\x11""rollFRSStochastic"),
            TempStrNoSize("\x39""OutputObjectID,InputFRS,To Dates,Random Numbers,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x48""Role a FRS Stochastically to a set of forward dates and return their ids"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x27""Forward dates for the new, rolled curve"),
            TempStrNoSize("\x27""Random numbers for the evolved curves"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
}

void unregisterFuturesCurves(const XLOPER &xDll) {
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 20, &xDll,
            TempStrNoSize("\x12""createForwardCurve"),
            TempStrNoSize("\x0C""CCPPPPLLCLL#"),
            TempStrNoSize("\x12""createForwardCurve"),
            TempStrNoSize("\x4E""ObjectID,Anchor Date,Dates,Futures,Interp,CMA,Stochastic,VRS,SpotVol,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x2C""Constructs a ForwardCurve and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\xB""Curve dates"),
            TempStrNoSize("\xE""Futures inputs"),
            TempStrNoSize("\x24""Interpolation Type (LINEAR or CUBIC)"),
            TempStrNoSize("\x39""True = Calendar Month Average Inputs, False = Spot Inputs"),
            TempStrNoSize("\x48""True = Curve Rolls Stochastically, False = Curve Rolls deterministically"),
            TempStrNoSize("\x40""Volatility rate source to bump forward curve if it is stochastic"),
            TempStrNoSize("\x43""Ture = Roll using spot and Fwd Fwd vols, False = roll using fwd vol"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x12""createForwardCurve"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x14""createForwardCurveFX"),
            TempStrNoSize("\x0B""CCPBPCCLCL#"),
            TempStrNoSize("\x14""createForwardCurveFX"),
            TempStrNoSize("\x51""ObjectID,AnchorDate,Spot,SpotDate,DomesticDRS,ForeignDRS,Stochastic,VRS,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x4E""Constructs a forward curve using a spot rate and an interest rate differential"),
            TempStrNoSize("\x1A""ID of object to be created"), 
            TempStrNoSize("\x19""Anchor date for the curve"),
            TempStrNoSize("\x09""Spot Rate"),
            TempStrNoSize("\x25""Spot Date (must be after anchor date)"),
            TempStrNoSize("\x20""Domestic Discounting Rate Source"),
            TempStrNoSize("\x1F""Foreign Discounting Rate Source"),
            TempStrNoSize("\x48""True = Curve Rolls Stochastically, False = Curve Rolls deterministically"),
            TempStrNoSize("\x40""Volatility rate source to bump forward curve if it is stochastic"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x14""createForwardCurveFX"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\xA""getForward"),
            TempStrNoSize("\x05""PCPP#"),           
            TempStrNoSize("\xA""getForward"),
            TempStrNoSize("\x16""ObjectID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x64""Returns the interpolated future for the relevant date"),
            TempStrNoSize("\x34""ID of the ForwardRateSource to use for interpolation"),
            TempStrNoSize("\x05""Dates"),  
            TempStrNoSize("\x07""Trigger"));  

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xA""getForward"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x12""createFRSByBumping"),
            TempStrNoSize("\x06""CCCBL#"),              
            TempStrNoSize("\x12""createFRSByBumping"),
            TempStrNoSize("\x22""OutputID,InputFRS,Spread,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x09""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Construct an object of class ForwardRateSource and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x1D""id of base curve to be bumped"),
            TempStrNoSize("\x06""Spread"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x12""createFRSByBumping"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x07""rollFRS"),
            TempStrNoSize("\x06""CCPCL#"),             
            TempStrNoSize("\x07""rollFRS"),
            TempStrNoSize("\x29""OutputObjectID,To Date,InputFRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x40""Role a FRS deterministically to a forward date and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x25""Anchor date for the new, rolled curve"),
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
    
        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x07""rollFRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x11""rollFRSStochastic"),
            TempStrNoSize("\x7""PCCPPL#"),             
            TempStrNoSize("\x11""rollFRSStochastic"),
            TempStrNoSize("\x39""OutputObjectID,InputFRS,To Dates,Random Numbers,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x48""Role a FRS Stochastically to a set of forward dates and return their ids"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x13""Rate source to roll"),
            TempStrNoSize("\x27""Forward dates for the new, rolled curve"),
            TempStrNoSize("\x27""Random numbers for the evolved curves"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x11""rollFRSStochastic"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
