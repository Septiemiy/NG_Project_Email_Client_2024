#ifndef CREATECONTACT_H
#define CREATECONTACT_H

#include <QFrame>
#include <QCloseEvent>

namespace Ui {
class CreateContact;
}

class CreateContact : public QFrame
{
    Q_OBJECT

public:
    explicit CreateContact(QWidget *parent = nullptr);
    ~CreateContact();

private slots:
    void onCancelClick();

private:
    void closeEvent(QCloseEvent *event);

public:
    Ui::CreateContact *ui;
};

#endif // CREATECONTACT_H
