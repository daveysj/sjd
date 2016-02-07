#ifndef SJD_BLACKSCHOLES_INCLUDED
#define SJD_BLACKSCHOLES_INCLUDED

#include "OptionsFormula.h"
#include <boost/math/distributions/normal.hpp> // for normal_distribution
#include <string>

namespace sjd {

   /*======================================================================================
    NOTE: At this level we ONLY price options. Once the option expires and becomes
          a forward or a cashflow you MUST NOT call these formula. The decision as to which 
          formula to use (option, future, forward or cashflow) is deferred to later, wrapper
          classes with more logic. This is becuase:
            1) options can mature many ways;
            2) we implement options with unitless inputs (discount factors (df) instead of 
               interest rates; standard deviations (sd) instead of vols etc) and need to offer
               some type of sensible sanity testing for these inputs (sd > 0, df > 0 etc). 
               Once past maturity or settlement we would not be able implement these.
            3) To keep the code as clean and efficient as possible (I only want to check
               if the option is a put or a call once, I only want to check if the option
               is has matured but not settled one etc)
    =======================================================================================*/



   /*======================================================================================
   BlackInputs
    A class to store and retrieve the inputs necessary to calculate the value of a Black 76
    option plus some basic sanity testing. 
    Note: These are inputs to the very dumb formula only! The formula can only be used BEFORE
          option expiry (see notes at the beginning of this file and in the 
          description of EuropeanOptionBlack76 for some reasoning). Sanity testing is done 
          in this spirit and for example will return an error if for example
                        standard deviation <= 0
    =======================================================================================*/
    class BlackInputs {
    public:
        BlackInputs();
        BlackInputs(double forward, double strike, double yearsToExpiry, double yearsToSettlement, 
                    double standardDeviation, double discountFactor);

        void setValues(double forward, double strike, double yearsToExpiry, double yearsToSettlement, 
                       double standardDeviation, double discountFactor);

        bool isOK();
        std::string getErrorMessage()                           {return errorMessage;};

        void setForward(double forward)                         {F = forward;};
        double getForward()                                     {return F;};
        void setStrike(double strike)                           {X = strike;};
        double getStrike()                                      {return X;};        
        void setYearsToExpiry(double yearsToExpiry)             {yte = yearsToExpiry;};
        double getYearsToExpiry()                               {return yte;};
        void setYearsToSettlement(double yearsToSettlement)     {yte = yearsToSettlement;};
        double getYearsToSettlement()                           {return yts;};
        void setStandardDeviation(double standardDeviation)     {sd = standardDeviation;};
        double getStandardDeviation()                           {return sd;};
        void setDiscountFactor(double discountFactor)           {df = discountFactor;};
        double getDiscountFactor()                              {return df;};


    private:
        double F, X, yte, yts, sd, df;
        std::string errorMessage;
    };


    /*======================================================================================
   BlackScholesGreeks 
    A class to store and retrieve the option premium and all greeks that can be calculated 
    using a European Black Scholes Option
    =======================================================================================*/
    class BlackScholesGreeks {
    public:
        BlackScholesGreeks() {};
        BlackScholesGreeks(double premium, double delta, double gamma, double theta, double vega, double rho);

        void setValues(double premium, double delta, double gamma, double theta, double vega, double rho);

        void setPremium(double premiumInput)    {premium = premiumInput;};
        double getPremium()                     {return premium;};
        void setDelta(double deltaInput)        {delta = deltaInput;};
        double getDelta()                       {return delta;};
        void setGamma(double gammaInput)        {gamma = gammaInput;};
        double getGamma()                       {return gamma;};
        void setTheta(double thetaInput)        {theta = thetaInput;};
        double getTheta()                       {return theta;};
        void setVega(double vegaInput)          {vega = vegaInput;};
        double getVega()                        {return vega;};
        void setRho(double rhoInput)            {rho = rhoInput;};
        double getRho()                         {return rho;};

        // order = premium, delta, gamma, vega, rho, theta
        std::vector<double> getAllGreeks();
        std::vector<std::string> getAllGreeksDescription();

    private:
        double premium, delta, gamma, theta, vega, rho;

    };

