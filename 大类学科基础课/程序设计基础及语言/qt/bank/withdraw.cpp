/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     withdraw.cpp                                               *
 * @brief    page for withdrawing money                                 *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "withdraw.h"
#include "bankserver.h"
#include "options.h"
#include "ui_withdraw.h"
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>

extern bankServer bankserver;

withdraw::withdraw(QWidget *parent) : QDialog(parent),
                                      ui(new Ui::withdraw)
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

withdraw::~withdraw()
{
    delete ui;
}

void withdraw::on_deposit_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}

void withdraw::on_deposit_okButton_clicked()
{
    switch (QMessageBox::warning(this, NULL,
                                 tr("确认取出") + (ui->deposit_moneyInput->text()) + tr("元吗?"),
                                 tr("确认"), tr("取消"), 0, 1))
    {
    case 0:
        if (bankserver.Withdraw((ui->deposit_moneyInput->text()).toDouble(),
                                (ui->login_employeeInput->text()).toStdString()))
        {
            QMessageBox::information(this, NULL,
                                     tr("成功取出") + (ui->deposit_moneyInput->text()) + tr("元！"),
                                     tr("确认"));
            options *options_windows;
            options_windows = new options();
            options_windows->show();
            this->close();
        }
        else
        {
            ui->deposit_moneyInput->setText("0");
            ui->deposit_moneyInput->setFocus();
            QMessageBox::information(this, NULL, tr("余额不足！"), tr("确认"));
        }
        break;
    }
}
