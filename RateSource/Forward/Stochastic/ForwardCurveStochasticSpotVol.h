#ifndef SJD_FORWARDCURVEROLLSATSPOTVOL_INCLUDED
#define SJD_FORWARDCURVEROLLSATSPOTVOL_INCLUDED

#include <sjd\Tools\DatedCurve.h>
#include <sjd\RateSource\Forward\ForwardRateSourceStochastic.h>
#include <sjd\RateSource\Forward\Deterministic\ForwardCurveDeterministic.h>


using namespace QuantLib;

namespace sjd 
{

   /*======================================================================================
   ForwardCurveStochasticSpotVol 
    
    A curve containing date / rate inputs and a way to interpolate between these points.
    
    Note: If the first observation date is greater than the anchor date then, we will use
          the first observation date for all points between the two if 
          allowExtrapolation == false
    =======================================================================================*/
    class ForwardCurveStochasticSpotVol : public ForwardRateSourceStochastic
    {
    public:
        ForwardCurveStochasticSpotVol() {};

        ForwardCurveStochasticSpotVol(Date anchorDate, 
                                      vector<Date> observationDates, 
                                      vector<double> forwards,                                       
                                      ArrayInterpolatorType type,
                                      bool allowExtrapolation,
                                      boost::shared_ptr<VolatilityRateSource> vrs);

        virtual ~ForwardCurveStochasticSpotVol();

        void setParameters(Date anchorDate, 
                           vector<Date> observationDates, 
                           vector<double> forwards,                            
                           ArrayInterpolatorType type,
                           bool allowExtrapolation,
                           boost::shared_ptr<VolatilityRateSource> vrs);

        virtual bool isOK();

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
        vector<size_t> forwardIndexes;
        vector<double>  standardDeviations;
        boost::shared_ptr<DatedCurve> datedCurve;
        vector<boost::shared_ptr<ForwardCurveDeterministic> > frss;
    };

}

#endif