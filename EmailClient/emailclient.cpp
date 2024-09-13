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
    m_sendToUser = new SendToUser();

    ui->tw_contacts->setTabText(Contacts::Recent, "Recent");
    ui->tw_contacts->setTabText(Contacts::Contact, "Contacts");

    ui->centralwidget->setDisabled(true);

    setsBlurToMainWindow();
    connectToSmtp();
    setupLoginDialog();

    connect(m_loginWidget->ui->b_login, &QPushButton::clicked, this, &EmailClient::checkUserLoginData);
    connect(ui->b_sendEmail, &QPushButton::clicked, this, &EmailClient::onSendEmailClick);
    connect(m_sendToUser->ui->b_send_user_send_email, &QPushButton::clicked, this, &EmailClient::sendEmailToUser);
}

EmailClient::~EmailClient()
{
    m_smtp->quit();
    delete ui;
}

void EmailClient::checkUserLoginData()
{
    if(m_loginWidget->ui->e_login_name->text().simplified().size() == 0)
    {
        m_loginWidget->ui->e_login_message->setText("Name can`t be empty");
        return;
    }

    m_smtp->login(m_loginWidget->ui->e_login_email->text(), m_loginWidget->ui->e_login_app_password->text());
    if(!m_smtp->waitForAuthenticated(3000))
    {
        qDebug() << "Login failed";
        m_loginWidget->ui->e_login_message->setText("Invalid email or app password");
    }
    else
    {
        qDebug() << "User connected";
        m_loginWidget->ui->e_login_message->setText("Successful connection");
        m_loginWidget->ui->e_login_message->setStyleSheet("color: green; background: transparent; border: 0px;");
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
        m_loginWidget->ui->e_login_message->setText("Failed to connect to smtp server");
    }
    else
    {
        qDebug() << "Connect to smtp server";
    }
}

void EmailClient::switchToEmailClientWindow()
{
    m_loginWidget->hide();
    m_loginWidget->ui->e_login_message->clear();
    m_loginWidget->ui->e_login_message->setStyleSheet("color: red; background: transparent; border: 0px;");
    ui->centralwidget->setEnabled(true);
    m_blurEffect->setEnabled(false);
}

void EmailClient::createUserStruct()
{
    m_user.name = m_loginWidget->ui->e_login_name->text();
    m_user.email = m_loginWidget->ui->e_login_email->text();
    m_user.appPassword = m_loginWidget->ui->e_login_app_password->text();
}

void EmailClient::onSendEmailClick()
{
    m_sendToUser->show();
}

void EmailClient::sendEmailToUser()
{
    MimeMessage message;

    EmailAddress sender(m_user.email, m_user.name);
    message.setSender(sender);

    QStringListModel *usersModel = (QStringListModel*)m_sendToUser->ui->lv_send_email_to_users->model();
    QStringList users = usersModel->stringList();

    for(int index = 0; index < users.size(); index++)
    {
        QStringList userData = stringToRecipient(users.at(index));
        EmailAddress recipient(userData.at(UserData::Email), userData.at(UserData::Name));
        message.addRecipient(recipient);
    }

    message.setSubject(m_sendToUser->ui->e_send_user_subject->text());

    MimeText text;

    text.setText(m_sendToUser->ui->te_send_user_text->toPlainText());

    message.addPart(&text);

    QStringListModel *filesModel = (QStringListModel*)m_sendToUser->ui->lv_send_user_files->model();
    QStringList files = filesModel->stringList();


    for(int index = 0; index < files.size(); index++)
    {
        QFile *file = new QFile(files[index]);
        MimeAttachment *attachment = new MimeAttachment(file);
        message.addPart(attachment, true);
    }

    m_smtp->sendMail(message);
    if(!m_smtp->waitForMailSent(3000))
    {
        qDebug() << "Send email failed";
        m_sendToUser->ui->e_send_user_message->setText("Send email failed");
    }
    else
    {
        qDebug() << "Email send";
        m_sendToUser->ui->e_send_user_message->setText("Send email success");
        m_sendToUser->ui->e_send_user_message->setStyleSheet("color: green; background: transparent; border: 0px;");
        addToRecent(users);
    }
}

QStringList EmailClient::stringToRecipient(QString user)
{
    return user.split(";");
}

void EmailClient::addToRecent(QStringList &users)
{
    for(int index = 0; index < users.size(); index++)
    {
        QStringList userData = stringToRecipient(users.at(index));
        ui->lw_recent->addItem(userData.at(UserData::Email) + "\n" + QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy"));
    }
}
