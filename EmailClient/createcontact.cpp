#include "createcontact.h"
#include "ui_createcontact.h"

CreateContact::CreateContact(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::CreateContact)
{
    ui->setupUi(this);

    ui->e_contact_message->setReadOnly(true);
    ui->e_contact_message->setAlignment(Qt::AlignHCenter);
    ui->e_contact_message->setStyleSheet("color: red; background: transparent; border: 0px;");

    connect(ui->b_contact_cancel, &QPushButton::clicked, this, &CreateContact::onCancelClick);
}

CreateContact::~CreateContact()
{
    delete ui;
}

void CreateContact::onCancelClick()
{
    close();
}

void CreateContact::closeEvent(QCloseEvent *event)
{
    ui->e_contact_message->clear();
    ui->e_contact_message->setStyleSheet("color: red; background: transparent; border: 0px;");
    ui->e_contact_name->clear();
    ui->e_contact_email->clear();
    event->accept();
}
