#ifndef ASIANOPTION_INCLUDED
#define ASIANOPTION_INCLUDED

#include <vector>
#include <sjd\maths\maths.h>
#include <sjd\OptionPricing\BlackScholesFormula.h>


using namespace std;

namespace sjd {


   /*======================================================================================
   AverageRateOption 
    Base class for Average Rate or Asian Options
    
    Like the Black Scholes example we use inputs in their unitless for where possible i.e.
        discount factors not interest rates
        standard deviation not volatility
        year fractions not days

    In this documentation we will use the notation
        - N = the total number of averaging points; 
        - M = the number of points that have already rate set (0 <= M <= N)
    =======================================================================================*/
    class AverageRateOption
    {
    public:
        // Please enssure the reference to the Black 76 option is deleted in the class that
        // calls "new"
        virtual ~AverageRateOption() {};
        //BlackScholesGreeks& operator= (const BlackScholesGreeks &inputs);


        //Note on how to use these inputs DURING the averaging 
        //- Both vectors (fwds and sd) MUST ALWAYS have dimension N,
        //- the first M forwards are assumed to be rate set and will impact the 
        //     effective strike of the option
        //- the first M standard deviations are ignored completely but are still 
        //     required as inputs 
        void setParameters(vector<double> yearFractionsToForwards,
                           vector<double> fixingAndForwardRates, 
                           double strike, 
                           double yearsToExpiry, 
                           double yearsToSettlement, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);   
   
       //Returns true if there has been an error calculating any of the 
       //parameters, for example if there is a negative fwd fwd
       //volatiltiy, etc. The "getParameter()" method should always
       //check to ensure that there has been no error in calculation
       //before proceeding with the calculation.
       virtual bool isOK();

        string getErrorMessage()                {return errorMessage;};

        void setYearFractionsToRateSets(vector<double> time); 
        vector<double> getYearFractionsToRateSets() {return time;};

        void setForwards(vector<double> fixingAndForwardRates); 
        vector<double> getForwards()            {return forwards;};
        size_t getNumberOfRateSetForwards()     {return numberOfRateSetForwards;};
        void setStrike(double strike);
        double getStrike()                      {return strike;};
        void setTimeToExpity(double yearsToExpiry);
        double getTimeToExpiry()                {return yearsToExpiry;};
        void setTimeToSettlement(double yearsToSettlement);
        double getTimeToSettlement()            {return yearsToSettlement;};
        void setStandardDeviations(vector<double> standardDeviation);
        vector<double> getStandardDeviations()  {return standardDeviations;};
        void setDiscountFactor(double discountFactorToSettlement);   
        double getDiscountFactor()              {return dfToSettlement;};

        double virtual getPremium() = 0;

        // All Analytic Greeks are for the underlying Black Scholes option but are deferred to those
        // classes becuase of the possibility that the option has become a forward because
        // of very high rate sets
        virtual double getDeltaAnalytic() = 0;
        double getDeltaNumeric(double absoluteChangeInFormawrd);
        virtual double getGammaAnalytic() = 0;
        double getGammaNumeric(double absoluteChangeInFormawrd);
        virtual double getVegaAnalytic() = 0;
        double getVegaNumeric(double absoluteChangeVolatility);
        virtual double getThetaAnalytic() = 0;
        // NOTE: there is a special case where the change in time takes us from
        //  pre- to post-expiry. This case needs to be handled in the call and 
        //  put class
        virtual double getThetaNumeric(double absoluteChangeInYearFractions);
        virtual double getRhoAnalytic() = 0;
        double getRhoNumeric(double absoluteChangeInNominalInterestRate);
        virtual BlackScholesGreeks getOptionPriceAndGreeksAnalytic() = 0;
        virtual BlackScholesGreeks getOptionPriceAndGreeksNumeric(double absoluteDF, double absoluteDV, double absoluteDt, double absoluteDr);

       //There is a possibility that, during the averaging, an average rate
       //option ceses to be an option and becomes a forward. This happens
       //when the average of the rates which have set is greater than the 
       //original strike. This may be unlikely in practice but the mathematical
       //possibility existes and must be catered for;
        double getAmountByWhichAverageExceedsStrike()   {amountByWhichAverageExceedsStrike;};
      
