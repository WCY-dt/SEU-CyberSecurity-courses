/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     flowlist.cpp                                               *
 * @brief    page for showing flow list                                 *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "flowlist.h"
#include "bank.h"
#include "bankserver.h"
#include "options.h"
#include "ui_flowlist.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <stdio.h>

extern bankServer bankserver;

flowlist::flowlist(QWidget *parent) : QDialog(parent),
                                      ui(new Ui::flowlist)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    SetDefault();
    ui->label_4->setText(QString::number(bankserver.GetTotal()));

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

flowlist::~flowlist()
{
    delete ui;
}

void flowlist::on_accountlist_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}

void flowlist::on_accountlist_searchEdit_textChanged(const QString &arg1)
{
    string strSearchString = arg1.toStdString();
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->accountlist_tableView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("操作时间"));
    model->setHorizontalHeaderItem(1, new QStandardItem("操作类型"));
    model->setHorizontalHeaderItem(2, new QStandardItem("金额"));
    model->setHorizontalHeaderItem(3, new QStandardItem("操作员"));

    ui->accountlist_tableView->setColumnWidth(0, 200);
    ui->accountlist_tableView->setColumnWidth(1, 150);
    ui->accountlist_tableView->setColumnWidth(2, 200);
    ui->accountlist_tableView->setColumnWidth(3, 80);

    //ui->accountlist_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->accountlist_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->accountlist_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    int iNumberOfFlows = bankserver.GetNumberOfFlows();

    int iItemsFinded = -1;
    for (int i = 0; i < iNumberOfFlows; i++)
    {
        int iTmpType = bankserver.GetOperationType(i);
        time_t tTime = bankserver.GetTime(i);

        string strTypeString;
        switch (iTmpType)
        {
        case 0:
            strTypeString = "存款";
            break;
        case 1:
            strTypeString = "取款";
            break;
        }

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

        delete pTm;
        pTm = NULL;

        if (bankserver.GetMoney(i) == arg1.toDouble() ||
            strTypeString.find(strSearchString) != string::npos ||
            strTime.find(strSearchString) != string::npos ||
            bankserver.GetOperator(i).find(strSearchString) != string::npos)
        {
            iItemsFinded++;
            model->setItem(iItemsFinded, 0, new QStandardItem(QString::fromStdString(strTime)));
            model->setItem(iItemsFinded, 1, new QStandardItem(QString::fromStdString(strTypeString)));
            model->setItem(iItemsFinded, 2, new QStandardItem(QString::number(bankserver.GetMoney(i))));
            model->setItem(iItemsFinded, 3, new QStandardItem(QString::fromStdString(bankserver.GetFlowOperator(i))));
        }
    }
    ui->label_2->setNum(iItemsFinded + 1);
}

void flowlist::SetDefault()
{
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->accountlist_tableView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("操作时间"));
    model->setHorizontalHeaderItem(1, new QStandardItem("操作类型"));
    model->setHorizontalHeaderItem(2, new QStandardItem("金额"));
    model->setHorizontalHeaderItem(3, new QStandardItem("操作员"));

    ui->accountlist_tableView->setColumnWidth(0, 200);
    ui->accountlist_tableView->setColumnWidth(1, 150);
    ui->accountlist_tableView->setColumnWidth(2, 200);
    ui->accountlist_tableView->setColumnWidth(3, 80);

    //ui->accountlist_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->accountlist_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->accountlist_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    int iNumberOfFlows = bankserver.GetNumberOfFlows();

    for (int i = 0; i < iNumberOfFlows; i++)
    {
        int iTmpType = bankserver.GetOperationType(i);
        time_t tTime = bankserver.GetTime(i);
        //qDebug()<<("Type=")<<(iTmpType);
        string strTypeString;
        switch (iTmpType)
        {
        case 0:
            strTypeString = "存款";
            break;
        case 1:
            strTypeString = "取款";
            break;
        }
        //qDebug()<<("String=")<<(QString::fromStdString(strTypeString));

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

        model->setItem(i, 0, new QStandardItem(QString::fromStdString(strTime)));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(strTypeString)));
        model->setItem(i, 2, new QStandardItem(QString::number(bankserver.GetMoney(i))));
        model->setItem(i, 3, new QStandardItem(QString::fromStdString(bankserver.GetFlowOperator(i))));
    }
    ui->label_2->setNum(iNumberOfFlows);
}

void flowlist::on_accountlist_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("导出"),
                                                    "",
                                                    tr("超文本标记语言文件 (*.html)"));
    if (!fileName.isNull())
    {
        //fileName是文件名
        bankserver.ExportFlowFile(fileName.toStdString());
        QMessageBox::information(this, NULL,
                                 tr("导出成功"),
                                 tr("确认"));
    }
}
