#ifndef PORTFOLIOINSURANCE_INCLUDED
#define PORTFOLIOINSURANCE_INCLUDED
#pragma once

#include <vector>
#include <sjd\maths\maths.h>
#include <ql\time\date.hpp>
#include <sjd\RateSource\Discount\DiscountRateSource.h>
#include <Tools\Tools.h>

using namespace std;
using namespace QuantLib;

namespace sjd {

   /*======================================================================================
   Portfolio Insurance

    =======================================================================================*/
    class PortfolioInsurance
    {
    public:
        virtual ~PortfolioInsurance() {};
        
        void setParameters(Date policyStartDate, 
                           Date policyMaturityDate, 
                           boost::shared_ptr<DiscountRateSource> standbyDRS, 
                           boost::shared_ptr<DiscountRateSource> utilisationDRS);

        // The function that gets the sum insured should not return a negative value
        virtual double getSumInsured(Date date) = 0;
        boost::shared_ptr<DiscountRateSource> getStandbyDRS();
        boost::shared_ptr<DiscountRateSource> getUtilisationDRS();

      // Assumes all vectors are of the same length. 
        virtual void getInsurancePayments(vector<Date> dates, vector<double> mtm, vector<double> &insurancePayments);

        bool isOK();
        vector<string> getErrorMessages() const;

        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

    protected:
        void checkSizesAndCalcuateSpreads(vector<Date> dates, vector<double> mtm, vector<double> &insurancePayments);

        vector<Date> dates;
        vector<double> fwdFwdStandbyFactor, fwdFwdUtilisationFactor;


        Date policyStartDate;
        Date policyMaturityDate;
        boost::shared_ptr<DiscountRateSource> standbyDRS;
        boost::shared_ptr<DiscountRateSource> utilisationDRS;
        string className;
    };

    /*======================================================================================
   SimpleInsurance: Insurance paid in arrears based on the MtM at the end of the period

    =======================================================================================*/
    class SimpleInsurance : public PortfolioInsurance
    {
    public:
        SimpleInsurance(Date policyStartDate, Date policyMaturityDate, double constantSumInsured, 
                        boost::shared_ptr<DiscountRateSource> standbyDRS, 
                        boost::shared_ptr<DiscountRateSource> utilisationDRS);

        virtual double getSumInsured(Date date);

    private:

        double sumInsured;
    };

    /*======================================================================================
   SimpleInsuranceWithTermStructure: Insurance paid in arrears based on the MtM at the end 
    of the period

    =======================================================================================*/
    class SimpleInsuranceWithTermStructure : public PortfolioInsurance
    {
    public:
        SimpleInsuranceWithTermStructure(Date policyStartDate, Date policyMaturityDate, 
                                        vector<Date> dates, vector<double> insuredAmounts,
                                        boost::shared_ptr<DiscountRateSource> standbyDRS, 
                                        boost::shared_ptr<DiscountRateSource> utilisationDRS);

        virtual double getSumInsured(Date date);
        virtual bool isOK();

    private:

        vector<Date> dates;
        vector<double> insuredAmounts;
    };


}

#endif

