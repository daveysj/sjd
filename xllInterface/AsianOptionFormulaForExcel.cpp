#include "AsianOptionFormulaForExcel.h"

namespace sjd {


   /*======================================================================================
   AverageRateOption 
    =======================================================================================*/
    void AverageRateOption::setParameters(vector<double> timeInput,
                                          vector<double> forwardsInput, 
                                          double strikeInput, 
                                          double yearsToExpiryInput, 
                                          double yearsToSettlementInput, 
                                          vector<double> standardDeviationsInput, 
                                          double dfToSettlementInput)
    {
        time = timeInput;
        forwards = forwardsInput;
        numberOfRateSetForwards = 0;
        vector<double>::iterator it = time.begin();
        while ((it != time.end()) && (*it <= 0)) {
            ++numberOfRateSetForwards;
            ++it;
        }
        standardDeviations = standardDeviationsInput;
        strike = strikeInput;
        yearsToExpiry = yearsToExpiryInput;
        yearsToSettlement = yearsToSettlementInput;
        dfToSettlement = dfToSettlementInput;
        callWhenSettingAnyInternalParameter();
    }

    void AverageRateOption::callWhenSettingAnyInternalParameter()
    {
        amountByWhichAverageExceedsStrike = 0;
        effectiveForward = 0; 
        effectiveStrike = 0;
        effectiveStandardDeviation = 0;
        option->setParameters(0,0,0,0,0,0);
        internalParamtersSet = false;
        errorMessage = "AverageRateOption->Error checking has not happened, please call isOK() first to check for errors in input parameters.";
    }

   void AverageRateOption::calculateInternalOptionParameters()
    {
        getEquivalentBlack76Paramters(forwards, numberOfRateSetForwards, strike, standardDeviations, 
                                      effectiveForward, effectiveStrike, effectiveStandardDeviation,
                                      amountByWhichAverageExceedsStrike);
        if (amountByWhichAverageExceedsStrike > 0)
            option->setParameters(0,0,0,0,0,0);
        else
            option->setParameters(effectiveForward, effectiveStrike, yearsToExpiry, yearsToSettlement, effectiveStandardDeviation, dfToSettlement);
        internalParamtersSet = true;
    }

    void AverageRateOption::getEquivalentBlack76Paramters(vector<double> fwds, size_t M, double X, 
                                                          vector<double> sds, double& equivalentFwd, 
                                                          double& equivalentStrike, double& equivalentSD,
                                                          double& fwdExceedsStrike)
    {
       fwdExceedsStrike = 0;   
        size_t N = fwds.size();
        vector<double> volatilityMeasure;
        equivalentFwd = 0; equivalentStrike = 0;
        for (size_t i = 0; i < M; ++i)
        {
            volatilityMeasure.push_back(sds[i] * sds[i]);
          equivalentStrike += fwds[i];
        }
        for (size_t i = M; i < N; ++i) {
            volatilityMeasure.push_back(sds[i] * sds[i]);
            equivalentFwd += fwds[i];
        }

        // During the averaging period there is a real chance that the Average to Date (i.e. the
        // sum of all the rate set rate divided by the TOTAL number of averaging points) is greater
        // Than the option's strike. If this is the case then
        //  - Puts will expire worthless: Even if the rest of the observation points are 0, the
        //      average rate will still exceed the strike and there will ne no payoff
        //  - Calls become forwards: Even if the rest of the observation points are 0, the
        //      average rate will still exceed the strike and there will be a payoff
       if (equivalentStrike > X * N) {                                   // option has ceased to be an option, it is now a forward
          fwdExceedsStrike = equivalentStrike -  X * N; // part of the "forward" has rate set
          fwdExceedsStrike += equivalentFwd;            // part of the "forward" is in the future and still variable
          fwdExceedsStrike /= N;                           // Always positive!
       }
        equivalentFwd /= (double) N;
        equivalentStrike /= (-(double) N);
          equivalentStrike += X; // may be negative if average exceeds spot
   
        // Note I do not check fwd fwd variance becuase I only use the vol out of T0 

       double NSquared = N*N;
        equivalentSD = 0;
        for (size_t j = M; j < N; ++j)
        {
          for (size_t i = j; i < N; ++i)
          {
            equivalentSD += 2* fwds[i]*fwds[j] / NSquared * 
              exp( volatilityMeasure[j] );
          }
          equivalentSD -= fwds[j]*fwds[j] / NSquared * 
          exp( volatilityMeasure[j] );
        }
        equivalentSD = log(equivalentSD / ( equivalentFwd * equivalentFwd ));
        equivalentSD = sqrt(equivalentSD);
    }

