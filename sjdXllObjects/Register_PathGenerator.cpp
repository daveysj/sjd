#include <xlsdk/xlsdkdefines.hpp>


void registerPathGenerator(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x15""createESGSingleFactor"),
            TempStrNoSize("\x05""CCCL#"),              
            TempStrNoSize("\x15""createESGSingleFactor"),
            TempStrNoSize("\x18""ObjectID,Rates,Permanent"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),         
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x4C""Construct an object of class SingleFactorGBMPathGenerator and returns its id"),
            TempStrNoSize("\x1A""ID of object to be created"),  
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x0F""getESGPathForCI"),
            TempStrNoSize("\x06""PCPBP#"),
            TempStrNoSize("\x0F""getESGPathForCI"),
            TempStrNoSize("\x28""ObjectID,Dates,ConfInt,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x55""Returns a path of forward rates environments corresponding to the Confidence Interval"),
            TempStrNoSize("\x18""ID of the path generator"),
            TempStrNoSize("\xD""Forward dates"),            
            TempStrNoSize("\x23""Confidence interval NB 0 < c.i. < 1"),
            TempStrNoSize("\x7""Trigger"));

        
        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x11""getESGPathForSpot"),
            TempStrNoSize("\x06""PCPPP#"),
            TempStrNoSize("\x11""getESGPathForSpot"),
            TempStrNoSize("\x1F""ObjectID,Dates,SpotPath,Trigger"),
            TempStrNoSize("\x01""1"),           
            TempStrNoSize("\x9""prObjects"),    
            TempStrNoSize("\x00"""),            
            TempStrNoSize("\x00"""),            
            TempStrNoSize("\x4F""Returns a path of rates environments corresponding to the input input spot path"),
            TempStrNoSize("\x18""ID of the path generator"),
            TempStrNoSize("\xD""Forward dates"),            
            TempStrNoSize("\x09""Spot Path"),
            TempStrNoSize("\x7""Trigger"));
}

void unregisterPathGenerator(const XLOPER &xDll) 
{
    
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.
        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x15""createESGSingleFactor"),
            TempStrNoSize("\x05""CCCL#"),              
            TempStrNoSize("\x15""createESGSingleFactor"),
            TempStrNoSize("\x18""ObjectID,Rates,Permanent"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),         
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x4C""Construct an object of class SingleFactorGBMPathGenerator and returns its id"),
            TempStrNoSize("\x1A""ID of object to be created"),  
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x15""createESGSingleFactor"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x0F""getESGPathForCI"),
            TempStrNoSize("\x06""PCPBP#"),
            TempStrNoSize("\x0F""getESGPathForCI"),
            TempStrNoSize("\x28""ObjectID,Dates,ConfInt,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x55""Returns a path of forward rates environments corresponding to the Confidence Interval"),
            TempStrNoSize("\x18""ID of the path generator"),
            TempStrNoSize("\xD""Forward dates"),            
            TempStrNoSize("\x23""Confidence interval NB 0 < c.i. < 1"),
            TempStrNoSize("\x7""Trigger"));
        
        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x0F""getESGPathForCI"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);


        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x11""getESGPathForSpot"),
            TempStrNoSize("\x06""PCPPP#"),
            TempStrNoSize("\x11""getESGPathForSpot"),
            TempStrNoSize("\x1F""ObjectID,Dates,SpotPath,Trigger"),
            TempStrNoSize("\x01""1"),           
            TempStrNoSize("\x9""prObjects"),    
            TempStrNoSize("\x00"""),            
            TempStrNoSize("\x00"""),            
            TempStrNoSize("\x4F""Returns a path of rates environments corresponding to the input input spot path"),
            TempStrNoSize("\x18""ID of the path generator"),
            TempStrNoSize("\xD""Forward dates"),            
            TempStrNoSize("\x09""Spot Path"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x11""getESGPathForSpot"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}

