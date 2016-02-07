#ifndef SJD_FORWARDCURVERATESOURCE_INCLUDED
#define SJD_FORWARDCURVERATESOURCE_INCLUDED

#include <sjd\Tools\DatedCurve.h>
#include <sjd\RateSource\Forward\ForwardRateSourceDeterministic.h>


using namespace QuantLib;

namespace sjd 
{

   /*======================================================================================
   ForwardCurveDeterministic 
    
    A curve containing date / rate inputs and a way to interpolate between these points.
    
    Note: If the first observation date is greater than the anchor date then, we will use
          the first observation date for all points between the two if 
          allowExtrapolation == false
    =======================================================================================*/
    class ForwardCurveDeterministic : public ForwardRateSourceDeterministic
    {
    public:
        ForwardCurveDeterministic() {};

        ForwardCurveDeterministic(Date anchorDate, 
                                  vector<Date> observationDates, 
                                  vector<double> forwards, 
                                  ArrayInterpolatorType type = LINEAR,
                                  bool allowExtrapolation = false);

        virtual bool isOK();

        void setParameters(Date anchorDate, 
                           vector<Date> observationDates, 
                           vector<double> forwards, 
                           ArrayInterpolatorType type = LINEAR,
                           bool allowExtrapolation = false);

        virtual double getForward(Date toDate) const;
        using ForwardRateSource::getForward;

        vector<Date> getObservationDates();
        vector<double> getForwards();
        // ensure the updated forwards have the correct size before calling this method.
        void setForwards(vector<double> updatedForwards);

        virtual boost::shared_ptr<ForwardRateSource> parallelBump(double spread);
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate);
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, 
                                                                 vector<Date> rateSetDates,
                                                                 vector<double> &rateSetsFromAnchorToDate);

        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward();
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward(vector<Date> rateSetDates,
                                                              vector<double> &rateSetsFromAnchorToFinalRollDate);


    protected:
        boost::shared_ptr<DatedCurve> datedCurve;
    };

}

#endif