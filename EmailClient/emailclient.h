#ifndef EMAILCLIENT_H
#define EMAILCLIENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class EmailClient;
}
QT_END_NAMESPACE

class EmailClient : public QMainWindow
{
    Q_OBJECT

public:
    EmailClient(QWidget *parent = nullptr);
    ~EmailClient();

private:
    Ui::EmailClient *ui;
};
#endif // EMAILCLIENT_H
