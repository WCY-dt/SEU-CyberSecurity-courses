/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     bankserver.cpp                                             *
 * @brief    Bankserver is a layer upon bankdb and beneath ui layer.    *
 *           it is designed for muti-users. however,                    *
 *           i do not have enough time complete it.                     *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "bankserver.h"
#include "bankdb.h"
#include "economiccalc.h"
#include <fstream>
#include <QDebug>

/** 
 * @brief initialization
 */
bankServer::bankServer()
{
}

/** 
 * @brief judge whether the account exists
 * 
 * @param [in] strNum number of the account
 * 
 * @retval true  finded
 *         false not finded
 */
bool bankServer::AccountExist(string strNumber)
{
    return (db.ExistAccount(strNumber));
}

/** 
 * @brief add an account
 * 
 * @param [in] strNum number of the account
 * @param [in] strNam name of the account owner
 * @param [in] strPas password of the account
 * @param [in] strAdd address of the account owner
 * @param [in] iType type of the account
 * @param [in] dInt interest of the account
 * @param [in] strOpe operator of the deal
 * 
 * @retval true  input legal
 *         false illegal
 * 
 * requirments of these params @see bankdb.h
 */
bool bankServer::AccountAdd(string strNum, string strNam, string strPas, string strAdd, int iTyp, double dInt, string strOpe)
{
    if (strNam != "" &&
        strPas != "" &&
        strAdd != "" &&
        strOpe != "")
    {
        db.AddAccount(strNum, strNam, strPas, strAdd, iTyp, dInt, strOpe);
        return true;
    }
    return false;
}

/** 
 * @brief edit an account
 * 
 * @param [in] strNam name of the account owner
 * @param [in] strAdd address of the account owner
 * @param [in] iType type of the account
 * @param [in] dInt interest of the account
 * 
 * @retval true  account exist
 *         false not exist
 * 
 * requirments of these params @see bankdb.h
 */
bool bankServer::AccountEdit(string strNam, string strAdd, int iTyp, double dInt)
{
    if (strNam != "" && strAdd != "")
    {
        db.EditAccount(strCurrentNumber, strNam, strAdd, iTyp, dInt);
        return true;
    }
    return false;
}

/** 
 * @brief edit password
 * 
 * @param [in] strPas password of the account
 * 
 * @retval true  account exist
 *         false not exist
 * 
 * requirments of these params @see bankdb.h
 */
bool bankServer::PasswdEdit(string strOldPasswd, string strPasswd)
{
    if (!db.CheckAccount(strCurrentNumber, strOldPasswd))
    {
        return false;
    }
    db.EditPasswd(strCurrentNumber, strPasswd);
    return true;
}

/** 
 * @brief judge whether the account number matches the password
 * 
 * @param [in] strNumber number of the account
 * @param [in] strPasswd password of the account
 * 
 * @retval true  match
 *         false not match
 */
bool bankServer::Login(string strNumber, string strPasswd)
{
    if (!db.CheckAccount(strNumber, strPasswd))
        return false;

    strCurrentNumber = strNumber; /// set the account which has login
    return true;
}

/** 
 * @brief marked as lost
 * 
 * @param [in] tTim time of the operation
 * 
 * requirments of these params @see bankdb.h
 */
void bankServer::ReportLost(time_t tTime)
{
    db.SetLost(strCurrentNumber, tTime);
}

void bankServer::RidLost()
{
    db.RidLost(strCurrentNumber);
}

/** 
 * @brief deposit money
 * 
 * @param [in] dMon money to deposit
 * @param [in] strOpe operator
 * 
 * format of params @see bandb.h
 */
void bankServer::Deposit(double dMoney, string strOpe)
{
    time_t tTime = time(NULL);
    db.DepositMoney(strCurrentNumber, tTime, dMoney, strOpe);
}

/** 
 * @brief withdraw money
 * 
 * @param [in] dMon money to withdraw
 * @param [in] strOpe operator
 * 
 * @retval true  succeed
 *         false money wanted > money left
 * 
 * format of params @see bandb.h
 */
bool bankServer::Withdraw(double dMoney, string strOpe)
{
    time_t tTime = time(NULL);
    return db.WithdrawMoney(strCurrentNumber, tTime, dMoney, strOpe);
}

/** 
 * @brief get name
 * 
 * @return the name @see bandb.h
 */
string bankServer::GetName()
{
    return db.GetName(strCurrentNumber);
}

/** 
 * @brief get address
 * 
 * @return the address @see bandb.h
 */
string bankServer::GetAddress()
{
    return db.GetAddress(strCurrentNumber);
}

/** 
 * @brief get type
 * 
 * @return the type @see bandb.h
 */
int bankServer::GetType()
{
    return db.GetType(strCurrentNumber);
}

/** 
 * @brief get interest
 * 
 * @return the interest @see bandb.h
 */
