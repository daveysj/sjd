#include <xlsdk/xlsdkdefines.hpp>


void registerPortfolio(const XLOPER &xDll) 
{
        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x19""getPortfolioMarginBalance"),
            TempStrNoSize("\x04""PCP#"),
            TempStrNoSize("\x19""getPortfolioMarginBalance"),
            TempStrNoSize("\x10""ObjectID,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2E""Returns the portfolio's margin account balance"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x7""Trigger"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getPortfolioMarginCall"),
            TempStrNoSize("\x05""PCCP#"),
            TempStrNoSize("\x16""getPortfolioMarginCall"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x23""Returns the portfolio's margin call"),
            TempStrNoSize("\x11""Rates environment"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x7""Trigger"));

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x0F""createPortfolio"),
            TempStrNoSize("\x07""CCPCPL#"),
            TempStrNoSize("\x0F""createPortfolio"),
            TempStrNoSize("\x2D""ObjectID,ContractIDs,Margin,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x45""Returns the ID of a portfolio which consists of a number of contracts"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x48""A vector containing the IDs of all the contracts making up the portfolio"),
            TempStrNoSize("\xF""Margin - if any"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x12""getPortfolioRegCap"),
            TempStrNoSize("\x5""PCCP#"),
            TempStrNoSize("\x12""getPortfolioRegCap"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x30""Returns the Regulatory Capital for the structure"),
            TempStrNoSize("\xF""ID of Portfolio"),
            TempStrNoSize("\x11""Rates environment"),
            TempStrNoSize("\x7""Trigger"));

        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x14""createPortfolioB2CEM"),
            TempStrNoSize("\x0D""CCPCBBPCPL#"),
            TempStrNoSize("\x14""createPortfolioB2CEM"),
            TempStrNoSize("\x45""ObjectID,ContractIDs,Factors,CPW,CC,optimise,margin,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),          
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x45""Returns the ID of a portfolio which consists of a number of contracts"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x48""A vector containing the IDs of all the contracts making up the portfolio"),
            TempStrNoSize("\x2A""Factor = {IR,FX,Equity,Precious,Commodity}"),
            TempStrNoSize("\x13""Counterparty Weight"),
            TempStrNoSize("\xE""Capital Charge"),
            TempStrNoSize("\x65""True = optimise the individual contracts into structures, False = treat input contracts as structures"),
            TempStrNoSize("\xF""Margin - if any"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));
/*
        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x14""createPortfolioB3CEM"),
            TempStrNoSize("\xD""CCPCBBCLPL#"),
            TempStrNoSize("\x14""createPortfolioB3CEM"),
            TempStrNoSize("\x3E""ObjectID,ContractIDs,Factors,CPW,CC,CR,optimise,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),          
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x45""Returns the ID of a portfolio which consists of a number of contracts"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x48""A vector containing the IDs of all the contracts making up the portfolio"),
            TempStrNoSize("\x2A""Factor = {IR,FX,Equity,Precious,Commodity}"),
            TempStrNoSize("\x13""Counterparty Weight"),
            TempStrNoSize("\xE""Capital Charge"),
            TempStrNoSize("\x23""Credit Rating (AAA,AA,A,BBB,BB,B,C)"),
            TempStrNoSize("\x65""True = optimise the individual contracts into structures, False = treat input contracts as structures"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));


        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x13""getSimDatesForDates"),
            TempStrNoSize("\x5""PCPP#"),
            TempStrNoSize("\x13""getSimDatesForDates"),
            TempStrNoSize("\x19""PortfolioID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x58""Returns the simulation dates (incl. dates withholding dates) for a set of exposure dates"),
            TempStrNoSize("\x13""ID of the Portfolio"),
         TempStrNoSize("\x26""Exposure Dates (excl. holding periods)"),
            TempStrNoSize("\x7""Trigger"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x1B""getPortfolioMarginAlongPath"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x1B""getPortfolioMarginAlongPath"),
            TempStrNoSize("\x1B""ObjectID,Dates,Path,Trigger"),
            TempStrNoSize("\x01""1"), 
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x65""Returns the margin balance of a portfolio for the given price path. NB SET THE PORTFOLIO MARGIN FIRST"),
            TempStrNoSize("\xF""ID of Portfolio"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\x1C""Spot rates for the valuation"),
            TempStrNoSize("\x7""Trigger"));

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x17""runPortfolioSimulations"),
            TempStrNoSize("\x7""PCCPIP#"),
            TempStrNoSize("\x17""runPortfolioSimulations"),
            TempStrNoSize("\x2B""PortfolioID,PathGenID,Dates,NumSims,Trigger"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x50""Runs a monte carlo simulation and collects statistics - nothing is returned here"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x34""ID of the Path Generator to use for the random paths"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\x15""Number of simulations"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x16""getPortfolioMCAnalysis"),
            TempStrNoSize("\xA""PCPCCLCCP#"),
            TempStrNoSize("\x16""getPortfolioMCAnalysis"),
            TempStrNoSize("\x45""PortfolioID,ValueDate,ZeroDRS,SpreadDRS,Incl Headings,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x6A""Returns summary analysis of the Portfolio (CVA, FVA etc) from the Monte Carlo Simulations of the portfolio"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x5F""Value date (this is necessary in case the value date is not one of the forward valuation dates)"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\xE""CVA - optional"),
            TempStrNoSize("\xE""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x18""getPortfolioMCStatistics"),
            TempStrNoSize("\x6""PCLLP#"),
            TempStrNoSize("\x18""getPortfolioMCStatistics"),
            TempStrNoSize("\x2B""PortfolioID,Incl Header, Incl Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x73""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) from a Monte Carlo Simulation of the portfolio"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\x33""Include the simulation dates in the returned values"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 20, &xDll,
            TempStrNoSize("\x18""getPortfolioPathAnalysis"),
            TempStrNoSize("\xC""PCPPPCCLCCP#"),
            TempStrNoSize("\x18""getPortfolioPathAnalysis"),
            TempStrNoSize("\x50""PortfolioID,ValueDate,Dates,Path,ZeroDRS,SpreadDRS,Incl Headings,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x4D""Returns summary analysis of the Portfolio (CVA, FVA etc) using the input path"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x5F""Value date (this is necessary in case the value date is not one of the forward valuation dates)"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\xF""Input spot path"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\xE""CVA - optional"),
            TempStrNoSize("\xE""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x1A""getPortfolioPathStatistics"),
            TempStrNoSize("\x8""PCPPLLP#"),
            TempStrNoSize("\x1A""getPortfolioPathStatistics"),
            TempStrNoSize("\x36""PortfolioID,Dates,Path,Incl Header, Incl Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x74""Returns the intermediate calculations (exposure, expected exposure, Margin, Reg Cap etc) for a path of the portfolio"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\xF""Input spot path"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\x33""Include the simulation dates in the returned values"),
            TempStrNoSize("\x8""Trigger"));


        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x1B""getPortfolioRegCapAlongPath"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x1B""getPortfolioRegCapAlongPath"),
            TempStrNoSize("\x1B""ObjectID,Dates,Path,Trigger"),
            TempStrNoSize("\x01""1"),                
            TempStrNoSize("\x9""prObjects"),         
            TempStrNoSize("\x00"""),                 
            TempStrNoSize("\x00"""),                 
            TempStrNoSize("\x69""Returns the regulatory capital of a porfolio for the given price path. NB SET THE PORTFOLIO REG CAP FIRST"),
            TempStrNoSize("\xF""ID of Portfolio"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\x1C""Spot rates for the valuation"),
            TempStrNoSize("\x7""Trigger"));
            */
}

void unregisterPortfolio(const XLOPER &xDll) 
{
    XLOPER xlRegID;
    // Unregister each function.  Due to a bug in Excel's C API this is a
    // two-step process.  Thanks to Laurent Longre for discovering the
    // workaround implemented here.

        Excel(xlfRegister, 0, 12, &xDll,
            TempStrNoSize("\x19""getPortfolioMarginBalance"),
            TempStrNoSize("\x04""PCP#"),
            TempStrNoSize("\x19""getPortfolioMarginBalance"),
            TempStrNoSize("\x10""ObjectID,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x2E""Returns the portfolio's margin account balance"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x19""getPortfolioMarginBalance"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x16""getPortfolioMarginCall"),
            TempStrNoSize("\x05""PCCP#"),
            TempStrNoSize("\x16""getPortfolioMarginCall"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x23""Returns the portfolio's margin call"),
            TempStrNoSize("\x11""Rates environment"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getPortfolioMarginCall"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x0F""createPortfolio"),
            TempStrNoSize("\x07""CCPCPL#"),
            TempStrNoSize("\x0F""createPortfolio"),
            TempStrNoSize("\x2D""ObjectID,ContractIDs,Margin,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x45""Returns the ID of a portfolio which consists of a number of contracts"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x48""A vector containing the IDs of all the contracts making up the portfolio"),
            TempStrNoSize("\xF""Margin - if any"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\xF""createPortfolio"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x12""getPortfolioRegCap"),
            TempStrNoSize("\x5""PCCP#"),
            TempStrNoSize("\x12""getPortfolioRegCap"),
            TempStrNoSize("\x16""ObjectID,Rates,Trigger"),
            TempStrNoSize("\x01""1"),                   // function type (0 = hidden function, 1 = worksheet function, 2 = command macro)
            TempStrNoSize("\x9""prObjects"),            // function category            
            TempStrNoSize("\x00"""),                    // shortcut text (command macros only)            
            TempStrNoSize("\x00"""),                    // path to help file
            TempStrNoSize("\x30""Returns the Regulatory Capital for the structure"),
            TempStrNoSize("\xF""ID of Portfolio"),
            TempStrNoSize("\x11""Rates environment"),
            TempStrNoSize("\x7""Trigger"));
        
        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x12""getPortfolioRegCap"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x14""createPortfolioB2CEM"),
            TempStrNoSize("\x0D""CCPCBBPCPL#"),
            TempStrNoSize("\x14""createPortfolioB2CEM"),
            TempStrNoSize("\x45""ObjectID,ContractIDs,Factors,CPW,CC,optimise,margin,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),          
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x45""Returns the ID of a portfolio which consists of a number of contracts"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x48""A vector containing the IDs of all the contracts making up the portfolio"),
            TempStrNoSize("\x2A""Factor = {IR,FX,Equity,Precious,Commodity}"),
            TempStrNoSize("\x13""Counterparty Weight"),
            TempStrNoSize("\xE""Capital Charge"),
            TempStrNoSize("\x65""True = optimise the individual contracts into structures, False = treat input contracts as structures"),
            TempStrNoSize("\xF""Margin - if any"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x14""createPortfolioB2CEM"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        /*
        Excel(xlfRegister, 0, 19, &xDll,
            TempStrNoSize("\x14""createPortfolioB3CEM"),
            TempStrNoSize("\xD""CCPCBBCLPL#"),
            TempStrNoSize("\x14""createPortfolioB3CEM"),
            TempStrNoSize("\x3E""ObjectID,ContractIDs,Factors,CPW,CC,CR,optimise,Trigger,Permanent"),
            TempStrNoSize("\x01""1"),          
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x00"""),           
            TempStrNoSize("\x45""Returns the ID of a portfolio which consists of a number of contracts"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x48""A vector containing the IDs of all the contracts making up the portfolio"),
            TempStrNoSize("\x2A""Factor = {IR,FX,Equity,Precious,Commodity}"),
            TempStrNoSize("\x13""Counterparty Weight"),
            TempStrNoSize("\xE""Capital Charge"),
            TempStrNoSize("\x23""Credit Rating (AAA,AA,A,BBB,BB,B,C)"),
            TempStrNoSize("\x65""True = optimise the individual contracts into structures, False = treat input contracts as structures"),
            TempStrNoSize("\x7""Trigger"),
            TempStrNoSize("\x1D""object permanent/nonpermanent"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x14""createPortfolioB3CEM"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 13, &xDll,
            TempStrNoSize("\x13""getSimDatesForDates"),
            TempStrNoSize("\x5""PCPP#"),
            TempStrNoSize("\x13""getSimDatesForDates"),
            TempStrNoSize("\x19""PortfolioID,Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x58""Returns the simulation dates (incl. dates withholding dates) for a set of exposure dates"),
            TempStrNoSize("\x13""ID of the Portfolio"),
         TempStrNoSize("\x26""Exposure Dates (excl. holding periods)"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x13""getSimDatesForDates"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x1B""getPortfolioMarginAlongPath"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x1B""getPortfolioMarginAlongPath"),
            TempStrNoSize("\x1B""ObjectID,Dates,Path,Trigger"),
            TempStrNoSize("\x01""1"), 
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x00"""),        
            TempStrNoSize("\x65""Returns the margin balance of a portfolio for the given price path. NB SET THE PORTFOLIO MARGIN FIRST"),
            TempStrNoSize("\xF""ID of Portfolio"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\x1C""Spot rates for the valuation"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1B""getPortfolioMarginAlongPath"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 15, &xDll,
            TempStrNoSize("\x17""runPortfolioSimulations"),
            TempStrNoSize("\x7""PCCPIP#"),
            TempStrNoSize("\x17""runPortfolioSimulations"),
            TempStrNoSize("\x2B""PortfolioID,PathGenID,Dates,NumSims,Trigger"),
            TempStrNoSize("\x01""1"),
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x00"""), 
            TempStrNoSize("\x50""Runs a monte carlo simulation and collects statistics - nothing is returned here"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x34""ID of the Path Generator to use for the random paths"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\x15""Number of simulations"),
            TempStrNoSize("\x8""Trigger"));
            
        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x17""runPortfolioSimulations"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 18, &xDll,
            TempStrNoSize("\x16""getPortfolioMCAnalysis"),
            TempStrNoSize("\xA""PCPCCLCCP#"),
            TempStrNoSize("\x16""getPortfolioMCAnalysis"),
            TempStrNoSize("\x45""PortfolioID,ValueDate,ZeroDRS,SpreadDRS,Incl Headings,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x6A""Returns summary analysis of the Portfolio (CVA, FVA etc) from the Monte Carlo Simulations of the portfolio"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x5F""Value date (this is necessary in case the value date is not one of the forward valuation dates)"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\xE""CVA - optional"),
            TempStrNoSize("\xE""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x16""getPortfolioMCAnalysis"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x18""getPortfolioMCStatistics"),
            TempStrNoSize("\x6""PCLLP#"),
            TempStrNoSize("\x18""getPortfolioMCStatistics"),
            TempStrNoSize("\x2B""PortfolioID,Incl Header, Incl Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x73""Returns the intermediate calculations (EE, EPE, Margin, Reg Cap etc) from a Monte Carlo Simulation of the portfolio"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\x33""Include the simulation dates in the returned values"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x18""getPortfolioMCStatistics"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 20, &xDll,
            TempStrNoSize("\x18""getPortfolioPathAnalysis"),
            TempStrNoSize("\xC""PCPPPCCLCCP#"),
            TempStrNoSize("\x18""getPortfolioPathAnalysis"),
            TempStrNoSize("\x50""PortfolioID,ValueDate,Dates,Path,ZeroDRS,SpreadDRS,Incl Headings,CVA,DVA,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x4D""Returns summary analysis of the Portfolio (CVA, FVA etc) using the input path"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x5F""Value date (this is necessary in case the value date is not one of the forward valuation dates)"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\xF""Input spot path"),
            TempStrNoSize("\x21""Risk free discounting rate source"),
            TempStrNoSize("\x1D""Risky discounting rate source"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\xE""CVA - optional"),
            TempStrNoSize("\xE""DVA - optional"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x18""getPortfolioPathAnalysis"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 16, &xDll,
            TempStrNoSize("\x1A""getPortfolioPathStatistics"),
            TempStrNoSize("\x8""PCPPLLP#"),
            TempStrNoSize("\x1A""getPortfolioPathStatistics"),
            TempStrNoSize("\x36""PortfolioID,Dates,Path,Incl Header, Incl Dates,Trigger"),
            TempStrNoSize("\x01""1"),                   
            TempStrNoSize("\x9""prObjects"),
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x00"""),                    
            TempStrNoSize("\x74""Returns the intermediate calculations (exposure, expected exposure, Margin, Reg Cap etc) for a path of the portfolio"),
            TempStrNoSize("\x13""ID of the Portfolio"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\xF""Input spot path"),
            TempStrNoSize("\x15""Include a heading row"),
            TempStrNoSize("\x33""Include the simulation dates in the returned values"),
            TempStrNoSize("\x8""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1A""getPortfolioPathStatistics"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);

        Excel(xlfRegister, 0, 14, &xDll,
            TempStrNoSize("\x1B""getPortfolioRegCapAlongPath"),
            TempStrNoSize("\x6""PCPPP#"),
            TempStrNoSize("\x1B""getPortfolioRegCapAlongPath"),
            TempStrNoSize("\x1B""ObjectID,Dates,Path,Trigger"),
            TempStrNoSize("\x01""1"),                
            TempStrNoSize("\x9""prObjects"),         
            TempStrNoSize("\x00"""),                 
            TempStrNoSize("\x00"""),                 
            TempStrNoSize("\x69""Returns the regulatory capital of a porfolio for the given price path. NB SET THE PORTFOLIO REG CAP FIRST"),
            TempStrNoSize("\xF""ID of Portfolio"),
            TempStrNoSize("\x17""Forward valuation dates"),
            TempStrNoSize("\x1C""Spot rates for the valuation"),
            TempStrNoSize("\x7""Trigger"));

        Excel4(xlfRegisterId, &xlRegID, 2, &xDll,
            TempStrNoSize("\x1B""getPortfolioRegCapAlongPath"));
        Excel4(xlfUnregister, 0, 1, &xlRegID);
        */
}