   bool AverageRateOption::isOK()
    {
        // Input vectors must be the same length
        if ((time.size() != forwards.size()) || (standardDeviations.size() != forwards.size()))
        {
           errorMessage = "AverageRateOption->All input vectors must be the same size EVEN during averaging";
           return false;
        }
        // some forwards must be live
        size_t numberOfAveragingPoints = forwards.size();
        if (numberOfRateSetForwards == numberOfAveragingPoints) { 
            errorMessage = "AverageRateOption->All forwards have rate set, this is not an option anymore";
            return false;
        }
        // time must be increasing
        if (!isStrictlyIncreasing<double>(time)) {
            errorMessage = "AverageRateOption->The vector of times to the rate set days is not strictly increasing";
            return false;
        }
        // Averages cannot rate set after expiry
        if ((time.back()) > yearsToExpiry) {
            errorMessage = "AverageRateOption->Option cannot expire before all the forwards have rate set";
            return false;
        }
        // option cannot settle before it expires
        if (yearsToSettlement < yearsToExpiry) {
            errorMessage = "AverageRateOption->Option cannot settle before it expires";
            return false;
        }
        // inputs must be positive
        for (size_t i = 0; i < forwards.size(); ++i) {
            if (forwards[i] <= 0) {
               errorMessage = "AverageRateOption->all rate sets or forwards must be positive";
               return false;
            }
        }
        for (size_t i = numberOfRateSetForwards; i < standardDeviations.size(); ++i) {
            if (standardDeviations[i] <= 0) {
               errorMessage = "AverageRateOption->all (live) forwards must have positive standard deviations";
               return false;
            }
        }
        if (strike <= 0) {
            errorMessage = "AverageRateOption->strike must be positive";
            return false;
        }
        if (yearsToExpiry <= 0) {
            errorMessage = "AverageRateOption->Time to expiry must be positive";
            return false;
        }
        if (yearsToSettlement <= 0) {
            errorMessage = "AverageRateOption->Time to settlement must be positive";
            return false;
        }
        if (dfToSettlement <= 0) {
            errorMessage = "AverageRateOption->Dsicount factor must be positive";
            return false;
        }
        // I don't check for -ve fwd fwd variances becuase I only use variance out of T0
        errorMessage = "AverageRateOption->No errors";        
        return true;
    }

   void AverageRateOption::setYearFractionsToRateSets(vector<double> timeInput)
    {
        time = timeInput;
        numberOfRateSetForwards = 0;
        vector<double>::iterator it = time.begin();
        while ((it != time.end()) && (*it <= 0)) {
            ++numberOfRateSetForwards;
            ++it;
        }
        callWhenSettingAnyInternalParameter();
    }
    void AverageRateOption::setForwards(vector<double> fixingAndForwardRates)
    {
        forwards = fixingAndForwardRates;
        callWhenSettingAnyInternalParameter();
    }
    void AverageRateOption::setStrike(double strikeInput)
    {
        strike = strikeInput;
        callWhenSettingAnyInternalParameter();
    }
    void AverageRateOption::setTimeToExpity(double yte)
    {
        yearsToExpiry = yte;
        callWhenSettingAnyInternalParameter();
    }
    void AverageRateOption::setTimeToSettlement(double yts)
    {
        yearsToSettlement = yts;
        callWhenSettingAnyInternalParameter();
    }
    void AverageRateOption::setStandardDeviations(vector<double> standardDeviation)
    {
        standardDeviations = standardDeviation;
        callWhenSettingAnyInternalParameter();
    }
    void AverageRateOption::setDiscountFactor(double df)   
    {
        dfToSettlement = df;
        callWhenSettingAnyInternalParameter();
    }

