/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     editprofile.cpp                                            *
 * @brief    page for editing profile                                   *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "editprofile.h"
#include "bankserver.h"
#include "options.h"
#include "ui_editprofile.h"
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>

extern bankServer bankserver;

editprofile::editprofile(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::editprofile)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    ui->login_interestInput->setStyleSheet("background:#F0F0F0;");
    ui->login_nameInput->setFocus();
    ui->login_nameInput->setText(QString::fromStdString(bankserver.GetName()));
    ui->login_addressInput->setText(QString::fromStdString(bankserver.GetAddress()));
    ui->login_typeCombobox->setCurrentIndex(bankserver.GetType());
    ui->login_interestInput->setText(QString::number(bankserver.GetInterest()));

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

editprofile::~editprofile()
{
    delete ui;
}

void editprofile::on_login_typeCombobox_currentIndexChanged(int index)
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

void editprofile::on_login_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}

void editprofile::on_login_okButton_clicked()
{
    int result = QMessageBox::warning(this, tr(""), tr("确认修改个人资料吗？"), tr("确认"), tr("返回"), 0, 1);
    if (result == 0)
    {
        string strNam = ui->login_nameInput->text().toStdString();
        string strAdd = ui->login_addressInput->text().toStdString();
        int iTyp = ui->login_typeCombobox->currentIndex();
        double dInt = ui->login_interestInput->text().toDouble();
        if (bankserver.AccountEdit(strNam, strAdd, iTyp, dInt))
        {
            QMessageBox::information(this, tr(""), tr("账户信息修改成功！"), tr("确认"), 0);
            options *options_windows;
            options_windows = new options();
            options_windows->show();
            this->close();
            delete this;
        }
        else
        {
            QMessageBox::critical(this, tr("错误"), tr("请重新检查你的输入！"), tr("确认"), 0);
        }
    }
}
