#include "sendtouser.h"
#include "ui_sendtouser.h"

SendToUser::SendToUser(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::SendToUser)
{
    ui->setupUi(this);
}

SendToUser::~SendToUser()
{
    delete ui;
}
