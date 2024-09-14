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
    m_createContact = new CreateContact();

    ui->tw_contacts->setTabText(Contacts::Recent, "Recent");
    ui->tw_contacts->setTabText(Contacts::Contact, "Contacts");

    ui->centralwidget->setDisabled(true);

    setsBlurToMainWindow();
    connectToSmtp();
    setupLoginDialog();

    connect(m_loginWidget->ui->b_login, &QPushButton::clicked, this, &EmailClient::checkUserLoginData);
    connect(ui->b_sendEmail, &QPushButton::clicked, this, &EmailClient::onSendEmailClick);
    connect(m_sendToUser->ui->b_send_user_send_email, &QPushButton::clicked, this, &EmailClient::sendEmailToUser);
    connect(this, &EmailClient::listWidgetChanged, this, &EmailClient::saveIntoFile);
    connect(ui->b_createNewConctact, &QPushButton::clicked, this, &EmailClient::onCreateContactClick);
    connect(m_createContact->ui->b_contact_create, &QPushButton::clicked, this, &EmailClient::addToContacts);
    connect(ui->lw_recent, &QListWidget::itemDoubleClicked, this, &EmailClient::listWidgetItemDoubleClicked);

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
        loadRecentFromFile();
        loadContactsFromFile();
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

    addToRecent(users);

    // m_smtp->sendMail(message);
    // if(!m_smtp->waitForMailSent(3000))
    // {
    //     qDebug() << "Send email failed";
    //     m_sendToUser->ui->e_send_user_message->setText("Send email failed");
    // }
    // else
    // {
    //     qDebug() << "Email send";
    //     m_sendToUser->ui->e_send_user_message->setText("Send email success");
    //     m_sendToUser->ui->e_send_user_message->setStyleSheet("color: green; background: transparent; border: 0px;");

    // }
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
        ui->lw_recent->insertItem(0, userData.at(UserData::Email) + " / " + userData.at(UserData::Name) + "\n" + QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy"));
    }

    emit listWidgetChanged(FileType::RecentFile);
}

void EmailClient::saveIntoFile(int fileType)
{
    // QDir recentDir;
    // if(!recentDir.exists(m_recentPath))
    // {
    //     recentDir.mkpath(m_recentPath);
    // }


    if(fileType == FileType::RecentFile)
    {
        QFile recentFile(m_user.email.split("@").first() + " recent.json");
        if(recentFile.open(QIODevice::WriteOnly))
        {
            writeJsonIntoFile(recentFile, fileType);
        }
        else
        {
            qDebug() << "Can`t open file: " << recentFile.fileName();
        }
    }
    else if(fileType == FileType::ContactsFile)
    {
        QFile contactsFile(m_user.email.split("@").first() + " contacts.json");
        if(contactsFile.open(QIODevice::WriteOnly))
        {
            writeJsonIntoFile(contactsFile, fileType);
        }
        else
        {
            qDebug() << "Can`t open file: " << contactsFile.fileName();
        }
    }
}

void EmailClient::onCreateContactClick()
{
    m_createContact->show();
}

void EmailClient::addToContacts()
{
    m_createContact->ui->e_contact_message->setStyleSheet("color: red; background: transparent; border: 0px;");
    if(m_createContact->ui->e_contact_email->text().simplified().size() == 0 || m_createContact->ui->e_contact_name->text().simplified().size() == 0)
    {
        m_createContact->ui->e_contact_message->setText("Name and email can`t be empty");
        return;
    }

    for(int index = 0; index < ui->lw_contacts->count(); index++)
    {
        QString contactsItemText = ui->lw_contacts->item(index)->text();
        if(contactsItemText.contains(m_createContact->ui->e_contact_email->text()))
        {
            m_createContact->ui->e_contact_message->setText("This user already exists");
            return;
        }
    }

    ui->lw_contacts->addItem(m_createContact->ui->e_contact_name->text() + "\n" + m_createContact->ui->e_contact_email->text());
    m_createContact->ui->e_contact_message->setText("Contact created");
    m_createContact->ui->e_contact_message->setStyleSheet("color: green; background: transparent; border: 0px;");

    emit listWidgetChanged(FileType::ContactsFile);
}

void EmailClient::listWidgetItemDoubleClicked(QListWidgetItem *item)
{
    QStringList itemTextList = item->text().split("\n").first().split(" / ");
    m_createContact->ui->e_contact_email->setText(itemTextList[UserData::Email]);
    m_createContact->ui->e_contact_name->setText(itemTextList[UserData::Name]);
    m_createContact->show();
}

void EmailClient::writeJsonIntoFile(QFile &file, int fileType)
{
    if(fileType == FileType::RecentFile)
    {
        QStringList recentStrings;
        for(int index = 0; index < ui->lw_recent->count(); index++)
        {
            QString recentItemText = ui->lw_recent->item(index)->text();
            recentStrings << recentItemText;
        }

        QJsonArray recentArray = QJsonArray::fromStringList(recentStrings);

        QJsonObject recentJson;
        recentJson["Recent"] = recentArray;

        QJsonDocument recentJsonDocument { recentJson };
        file.write(recentJsonDocument.toJson());
        file.close();
    }
    else if(fileType == FileType::ContactsFile)
    {
        QStringList contactsStrings;
        for(int index = 0; index < ui->lw_contacts->count(); index++)
        {
            QString contactsItemText = ui->lw_contacts->item(index)->text();
            contactsStrings << contactsItemText;
        }

        QJsonArray contactsArray = QJsonArray::fromStringList(contactsStrings);

        QJsonObject contactsJson;
        contactsJson["Contacts"] = contactsArray;

        QJsonDocument contactsJsonDocument { contactsJson };
        file.write(contactsJsonDocument.toJson());
        file.close();
    }
}

void EmailClient::loadRecentFromFile()
{
    QFile recentFile(m_user.email.split("@").first() + " recent.json");
    if(recentFile.exists())
    {
        if(recentFile.open(QIODevice::ReadOnly))
        {
            QByteArray recentFileData = recentFile.readAll();
            loadJsonIntoListWidget(recentFileData, FileType::RecentFile);
        }
    }
    recentFile.close();
}

void EmailClient::loadContactsFromFile()
{
    QFile contactsFile(m_user.email.split("@").first() + " contacts.json");
    if(contactsFile.exists())
    {
        if(contactsFile.open(QIODevice::ReadOnly))
        {
            QByteArray contactsFileData = contactsFile.readAll();
            loadJsonIntoListWidget(contactsFileData, FileType::ContactsFile);
        }
    }
    contactsFile.close();
}

void EmailClient::loadJsonIntoListWidget(QByteArray &recentFileData, int fileType)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(recentFileData);

    if(fileType == FileType::RecentFile)
    {
        QJsonArray recentJsonArray = jsonDocument.object().value("Recent").toArray();

        for(int index = 0; index < recentJsonArray.size(); index++)
        {
            ui->lw_recent->addItem(recentJsonArray.at(index).toString());
        }
    }
    else if(fileType == FileType::ContactsFile)
    {
        QJsonArray contactsJsonArray = jsonDocument.object().value("Contacts").toArray();

        for(int index = 0; index < contactsJsonArray.size(); index++)
        {
            ui->lw_contacts->addItem(contactsJsonArray.at(index).toString());
        }
    }

}