   /*======================================================================================
    Black '76 Futures Option pricing algorithm, including all theoretical and numeric Greeks. 
    
    NOTE: At this level we only price options. Once the option expires and becomes
          a forward or a cashflow you need to use different logic and a different 
          formula. The decision as to which formula to use is deferred to later
          classes with more logic.
    
    Wherever possible, inputs to this forumula are unitless so. In particular
    the formula makes use of
      - discount factors rather than interest rates; 
      - standard deviation rather than volatility; and 
      - year fractions rather than days.  
    
    There are a number of reasons to use unitless inputs but the most important
    can be summarised as
      - In practice there are adjustments that need to happen to some inputs 
        before they can be used in the formula. Consider time as an example. 
        In this formula there is only one time variable. In practice there are
        two:
            - time to expiry: used with volatility to determine the amount of 
              variance in the option; and
            - time to settlement: used with the interest rate to determine a 
              disount factor
        These adjustments must sit with the class that implements business
        logic and not at the pricing formula level.
      - To integrate with other financial logic. Consider the 
        <code>discountFactor</code>. This  parameter will be extracted from 
        some interest rate source. This source may come in many forms but 
        will always have the ability to generate a discount factor. The 
        thinking is that rather than ask the interest rate source to calculate a 
        rate WITH a convnetion (NACA, NACS, NACC etc) and a day count convention
        (Actual/Actual, Actual/365, 30/360 etc) which is then only used to 
        generate a discount factor in the Black-Scholes formula, the interest 
        rate source should be queried for this information directly. 
        
    Even though there is significant code and information duplication, calls 
    and puts are implemented in separate objects. The responsibility of calling
    the correct (P)ut or (C)all option is deferred to the calling class
    
    There are two methods to calculate greeks
        - Analytic: using the formulas for the analytic derivative of the Black76 
          forumla; and
        - Numeric: the change in the (PV) option premium by changing 
          the input variable by the absolute amount specified
    These two greeks provide return values that have different units and are therefore
    not directly comparable.

    Wherever necessary, the following shorthand is used
        - OP: option premium
        - F: forward
        - X: strike
        - yte: year fractions to expiry 
        - yts: year fractions to settlement 
        - sd: standard deviation
        - df: discount factor

    @author Steve Davey
    @version 1.0 03/99
    =======================================================================================*/
    class EuropeanOptionBlack76
    {

    public :
  
        void setParameters(BlackInputs blackInputs);
       void setParameters(double forward, double strike, double yearsToExpiry, double yearsToSettlement, 
                           double standardDeviation, double discountFactor);

        //It is recommended, but optional, to call this method after setting any parameters.
        //If this method is not called, no error checking will happen so it is up to the 
        //user to either check the parameters are ok before trying to use the methods
        //in this class or to call this isOK() method.
        //This method will return false if any of the input variables are negative
        virtual bool isOK();
        //The error message will only be populated if the isOK() method is called and
        //there is a problem with one or more of the inputs. Only the first error
        //will be displayed in this message.
        std::string getErrorMessage()           {return errorMessage;};
        
        double getForward()                     {return forward;};
        void setForward(double forwardInput);
        double getStrike()                      {return strike;};
        void setStrike(double strikeInput);
        double getTimeToSettlement()            {return yearsToSettlement;};
        void setTimeToSettlement(double yts);
        double getTimeToExpiry()                {return yearsToExpiry;};
        void setTimeToExpiry(double yte);
        double getStandardDeviation()           {return standardDeviation;};
        void setStandardDeviation(double sd); 
        double getDiscountFactor()              {return discountFactor;};
        void setDiscountFactor(double df);

        virtual double getPremium() = 0;
        
        //NB: In the case of the Black'76 option pricing formula delta is a 
        //FORWARD DELTA position       
       virtual double getDeltaAnalytic() = 0;
        //returns op(F+Delta_F, X, yte, ytm, sd, df) - op(F, X, yte, ytm, sd, df)
        double getDeltaNumeric(double absoluteChangeInFormawrd);
               
        virtual double getGammaAnalytic() = 0;
        //returns (op(F+Delta_F, X, yte, ytm, sd, df) - 2 x (op(F, X, yte, ytm, sd, df)) +  op(F-Delta_F, X, yte, ytm, sd, df))
        //or delta(F) - delta(F-delta_F)
        double getGammaNumeric(double absoluteChangeInFormawrd);
                        
        virtual double getVegaAnalytic() = 0;
        //returns op(F, X, yte, ytm, sd_1, df) - op(F, X, yte, ytm, sd, df)
        //where 
        //    sd_1 = (sd / sqrt(yte) + absoluteChangeVolatility) * sqrt(yte)
        double getVegaNumeric(double absoluteChangeVolatility);

