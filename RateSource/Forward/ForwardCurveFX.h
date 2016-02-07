#ifndef SJD_FORWARDCURVEFX_INCLUDED
#define SJD_FORWARDCURVEFX_INCLUDED

#include <sjd\RateSource\Forward\ForwardRateSource.h>
#include <sjd\RateSource\Forward\UsesInterestRateDifferential.h>
#include <sjd\RateSource\Discount\DiscountRateSource.h>

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
    class ForwardCurveFX : public ForwardRateSource, 
                           public UsesInterestRateDifferential
    {
    public:
        ForwardCurveFX() {};

        ForwardCurveFX(Date anchorDate, 
                       double spot,
                       Date spotDate,
                       boost::shared_ptr<DiscountRateSource> domesticDRS,
                       boost::shared_ptr<DiscountRateSource> foreignDRS);

        ForwardCurveFX(Date anchorDate, 
                       double spotAtT0,
                       boost::shared_ptr<DiscountRateSource> domesticDRS,
                       boost::shared_ptr<DiscountRateSource> foreignDRS);

        virtual ~ForwardCurveFX();

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


    protected:
        double spotAtT0;

        boost::shared_ptr<DiscountRateSource> domesticDRS;
        boost::shared_ptr<DiscountRateSource> foreignDRS;
    };

}

#endif