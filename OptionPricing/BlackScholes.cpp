#include "BlackScholes.h"

namespace sjd {

	/*======================================================================================
	BlackScholesGreeks 
	=======================================================================================*/
    BlackScholesGreeks::BlackScholesGreeks(double premium, double delta, double gamma, double theta, double vega, double rho) 
    {
        setValues(premium, delta, gamma, theta, vega, rho);
    }

    void BlackScholesGreeks::setValues(double premium, double delta, double gamma, double theta, double vega, double rho)
    {
        setPremium(premium);
        setDelta(delta); 
        setGamma(gamma);
        setTheta(theta);
        setVega(vega); 
        setRho(rho); 
    }

    std::vector<double> BlackScholesGreeks::getAllGreeks()
    {
        std::vector<double> data;
        data.push_back(getPremium());
        data.push_back(getDelta());
        data.push_back(getGamma());
        data.push_back(getVega());
        data.push_back(getRho());
        data.push_back(getTheta());
        return data;
    }

    std::vector<std::string> BlackScholesGreeks::getAllGreeksDescription()
    {
        std::vector<std::string> headings;
        headings.push_back("Premium");
        headings.push_back("Delta");
        headings.push_back("Gamma");
        headings.push_back("Vega");
        headings.push_back("Rho");
        headings.push_back("Theta");
        return headings;
    }

	/*======================================================================================
	EuropeanOptionBlack76
	=======================================================================================*/
	void EuropeanOptionBlack76::setParameters(double forwardInput, double strikeInput, double yearsToExpiryInput, double yearsToSettlementInput, 
                                              double standardDeviationInput, double discountFactorInput)
    {
        forward = forwardInput;
        strike = strikeInput;
        yearsToExpiry = yearsToExpiryInput;
        yearsToSettlement = yearsToSettlementInput;
        standardDeviation = standardDeviationInput;
        discountFactor = discountFactorInput;
        callWhenSettingAnyInternalParameter();
    }

    void EuropeanOptionBlack76::callWhenSettingAnyInternalParameter()
    {
        d1 = 0;
        d2 = 0;
        Nd1 = 0;
        Nd2 = 0;
        internalParamtersSet = false;
        errorMessage = "EuropeanOptionBlack76->Error checking has not happened, please call isOK() first to check for errors in input parameters.";
    }

    bool EuropeanOptionBlack76::isOK() 
    {
        if (forward <= 0) {
            errorMessage = "EuropeanOptionBlack76->Forward is <= 0";        
            return false;
        }
        if (strike <= 0) {
            errorMessage = "EuropeanOptionBlack76->Strike is <= 0";        
            return false;
        }
        if (yearsToExpiry <= 0) {
            errorMessage = "EuropeanOptionBlack76->Years to expiry are <= 0";        
            return false;
        }
        if (yearsToSettlement <= 0) {
            errorMessage = "EuropeanOptionBlack76->Years to settlement are <= 0";        
            return false;
        }
        if (standardDeviation <= 0) {
            errorMessage = "EuropeanOptionBlack76->Standard Deviation is <= 0";        
            return false;
        }
        if (discountFactor <= 0) {
            errorMessage = "EuropeanOptionBlack76->Discount Factor is <= 0";        
            return false;
        }
        if (yearsToSettlement < yearsToExpiry) {
            errorMessage = "EuropeanOptionBlack76->Option cannot settle before it expires";        
            return false;
        }
        errorMessage = "EuropeanOptionBlack76->No errors";        
        return true;
    }


	void EuropeanOptionBlack76::calculateInternalOptionParameters()
    {
	    d1 = (log(forward/strike) / standardDeviation + standardDeviation / 2.0);
	    d2 = d1 - (standardDeviation);	  
	    Nd1 = cdf(n_0_1, d1);
	    Nd2 = cdf(n_0_1, d2);
        internalParamtersSet = true;
    }

    void EuropeanOptionBlack76::setForward(double forwardInput)    
    {
        forward = forwardInput;
        callWhenSettingAnyInternalParameter();
    }

    void EuropeanOptionBlack76::setStrike(double strikeInput)      
    {
        strike = strikeInput;
        callWhenSettingAnyInternalParameter();
    }