    double AverageRateOption::getDeltaNumeric(double absoluteChangeInFormawrd) 
    {        
        vector<double> originalFwds = forwards;
        vector<double> adjustedFwds;
        for (size_t i = 0; i < forwards.size(); ++i)
            adjustedFwds.push_back(forwards[i] + absoluteChangeInFormawrd);
        setForwards(adjustedFwds);
        double price1 = getPremium();
        setForwards(originalFwds);
        double price = getPremium();
        return price1 - price;
    }
    double AverageRateOption::getGammaNumeric(double absoluteChangeInFormawrd) 
    {
        vector<double> originalFwds = forwards;
        vector<double> fwdsPlus, fwdsMinus;
        for (size_t i = 0; i < forwards.size(); ++i) {
            fwdsPlus.push_back(forwards[i]+absoluteChangeInFormawrd);
            fwdsMinus.push_back(forwards[i]-absoluteChangeInFormawrd);
        }
        setForwards(fwdsPlus);
        double price1 = getPremium();
        setForwards(fwdsMinus);
        double price2 = getPremium();
        setForwards(originalFwds);
        double price = getPremium();
        return price1 - 2 * price + price2;
    }
    double AverageRateOption::getVegaNumeric(double absoluteChangeVolatility)  
    {
        vector<double> originalSDs = standardDeviations;
        vector<double> adjustedSDs;
        for (size_t i = 0; i < time.size(); ++i) {
            if (time[i] == 0)
                adjustedSDs.push_back(0);
            else
                adjustedSDs.push_back((originalSDs[i] / sqrt(time[i]) + absoluteChangeVolatility) * sqrt(time[i]));
        }
        setStandardDeviations(adjustedSDs);
        double price1 = getPremium();
        setStandardDeviations(originalSDs);
        double price = getPremium();
        return price1 - price;
    }
    double AverageRateOption::getThetaNumeric(double absoluteChangeInYearFractions)    
    {
        if (time.back() < absoluteChangeInYearFractions)
            return getPremium();
        vector<double> originalSDs = standardDeviations;
        vector<double> adjustedSDs;
        vector<double> originalTime = time;
        vector<double> adjustedTime;
        for (size_t i = 0; i < time.size(); ++i) {
            adjustedTime.push_back(time[i] - absoluteChangeInYearFractions);
            if (adjustedTime[i] < 0)
                adjustedSDs.push_back(0);
            else
                adjustedSDs.push_back(originalSDs[i] * sqrt(adjustedTime[i] / time[i]));
        }
        if (adjustedTime.back() <= 0)
            return getPremium();
        double originalYTE = yearsToExpiry;
        double adjustedYTE = yearsToExpiry - absoluteChangeInYearFractions;
        double originalYTS = yearsToSettlement;
        double adjustedYTS = yearsToSettlement - absoluteChangeInYearFractions;
        double originalDF = dfToSettlement;
        double adjustedDF= exp(log(dfToSettlement) / yearsToSettlement * adjustedYTS);

        setParameters(adjustedTime, forwards, strike, adjustedYTE, adjustedYTS, 
                     adjustedSDs, adjustedDF);
        double price1 = getPremium();
        setParameters(originalTime, forwards, strike, originalYTE, originalYTS, 
                      originalSDs, originalDF);
        double price = getPremium();
        return price1 - price;
    }
    double AverageRateOption::getRhoNumeric(double absoluteChangeInNominalInterestRate)    
    {
        double originalDF = dfToSettlement;
        double adjustedDF = exp((log(dfToSettlement) / yearsToSettlement - absoluteChangeInNominalInterestRate) * yearsToSettlement);
        setDiscountFactor(adjustedDF);
        double price1 = getPremium();
        setDiscountFactor(originalDF);        
        double price = getPremium();
        return price1 - price;
    }
    BlackScholesGreeks AverageRateOption::getOptionPriceAndGreeksNumeric(double absoluteDF, double absoluteDV, double absoluteDt, double absoluteDr)
    {
        vector<double> originalTime = time;
        vector<double> originalFwds = forwards;
        vector<double> originalSDs = standardDeviations;
        double originalYTE = yearsToExpiry;
        double originalYTS = yearsToSettlement;
        double originalDF = dfToSettlement;

        double price = getPremium();
        // Delta and gamma
        vector<double> fwdsPlus, fwdsMinus;
        for (size_t i = 0; i < forwards.size(); ++i) {
            fwdsPlus.push_back(forwards[i] + absoluteDF);
            fwdsMinus.push_back(forwards[i] - absoluteDF);
        }
        setForwards(fwdsPlus);
        double price1 = getPremium();
        setForwards(fwdsMinus);
        double price2 = getPremium();
        setForwards(originalFwds);
        double delta = price1 - price;
        double gamma = price1 - 2 * price + price2;
        // vega
        vector<double> adjustedSDs;
        for (size_t i = 0; i < time.size(); ++i) {
            if (time[i] == 0)
                adjustedSDs.push_back(0);
            else
                adjustedSDs.push_back((originalSDs[i] / sqrt(time[i]) + absoluteDV) * sqrt(time[i]));
        }
        setStandardDeviations(adjustedSDs);
        price1 = getPremium();
        setStandardDeviations(originalSDs);
        double vega = price1 - price;
        // theta
        double theta;
        if (abs(time.back() - absoluteDt) < 1e-14)
            theta = getPremium();
        else {
            adjustedSDs.clear();
            vector<double> adjustedTime;
            for (size_t i = 0; i < time.size(); ++i) {
                adjustedTime.push_back(time[i] - absoluteDt);
                if (adjustedTime[i] < 0)
                    adjustedSDs.push_back(0);
                else
                    adjustedSDs.push_back(originalSDs[i] * sqrt(adjustedTime[i] / time[i]));
            }
            if (adjustedTime.back() <= 0)
                theta = price;
            double adjustedYTE = yearsToExpiry - absoluteDt;
            double adjustedYTS = yearsToSettlement - absoluteDt;
            double adjustedDF= exp(log(dfToSettlement) / yearsToSettlement * adjustedYTS);
            setParameters(adjustedTime, forwards, strike, adjustedYTE, adjustedYTS, 
                         adjustedSDs, adjustedDF);
            price1 = getPremium();
            setParameters(originalTime, originalFwds, strike, originalYTE, originalYTS, 
                          originalSDs, originalDF);
            theta = price1 - price;
        }
        // rho
        double adjustedDF = exp((log(dfToSettlement) / yearsToSettlement - absoluteDr) * yearsToSettlement);
        setDiscountFactor(adjustedDF);
        price1 = getPremium();
        double rho = price1 - price;
        setParameters(originalTime, originalFwds, strike, originalYTE, originalYTS, 
                      originalSDs, originalDF);
        return BlackScholesGreeks(price, delta, gamma, theta, vega, rho);
    }


