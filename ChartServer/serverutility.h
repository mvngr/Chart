#ifndef SERVERUTILITY_H
#define SERVERUTILITY_H

#include <QObject>
#include <QTimer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>

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

    const quint16 port = 18383; //TODO Вынести в настройки
    const quint16 broadcastPort = 18384;
    const int msecDelayForTimer = 5000;

private:
    ServerUtility();
    ServerUtility(const ServerUtility&) = delete; ///запрещаем создание копирующего конструктора по-умолчанию
    ServerUtility& operator=(const ServerUtility&) = delete; ///запрещаем создание оператора присваивания по-умолчанию

    static std::atomic<ServerUtility *> object_; ///Объект класса (нужен для синглтона)
    static std::mutex mutex_; ///Мьютекс (нужен для синглтона)


    std::unique_ptr<QTcpServer> server_ = nullptr; ///TCP сервер
    std::unique_ptr<QUdpSocket> udpSender_ = nullptr; ///отправляет udp бродкасты о наличии сервера
    std::unordered_map<int, std::shared_ptr<QTcpSocket>> sockets_; ///список подключенных сокетов

    QTimer broadcastSenderTimer_; ///Таймер отправки данных о существовании сервера

private slots:
    void slotNewConnection(); //Отвечает за приём данных на стороне сервера
    void slotDisconnectClients(); //Посылает сообщения на закрытие соединения
    void slotListenClinet();
    void slotSendBroadcast();
};

#endif // SERVERUTILITY_H