    void EuropeanOptionBlack76::setTimeToSettlement(double yts)     
    {
        yearsToSettlement = yts;
        callWhenSettingAnyInternalParameter();
    }

    void EuropeanOptionBlack76::setTimeToExpiry(double yte)       
    {
        yearsToExpiry = yte;
        callWhenSettingAnyInternalParameter();
    }

    void EuropeanOptionBlack76::setStandardDeviation(double sd)    
    {
        standardDeviation = sd;
        callWhenSettingAnyInternalParameter();
    }

    void EuropeanOptionBlack76::setDiscountFactor(double df)       
    {
        discountFactor = df;
        callWhenSettingAnyInternalParameter();
    }


    double EuropeanOptionBlack76::getDeltaNumeric(double absoluteChangeInFormawrd)
    {
        double F = forward;
        double dF = forward + absoluteChangeInFormawrd;

        setForward(dF);
        double price1 = getPremium();
        setForward(F);
        double price = getPremium();
        return price1 - price;
    }

    double EuropeanOptionBlack76::getGammaNumeric(double absoluteChangeInFormawrd)
    {
        double F = forward;
        double dF = forward + absoluteChangeInFormawrd;
        double dF1 = forward - absoluteChangeInFormawrd;

        setForward(dF);
        double price1 = getPremium();
        setForward(dF1);
        double price2 = getPremium();
        setForward(F);
        double price = getPremium();
        return (price1 - 2 * price + price2);
    }
                        
    double EuropeanOptionBlack76::getVegaNumeric(double absoluteChangeVolatility)
    {
        double sd = standardDeviation;
        double sqrtdt = sqrt(yearsToExpiry);
        double originalVol = standardDeviation / sqrtdt;
        double dSD = (originalVol + absoluteChangeVolatility) * sqrtdt;

        setStandardDeviation(dSD);
        double price1 = getPremium();
        setStandardDeviation(sd);
        double price = getPremium();
        return price1 - price;
    }

    double EuropeanOptionBlack76::getThetaNumeric(double absoluteChangeInYearFractions)
    {
        double sd = standardDeviation;
        double df = discountFactor;
        double yte = yearsToExpiry;
        double yts = yearsToSettlement;
        if (yte < absoluteChangeInYearFractions) // and by dfn yte <= yts so we only check the smaller
            return getPremium();
        double sd1 = (standardDeviation / sqrt(yearsToExpiry)) * sqrt(yearsToExpiry - absoluteChangeInYearFractions);
        double df1 = exp(log(discountFactor) / yearsToSettlement * (yearsToSettlement - absoluteChangeInYearFractions));

        setParameters(forward, strike, yte - absoluteChangeInYearFractions, 
                      yts - absoluteChangeInYearFractions, sd1, df1);
        double price1 = getPremium();
        setParameters(forward, strike, yte, yts, sd, df);
        double price = getPremium();
        return price1 - price;
    }
	    
    double EuropeanOptionBlack76::getRhoNumeric(double absoluteChangeInNominalInterestRate)
    {
        double df = discountFactor;
        double df1 = exp((log(discountFactor) / yearsToSettlement - absoluteChangeInNominalInterestRate) * yearsToSettlement);

        setDiscountFactor(df1);
        double price1 = getPremium();
        setDiscountFactor(df);
        double price = getPremium();
        return price1 - price;
    }

    BlackScholesGreeks EuropeanOptionBlack76::getOptionPriceAndGreeksAnalytic()
    {
        double premium = getPremium();
        double delta = getDeltaAnalytic();
        double gamma = getGammaAnalytic();
        double vega = getVegaAnalytic();
        double theta = getThetaAnalytic();
        double rho = getRhoAnalytic();
        return BlackScholesGreeks(premium, delta, gamma, theta, vega, rho);
    }