   /*======================================================================================
   AverageRateCall 
    =======================================================================================*/
    AverageRateCall::AverageRateCall(vector<double> yearFractionsToForwards,
                           vector<double> fixingAndForwardRates, 
                           double strike, 
                           double yearsToExpiry, 
                           double yearsToSettlement, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement)

    {
        option = new EuropeanCallBlack76(0,0,0,0,0,0);
       setParameters(yearFractionsToForwards, fixingAndForwardRates, strike, yearsToExpiry, 
                      yearsToSettlement, standardDeviation, discountFactorToSettlement);
    };

    AverageRateCall::~AverageRateCall()
    {
        delete option;
    }

    bool AverageRateCall::isOK()
    {
        bool isOKVariable = AverageRateOption::isOK();
        errorMessage = "AverageRateCall->" + errorMessage;
        return isOKVariable;
    }

     double AverageRateCall::getPremium()
     {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (effectiveStrike > 0)
            return option->getPremium();
        else 
            return amountByWhichAverageExceedsStrike * dfToSettlement;
     }

    double AverageRateCall::getPremium(vector<double> forwards, size_t M, double strike, vector<double> sds, 
                                        double df)
    {
        double bsF, bsX, bsSD, moneyness;
        getEquivalentBlack76Paramters(forwards, M, strike, sds, bsF, bsX, bsSD, moneyness);
        if (moneyness > 0)
            return moneyness * df;
        else
            return EuropeanCallBlack76::getPremium(bsF, bsX, bsSD, df);
    }


