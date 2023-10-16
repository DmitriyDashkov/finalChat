#pragma once

#include <QMainWindow>
#include "database.h"
#include <memory>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow;
static QVector<MainWindow*> clientWindows;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(uint userID,
                        QString userName,
                        std::shared_ptr<Database> dbPtr = nullptr,
                        QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* createClient(std::shared_ptr<Database> dbPtr = nullptr);
    static int kInstanceCount;

    QString userName() const;

private slots:
    void on_messageLineEdit_returnPressed();
    void on_sendMessageButton_clicked();
    void on_privateMessageSendButton_clicked();
    void on_actionOpen_another_client_triggered();
    void on_actionServer_view_triggered();

public slots:
    void on_actionCloseClient_triggered();


private:
    Ui::MainWindow *ui;
    std::shared_ptr<Database> m_dbPtr;
    uint m_userId;
    QString m_userName;
    void updateChats();
};


