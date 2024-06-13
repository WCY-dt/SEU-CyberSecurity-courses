/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     creataccount.cpp                                           *
 * @brief    page for creating accout                                   *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "createaccount.h"
#include "bank.h"
#include "bankserver.h"
#include "ui_createaccount.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <stdlib.h>
#include <time.h>

extern bankServer bankserver;

createaccount::createaccount(QWidget *parent) : QDialog(parent),
                                                ui(new Ui::createaccount)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    ui->login_accountInput->setStyleSheet("background:#F0F0F0;");
    ui->login_interestInput->setStyleSheet("background:#F0F0F0;");
    ui->login_nameInput->setFocus();

    srand((unsigned)time(NULL));
    string strNumber = "";
    for (int i = 1; i <= 19; i++)
    {
        strNumber += (char)((rand() % 10) + '0');
        //qDebug()<<(QString::fromStdString(strNumber));
    }
    ui->login_accountInput->setText(QString::fromStdString(strNumber));

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

createaccount::~createaccount()
{
    delete ui;
}

void createaccount::on_login_backButton_clicked()
{
    bank *bank_windows;
    bank_windows = new bank();
    bank_windows->show();
    this->close();
}

void createaccount::on_login_okButton_clicked()
{
    if (ui->login_passwdInput->text() != ui->login_confirmPasswdInput->text())
    {
        QMessageBox::critical(this, tr("错误"), tr("请重新检查你的输入！"), tr("确认"), 0);
        return;
    }
    string strNum = ui->login_accountInput->text().toStdString();
    string strNam = ui->login_nameInput->text().toStdString();
    string strPas = ui->login_passwdInput->text().toStdString();
    string strAdd = ui->login_addressInput->text().toStdString();
    int iTyp = ui->login_typeCombobox->currentIndex();
    double dInt = ui->login_interestInput->text().toDouble();
    string strOpe = ui->login_employeeInput->text().toStdString();
    if (bankserver.AccountAdd(strNum, strNam, strPas, strAdd, iTyp, dInt, strOpe))
    {
        QMessageBox::information(this, tr(""), tr("新账户已成功创建！"), tr("确认"), 0);
        bank *bank_windows;
        bank_windows = new bank();
        bank_windows->show();
        this->close();
        delete this;
    }
    else
    {
        ui->login_passwdInput->setText(QString::fromStdString(""));
        ui->login_confirmPasswdInput->setText(QString::fromStdString(""));
        QMessageBox::critical(this, tr("错误"), tr("请重新检查你的输入！"), tr("确认"), 0);
    }
}

void createaccount::on_login_confirmPasswdInput_textEdited(const QString &arg1)
{
    if (arg1 != ui->login_passwdInput->text())
    {
        ui->login_repeatPasswdState->setStyleSheet("color:red;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("× 密码重复不正确"));
    }
    else
    {
        ui->login_repeatPasswdState->setStyleSheet("color:green;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("√"));
    }
}

void createaccount::on_login_passwdInput_textEdited(const QString &arg1)
{
    if (arg1 != ui->login_confirmPasswdInput->text())
    {
        ui->login_repeatPasswdState->setStyleSheet("color:red;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("× 密码重复不正确"));
    }
    else
    {
        ui->login_repeatPasswdState->setStyleSheet("color:green;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("√"));
    }
}

void createaccount::on_login_typeCombobox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->login_interestInput->setText(QString::fromStdString("1.98"));
        break;
    case 1:
        ui->login_interestInput->setText(QString::fromStdString("2.25"));
        break;
    case 2:
        ui->login_interestInput->setText(QString::fromStdString("3.5"));
        break;
    default:
        ui->login_interestInput->setText(QString::fromStdString("1.98"));
    }
}
