#ifndef RATESENVIRONMENT_INCLUDED
#define RATESENVIRONMENT_INCLUDED

#include <sjd\RateSource\Forward\ForwardRateSource.h>
#include <sjd\RateSource\Volatility\VolatilityRateSource.h>
#include <sjd\RateSource\Discount\DiscountRateSource.h>
#include <sjd\RateSource\Fixing\FixingRateSource.h>

using namespace QuantLib;

namespace sjd 
{
    /*======================================================================================
    RatesEnvironment 
    
    A RatesEnvironment is a collection of rate sources, all with the same anchor date, that 
    are required for the valuation of a contract. Currently this object is set up for single 
    asset (i.e. the forward and volatility rate sources are for fx, commodity or equity). 
    The naming convention may need to change in the future to cater for rates as an asset class. 

    Ideally a rates environment will be created for a portfolio of single asset contracts and 
    could contain more information than strictly needed. For example you could construct a 
    RatesEnvironment with a volatility rate source even if the contract that was being 
    valued consisted only of forwards and fixed cash flows. The anticipated use 

        Contract.validate(RatesEnvironment);
        if (Contract.isOk())
            Contract.value(RatesEnvironment);
        else
            errorMessage
    =======================================================================================*/
    class RatesEnvironment : public RateSource
    {
    public:
        RatesEnvironment(Date anchorDate);

        virtual bool isOK();

        void setFRS(boost::shared_ptr<ForwardRateSource> frs);
        bool hasFRS();
        boost::shared_ptr<ForwardRateSource> getFRS();

        void setDRS(boost::shared_ptr<DiscountRateSource> drs);
        bool hasDRS();
        boost::shared_ptr<DiscountRateSource> getDRS();

        void setForeignDRS(boost::shared_ptr<DiscountRateSource> foreighDRS);
        bool hasForeignDRS();
        boost::shared_ptr<DiscountRateSource> getForeignDRS();

        void setVRS(boost::shared_ptr<VolatilityRateSource> vrs);
        bool hasVRS();
        boost::shared_ptr<VolatilityRateSource> getVRS();

        void setFixingRS(boost::shared_ptr<FixingRateSource> fixingrs);
        bool hasFixingRS();
        boost::shared_ptr<FixingRateSource> getFixingRS();

    protected:
        void updateDataAfterSettingRate();

        boost::shared_ptr<ForwardRateSource> frs;
        bool frsSet;
        boost::shared_ptr<VolatilityRateSource> vrs;
        bool vrsSet;
        boost::shared_ptr<DiscountRateSource> drs;
        bool drsSet;
        boost::shared_ptr<DiscountRateSource> foreigndrs;
        bool foreignDrsSet;
        boost::shared_ptr<FixingRateSource> fixingrs;
        bool fixingrsSet;
    };
}

#endif