    double AverageRateCall::getDeltaAnalytic()   
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0) // the option is actually a forward
            return dfToSettlement;
        return option->getDeltaAnalytic();
    }
    double AverageRateCall::getGammaAnalytic()   
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return 0;
        return option->getGammaAnalytic();
    }
    double AverageRateCall::getVegaAnalytic()    
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return 0;
        return option->getVegaAnalytic();
    }
    double AverageRateCall::getThetaAnalytic()   
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return amountByWhichAverageExceedsStrike * dfToSettlement * (log(dfToSettlement) / yearsToSettlement);
        return option->getThetaAnalytic();
    }
    double AverageRateCall::getRhoAnalytic()     
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return amountByWhichAverageExceedsStrike * dfToSettlement * (-yearsToSettlement);
        return option->getRhoAnalytic();
    }
    BlackScholesGreeks AverageRateCall::getOptionPriceAndGreeksAnalytic()    
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
        {
            double premium = amountByWhichAverageExceedsStrike * dfToSettlement;
            double delta = dfToSettlement;
            double gamma = 0;
            double vega = 0;
            double theta = amountByWhichAverageExceedsStrike * dfToSettlement * (log(dfToSettlement) / yearsToSettlement);
            double rho = amountByWhichAverageExceedsStrike * dfToSettlement * (-yearsToSettlement);
            BlackScholesGreeks greeksForFwd(premium, delta, gamma, theta, vega, rho);
            return greeksForFwd;
        }
        return option->getOptionPriceAndGreeksAnalytic();
    }

    double AverageRateCall::getThetaNumeric(double absoluteDt)
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (absoluteDt < yearsToExpiry)
            return AverageRateOption::getThetaNumeric(absoluteDt);
        else
            // This will also work with "-ve" strikes, i.e. when the option is already a fwd
            return getPremium() - std::max(0.0, effectiveForward - effectiveStrike);
    }

    BlackScholesGreeks AverageRateCall::getOptionPriceAndGreeksNumeric(double dF, double dV, double dt, double dr)
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (dt < yearsToExpiry)
            return AverageRateOption::getOptionPriceAndGreeksNumeric(dF, dV, dt, dr);
        else {
            BlackScholesGreeks greeks = AverageRateOption::getOptionPriceAndGreeksNumeric(dF, dV, 0, dr);
            // This will also work with "-ve" strikes, i.e. when the option is already a fwd
            double theta = getPremium() - std::max(0.0, effectiveForward - effectiveStrike);
            greeks.setTheta(theta);
            return greeks;
        }
    }


   /*======================================================================================
   AverageRatePut 
    =======================================================================================*/
    AverageRatePut::AverageRatePut(vector<double> yearFractionsToForwards,
                           vector<double> fixingAndForwardRates, 
                           double strike, 
                           double yearsToExpiry, 
                           double yearsToSettlement, 
                           vector<double> standardDeviation, 
                           double discountFactorToSettlement)

    {
        option = new EuropeanPutBlack76(0,0,0,0,0,0);
       setParameters(yearFractionsToForwards, fixingAndForwardRates, strike, yearsToExpiry, 
                      yearsToSettlement, standardDeviation, discountFactorToSettlement);
    };

    AverageRatePut::~AverageRatePut() 
    {
        delete option;
    }

    bool AverageRatePut::isOK()
    {
        bool isOKVariable = AverageRateOption::isOK();
        errorMessage = "AverageRatePut->" + errorMessage;
        return isOKVariable;
    }

     double AverageRatePut::getPremium()
     {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (effectiveStrike > 0)
            return option->getPremium();
        else 
            return 0;
     }

    double AverageRatePut::getPremium(vector<double> forwards, size_t M, double strike, vector<double> sds, 
                                        double df)
    {
        double bsF, bsX, bsSD, moneyness;
        getEquivalentBlack76Paramters(forwards, M, strike, sds, bsF, bsX, bsSD, moneyness);
        if (moneyness > 0)
            return 0;
        else
            return EuropeanPutBlack76::getPremium(bsF, bsX, bsSD, df);
    }

    double AverageRatePut::getDeltaAnalytic()   
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0) // the option is actually a forward
            return 0;
        return option->getDeltaAnalytic();
    }
    double AverageRatePut::getGammaAnalytic()   
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return 0;
        return option->getGammaAnalytic();
    }
    double AverageRatePut::getVegaAnalytic()    
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return 0;
        return option->getVegaAnalytic();
    }
    double AverageRatePut::getThetaAnalytic()   
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return 0;
        return option->getThetaAnalytic();
    }
    double AverageRatePut::getRhoAnalytic()     
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
            return 0;
        return option->getRhoAnalytic();
    }
    BlackScholesGreeks AverageRatePut::getOptionPriceAndGreeksAnalytic()    
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (amountByWhichAverageExceedsStrike > 0)
        {
            double premium = 0;
            double delta = 0;
            double gamma = 0;
            double vega = 0;
            double theta = 0;
            double rho = 0;
            BlackScholesGreeks greeksForFwd(premium, delta, gamma, theta, vega, rho);
            return greeksForFwd;
        }
        return option->getOptionPriceAndGreeksAnalytic();
    }

    double AverageRatePut::getThetaNumeric(double absoluteDt)
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (absoluteDt < yearsToExpiry)
            return AverageRateOption::getThetaNumeric(absoluteDt);
        else
            // This will also work with "-ve" strikes, i.e. when the option is already a fwd
            return getPremium() - std::max(0.0, effectiveStrike - effectiveForward);
    }

    BlackScholesGreeks AverageRatePut::getOptionPriceAndGreeksNumeric(double dF, double dV, double dt, double dr)
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        if (dt < yearsToExpiry)
            return AverageRateOption::getOptionPriceAndGreeksNumeric(dF, dV, dt, dr);
        else {
            BlackScholesGreeks greeks = AverageRateOption::getOptionPriceAndGreeksNumeric(dF, dV, 0, dr);
            // This will also work with "-ve" strikes, i.e. when the option is already a fwd
            double theta = getPremium() - std::max(0.0, effectiveStrike - effectiveForward); 
            greeks.setTheta(theta);
            return greeks;
        }
    }

}