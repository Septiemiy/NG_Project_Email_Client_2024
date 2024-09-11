#include "emailclient.h"
#include "ui_emailclient.h"

//medo fwmd glzz naca

EmailClient::EmailClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmailClient)
{
    ui->setupUi(this);

    m_blurEffect = new QGraphicsBlurEffect();
    m_loginWidget = new Login(this);

    ui->centralwidget->setDisabled(true);

    setsBlurToMainWindow();
    connectToSmtp();
    setupLoginDialog();

    connect(m_loginWidget->ui->b_login, &QPushButton::clicked, this, &EmailClient::checkUserLoginData);
}

EmailClient::~EmailClient()
{
    m_smtp->quit();
    delete ui;
}

void EmailClient::checkUserLoginData()
{
    m_smtp->login(m_loginWidget->ui->e_login_email->text(), m_loginWidget->ui->e_login_app_password->text());
    if(!m_smtp->waitForAuthenticated(3000))
    {
        qDebug() << "Login failed: Invalid email or app password";
        m_loginWidget->ui->e_message->setText("Invalid email or app password");
    }
    else
    {
        qDebug() << "User connected";
        m_loginWidget->ui->e_message->setText("Successful connection");
        m_loginWidget->ui->e_message->setStyleSheet("color: green; background: transparent; border: 0px;");
        createUserStruct();
        QTimer::singleShot(1500, this, &EmailClient::switchToEmailClientWindow);
    }
}

void EmailClient::setupLoginDialog()
{
    m_loginWidget->setWindowModality(Qt::ApplicationModal);
    m_loginWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    m_loginWidget->setFrameShape(QFrame::Box);
    m_loginWidget->show();
}

void EmailClient::setsBlurToMainWindow()
{
    m_blurEffect->setBlurRadius(10);
    ui->centralwidget->setGraphicsEffect(m_blurEffect);
}

void EmailClient::connectToSmtp()
{
    m_smtp = new SmtpClient("smtp.gmail.com", 465, SmtpClient::SslConnection);
    m_smtp->connectToHost();
    if(!m_smtp->waitForReadyConnected(5000))
    {
        qDebug() << "Failed to connect to smtp server";
        m_loginWidget->ui->e_message->setText("Failed to connect to smtp server");
    }
    else
    {
        qDebug() << "Connect to smtp server";
    }
}

void EmailClient::switchToEmailClientWindow()
{
    m_loginWidget->hide();
    ui->centralwidget->setEnabled(true);
    m_blurEffect->setEnabled(false);
}

void EmailClient::createUserStruct()
{
    User user;
    user.email = m_loginWidget->ui->e_login_email->text();
    user.appPassword = m_loginWidget->ui->e_login_app_password->text();
}