        // At it's most basic level, the valuation of an Average Rate Option involves calling the 
        // Black 76 option formula with certain paramters (NB THERE ARE EXCEPTIONS TO THIS WHEN
        // THE OPTION BECOMES A FORWARD SO MAKE SURE YOU KNOW WHAT ANSWERS YOU MAY GET WHEN CALLING
        // THIS METHOD) There are times when we may need to see what these paramters are and this
        // method provides access to this calculation.
        // static because we want to call this from a static getPremium(...) method in the call and put
        static void getEquivalentBlack76Paramters(vector<double> fwds, size_t numberOfRateSetForwards,
                           double strike, vector<double> sds, 
                           double& equivalentFwd, double& equivalentStrike, double& equivalentSD,
                           double& amountByWhichForwardExceedsStrike);

    protected:

        // This resets all internal variables when an option paramter is set / reset
        void callWhenSettingAnyInternalParameter();

       //When changing any of the input parameters we need to recalculate the effective
        //black scholes input paramters
       void calculateInternalOptionParameters();


        string errorMessage;
        vector<double> time, forwards, standardDeviations;
        double strike, yearsToExpiry, yearsToSettlement, dfToSettlement;
        size_t numberOfRateSetForwards;

        bool internalParamtersSet;
        double amountByWhichAverageExceedsStrike;
        double effectiveForward, effectiveStrike, effectiveStandardDeviation;
       EuropeanOptionBlack76 *option;
    };


   /*======================================================================================
   AverageRateCall 
    =======================================================================================*/
    class AverageRateCall : public AverageRateOption
    {

    public:
       AverageRateCall(vector<double> yearFractionsToForwards,
                           vector<double> fixingAndForwardRates, 
                           double strike, 
                           double yearsToExpiry, 
                           double yearsToSettlement, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);
        
        ~AverageRateCall();

        bool isOK();

        // A method with the minimum info to return the price only of the asian option
        static double getPremium(vector<double> fixingAndForwardRates, 
                           size_t numberOfRateSetForwards,
                           double strike, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);

        double getPremium();
        double getDeltaAnalytic();
        double getGammaAnalytic();
        double getVegaAnalytic();
        double getThetaAnalytic();
        double getRhoAnalytic();
        BlackScholesGreeks getOptionPriceAndGreeksAnalytic();

        // NOTE: there is a special case where the change in time takes us from
        //  pre- to post-expiry. This case needs to be handled in the call and 
        //  put class
        double getThetaNumeric(double absoluteChangeInYearFractions);
        BlackScholesGreeks getOptionPriceAndGreeksNumeric(double absoluteDF, double absoluteDV, double absoluteDt, double absoluteDr);
   
    };

   /*======================================================================================
   AverageRatePut 
    =======================================================================================*/
    class AverageRatePut : public AverageRateOption
    {

    public:
       AverageRatePut(vector<double> yearFractionsToForwards,
                           vector<double> fixingAndForwardRates, 
                           double strike, 
                           double yearsToExpiry, 
                           double yearsToSettlement, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);
        ~AverageRatePut();
        bool isOK();

        // A method with the minimum info to return the price only of the asian option
        static double getPremium(vector<double> fixingAndForwardRates, 
                           size_t numberOfRateSetForwards,
                           double strike, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);

        double getPremium();
        double getDeltaAnalytic();
        double getGammaAnalytic();
        double getVegaAnalytic();
        double getThetaAnalytic();
        double getRhoAnalytic();
        BlackScholesGreeks getOptionPriceAndGreeksAnalytic();

        // NOTE: there is a special case where the change in time takes us from
        //  pre- to post-expiry. This case needs to be handled in the call and 
        //  put class
        double getThetaNumeric(double absoluteChangeInYearFractions);
        BlackScholesGreeks getOptionPriceAndGreeksNumeric(double absoluteDF, double absoluteDV, double absoluteDt, double absoluteDr);
   
    };

}
#endif
