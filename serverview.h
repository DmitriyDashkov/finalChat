#ifndef SERVERVIEW_H
#define SERVERVIEW_H

#include <QWidget>
#include <QDialog>
#include "database.h"
#include <memory>
#include "mainwindow.h"

namespace Ui {
class ServerView;
}

class ServerView : public QDialog
{
    Q_OBJECT

public:
    explicit ServerView(std::shared_ptr<Database> dbPtr,
                        QVector<MainWindow*> *clientWindows,
                        QWidget *parent = nullptr);
    ~ServerView();

    std::shared_ptr<Database> dbPtr() const                     { return m_dbPtr; }
    void fillingUsersComboBox();
    void setConnect();
    void setStatusBun(bool);
    void setCheckBoxBun();
    User getCurrentUser();  //текущий пользователь

private:
    Ui::ServerView *ui;
    std::shared_ptr<Database> m_dbPtr;
    QVector<MainWindow*> *m_clientWindows;

public slots:
    void updateTextBrowsers(const QString &text);

private slots:
    void on_pushButton_clicked();
    void on_usersComboBox_currentIndexChanged(int index);
    void on_pushButton_2_clicked();
    void on_deleteUserPushButton_clicked();

signals:

};

#endif // SERVERVIEW_H
