#ifndef SJD_FORWARDRATESOURCE_INCLUDED
#define SJD_FORWARDRATESOURCE_INCLUDED
#pragma once

#include <vector>
#include <sjd\RateSource\RateSource.h>
#include <sjd\RateSource\Volatility\VolatilityRateSource.h>

using namespace QuantLib;
using namespace std;

namespace sjd 
{
    /*======================================================================================
    ForwardRateSource

    The base class for any forward or futures curve. Typical examples include:
        FX type forward curve (spot rate with interest rate differentials), 
        Futures Curve, 
        Calendar Month Average curves

    Note: Classes that inherit from this should use the convention that if the user does not 
        supply a forward / future observation point on the anchor date, then the forward / 
        future to every point between, and including, the anchor date and the first 
        observation date should be the same as the value on the first observable forward / 
        future i.e. by default the classes should *not* use extrapolation from the first 
        observation point back to the anchor date but should rather flatten the curve here.
    =======================================================================================*/
    class ForwardRateSource : public RateSource
    {
    public:
        virtual ~ForwardRateSource() {};

        virtual double getForward(Date toDate) const = 0;
        virtual vector<double> getForward(vector<QuantLib::Date> toDate) const;

        // A forward forward is the factor that is applied to get the forward to T2 given
        // the forward to T1 where T0 <= T1 <= T2
        virtual double getForwardFworwardAdditive(Date fromDate, Date toDate) const;
        virtual double getForwardFworwardMultiplicative(Date fromDate, Date toDate) const;
        virtual std::vector<double> getForwardFworwardAdditive(Date fromDate, vector<Date> toDates) const;
        virtual std::vector<double> getForwardFworwardMultiplicative(Date fromDate, vector<Date> toDates) const;
    };

}

#endif