        //It's probably better to defer BOTH theta calculation to a class with business 
        //logic because the time in the discount factor and the standard deviation calculation
        //could increment differently and they could increment in different conventions
        //but here is a theoretical implementation based on the derivative w.r.t.
        //yearsToSettlement
        virtual double getThetaAnalytic() = 0;
        //returns op(F, X, yte, ytm, sd_1, df_1) - op(F, X, yte, ytm, sd, df)
        //where 
        //    sd_1 = (sd / sqrt(yte)) * sqrt(yte - absoluteChangeInYearFractions); and
        //    df_1 = exp((ln(df) / ytm) * (ytm - absoluteChangeInYearFractions))
        // NOTE: there is a special case where the change in time takes us from
        //  pre- to post-expiry. This case needs to be handled in the call and 
        //  put class
        virtual double getThetaNumeric(double absoluteChangeInYearFractions);
       
        // Note that in the Black76 formula, rho does NOT take account of 
        // changes in the option price because of a changing forward or future.
        // This is only a sensitivity to a change in the Discount Factor
        virtual double getRhoAnalytic() = 0;
        //returns op(F, X, yte, ytm, sd, df_1) - op(F, X, yte, ytm, sd, df)
        //where
        //    df_1 = exp(((ln(df) / ytm) - absoluteChangeInNominalInterestRate) * ytm)
        //i.e. Rates are incrementd in NACC convention
        double getRhoNumeric(double absoluteChangeInNominalInterestRate);

        BlackScholesGreeks getOptionPriceAndGreeksAnalytic();
        // NOTE: there is a special case where the change in time takes us from
        //  pre- to post-expiry. This case needs to be handled in the call and 
        //  put class        
        virtual BlackScholesGreeks getOptionPriceAndGreeksNumeric(double absoluteChangeInFormawrd, 
                                                          double absoluteChangeVolatility, 
                                                          double absoluteChangeInYearFractions, 
                                                          double absoluteChangeInNominalInterestRate);


    protected :

        // This resets all internal variables when an option paramter is set / reset
        void callWhenSettingAnyInternalParameter();

       //When changing any of the input parameters we need to recalculate d1, d2, Nd1, Nd2
       void calculateInternalOptionParameters();
   
       double forward, standardDeviation, discountFactor, yearsToExpiry, yearsToSettlement, strike;
        bool internalParamtersSet;
        // boost N(0,1)
        boost::math::normal n_0_1;
        double d1, d2, Nd1, Nd2;
        std::string errorMessage;

    };

    /*======================================================================================
    EuropeanCallBlack76
    =======================================================================================*/
    class EuropeanCallBlack76 : public EuropeanOptionBlack76
    {
    public :
       EuropeanCallBlack76(double forward, double strike, double yearsToExpiry,
          double yearsToSettlement, double standardDeviation, double discountFactor);

        bool isOK();

        static double getPremium(double forward, double strike, double standardDeviation, 
                                 double discountFactor);
       double getPremium();
       double getDeltaAnalytic();
       double getGammaAnalytic();
       double getVegaAnalytic(); 
       double getThetaAnalytic(); 
       double getRhoAnalytic();
        // NOTE: there is a special case where the change in time takes us from
        //  pre- to post-expiry. This case needs to be handled in the call and 
        //  put class
        double getThetaNumeric(double absoluteChangeInYearFractions);
        BlackScholesGreeks getOptionPriceAndGreeksNumeric(double absoluteChangeInFormawrd, 
                                                          double absoluteChangeVolatility, 
                                                          double absoluteChangeInYearFractions, 
                                                          double absoluteChangeInNominalInterestRate);

    };

   /*======================================================================================
    EuropeanPutBlack76
    =======================================================================================*/
    class EuropeanPutBlack76 : public EuropeanOptionBlack76
    {
    public :
       EuropeanPutBlack76(double forward, double strike, double yearsToExpiry,
          double yearsToSettlement, double standardDeviation, double discountFactor);

        bool isOK();

        static double getPremium(double forward, double strike, double standardDeviation, 
                                 double discountFactor);

       double getPremium();
       double getDeltaAnalytic();
       double getGammaAnalytic();
       double getVegaAnalytic(); 
       double getThetaAnalytic(); 
       double getRhoAnalytic();
        // NOTE: there is a special case where the change in time takes us from
        //  pre- to post-expiry. This case needs to be handled in the call and 
        //  put class
        double getThetaNumeric(double absoluteChangeInYearFractions);
        BlackScholesGreeks getOptionPriceAndGreeksNumeric(double absoluteChangeInFormawrd, 
                                                          double absoluteChangeVolatility, 
                                                          double absoluteChangeInYearFractions, 
                                                          double absoluteChangeInNominalInterestRate);
      
    };
}

#endif