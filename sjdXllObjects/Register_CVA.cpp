#include <xlsdk/xlsdkdefines.hpp>


void registerCVA(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\xF""createCVAFlatPD"),
            TempStrNoSize("\x7""CCPBBL#"),
            TempStrNoSize("\xF""createCVAFlatPD"),
            TempStrNoSize("\x24""ObjectID,AnchorDate,PD,LGD,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x38""Constructs a CVA object with a flat PD and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\xB""Anchor date"),  
            TempStrNoSize("\x2A""Probability of default, in the range [0,1]"),
            TempStrNoSize("\x26""Loss given default, in the range [0,1]"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\xD""createCVAFCDF"),
            TempStrNoSize("\x8""CCPPPPL#"),
            TempStrNoSize("\xD""createCVAFCDF"),
            TempStrNoSize("\x2B""ObjectID,AnchorDate,Dates,CDP,LGD,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x5A""Constructs a CVA object using a curve of Cumulative Default Probability and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\xB""Anchor date"),  
            TempStrNoSize("\x5""Dates"),
            TempStrNoSize("\x49""Cumulative default probabilities (must be increasing and between 0 and 1)"),
            TempStrNoSize("\x26""Loss given default, in the range [0,1]"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x9""getCVALGD"),
            TempStrNoSize("\x5""PCPP#"),              
            TempStrNoSize("\x9""getCVALGD"),
            TempStrNoSize("\x17""ObjectID,ToDate,Trigger"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x42""Returns the Loss Given Default, assumed to be constat for all time"),
         TempStrNoSize("\x10""ID of CVA object"),
            TempStrNoSize("\x21""From the Anchor date to this date"),
            TempStrNoSize("\x7""Trigger"));           

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x8""getCVAPD"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x8""getCVAPD"),
            TempStrNoSize("\x1C""ObjectID,date1,date2,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x38""Returns the probability of default between any two dates"),
            TempStrNoSize("\x10""ID of CVA object"),
            TempStrNoSize("\xC""Earlier Date"),
            TempStrNoSize("\xA""Later Date"),
            TempStrNoSize("\x8""Trigger"));
}

void unregisterCVA(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\xF""createCVAFlatPD"),
            TempStrNoSize("\x7""CCPBBL#"),
            TempStrNoSize("\xF""createCVAFlatPD"),
            TempStrNoSize("\x24""ObjectID,AnchorDate,PD,LGD,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x38""Constructs a CVA object with a flat PD and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\xB""Anchor date"),  
            TempStrNoSize("\x2A""Probability of default, in the range [0,1]"),
            TempStrNoSize("\x26""Loss given default, in the range [0,1]"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xF""createCVAFlatPD"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\xD""createCVAFCDF"),
            TempStrNoSize("\x8""CCPPPPL#"),
            TempStrNoSize("\xD""createCVAFCDF"),
            TempStrNoSize("\x2B""ObjectID,AnchorDate,Dates,CDP,LGD,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x5A""Constructs a CVA object using a curve of Cumulative Default Probability and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\xB""Anchor date"),  
            TempStrNoSize("\x5""Dates"),
            TempStrNoSize("\x49""Cumulative default probabilities (must be increasing and between 0 and 1)"),
            TempStrNoSize("\x26""Loss given default, in the range [0,1]"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xD""createCVAFCDF"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x9""getCVALGD"),
            TempStrNoSize("\x5""PCPP#"),              
            TempStrNoSize("\x9""getCVALGD"),
            TempStrNoSize("\x17""ObjectID,ToDate,Trigger"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x42""Returns the Loss Given Default, assumed to be constat for all time"),
         TempStrNoSize("\x10""ID of CVA object"),
            TempStrNoSize("\x21""From the Anchor date to this date"),
            TempStrNoSize("\x7""Trigger"));           

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x9""getCVALGD"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x8""getCVAPD"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x8""getCVAPD"),
            TempStrNoSize("\x1C""ObjectID,date1,date2,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x38""Returns the probability of default between any two dates"),
            TempStrNoSize("\x10""ID of CVA object"),
            TempStrNoSize("\xC""Earlier Date"),
            TempStrNoSize("\xA""Later Date"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x8""getCVAPD"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
}
