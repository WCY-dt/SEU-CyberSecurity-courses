/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     economicclac.cpp                                           *
 * @brief    Economic calc is used as auxiliary for database and server.*
 *           it can calculate the interest.                             *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "economiccalc.h"
#include <QDebug>

/** 
 * @brief initialization
 */
economiccalc::economiccalc()
{
    dMoney = 0;
    vFlow.clear();
}

/** 
 * @brief set type
 * 
 * @param [in] iSetType type
 * 
 * @see bankdb.h
 */
void economiccalc::SetType(int iSetType)
{
    iType = iSetType;
}

/** 
 * @brief deposit
 * 
 * @param [in] dMoney money to deposit
 * @param [in] tTime time
 */
void economiccalc::Add(double dMoney, time_t tTime)
{
    Flow tmpFlow;
    tmpFlow.tTime = tTime;
    tmpFlow.dMoney = dMoney;
    vFlow.push_back(tmpFlow);
}

/** 
 * @brief withdraw
 * 
 * @param [in] dMoney money to withdraw
 * @param [in] tTime time
 */
void economiccalc::Sub(double dMoney, time_t tTime)
{
    double dMoneyNeeded = dMoney;
    int iTmpPos = vFlow.size() - 1; /// pointer to the flow
    while (dMoneyNeeded > 0 && iTmpPos >= 0)
    {
        if (tTime < vFlow[iTmpPos].tTime) /// time no match
        /**
         * @note actually this will not happen.
         *       i write this just to debug.
         */
            iTmpPos--;
        else if (vFlow[iTmpPos].dMoney > dMoneyNeeded) /// enough money
        {
            vFlow[iTmpPos].dMoney -= dMoneyNeeded;
            dMoneyNeeded = 0;
            iTmpPos--;
        }
        else /// money not enough
        {
            dMoneyNeeded -= vFlow[iTmpPos].dMoney;
            vFlow[iTmpPos].dMoney = 0;
            iTmpPos--;
        }
    }
}

/** 
 * @brief calculate the interest
 * 
 * @param [in] tTime time
 * 
 * @return money in the account till the time
 */
double economiccalc::Query(time_t tTime)
{
    double dAns = 0; /// ans for the query
    int iTmpTime; /// relate to type
    double dInterest; /// store the interest
    switch (iType)
    {
    case 0: /// 1 year
        iTmpTime = 31536000;
        dInterest = 1.98;
        break;
    case 1: /// 3 year
        iTmpTime = 94608000;
        dInterest = 2.25;
        break;
    case 2: /// 5 year
        iTmpTime = 157680000;
        dInterest = 3.5;
        break;
    }

    for (int i = 0; i < (int)vFlow.size(); i++)
    {
        /**
         * @note it can calculate yield compound interest
         */
        int iYieldInterestYear = ((int)difftime(tTime, vFlow[i].tTime)) / iTmpTime;
        double dYieldInterest = vFlow[i].dMoney;
        for (int j = 1; j <= iYieldInterestYear; j++)
            dYieldInterest = dYieldInterest * (1 + (dInterest / 100));
        dAns += dYieldInterest;
    }
    return dAns;
}

/** 
 * @brief calculate the money to prepare
 * 
 * @param [in] tTime2 time
 * 
 * @return money to prepare till the time
 */
double economiccalc::QueryPrepare(time_t tTime2)
{
    time_t tTime1 = time(nullptr);
    double dAns = 0; /// ans for the query
    int iTmpTime; /// relate to type
    double dInterest; /// store the interest
    switch (iType)
    {
    case 0: /// 1 year
        iTmpTime = 31536000;
        dInterest = 1.98;
        break;
    case 1: /// 3 year
        iTmpTime = 94608000;
        dInterest = 2.25;
        break;
    case 2: /// 5 year
        iTmpTime = 157680000;
        dInterest = 3.5;
        break;
    }

    for (int i = 0; i < (int)vFlow.size(); i++)
    {
        int iDifftime1 = (int)difftime(tTime1, vFlow[i].tTime);
        int iDifftime2 = (int)difftime(tTime2, vFlow[i].tTime);
        if (((iDifftime1 / iTmpTime) + 1) * iTmpTime <= iDifftime2)
        {
            /**
             * @note the judgement above means the money matures
             */
            int iYieldInterestYear = iDifftime2 / iTmpTime;
            double dYieldInterest = vFlow[i].dMoney;
            for (int j = 1; j <= iYieldInterestYear; j++)
                dYieldInterest = dYieldInterest * (1 + (dInterest / 100));
            dAns += dYieldInterest;
        }
    }
    return dAns;
}