double bankServer::GetInterest()
{
    return db.GetInterest(strCurrentNumber);
}

/** 
 * @brief get lost state
 * 
 * @return the lost state @see bandb.h
 */
bool bankServer::GetLost()
{
    return db.GetLost(strCurrentNumber);
}

/** 
 * @brief get lost time
 * 
 * @return the lost time @see bandb.h
 */
time_t bankServer::GetLostTime()
{
    return db.GetLostTime(strCurrentNumber);
}

/** 
 * @brief get operator
 * 
 * @return the operator @see bandb.h
 */
string bankServer::GetOperator()
{
    return db.GetOperator(strCurrentNumber);
}

/** 
 * @brief get number by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the number @see bandb.h
 */
string bankServer::GetNumber(int iNum)
{
    return db.GetNumber(iNum);
}

/** 
 * @brief get name by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the name @see bandb.h
 */
string bankServer::GetName(int iNum)
{
    return db.GetName(iNum);
}

/** 
 * @brief get address by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the address @see bandb.h
 */
string bankServer::GetAddress(int iNum)
{
    return db.GetAddress(iNum);
}

/** 
 * @brief get type by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the type @see bandb.h
 */
int bankServer::GetType(int iNum)
{
    return db.GetType(iNum);
}

/** 
 * @brief get interest by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the interest @see bandb.h
 */
double bankServer::GetInterest(int iNum)
{
    return db.GetInterest(iNum);
}

/** 
 * @brief get lost state by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the lost state @see bandb.h
 */
bool bankServer::GetLost(int iNum)
{
    return db.GetLost(iNum);
}

/** 
 * @brief get lost time by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the lost time @see bandb.h
 */
time_t bankServer::GetLostTime(int iNum)
{
    return db.GetLostTime(iNum);
}

/** 
 * @brief get operator by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the operator @see bandb.h
 */
string bankServer::GetOperator(int iNum)
{
    return db.GetOperator(iNum);
}

/** 
 * @brief get number of accounts
 * 
 * @return the number @see bandb.h
 */
int bankServer::GetNumberOfAccounts()
{
    return db.GetNumberOfAccounts();
}

/** 
 * @brief get number of flows by the account number
 * 
 * @return the number @see bandb.h
 */
int bankServer::GetNumberOfFlows()
{
    return db.GetNumberOfFlows(strCurrentNumber);
}

/** 
 * @brief get time by the index of the flow
 * 
 * @param [in] iFlow index in vFlow
 * 
 * @return the time @see bandb.h
 */
time_t bankServer::GetTime(int iFlow)
{
    return db.GetTime(strCurrentNumber, iFlow);
}

/** 
 * @brief get money by the index of the flow
 * 
 * @param [in] iFlow index in vFlow
 * 
 * @return the money @see bandb.h
 */
double bankServer::GetMoney(int iFlow)
{
    return db.GetMoney(strCurrentNumber, iFlow);
}

/** 
 * @brief get operation type by the index of the flow
 * 
 * @param [in] iFlow index in vFlow
 * 
 * @return the operation @see bandb.h
 */
int bankServer::GetOperationType(int iFlow)
{
    return db.GetOperationType(strCurrentNumber, iFlow);
}

/** 
 * @brief get operator by the index of the flow
 * 
 * @param [in] iFlow index in vFlow
 * 
 * @return the operator @see bandb.h
 */
string bankServer::GetFlowOperator(int iFlow)
{
    return db.GetOperator(strCurrentNumber, iFlow);
}


/** 
 * @brief get total money the account owns
 * 
 * @return the money @see bandb.h
 */
double bankServer::GetTotal()
{
    economiccalc Ecocalc;
    Ecocalc.SetType(GetType()); /// boot up a calculate service
    
    for (int i = 0; i < GetNumberOfFlows(); i++)
    {
        if (GetOperationType(i) == 0) /// deposit
            Ecocalc.Add(GetMoney(i), GetTime(i));
        else /// withdraw
            Ecocalc.Sub(GetMoney(i), GetTime(i));
    }

    return Ecocalc.Query(time(nullptr));
}

/** 
 * @brief calculate how much money needed for some day
 * 
 * @param [in] iDay date to be calculated
 * 
 * @return the money @see bandb.h
 */
double bankServer::GetPrepare(time_t iDay)
{
    double dAns = 0; /// store the ans

    time_t tTime1 = time(nullptr);
    time_t tTime2 = iDay;

    if (tTime1 >= tTime2) /// illegal time
        return 0;

    for (int k = 0; k < GetNumberOfAccounts(); k++) /// go over all the accounts
    {
        strCurrentNumber = GetNumber(k);

        economiccalc Ecocalc; /// boot up a calculate service
        
        Ecocalc.SetType(GetType());
        
        for (int i = 0; i < GetNumberOfFlows(); i++)
        {
            if (GetOperationType(i) == 0) /// deposit
                Ecocalc.Add(GetMoney(i), GetTime(i));
            else /// withdraw
                Ecocalc.Sub(GetMoney(i), GetTime(i));
        }

        dAns += (Ecocalc.QueryPrepare(tTime2));
    }

    return dAns;
}

