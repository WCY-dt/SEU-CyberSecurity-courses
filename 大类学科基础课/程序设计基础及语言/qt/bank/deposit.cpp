/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     deposit.cpp                                                *
 * @brief    page for depositing money                                  *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "deposit.h"
#include "bankserver.h"
#include "options.h"
#include "ui_deposit.h"
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>

extern bankServer bankserver;

deposit::deposit(QWidget *parent) : QDialog(parent),
                                    ui(new Ui::deposit)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    ui->deposit_moneyInput->setText("0");
    ui->deposit_moneyInput->setFocus();

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

deposit::~deposit()
{
    delete ui;
}

void deposit::on_deposit_okButton_clicked()
{
    switch (QMessageBox::warning(this, NULL,
                                 tr("确认存入") + (ui->deposit_moneyInput->text()) + tr("元吗?"),
                                 tr("确认"), tr("取消"), 0, 1))
    {
    case 0:
        bankserver.Deposit((ui->deposit_moneyInput->text()).toDouble(),
                           (ui->login_employeeInput->text()).toStdString());
        QMessageBox::information(this, NULL,
                                 tr("成功存入") + (ui->deposit_moneyInput->text()) + tr("元！"),
                                 tr("确认"));
        options *options_windows;
        options_windows = new options();
        options_windows->show();
        this->close();
        break;
    }
}

void deposit::on_deposit_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}
