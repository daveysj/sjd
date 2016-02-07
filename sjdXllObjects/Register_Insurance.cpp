#include <xlsdk/xlsdkdefines.hpp>


void registerInsurance(const XLOPER &xDll) 
{
    /*
        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x1E""createPortfolioInsuranceSimple"),
            TempStrNoSize("\xA""CCPPPPCCL#"),
            TempStrNoSize("\x1E""createPortfolioInsuranceSimple"),
            TempStrNoSize("\x48""ObjectID,StartDate,MatDate,InsDates,InsCaps,StandbyDRS,UtilDRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x44""Construct an object of class SBLPortfolioInsurance and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x16""Date the policy starts"),
            TempStrNoSize("\x17""Date the policy matures"),
            TempStrNoSize("\x2A""Dates where maximum insured amount changes"),
            TempStrNoSize("\x17""Maximum insured amounts"),
            TempStrNoSize("\x32""Discounting Rate Source to apply to standby amount"),
            TempStrNoSize("\x36""Discounting Rate Source to apply to utilisation amount"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x11""getInsuredMaximum"),
            TempStrNoSize("\x05""PCPP#"),              
            TempStrNoSize("\x11""getInsuredMaximum"),
            TempStrNoSize("\x15""ObjectID,Date,Trigger"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x34""Returns the maximum sum assured on a particular date"),
            TempStrNoSize("\x16""ID of insurance object"),
            TempStrNoSize("\x4""Date"),
            TempStrNoSize("\x7""Trigger"));           

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x14""getInsurancePayments"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x14""getInsurancePayments"),
            TempStrNoSize("\x1B""ObjectID,Dates,mtms,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x3C""Returns the insurance payments associated with a set of MtMs"),
            TempStrNoSize("\x16""ID of insurance object"),
            TempStrNoSize("\x27""Dates on which there are Portfolio MtMs"),
            TempStrNoSize("\x4""MtMs"),
            TempStrNoSize("\x8""Trigger"));
            */
}

void unregisterInsurance(const XLOPER &xDll) 
{
    /*
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x1E""createPortfolioInsuranceSimple"),
            TempStrNoSize("\xA""CCPPPPCCL#"),
            TempStrNoSize("\x1E""createPortfolioInsuranceSimple"),
            TempStrNoSize("\x48""ObjectID,StartDate,MatDate,InsDates,InsCaps,StandbyDRS,UtilDRS,Permanent"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),          
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x44""Construct an object of class SBLPortfolioInsurance and return its id"),
            TempStrNoSize("\x1A""id of object to be created"),  
            TempStrNoSize("\x16""Date the policy starts"),
            TempStrNoSize("\x17""Date the policy matures"),
            TempStrNoSize("\x2A""Dates where maximum insured amount changes"),
            TempStrNoSize("\x17""Maximum insured amounts"),
            TempStrNoSize("\x32""Discounting Rate Source to apply to standby amount"),
            TempStrNoSize("\x36""Discounting Rate Source to apply to utilisation amount"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1E""createPortfolioInsuranceSimple"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x11""getInsuredMaximum"),
            TempStrNoSize("\x05""PCPP#"),              
            TempStrNoSize("\x11""getInsuredMaximum"),
            TempStrNoSize("\x15""ObjectID,Date,Trigger"),
            TempStrNoSize("\x01""1"),                  
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x00"""),                   
            TempStrNoSize("\x34""Returns the maximum sum assured on a particular date"),
            TempStrNoSize("\x16""ID of insurance object"),
            TempStrNoSize("\x4""Date"),
            TempStrNoSize("\x7""Trigger"));           

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x11""getInsuredMaximum"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x14""getInsurancePayments"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x14""getInsurancePayments"),
            TempStrNoSize("\x1B""ObjectID,Dates,mtms,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x3C""Returns the insurance payments associated with a set of MtMs"),
            TempStrNoSize("\x16""ID of insurance object"),
            TempStrNoSize("\x27""Dates on which there are Portfolio MtMs"),
            TempStrNoSize("\x4""MtMs"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x14""getInsurancePayments"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
        */
}
