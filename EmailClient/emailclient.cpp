#include "emailclient.h"
#include "ui_emailclient.h"

EmailClient::EmailClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmailClient)
{
    ui->setupUi(this);
}

EmailClient::~EmailClient()
{
    delete ui;
}
