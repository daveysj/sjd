#include "ObjectCommon.h"

namespace sjdObjects 
{

   bool convertBuySellInput(char* buySell, sjd::BuySell &bs)
    {
        string buySellString(buySell);
        boost::to_lower(buySellString);
        if ((buySellString.compare("buy") == 0) || (buySellString.compare("b") == 0)) 
        {
            bs = sjd::BUY;
            return true;
        }
        else if ((buySellString.compare("sell") == 0) || (buySellString.compare("s") == 0)) 
        {
            bs = sjd::SELL;
            return true;
        }
        return false;
    }

   bool convertPutCallInput(char* putCall, sjd::PutCall &pc)
    {
        string putCallString(putCall);
        boost::to_lower(putCallString);

        if ((putCallString.compare("put") == 0) || (putCallString.compare("p") == 0) )
        {
            pc = sjd::PUT;
            return true;
        }
        else if ((putCallString.compare("call") == 0) || (putCallString.compare("c") == 0) )
        {
            pc = sjd::CALL;
            return true;
        }
        return false;
    }

}
