#include "PortfolioInsurance.h"


namespace sjdObjects 
{

    /*======================================================================================
    PortfolioInsurance

    =======================================================================================*
   bool PortfolioInsurance::isOK()
    {
        return libraryObject_->isOK();
    }

   string PortfolioInsurance::getErrorMessage()
    {
        return libraryObject_->errorTracking->getErrorMessagesAsString();
    }

    double PortfolioInsurance::getSumInsured(Date date)
    {
        return libraryObject_->getSumInsured(date);
    }

    vector<double> PortfolioInsurance::getInsurancePayments(vector<Date> dates, vector<double> mtm)
    {
      vector<double> insurance = vector<double>(dates.size(), 0);
        libraryObject_->getInsurancePayments(dates, mtm, insurance);
      return insurance;
    }



    /*======================================================================================
    SimpleInsuranceWithTermStructure

    =======================================================================================*
    SimpleInsuranceWithTermStructure::SimpleInsuranceWithTermStructure(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                              Date policyStartDate, Date policyMaturityDate, 
                                        vector<Date> dates, vector<double> insuredAmounts,
                                        boost::shared_ptr<DiscountRateSource> standbyDRS, 
                                        boost::shared_ptr<DiscountRateSource> utilisationDRS,
                              bool permanent) : PortfolioInsurance(properties, permanent)
    {
      boost::shared_ptr<sjd::DiscountRateSource> sbDRS, utlDRS;
      standbyDRS->getLibraryObject(sbDRS);
      utilisationDRS->getLibraryObject(utlDRS);
        libraryObject_ = boost::shared_ptr<sjd::PortfolioInsurance>(new 
         sjd::SimpleInsuranceWithTermStructure(policyStartDate, policyMaturityDate, dates, insuredAmounts,
                                       sbDRS, utlDRS));
    }

    */
}