    BlackScholesGreeks EuropeanOptionBlack76::getOptionPriceAndGreeksNumeric(double dF, double dV, double dY, double dR) 
    {
        double F = forward;
        double sd = standardDeviation;
        double df = discountFactor;
        double yte = yearsToExpiry;
        double yts = yearsToSettlement;
        double X = strike;
        double sqrtdt = sqrt(yte);

        double price = getPremium();
        // Delta
        setForward(F + dF);
        double deltaPremium = getPremium();
        double delta = deltaPremium - price;
        // Gamma
        setForward(F - dF);
        double gammaPremium = getPremium();
        double gamma = (deltaPremium - 2 * price + gammaPremium);        
        setForward(F);
        // Vega
        double originalVol = sd / sqrtdt;
        double sd1 = (originalVol + dV) * sqrtdt;
        setStandardDeviation(sd1);
        double priceVega = getPremium();
        double vega = priceVega - price;
        // setStandardDeviation(sd1); will do a setParamters in Theta
        // Theta
        sd1 = (sd / sqrt(yte)) * sqrt(yte - dY);
        double df1 = exp(log(df) / yts * (yts - dY));
        setParameters(F, X, yte - dY, yts - dY, sd1, df1);
        double priceTheta = getPremium();
        double theta = priceTheta - price;
        setParameters(F, X, yte, yts, sd, df);
        // Rho
        df1 = exp((log(df) / yts - dR) * yts);
        setDiscountFactor(df1);
        double priceRho = getPremium();
        double rho = priceRho - price;
        setParameters(F, X, yte, yts, sd, df);
        return BlackScholesGreeks(price, delta, gamma, theta, vega, rho);
    }

    /*======================================================================================
    EuropeanCallBlack76
    =======================================================================================*/
	EuropeanCallBlack76::EuropeanCallBlack76(double f, double x, double yte,
		    double yts, double sd, double df)
    {
	    setParameters(f, x, yte, yts, sd, df);
    }

    bool EuropeanCallBlack76::isOK() 
    {
        bool isOKVariable = EuropeanOptionBlack76::isOK();
        errorMessage = "EuropeanCallBlack76->" + errorMessage;
        return isOKVariable;
    }

    double EuropeanCallBlack76::getPremium(double forward, double strike, double standardDeviation, 
                                           double discountFactor)
    {
	    double d1 = (log(forward/strike) / standardDeviation + standardDeviation / 2.0);
	    double d2 = d1 - (standardDeviation);	  
        boost::math::normal s; // Construct a standard normal distribution s
	    double Nd1 = cdf(s, d1);
	    double Nd2 = cdf(s, d2);
        return discountFactor * (forward *  Nd1 - strike * Nd2);        
    }


	double EuropeanCallBlack76::getPremium() 
    {
        if (!internalParamtersSet) {
            calculateInternalOptionParameters();
        }
        return discountFactor * (forward *  Nd1 - strike * Nd2);
    }

