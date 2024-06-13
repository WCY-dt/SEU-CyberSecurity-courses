/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     bank.cpp                                                   *
 * @brief    Homepage                                                   *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "bank.h"
#include "accountlist.h"
#include "bankserver.h"
#include "createaccount.h"
#include "login.h"
#include "ui_bank.h"
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QString>

extern bankServer bankserver;

bank::bank(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::bank)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

bank::~bank()
{
    delete ui;
}

void bank::on_loginButton_clicked()
{
    login *login_windows;
    login_windows = new login();
    login_windows->show();
    this->close();
}

void bank::on_createAccountButton_clicked()
{
    createaccount *createaccount_windows;
    createaccount_windows = new createaccount();
    createaccount_windows->show();
    this->close();
}

void bank::on_exitButton_clicked()
{
    switch (QMessageBox::critical(this, tr("警告"), tr("确认退出系统吗?"), tr("确认"), tr("取消"), 0, 1))
    {
    case 0:
        this->close();
        break;
    default:
        break;
    }
}

void bank::on_printAccountButton_clicked()
{
    accountlist *accountlist_windows;
    accountlist_windows = new accountlist();
    accountlist_windows->show();
    this->close();
}
