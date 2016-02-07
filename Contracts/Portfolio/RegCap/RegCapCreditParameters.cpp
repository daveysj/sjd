#include "RegCapCreditParameters.h"

namespace sjd {

    RegCapCreditParameters::RegCapCreditParameters(double counterpartyWeighting, double capitalCharge)
    {
        cpw = counterpartyWeighting;
        cc = capitalCharge;
    }

    double RegCapCreditParameters::getCounterpartyWeighting() 
    {
        return cpw;
    }

    double RegCapCreditParameters::getCapitalCharge() 
    {
        return cc;
    }
            
}
