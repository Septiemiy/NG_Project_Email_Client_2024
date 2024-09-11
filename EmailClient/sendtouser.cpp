#include "sendtouser.h"
#include "ui_sendtouser.h"

SendToUser::SendToUser(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::SendToUser)
{
    ui->setupUi(this);

    hide();
    setWindowFlags(Qt::Window);

    connect(ui->b_send_user_cancel, &QPushButton::clicked, this, &SendToUser::onClickCancel);

}

SendToUser::~SendToUser()
{
    delete ui;
}

void SendToUser::onClickCancel()
{
    ui->e_send_user_email->clear();
    ui->e_send_user_name->clear();
    ui->e_send_user_subject->clear();
    ui->te_send_user_text->clear();
    close();
}
