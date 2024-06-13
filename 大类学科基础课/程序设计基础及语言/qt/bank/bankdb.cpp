/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     bankdb.cpp                                                 *
 * @brief    Bankdb simulates a database for the project. The speed of  *
 *           this database is slow for that we must ensure the safety   *
 *           of every deal.                                             *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "bankdb.h"
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <fstream>
#include <map>
#include <time.h>

using namespace std;

/** 
 * @brief initialization and read from the file
 */
bankdb::bankdb()
{
    GetFile();
}

/** 
 * @brief read from the file which stores the data
 */
void bankdb::GetFile()
{
    vAccount.clear();
    mAccount.clear();
    mMoney.clear();

    int iNumOfAccounts; /// number of accounts
    int iNumOfFlow;     /// number of flows

    ifstream iFile("bankdb.db", ios::binary);
    if (!iFile) /// the file not exist
    {
        iNumOfAccounts = 0; /// zero account
        WriteFile();
        return;
    }

    iFile >> iNumOfAccounts;

    for (int i = 0; i < iNumOfAccounts; i++)
    {
        accountInfo tmpAccount; /// get a record, and pushback to vector
        string strTmpString;    /// for the convenience of infile

        /**
         * @note here i use a lot of getline(), for that spaces may exist in the strings
         *       getline(iFile, strTmpString) is for jump over ENTER
         */
        iFile >> tmpAccount.strNumber;
        getline(iFile, strTmpString);
        getline(iFile, tmpAccount.strName);
        getline(iFile, tmpAccount.strPasswd);
        getline(iFile, tmpAccount.strAddress);
        iFile >> tmpAccount.iType >> tmpAccount.dInterest >> tmpAccount.bLost >> tmpAccount.tLostTime;
        getline(iFile, strTmpString);
        getline(iFile, tmpAccount.strOperator);
        iFile >> iNumOfFlow;

        mMoney[tmpAccount.strNumber] = 0; /// initialize the money in the account

        for (int j = 0; j < iNumOfFlow; j++)
        {
            flowInfo tmpFlow; /// get a flow, and pushback to vector

            iFile >> tmpFlow.tTime >> tmpFlow.dMoney >> tmpFlow.iOperationType;
            getline(iFile, strTmpString);
            getline(iFile, tmpFlow.strOperator);

            if (tmpFlow.iOperationType == 0) /// deposit
                mMoney[tmpAccount.strNumber] += tmpFlow.dMoney;
            else /// withdraw
                mMoney[tmpAccount.strNumber] -= tmpFlow.dMoney;
            tmpAccount.vFlow.push_back(tmpFlow);
        }

        mAccount[tmpAccount.strNumber] = vAccount.size(); /// map account number to the index
                                                          /// @see bankdb.h
        vAccount.push_back(tmpAccount);
    }
}

/** 
 * @brief write data to the file
 */
