#include "emailclient.h"
#include "ui_emailclient.h"

EmailClient::EmailClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmailClient)
{
    ui->setupUi(this);

    ui->centralwidget->setDisabled(true);
    m_blurEffect = new QGraphicsBlurEffect();
    m_blurEffect->setBlurRadius(10);
    ui->centralwidget->setGraphicsEffect(m_blurEffect);

    m_loginWidget = new Login(this);
    setupLoginDialog();

}

EmailClient::~EmailClient()
{
    delete ui;
}

void EmailClient::setupLoginDialog()
{
    m_loginWidget->setWindowModality(Qt::ApplicationModal);
    m_loginWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    m_loginWidget->setFrameShape(QFrame::Box);
    m_loginWidget->setFixedSize(400, 300);
    m_loginWidget->show();
}
