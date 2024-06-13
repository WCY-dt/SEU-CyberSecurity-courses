/************************************************************************
 * Copyright (C) 2021 Chenyang https://wcy-dt.github.io                 *
 *                                                                      *
 * @file     editpasswd.cpp                                             *
 * @brief    page for editing password                                  *
 * @author   Chenyang                                                   *
 * @date     2021 - 07                                                  *
 ************************************************************************/

#include "editpasswd.h"
#include "bankserver.h"
#include "options.h"
#include "ui_editpasswd.h"
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>

extern bankServer bankserver;

editpasswd::editpasswd(QWidget *parent) : QDialog(parent),
                                          ui(new Ui::editpasswd)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

editpasswd::~editpasswd()
{
    delete ui;
}

void editpasswd::on_login_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}

void editpasswd::on_login_okButton_clicked()
{
    int result = QMessageBox::warning(this, tr(""), tr("确认修改密码吗？"), tr("确认"), tr("返回"), 0, 1);
    if (result == 0)
    {
        if (ui->login_passwdInput->text() != ui->login_confirmPasswdInput->text())
        {
            ui->login_passwdInput->setText(QString::fromStdString(""));
            ui->login_confirmPasswdInput->setText(QString::fromStdString(""));
            QMessageBox::critical(this, tr("错误"), tr("请重新检查你的输入！"), tr("确认"), 0);
            return;
        }
        if (!bankserver.PasswdEdit(ui->login_passwdInput_old->text().toStdString(),
                                   ui->login_passwdInput->text().toStdString()))
        {
            ui->login_passwdInput->setText(QString::fromStdString(""));
            ui->login_confirmPasswdInput->setText(QString::fromStdString(""));
            ui->login_passwdInput_old->setText(QString::fromStdString(""));
            QMessageBox::critical(this, tr("错误"), tr("密码输入错误！"), tr("确认"), 0);
            return;
        }
        QMessageBox::information(this, tr(""), tr("密码修改成功！"), tr("确认"), 0);
        options *options_windows;
        options_windows = new options();
        options_windows->show();
        this->close();
        delete this;
    }
}

void editpasswd::on_login_passwdInput_textEdited(const QString &arg1)
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

void editpasswd::on_login_confirmPasswdInput_textEdited(const QString &arg1)
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
