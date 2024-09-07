#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}
