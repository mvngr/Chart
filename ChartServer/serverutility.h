#ifndef SERVERUTILITY_H
#define SERVERUTILITY_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include <memory>
#include <mutex>
#include <unordered_map>


class ServerUtility : public QObject
{
    Q_OBJECT
public:
    static ServerUtility* inst(); //потокобезопасная выдача инстанса класса
    ~ServerUtility();

    void start(); //Запускает сервер
    void start(bool &isOk);

private:
    ServerUtility() {}
    ServerUtility(const ServerUtility&) = delete; ///запрещаем создание копирующего конструктора по-умолчанию
    ServerUtility& operator=(const ServerUtility&) = delete; ///запрещаем создание оператора присваивания по-умолчанию

    static std::atomic<ServerUtility *> object_;
    static std::mutex mutex_;


    std::unique_ptr<QTcpServer> server_ = nullptr;
    std::unordered_map<int, std::shared_ptr<QTcpSocket>> sockets_;

private slots:
    void slotNewConnection();
    void slotDisconnectClients();
    void slotListenClinet();
};

#endif // SERVERUTILITY_H
