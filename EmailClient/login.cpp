#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->t_app_password_link->setReadOnly(true);
    ui->t_app_password_link->setStyleSheet("background: transparent; border: 0px;");

    ui->e_message->setReadOnly(true);
    ui->e_message->setAlignment(Qt::AlignHCenter);
    ui->e_message->setStyleSheet("color: red; background: transparent; border: 0px;");

    ui->e_login_app_password->setEchoMode(QLineEdit::Password);

    connect(ui->b_login_quit, &QPushButton::clicked, this, &Login::onClickExit);
}

Login::~Login()
{
    delete ui;
}

void Login::onClickExit()
{
    exit(0);
}
