#ifndef SJD_FORWARDCURVESTOCHASTICFX_INCLUDED
#define SJD_FORWARDCURVESTOCHASTICFX_INCLUDED

#include <sjd\RateSource\Forward\ForwardRateSourceStochastic.h>
#include <sjd\RateSource\Forward\ForwardCurveFX.h>
#include <sjd\RateSource\Discount\DiscountCurves.h>

using namespace QuantLib;
using namespace boost;

namespace sjd 
{

   /*======================================================================================
   ForwardCurveStochasticFX 
    
    A curve containing date / rate inputs and a way to interpolate between these points.
    
    Note: If the first observation date is greater than the anchor date then, we will use
          the first observation date for all points between the two if 
          allowExtrapolation == false
    =======================================================================================*/
    class ForwardCurveStochasticFX : public ForwardRateSourceStochastic, 
                                     public UsesInterestRateDifferential
    {
    public:
        ForwardCurveStochasticFX() {};

        ForwardCurveStochasticFX(Date anchorDate, 
                                 double spot,
                                 Date spotDate,
                                 boost::shared_ptr<DiscountRateSource> domesticDRS,
                                 boost::shared_ptr<DiscountRateSource> foreignDRS,
                                 boost::shared_ptr<VolatilityRateSource> vrs);

        ForwardCurveStochasticFX(Date anchorDate, 
                                 double spotAtT0,
                                 boost::shared_ptr<DiscountRateSource> domesticDRS,
                                 boost::shared_ptr<DiscountRateSource> foreignDRS,
                                 boost::shared_ptr<VolatilityRateSource> vrs);

        virtual ~ForwardCurveStochasticFX();

        virtual bool isOK();

        void setParameters(Date anchorDate, 
                           double spot,
                           boost::shared_ptr<DiscountRateSource> domesticDRS,
                           boost::shared_ptr<DiscountRateSource> foreignDRS,
                           boost::shared_ptr<VolatilityRateSource> vrs);

        virtual void setSpotAtT0(double spotAtT0);
        virtual double getSpotAtT0();

        virtual void setRateSources(boost::shared_ptr<DiscountRateSource> domesticDRS,
                                    boost::shared_ptr<DiscountRateSource> foreignDRS);


        virtual double getForward(Date toDate) const;
        using ForwardRateSource::getForward;

        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, double normalRV);
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, 
                                                                 double normalRV,
                                                                 vector<Date> rateSetDates,
                                                                 vector<double> &rateSetsFromAnchorToDate);

        virtual void setForwardValuationDates(set<Date> dates);
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward();
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward(vector<Date> rateSetDates,
                                                              vector<double> &rateSetsFromAnchorToFinalRollDate);


    protected:
        vector<Date> getRolledDatesAheadOf(Date date);

        double spotAtT0;

        boost::shared_ptr<DiscountRateSource> domesticDRS;
        boost::shared_ptr<DiscountRateSource> foreignDRS;

        vector<double>  standardDeviations;
        vector<boost::shared_ptr<ForwardCurveFX> > frss;
    };

}

#endif