#include <xlsdk/xlsdkdefines.hpp>


void registerRatesEnvironment(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x16""createRatesEnvironment"),
            TempStrNoSize("\x10""CCPCCCCCL#"),
            TempStrNoSize("\x16""createRatesEnvironment"),
            TempStrNoSize("\x3D""ObjectID,AnchorDate,DRS,FRS,VRS,FixingRS,ForeignDRS,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x48""Constructs a rates environment (required for pricing) and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\x0B""Anchor date"),
            TempStrNoSize("\x14""Discount Rate Source"),
            TempStrNoSize("\x13""Forward Rate Source"),
            TempStrNoSize("\x16""Volatility Rate Source"),
            TempStrNoSize("\x12""Fixing Rate Source"),
            TempStrNoSize("\x5C""Foreign Discount Rate Source. Only required when moving an FX type fwd curve forward in time"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentDRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x16""getRatesEnvironmentDRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x1D""getRatesEnvironmentForeignDRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x1D""getRatesEnvironmentForeignDRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x42""Extracts the foreign discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentFRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x16""getRatesEnvironmentFRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentVRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x16""getRatesEnvironmentVRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x1B""getRatesEnvironmentFixingRS"),
            TempStrNoSize("\x56""CCPL#"),
            TempStrNoSize("\x1B""getRatesEnvironmentFixingRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
}

void unregisterRatesEnvironment(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x16""createRatesEnvironment"),
            TempStrNoSize("\x10""CCPCCCCCL#"),
            TempStrNoSize("\x16""createRatesEnvironment"),
            TempStrNoSize("\x3D""ObjectID,AnchorDate,DRS,FRS,VRS,FixingRS,ForeignDRS,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x48""Constructs a rates environment (required for pricing) and returns its id"),
            TempStrNoSize("\x1A""id of object to be created"), 
            TempStrNoSize("\x0B""Anchor date"),
            TempStrNoSize("\x14""Discount Rate Source"),
            TempStrNoSize("\x13""Forward Rate Source"),
            TempStrNoSize("\x16""Volatility Rate Source"),
            TempStrNoSize("\x12""Fixing Rate Source"),
            TempStrNoSize("\x5C""Foreign Discount Rate Source. Only required when moving an FX type fwd curve forward in time"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""createRatesEnvironment"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentDRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x16""getRatesEnvironmentDRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentDRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x1D""getRatesEnvironmentForeignDRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x1D""getRatesEnvironmentForeignDRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x42""Extracts the foreign discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1D""getRatesEnvironmentForeignDRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentFRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x16""getRatesEnvironmentFRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentFRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentVRS"),
            TempStrNoSize("\x05""CCPL#"),
            TempStrNoSize("\x16""getRatesEnvironmentVRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentVRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x1B""getRatesEnvironmentFixingRS"),
            TempStrNoSize("\x56""CCPL#"),
            TempStrNoSize("\x1B""getRatesEnvironmentFixingRS"),
            TempStrNoSize("\x17""RE_ID,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x00"""),          
            TempStrNoSize("\x3A""Extracts the discount rate source from a rates environment"),
            TempStrNoSize("\x17""ID of Rates Environment"),
            TempStrNoSize("\x07""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getRatesEnvironmentFixingRS"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

}
