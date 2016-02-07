#ifndef CVA_INCLUDED
#define CVA_INCLUDED
//#pragma once

#include <Tools\Tools.h>
#include <math.h>
#include <string>
#include <ql\time\date.hpp>
#include <sjd\maths\maths.h>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace QuantLib;

/*======================================================================================
Overview

Probability of Default and Loss Given Default classes
=======================================================================================*/

namespace sjd {
   /*======================================================================================
   CVA

    =======================================================================================*/
    class CVA {
    public:
        virtual ~CVA() {};
       
      virtual bool isOK();
      vector<string> getErrorMessage() const;

      virtual double getLGD(Date d) = 0;
      // return 0 if t2 <= t1. 
      virtual double getPD(Date d) = 0;
      virtual double getPD(Date t1, Date t2);
      virtual bool extendsTo(Date date) = 0;
        virtual void setAnchorDate(Date date);

        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

    protected:        
        Date anchorDate;
        string className;
    };


    /*======================================================================================
   Flat PD

   Class for CVA assuming that the conditional PD each year is a constant number. Here we
    model default as a Poisson Process with the PD for a future period T defined by
        F(T) = 1 - exp(-hT)
    where h defines the hazard rate of default (or the conditional PD in an infinitesimal
    time period)
    =======================================================================================*/
    class FlatPD : public CVA
    {
    public:
        // PD is the conditional annual risk of default
        FlatPD(Date anchorDate, double pd, double lgd);
        virtual ~FlatPD() {};

      bool isOK();

      virtual void setLGD(double lgdInput);
         virtual double getLGD(Date d);

      virtual void setPD(double pd);
      virtual double getPD(Date d);
      virtual bool extendsTo(Date date);

    private:
      double pd, lgd, hazardRate;
   };

    /*======================================================================================
   CumulativeDefaultProbability

   Class for CVA assuming that the cumulative default probability is input
    =======================================================================================*/
    class CumulativeDefaultProbability : public CVA
    {
    public:
        // PD is the conditional annual risk of default
        CumulativeDefaultProbability(Date anchorDate, vector<Date> dates, vector<double> cdp, vector<double> lgd);
        virtual ~CumulativeDefaultProbability() {};

      bool isOK();
      virtual double getPD(Date d);
      virtual double getLGD(Date d);

      virtual bool extendsTo(Date date);

    private:
        Date anchorDate;
        vector<Date> observationDates;        
        vector<double> cumulativeDefaultProbability;
        vector<double> lossGivenDefault;
        boost::shared_ptr<ArrayInterpolator> defaultInterpolator;
        boost::shared_ptr<ArrayInterpolator> lossInterpolator;
   };
}

#endif

