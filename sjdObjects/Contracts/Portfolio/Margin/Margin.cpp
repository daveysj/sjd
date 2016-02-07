#include "Margin.h"


namespace sjdObjects 
{

    /*======================================================================================
    Margin

    =======================================================================================*/
    bool Margin::isOK()
    {
        return libraryObject_->isOK();
    }

    string Margin::getErrorMessage()
    {
        return libraryObject_->errorTracking->getErrorMessagesAsString();
    }

   void Margin::setMarginBalance(double balance)
    {
        return libraryObject_->setMarginBalance(balance);
    }

    double Margin::getMarginBalance()
    {
        return libraryObject_->getMarginBalance();
    }

    double Margin::getMarginCall(double mtm)
    {
        return libraryObject_->getMarginCall(mtm);
    }

    /*======================================================================================
    SimpleMargin 

    =======================================================================================*/
    SimpleMargin::SimpleMargin(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                               Date anchorDate,  
                               bool useMyThreshold, 
                               double myThreshold,
                               bool useCptyThreshold, 
                               double cptyThreshold,
                               double mta, 
                               double rounding,
                               bool permanent) : Margin(properties, permanent)
    {
        libraryObject_ = boost::shared_ptr<sjd::Margin>(new sjd::SimpleMargin(anchorDate, 
                                                                              useMyThreshold, 
                                                                              myThreshold, 
                                                                              useCptyThreshold, 
                                                                              cptyThreshold, 
                                                                              mta, 
                                                                              rounding));
    }
}