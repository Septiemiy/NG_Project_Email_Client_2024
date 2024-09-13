#include "sendtouser.h"
#include "ui_sendtouser.h"

SendToUser::SendToUser(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::SendToUser)
{
    ui->setupUi(this);

    hide();
    setWindowFlags(Qt::Window);

    m_modelFiles = new QStringListModel(this);
    m_modelUsers = new QStringListModel(this);

    ui->e_send_user_message->setReadOnly(true);
    ui->e_send_user_message->setAlignment(Qt::AlignHCenter);
    ui->e_send_user_message->setStyleSheet("color: red; background: transparent; border: 0px;");

    ui->lv_send_user_files->setViewMode(QListView::ListMode);
    ui->lv_send_user_files->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lv_send_user_files->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->lv_send_email_to_users->setViewMode(QListView::ListMode);
    ui->lv_send_email_to_users->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lv_send_email_to_users->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->b_send_user_cancel, &QPushButton::clicked, this, &SendToUser::onClickCancel);
    connect(ui->b_send_user_add_file, &QPushButton::clicked, this, &SendToUser::addFilePathToAttachmentList);
    connect(ui->lv_send_user_files, &QListView::customContextMenuRequested, this, &SendToUser::showContextMenuFiles);

    connect(ui->b_add_recipient, &QPushButton::clicked, this, &SendToUser::addUsersToUsersList);
    connect(ui->lv_send_email_to_users, &QListView::customContextMenuRequested, this, &SendToUser::showContextMenuUsers);
}

SendToUser::~SendToUser()
{
    delete ui;
}

void SendToUser::onClickCancel()
{
    close();
}

void SendToUser::addFilePathToAttachmentList()
{
    QStringList filePath = QFileDialog::getOpenFileNames(this, "Open files", "/");
    m_modelFiles->setStringList(filePath);
    ui->lv_send_user_files->setModel(m_modelFiles);
}

void SendToUser::addUsersToUsersList()
{
    if(ui->e_send_user_name->text().simplified().size() == 0 || ui->e_send_user_email->text().simplified().size() == 0)
    {
        ui->e_send_user_message->setText("Name and email can`t be empty");
        return;
    }

    QString user = ui->e_send_user_email->text() + ";" + ui->e_send_user_name->text();

    if(m_users.contains(user))
    {
        ui->e_send_user_message->setText("That user already in the list");
        return;
    }

    m_users << user;
    m_modelUsers->setStringList(m_users);
    ui->lv_send_email_to_users->setModel(m_modelUsers);
}

void SendToUser::showContextMenuFiles(const QPoint &pos)
{
    QMenu contextMenu("Context menu", this);
    QModelIndex index = ui->lv_send_user_files->indexAt(pos);

    QAction *deleteRow = new QAction("Delete this row", this);
    connect(deleteRow, &QAction::triggered, this, [=](){
        if(index.isValid())
        {
            m_modelFiles->removeRow(index.row());
        }
    });

    QAction *deleteRows = new QAction("Delete all", this);
    connect(deleteRows, &QAction::triggered, this, [=](){
        m_modelFiles->removeRows(0, m_modelFiles->rowCount());
    });

    contextMenu.addAction(deleteRow);
    contextMenu.addAction(deleteRows);
    contextMenu.exec(ui->lv_send_user_files->viewport()->mapToGlobal(pos));
}

void SendToUser::showContextMenuUsers(const QPoint &pos)
{
    QMenu contextMenu("Context menu", this);
    QModelIndex index = ui->lv_send_email_to_users->indexAt(pos);

    QAction *deleteRow = new QAction("Delete this row", this);
    connect(deleteRow, &QAction::triggered, this, [=](){
        if(index.isValid())
        {
            m_modelUsers->removeRow(index.row());
        }
    });

    QAction *deleteRows = new QAction("Delete all", this);
    connect(deleteRows, &QAction::triggered, this, [=](){
        m_modelUsers->removeRows(0, m_modelUsers->rowCount());
    });

    contextMenu.addAction(deleteRow);
    contextMenu.addAction(deleteRows);
    contextMenu.exec(ui->lv_send_email_to_users->viewport()->mapToGlobal(pos));
}

void SendToUser::closeEvent(QCloseEvent *event)
{
    ui->e_send_user_email->clear();
    ui->e_send_user_name->clear();
    ui->e_send_user_subject->clear();
    ui->te_send_user_text->clear();
    m_modelFiles->removeRows(0, m_modelFiles->rowCount());
    m_modelUsers->removeRows(0, m_modelUsers->rowCount());
    m_users.clear();
    event->accept();
}