void bankdb::WriteFile()
{
    int iNumOfAccounts = vAccount.size();

    /**
     * @note use trunc for that it is not much slower than find where to insert or edit
     */
    ofstream oFile("bankdb.db", ios::trunc | ios::binary);

    oFile << iNumOfAccounts << "\n";

    for (int i = 0; i < iNumOfAccounts; i++)
    {
        accountInfo tmpAccount = vAccount[i];

        int iNumOfFlow = tmpAccount.vFlow.size();

        oFile << tmpAccount.strNumber << "\n"
              << tmpAccount.strName << "\n"
              << tmpAccount.strPasswd << "\n"
              << tmpAccount.strAddress << "\n"
              << tmpAccount.iType << "\n"
              << tmpAccount.dInterest << "\n"
              << tmpAccount.bLost << "\n"
              << tmpAccount.tLostTime << "\n"
              << tmpAccount.strOperator << "\n"
              << iNumOfFlow << "\n";

        for (int j = 0; j < iNumOfFlow; j++)
        {
            flowInfo tmpFlow = tmpAccount.vFlow[j];

            oFile << tmpFlow.tTime << "\n"
                  << tmpFlow.dMoney << "\n"
                  << tmpFlow.iOperationType << "\n"
                  << tmpFlow.strOperator << "\n";
        }
    }
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
 * requirments of these params @see bankdb.h
 */
void bankdb::AddAccount(string strNum, string strNam, string strPas, string strAdd, int iTyp, double dInt, string strOpe)
{
    accountInfo tmpAccountInfo;
    tmpAccountInfo.strNumber = strNum;
    tmpAccountInfo.strName = strNam;
    tmpAccountInfo.strPasswd = strPas;
    tmpAccountInfo.strAddress = strAdd;
    tmpAccountInfo.iType = iTyp;
    tmpAccountInfo.dInterest = dInt;
    tmpAccountInfo.bLost = false;
    tmpAccountInfo.tLostTime = time(nullptr);
    tmpAccountInfo.strOperator = strOpe;
    vAccount.push_back(tmpAccountInfo);
    vAccount[vAccount.size() - 1].vFlow.clear();

    /**
     * @note do not delete Getfile()!
     *       i do not think it is necessary, but without it the program has bugs
     *       it is to refresh the memory
     */
    WriteFile();
    GetFile();
}

/** 
 * @brief edit an account
 * 
 * @param [in] strNum number of the account [unchangable]
 * @param [in] strNam name of the account owner
 * @param [in] strAdd address of the account owner
 * @param [in] iType type of the account
 * @param [in] dInt interest of the account
 * 
 * requirments of these params @see bankdb.h
 */
void bankdb::EditAccount(string strNum, string strNam, string strAdd, int iTyp, double dInt)
{
    vAccount[mAccount[strNum]].strName = strNam;
    vAccount[mAccount[strNum]].strAddress = strAdd;
    vAccount[mAccount[strNum]].iType = iTyp;
    vAccount[mAccount[strNum]].dInterest = dInt;
    WriteFile();
    GetFile();
}

/** 
 * @brief edit password
 * 
 * @param [in] strNum number of the account [unchangable]
 * @param [in] strPas password of the account
 * 
 * requirments of these params @see bankdb.h
 */
void bankdb::EditPasswd(string strNum, string strPas)
{
    vAccount[mAccount[strNum]].strPasswd = strPas;
    WriteFile();
}

/** 
 * @brief marked as lost
 * 
 * @param [in] strNum number of the account [unchangable]
 * @param [in] tTim time of the operation
 * 
 * requirments of these params @see bankdb.h
 */
void bankdb::SetLost(string strNum, time_t tTim)
{
    vAccount[mAccount[strNum]].bLost = true;
    vAccount[mAccount[strNum]].tLostTime = tTim;
    WriteFile();
}

void bankdb::RidLost(string strNum)
{
    vAccount[mAccount[strNum]].bLost = false;
    vAccount[mAccount[strNum]].tLostTime = 0;
    WriteFile();
}

/** 
 * @brief judge whether the account exists
 * 
 * @param [in] strNum number of the account
 * 
 * @retval true  finded
 *         false not finded
 */
bool bankdb::ExistAccount(string strNumber)
{
    /**
     * @note a common way to find sth. in a map
     */
    map<string, int>::iterator iter;
    iter = mAccount.find(strNumber);
    return (iter != mAccount.end());
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
bool bankdb::CheckAccount(string strNumber, string strPasswd)
{
    if (!ExistAccount(strNumber)) /// the account does not exist
        return false;
    
    return (vAccount[mAccount[strNumber]].strPasswd == strPasswd);
}

/** 
 * @brief get name by the account number
 * 
 * @param [in] strNum number of the account
 * 
 * @return the name @see bandb.h
 */
string bankdb::GetName(string strNumber)
{
    return vAccount[mAccount[strNumber]].strName;
}

/** 
 * @brief get address by the account number
 * 
 * @param [in] strNum number of the account
 * 
 * @return the address @see bandb.h
 */
string bankdb::GetAddress(string strNumber)
{
    return vAccount[mAccount[strNumber]].strAddress;
}

/** 
 * @brief get type by the account number
 * 
 * @param [in] strNum number of the account
 * 
 * @return the type @see bandb.h
 */
int bankdb::GetType(string strNumber)
{
    return vAccount[mAccount[strNumber]].iType;
}

/** 
 * @brief get interest by the account number
 * 
 * @param [in] strNum number of the account
 * 
 * @return the interest @see bandb.h
 */
double bankdb::GetInterest(string strNumber)
{
    return vAccount[mAccount[strNumber]].dInterest;
}

/** 
 * @brief get lost state by the account number
 * 
 * @param [in] strNum number of the account
 * 
 * @return the lost state @see bandb.h
 */
bool bankdb::GetLost(string strNumber)
{
    return vAccount[mAccount[strNumber]].bLost;
}

/** 
 * @brief get lost time by the account number
 * 
 * @param [in] strNum number of the account
 * 
 * @return the lost time @see bandb.h
 */
time_t bankdb::GetLostTime(string strNumber)
{
    return vAccount[mAccount[strNumber]].tLostTime;
}

/** 
 * @brief get operator by the account number
 * 
 * @param [in] strNum number of the account
 * 
 * @return the operator @see bandb.h
 */
string bankdb::GetOperator(string strNumber)
{
    return vAccount[mAccount[strNumber]].strOperator;
}

/** 
 * @brief get number by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the number @see bandb.h
 */
string bankdb::GetNumber(int iNum)
{
    return vAccount[iNum].strNumber;
}

/** 
 * @brief get name by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the name @see bandb.h
 */
string bankdb::GetName(int iNum)
{
    return vAccount[iNum].strName;
}

/** 
 * @brief get address by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the address @see bandb.h
 */
string bankdb::GetAddress(int iNum)
{
    return vAccount[iNum].strAddress;
}

/** 
 * @brief get type by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the type @see bandb.h
 */
int bankdb::GetType(int iNum)
{
    return vAccount[iNum].iType;
}

/** 
 * @brief get interest by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the interest @see bandb.h
 */
double bankdb::GetInterest(int iNum)
{
    return vAccount[iNum].dInterest;
}

/** 
 * @brief get lost state by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the lost state @see bandb.h
 */
bool bankdb::GetLost(int iNum)
{
    return vAccount[iNum].bLost;
}

/** 
 * @brief get lost time by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the lost time @see bandb.h
 */
time_t bankdb::GetLostTime(int iNum)
{
    return vAccount[iNum].tLostTime;
}

/** 
 * @brief get operator by the index
 * 
 * @param [in] iNum index in vAccount
 * 
 * @return the operator @see bandb.h
 */
string bankdb::GetOperator(int iNum)
{
    return vAccount[iNum].strOperator;
}

/** 
 * @brief get number of accounts
 * 
 * @return the number @see bandb.h
 */
int bankdb::GetNumberOfAccounts()
{
    return vAccount.size();
}

/** 
 * @brief get number of flows by the account number
 * 
 * @param [in] strNumber number of the accountt
 * 
 * @return the number @see bandb.h
 */
int bankdb::GetNumberOfFlows(string strNumber)
{
    return vAccount[mAccount[strNumber]].vFlow.size();
}

/** 
 * @brief get time by the number of the account and index of the flow
 * 
 * @param [in] strNumber number of the accountt
 * @param [in] iFlow index in vFlow
 * 
 * @return the time @see bandb.h
 */
time_t bankdb::GetTime(string strNumber, int iFlow)
{
    return vAccount[mAccount[strNumber]].vFlow[iFlow].tTime;
}

/** 
 * @brief get money by the number of the account and index of the flow
 * 
 * @param [in] strNumber number of the accountt
 * @param [in] iFlow index in vFlow
 * 
 * @return the money @see bandb.h
 */
double bankdb::GetMoney(string strNumber, int iFlow)
{
    return vAccount[mAccount[strNumber]].vFlow[iFlow].dMoney;
}

/** 
 * @brief get operation type by the number of the account and index of the flow
 * 
 * @param [in] strNumber number of the accountt
 * @param [in] iFlow index in vFlow
 * 
 * @return the operation @see bandb.h
 */
int bankdb::GetOperationType(string strNumber, int iFlow)
{
    return vAccount[mAccount[strNumber]].vFlow[iFlow].iOperationType;
}

/** 
 * @brief get operator by the number of the account and index of the flow
 * 
 * @param [in] strNumber number of the accountt
 * @param [in] iFlow index in vFlow
 * 
 * @return the operator @see bandb.h
 */
string bankdb::GetOperator(string strNumber, int iFlow)
{
    return vAccount[mAccount[strNumber]].vFlow[iFlow].strOperator;
}

/** 
 * @brief deposit money
 * 
 * @param [in] strNumber number of the accountt
 * @param [in] tTime time of the operation
 * @param [in] dMon money to deposit
 * @param [in] strOpe operator
 * 
 * format of params @see bandb.h
 */
void bankdb::DepositMoney(string strNumber, time_t tTim, double dMon, string strOpe)
{
    flowInfo tmpFlowInfo;
    tmpFlowInfo.tTime = tTim;
    tmpFlowInfo.dMoney = dMon;
    tmpFlowInfo.iOperationType = 0;
    tmpFlowInfo.strOperator = strOpe;
    vAccount[mAccount[strNumber]].vFlow.push_back(tmpFlowInfo);
    mMoney[strNumber] += dMon;
    WriteFile();
    GetFile();
}

/** 
 * @brief withdraw money
 * 
 * @param [in] strNumber number of the accountt
 * @param [in] tTime time of the operation
 * @param [in] dMon money to withdraw
 * @param [in] strOpe operator
 * 
 * @retval true  succeed
 *         false money wanted > money left
 * 
 * format of params @see bandb.h
 */
bool bankdb::WithdrawMoney(string strNumber, time_t tTim, double dMon, string strOpe)
{
    if (dMon > mMoney[strNumber])
    {
        return false;
    }
    flowInfo tmpFlowInfo;
    tmpFlowInfo.tTime = tTim;
    tmpFlowInfo.dMoney = dMon;
    tmpFlowInfo.iOperationType = 1;
    tmpFlowInfo.strOperator = strOpe;
    vAccount[mAccount[strNumber]].vFlow.push_back(tmpFlowInfo);
    mMoney[strNumber] -= dMon;
    WriteFile();
    GetFile();
    return true;
}
