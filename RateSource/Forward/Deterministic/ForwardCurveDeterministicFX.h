#ifndef SJD_FORWARDCURVEDETERMINISITCFX_INCLUDED
#define SJD_FORWARDCURVEDETERMINISITCFX_INCLUDED

#include <sjd\RateSource\Forward\ForwardRateSourceDeterministic.h>
#include <sjd\RateSource\Forward\UsesInterestRateDifferential.h>
#include <sjd\RateSource\Forward\ForwardCurveFX.h>
#include <sjd\RateSource\Discount\DiscountRateSource.h>
#include <sjd\RateSource\Discount\DiscountCurves.h>

using namespace QuantLib;

namespace sjd 
{

   /*======================================================================================
   ForwardCurveStochasticFX 
    
    A curve containing date / rate inputs and a way to interpolate between these points.
    
    Note: If the first observation date is greater than the anchor date then, we will use
          the first observation date for all points between the two if 
          allowExtrapolation == false
    =======================================================================================*/
    class ForwardCurveDeterministicFX : public ForwardRateSourceDeterministic, 
                                        public UsesInterestRateDifferential
    {
    public:
        ForwardCurveDeterministicFX() {};

        ForwardCurveDeterministicFX(Date anchorDate, 
                       double spot,
                       Date spotDate,
                       boost::shared_ptr<DiscountRateSource> domesticDRS,
                       boost::shared_ptr<DiscountRateSource> foreignDRS);

        ForwardCurveDeterministicFX(Date anchorDate, 
                       double spotAtT0,
                       boost::shared_ptr<DiscountRateSource> domesticDRS,
                       boost::shared_ptr<DiscountRateSource> foreignDRS);

        virtual ~ForwardCurveDeterministicFX();

        void setParameters(Date anchorDate, 
                           double spotAtT0,
                           boost::shared_ptr<DiscountRateSource> domesticDRS,
                           boost::shared_ptr<DiscountRateSource> foreignDRS);

        virtual void setSpotAtT0(double spotAtT0);
        virtual double getSpotAtT0();

        virtual void setRateSources(boost::shared_ptr<DiscountRateSource> domesticDRS,
                                    boost::shared_ptr<DiscountRateSource> foreignDRS);

        virtual bool isOK();

        virtual double getForward(Date toDate) const;
        using ForwardRateSource::getForward;


        virtual boost::shared_ptr<ForwardRateSource> parallelBump(double spread);
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate);

    protected:
        vector<Date> getRolledDatesAheadOf(Date date);

        double spotAtT0;

        boost::shared_ptr<DiscountRateSource> domesticDRS;
        boost::shared_ptr<DiscountRateSource> foreignDRS;
    };

}

#endif