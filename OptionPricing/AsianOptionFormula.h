#ifndef ASIANOPTION_INCLUDED
#define ASIANOPTION_INCLUDED

#include <vector>
#include <sjd\maths\maths.h>
#include "Black76Formula.h"


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

        //Note on how to use these inputs 
        //- Both vectors (fwds and sd) must have dimension N-M,
        //- The fixedRates determines the dimension M 
        void setParameters(vector<double> forwardRates,         // "Live" forwards
                           vector<double> fixedRates,
                           double strike, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);   
   
        void setParameters(size_t N,
                           size_t M,
                           double averageOfLiveForwards,         
                           double averageOfFixedRates,
                           double strike, 
                           double averageStandardDeviation, 
                           double discountFactorToSettlement);   

        //Returns true if there has been an error calculating any of the 
       //parameters, for example if there is a negative fwd fwd
       //volatiltiy, etc. The "getParameter()" method should always
       //check to ensure that there has been no error in calculation
       //before proceeding with the calculation.
       virtual bool isOK();

        string getErrorMessage()                        {return errorMessage;};

        vector<double> getForwards() const              {return forwards;};
        vector<double> getFixedRates() const            {return fixedRates;};
        size_t getNumberOfRateSetForwards()             {return M;};
        double getStrike() const                        {return strike;};
        vector<double> getStandardDeviations() const    {return standardDeviations;};
        double getDiscountFactor() const                {return dfToSettlement;};

        double virtual getPremium() = 0;
        virtual double getPremiumAfterMaturity(double rateSetRate, double discountFactor) = 0;
        virtual double getPremiumAfterSettlement() {return 0;};


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
        void getEquivalentBlack76ParamtersFromVectorInputs();

    protected:

        // This resets all internal variables when an option paramter is set / reset
        void callWhenSettingAnyInternalParameter();

       //When changing any of the input parameters we need to recalculate the effective
        //black scholes input paramters
       void calculateInternalOptionParameters();


        string errorMessage;
        vector<double> forwards, fixedRates, standardDeviations;
        // vector<double> time,
        double strike, dfToSettlement;
        size_t N, // total number of averaging dates
               M; // total number of historic averaging dates where we have rate sets

        bool internalParamtersSet;
        double amountByWhichAverageExceedsStrike;
        double effectiveForward, effectiveStrike, effectiveStandardDeviation;
       Black76Option *option;
    };


   /*======================================================================================
   AverageRateCall 
    =======================================================================================*/
    class AverageRateCall : public AverageRateOption
    {

    public:
       AverageRateCall(vector<double> forwardRates,         
                           vector<double> fixedRates,
                           double strike, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);
        
        ~AverageRateCall();

        bool isOK();

        double getPremium();   
        double getPremiumAfterMaturity(double rateSetRate, double discountFactor) {return std::max(rateSetRate - strike, 0.0) * discountFactor;};

    };

   /*======================================================================================
   AverageRatePut 
    =======================================================================================*/
    class AverageRatePut : public AverageRateOption
    {

    public:
       AverageRatePut(vector<double> forwardRates,         // "Live" forwards
                           vector<double> fixedRates,
                           double strike, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement);

        ~AverageRatePut();
        bool isOK();

        double getPremium();
        double getPremiumAfterMaturity(double rateSetRate, double discountFactor) {return std::max(strike - rateSetRate, 0.0) * discountFactor;};
   
    };

}
#endif
