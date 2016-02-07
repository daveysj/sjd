#ifndef SJD_VOLATILITYSURFACES_INCLUDED
#define SJD_VOLATILITYSURFACES_INCLUDED

#include <sjd\maths\maths.h>
#include <sjd\OptionPricing\Black76Formula.h>
#include <sjd\RateSource\Volatility\DeterministicVolatilityRateSource.h>

#include <sjd\RateSource\Forward\ForwardRateSource.h>

#include <ql\math\interpolations\bilinearinterpolation.hpp>
#include <ql\math\interpolations\bicubicsplineinterpolation.hpp>
#include <ql\math\interpolations\interpolation2d.hpp>
#include <ql\time\daycounters\all.hpp>
#include <ql\time\calendars\nullcalendar.hpp>
#include <ql/types.hpp>
#include <ql/math/solvers1d/bisection.hpp>

#include <boost/algorithm/string.hpp>

using namespace QuantLib;

enum SurfaceInterpolatorType 
{
    BILINEAR, 
    BICUBIC
};



namespace sjd 
{
    /*======================================================================================
    VolatiltiySurfaceRateSource

    Abstract Base class for volatiltiy surfaces. This class contains basic surface 
    functionality
    =======================================================================================*/
    class VolatiltiySurfaceRateSource : public DeterministicVolatilityRateSource
    {
    public:
        
        virtual ~VolatiltiySurfaceRateSource() {};

        virtual bool isOK();
        virtual vector<Date> getObservationDates();

        void setInputs(Date anchorDate, 
                       vector<Date> observationDates, 
                       vector<double> strikeDimension,
                       QuantLib::Matrix volatilityData,
                       SurfaceInterpolatorType interpolatorType,
                       bool extrapolate);

        virtual SurfaceInterpolatorType getInterpolatorType();
        virtual void setInterpolatorType(SurfaceInterpolatorType interpolatorType);

        virtual void getRolledVariables(Date rolltoDate,
                                        vector<Date>& rolledObservationDates, 
                                        QuantLib::Matrix& rolledVols);

    protected:
        QuantLib::Matrix convertVectorOfVectorsToMatrix(vector<vector<double>> volatilityData);
        vector<Date> observationDates;
        vector<double> time, strikeDimension;
        Matrix volatility;
         boost::shared_ptr<Interpolation2D> interpolator;
        SurfaceInterpolatorType interpolatorType;
    };
}

#endif