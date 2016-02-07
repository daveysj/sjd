#include "Register_All.h"

extern void registerVolatilityCurves(const XLOPER&);
extern void registerFuturesCurves(const XLOPER&);
extern void registerFixingCurves(const XLOPER&);
extern void registerDiscountCurves(const XLOPER&);
extern void registerRatesEnvironment(const XLOPER&);
extern void registerContracts(const XLOPER &xDll);
extern void registerContractsCashflow(const XLOPER &xDll);
extern void registerContractsForwardEuropean(const XLOPER &xDll);
extern void registerContractsForwardAsian(const XLOPER &xDll);
extern void registerContractsOptionEuropean(const XLOPER &xDll);
extern void registerContractsOptionAsian(const XLOPER &xDll);
extern void registerPortfolio(const XLOPER &xDll);
extern void registerPathGenerator(const XLOPER &xDll);
extern void registerMargin(const XLOPER &xDll);
extern void registerInsurance(const XLOPER &xDll);
extern void registerCVA(const XLOPER &xDll);
extern void registerPortfolioEvolver(const XLOPER &xDll);
extern void registerContractStressor(const XLOPER &xDll);


extern void unregisterVolatilityCurves(const XLOPER&);
extern void unregisterFuturesCurves(const XLOPER&);
extern void unregisterFixingCurves(const XLOPER&);
extern void unregisterDiscountCurves(const XLOPER&);
extern void unregisterRatesEnvironment(const XLOPER&);
extern void unregisterContracts(const XLOPER &xDll);
extern void unregisterContractsCashflow(const XLOPER &xDll);
extern void unregisterContractsForwardEuropean(const XLOPER &xDll);
extern void unregisterContractsForwardAsian(const XLOPER &xDll);
extern void unregisterContractsOptionEuropean(const XLOPER &xDll);
extern void unregisterContractsOptionAsian(const XLOPER &xDll);
extern void unregisterPortfolio(const XLOPER &xDll);
extern void unregisterPathGenerator(const XLOPER &xDll);
extern void unregisterMargin(const XLOPER &xDll);
extern void unregisterInsurance(const XLOPER &xDll);
extern void unregisterCVA(const XLOPER &xDll);
extern void unregisterPortfolioEvolver(const XLOPER &xDll);
extern void unregisterContractStressor(const XLOPER &xDll);

void registerSJDFunctions(const XLOPER &xDll)
{
    registerVolatilityCurves(xDll);
    registerFuturesCurves(xDll);
    registerFixingCurves(xDll);
    registerDiscountCurves(xDll);
    registerRatesEnvironment(xDll);
    registerContracts(xDll);
    registerContractsCashflow(xDll);
    registerContractsForwardEuropean(xDll);
    registerContractsForwardAsian(xDll);
    registerContractsOptionEuropean(xDll);
    registerContractsOptionAsian(xDll);
    registerPortfolio(xDll);
    registerPathGenerator(xDll);
    registerMargin(xDll);
    registerInsurance(xDll);
    registerCVA(xDll);
    registerPortfolioEvolver(xDll);
    registerContractStressor(xDll);
}

void unregisterSJDFunctions(const XLOPER &xDll)
{
    unregisterVolatilityCurves(xDll);
    unregisterFuturesCurves(xDll);
    unregisterFixingCurves(xDll);
    unregisterDiscountCurves(xDll);
    unregisterRatesEnvironment(xDll);
    unregisterContracts(xDll);
    unregisterContractsCashflow(xDll);
    unregisterContractsForwardEuropean(xDll);
    unregisterContractsForwardAsian(xDll);
    unregisterContractsOptionAsian(xDll);
    unregisterPortfolio(xDll);
    unregisterPathGenerator(xDll);
    unregisterMargin(xDll);
    unregisterInsurance(xDll);
    unregisterCVA(xDll);
    unregisterPortfolioEvolver(xDll);
    unregisterContractStressor(xDll);
}