/** 
 * @brief export the account list as html format
 * 
 * @param [out] strFileName outfile
 */
void bankServer::ExportAccountFile(string strFileName)
{
    ofstream oFile(strFileName, ios::trunc);

    oFile<<"<html>\n<body>\n<table border='1'>\n<tr>\n<th>账号</th>\n<th>姓名</th>\n<th>地址</th>\n<th>储种</th>\n"
         <<"<th>利息</th>\n<th>是否挂失</th>\n<th>操作员</th>\n</tr>\n";

    int iNumberOfAccounts = GetNumberOfAccounts();

    for (int i = 0; i < iNumberOfAccounts; i++)
    {
        int iTmpType = GetType(i);
        bool bLost = GetLost(i);

        string strTypeString; /// type to output
        string strInterestString; /// interest to output
        switch (iTmpType)
        {
            /// transform the format of type
        case 0:
            strTypeString = "一年期";
            strInterestString = "1.98";
            break;
        case 1:
            strTypeString = "三年期";
            strInterestString = "2.25";
            break;
        case 2:
            strTypeString = "五年期";
            strInterestString = "3.5";
            break;
        }

        string strLostString;
        if (bLost)
        {
            /// transform the format of date
            time_t tTime = GetLostTime(i);

            char szTime[100] = {'\0'};

            tm *pTm = new tm;
            localtime_s(pTm, &tTime);
            
            pTm->tm_year += 1900;
            pTm->tm_mon += 1;

            sprintf_s(szTime, "%04d年%02d月%02d日 %02d:%02d:%02d",
                      pTm->tm_year,
                      pTm->tm_mon,
                      pTm->tm_mday,
                      pTm->tm_hour,
                      pTm->tm_min,
                      pTm->tm_sec);

            strLostString = szTime;

            delete pTm;
            pTm = NULL;
        }
        else
        {
            strLostString = "否";
        }

        oFile<<"<tr>\n";
        oFile<<"<td>"<<GetNumber(i)<<"</td>\n";
        oFile<<"<td>"<<GetName(i)<<"</td>\n";
        oFile<<"<td>"<<GetAddress(i)<<"</td>\n";
        oFile<<"<td>"<<strTypeString<<"</td>\n";
        oFile<<"<td>"<<strInterestString<<"</td>\n";
        oFile<<"<td>"<<strLostString<<"</td>\n";
        oFile<<"<td>"<<GetOperator(i)<<"</td>\n";
        oFile<<"</tr>\n";
    }


    oFile<<"</table>\n</body>\n</html>\n";
}

/** 
 * @brief export the account list as html format
 * 
 * @param [out] strFileName outfile
 */
void bankServer::ExportFlowFile(string strFileName)
{
    ofstream oFile(strFileName, ios::trunc);

    oFile<<"<html>\n<body>\n<table border='1'>\n<tr>\n<th>操作时间</th>\n<th>操作类型</th>\n<th>金额</th>\n<th>操作员</th>\n</tr>\n";

    int iNumberOfFlows = GetNumberOfFlows();

    for (int i = 0; i < iNumberOfFlows; i++)
    {
        int iTmpType = GetOperationType(i);
        time_t tTime = GetTime(i);
        
        string strTypeString; /// type to output

        switch (iTmpType)
        {
            /// transform the format of type
        case 0:
            strTypeString = "存款";
            break;
        case 1:
            strTypeString = "取款";
            break;
        }
        
        /// transform the format of date
        string strTime;
        char szTime[100] = {'\0'};

        tm *pTm = new tm;
        localtime_s(pTm, &tTime);
        //pTm = localtime(&time_t_time);
        pTm->tm_year += 1900;
        pTm->tm_mon += 1;

        sprintf_s(szTime, "%04d年%02d月%02d日 %02d:%02d:%02d",
                  pTm->tm_year,
                  pTm->tm_mon,
                  pTm->tm_mday,
                  pTm->tm_hour,
                  pTm->tm_min,
                  pTm->tm_sec);

        strTime = szTime;
        //qDebug()<<"here";

        delete pTm;
        pTm = NULL;

        oFile<<"<tr>\n";
        oFile<<"<td>"<<strTime<<"</td>\n";
        oFile<<"<td>"<<strTypeString<<"</td>\n";
        oFile<<"<td>"<<GetMoney(i)<<"</td>\n";
        oFile<<"<td>"<<GetFlowOperator(i)<<"</td>\n";
        oFile<<"</tr>\n";
    }

    oFile<<"</table>\n</body>\n</html>\n";
}
