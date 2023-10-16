#include "mainwindow.h"
#include "startscreen.h"
#include "serverview.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QTimer>
#include <QMessageBox>


int MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(uint userID,
                       QString userName,
                       std::shared_ptr<Database> dbPtr,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_userId(userID),
    m_userName(userName)
{
    ui->setupUi(this);
    setWindowTitle(m_userName);
    kInstanceCount++;
    if (dbPtr) {
        m_dbPtr = dbPtr;
    }
    else
        m_dbPtr = make_shared<Database>();
    auto timer = new QTimer();
    connect (timer, &QTimer::timeout, this, &MainWindow::updateChats);
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
    kInstanceCount--;
    if(kInstanceCount<=0) {
        qApp->exit(0);
    }
}

MainWindow *MainWindow::createClient(std::shared_ptr<Database> dbPtr)
{
    StartScreen s(dbPtr);
    auto result = s.exec();
    if (result == QDialog::Rejected) {
       return nullptr;
    }
    MainWindow* w = new MainWindow(s.userID(), s.userName(), s.getDataBase());
    w->setAttribute(Qt::WA_DeleteOnClose);
    clientWindows.append(w);
    return w;
}

void MainWindow::on_messageLineEdit_returnPressed()
{
    on_sendMessageButton_clicked();
}

void MainWindow::on_sendMessageButton_clicked()
{
    for(uint i=0; i<m_dbPtr->getUsers().size(); i++) {
        if(m_dbPtr->getUsers().at(i).getuserName().c_str() ==
           m_userName.toStdString()) {
            if(!m_dbPtr->getUsers().at(i).isBun()) {
                m_dbPtr->addChatMessage(m_userName.toStdString(), ui->messageLineEdit->text().toStdString());
            }
            else {
                QMessageBox::warning(this,"Attantion",
                                     "You have been banned and cannot post messages");
            }
        }
    }
}

void MainWindow::on_privateMessageSendButton_clicked()
{
    QDialog dial(this);
    dial.setModal(true);
    auto l = new QVBoxLayout;
    dial.setLayout(l);
    auto userListWgt = new QListWidget(&dial);
    l->addWidget(userListWgt);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                          QDialogButtonBox::Cancel,
                                          &dial);
    l->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);

    auto userList = m_dbPtr->getUserList();
    for(auto user : userList) {
        userListWgt->addItem(QString::fromStdString(user));
    }
    userListWgt->setCurrentRow(0);
    auto result = dial.exec();
    if(result == QDialog::Accepted && userListWgt->currentItem()) {
        for(uint i=0; i<m_dbPtr->getUsers().size(); i++) {
            if(m_dbPtr->getUsers().at(i).getuserName().c_str() ==
               m_userName.toStdString()) {
                if(!m_dbPtr->getUsers().at(i).isBun()) {
                    m_dbPtr->addPrivateMessage(m_userName.toStdString(),
                                               userListWgt->currentItem()->text().toStdString(),
                                               ui->messageLineEdit->text().toStdString());
                }
                else {
                    QMessageBox::warning(this,"Attantion!",
                                         "You have been banned and cannot post messages");
                }
            }
        }
    }
}

void MainWindow::on_actionOpen_another_client_triggered()
{
    auto w = createClient(m_dbPtr);
    if(w)
        w->show();
}

void MainWindow::on_actionCloseClient_triggered()
{
    this->close();
}

void MainWindow::updateChats()
{
    auto chatMessages = m_dbPtr->getChatMessages();
    QString chat;
    for(const auto &msg : chatMessages) {
        chat.append(QString::fromStdString(msg) + "\n");
    }
    if(ui->commonChatBrowser->toPlainText() != chat) {
        ui->commonChatBrowser->setText(chat);
    }
    chat.clear();
    auto privateMessages = m_dbPtr->getPrivateMessage();
    for(const auto &msg : privateMessages) {
        if(QString::fromStdString(msg.getSender()) != m_userName && msg.getDest() != m_userId)
            continue;
        QString prefix;
        if(m_userName == QString::fromStdString(msg.getSender()) &&
           m_userId == msg.getDest()) {
            prefix = tr("self message") + ": ";
        }
        else if (m_userName == QString::fromStdString(msg.getSender())) {
            prefix = tr("message to") + QString(" <%1>: ").arg(QString::fromStdString(m_dbPtr->getUserName(msg.getDest())));

        }
        else {
            prefix = "<" + QString::fromStdString(msg.getSender()) + ">: " +
                    tr("say to you") + ": ";
        }
        chat.append(prefix + QString::fromStdString(msg.getText()) + "\n");
    }
    if(ui->privateChatBrowser->toPlainText() != chat) {
        ui->privateChatBrowser->setText(chat);
    }
}

void MainWindow::on_actionServer_view_triggered()
{
    auto sv = new ServerView(m_dbPtr, &clientWindows);
    sv->show();
}

QString MainWindow::userName() const
{
    return m_userName;
}