	double EuropeanCallBlack76::getDeltaAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
    	return Nd1 * discountFactor;
    }

	double EuropeanCallBlack76::getGammaAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
	    return pdf(n_0_1, d1) * discountFactor / (forward * standardDeviation);
    }

	double EuropeanCallBlack76::getVegaAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
    	return forward * sqrt(yearsToSettlement) * pdf(n_0_1, d1) * discountFactor;
    }

	double EuropeanCallBlack76::getThetaAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
	    double rfr = -log(discountFactor) / yearsToSettlement;
	    return -(forward * pdf(n_0_1, d1) * standardDeviation * discountFactor) / 
	    ( 2 * yearsToSettlement) + rfr * discountFactor * (forward * Nd1 - strike * Nd2);
    }

	double EuropeanCallBlack76::getRhoAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        // Note the next line is rho for a Black-Scholes option
	    // return -strike * yearsToSettlement * discountFactor * (1-Nd2);
        return -yearsToSettlement * discountFactor * (forward *  Nd1 - strike * Nd2);
    }

    double EuropeanCallBlack76::getThetaNumeric(double absoluteChangeInYearFractions)
    {
        if (absoluteChangeInYearFractions < yearsToExpiry)
            return EuropeanOptionBlack76::getThetaNumeric(absoluteChangeInYearFractions);
        else
            return getPremium() - std::max(0.0, forward - strike);
    }

    BlackScholesGreeks EuropeanCallBlack76::getOptionPriceAndGreeksNumeric(double dF, 
                                                                           double dV, 
                                                                           double dt, 
                                                                           double dr)
    {
        if (dt < yearsToExpiry)
            return EuropeanOptionBlack76::getOptionPriceAndGreeksNumeric(dF, dV, dt, dr);
        else {
            BlackScholesGreeks greeks = EuropeanOptionBlack76::getOptionPriceAndGreeksNumeric(dF, dV, 0, dr);
            double theta = getPremium() - std::max(0.0, forward - strike);
            greeks.setTheta(theta);
            return greeks;
        }
    }



	/*======================================================================================
    EuropeanPutBlack76
    =======================================================================================*/
	EuropeanPutBlack76::EuropeanPutBlack76(double forward, double strike, double yearsToExpiry,
		double yearsToSettlement, double standardDeviation, double discountFactor)
    {
	    setParameters(forward, strike, yearsToExpiry, yearsToSettlement, 
                      standardDeviation, discountFactor);
    }

    bool EuropeanPutBlack76::isOK() 
    {
        bool isOKVariable = EuropeanOptionBlack76::isOK();
        errorMessage = "EuropeanPutBlack76->" + errorMessage;
        return isOKVariable;
    }

    double EuropeanPutBlack76::getPremium(double forward, double strike, double standardDeviation, 
                                double discountFactor)
    {
	    double d1 = (log(forward/strike) / standardDeviation + standardDeviation / 2.0);
	    double d2 = d1 - (standardDeviation);	  
        boost::math::normal s; // Construct a standard normal distribution s
	    double Nd1 = cdf(s, d1);
	    double Nd2 = cdf(s, d2);
      	return (- forward * (1-Nd1) + strike * (1-Nd2)) * discountFactor;
    }

	double EuropeanPutBlack76::getPremium()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
      	return discountFactor * (- forward * (1-Nd1) + strike * (1-Nd2));
    }

    double EuropeanPutBlack76::getDeltaAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        return (Nd1 - 1.0) * discountFactor;
    }

    double EuropeanPutBlack76::getGammaAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        return pdf(n_0_1, d1) * discountFactor / (forward * standardDeviation);
    }

	double EuropeanPutBlack76::getVegaAnalytic() 
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        return forward * sqrt(yearsToSettlement) * pdf(n_0_1, d1) * discountFactor;
    }

    double EuropeanPutBlack76::getThetaAnalytic() 
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
	    double rfr = -log(discountFactor) / yearsToSettlement;		
	    return -(forward * pdf(n_0_1,d1) * standardDeviation * discountFactor) / 
	    ( 2 * yearsToSettlement) - rfr * discountFactor * (forward * (1-Nd1) - strike * (1-Nd2));

    }

	double EuropeanPutBlack76::getRhoAnalytic()
    {
        if (!internalParamtersSet)
            calculateInternalOptionParameters();
        // Note the next line is rho for a Black-Scholes option
    	//return -strike * yearsToSettlement * discountFactor * (1-Nd2);
        return - yearsToSettlement * discountFactor * (- forward * (1-Nd1) + strike * (1-Nd2));
    }

    double EuropeanPutBlack76::getThetaNumeric(double absoluteChangeInYearFractions)
    {
        if (absoluteChangeInYearFractions < yearsToExpiry)
            return EuropeanOptionBlack76::getThetaNumeric(absoluteChangeInYearFractions);
        else {
            return getPremium() - std::max(0.0, strike - forward);
        }
    }
    BlackScholesGreeks EuropeanPutBlack76::getOptionPriceAndGreeksNumeric(double dF, 
                                                                           double dV, 
                                                                           double dt, 
                                                                           double dr)
    {
        if (dt < yearsToExpiry)
            return EuropeanOptionBlack76::getOptionPriceAndGreeksNumeric(dF, dV, dt, dr);
        else {
            BlackScholesGreeks greeks = EuropeanOptionBlack76::getOptionPriceAndGreeksNumeric(dF, dV, 0, dr);
            double theta = getPremium() - std::max(0.0, strike - forward);
            greeks.setTheta(theta);
            return greeks;
        }
    }

}