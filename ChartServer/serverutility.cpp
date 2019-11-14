#include "serverutility.h"

#include <QDebug>

std::atomic<ServerUtility*> ServerUtility::object_ { nullptr }; //инициализируем
std::mutex ServerUtility::mutex_;

ServerUtility::~ServerUtility()
{
    slotDisconnectClients();
}

/*!
 * \brief Потокобезопасная выдача инстанса класса
 * \return указатель на созданный объект класса ServerUtility
 */
ServerUtility* ServerUtility::inst()
{

    if(object_ == nullptr) //первая "быстрая" проверка
    {
        std::lock_guard<std::mutex> lock(mutex_); //при полёте по nullptr лочим мьютекс и после доп. проверки создаем объект
        if(object_ == nullptr)
        {
            object_ = new ServerUtility();
        }
    }
    return object_;
}

/*!
 * \brief Запускает сервер
 *
 * В данном методе сервер подключается на порт
 */
void ServerUtility::start()
{
    bool unused;
    start(unused);
}

/*!
 * \brief Запускает сервер
 * \param isOk Удачно ли прошел запуск сервера
 *
 * В данном методе сервер подключается на порт
 */
void ServerUtility::start(bool &isOk)
{
    server_.reset(new QTcpServer(this));
    connect(server_.get(), &QTcpServer::newConnection, this, &ServerUtility::slotNewConnection);

    quint16 port = 18383; //TODO Вынести в настройки
    if(server_->listen(QHostAddress::Any, port))
        qDebug() << tr("Сервер запущен. Используемый порт: %1").arg(port);
    else
        qDebug() << tr("Не удалось подключиться к порту. Ошибка: %1").arg(server_->errorString());
    isOk = server_->isListening();
}

void ServerUtility::slotNewConnection()
{
    while(server_->hasPendingConnections())
    {
        std::shared_ptr<QTcpSocket> socket = std::make_shared<QTcpSocket>(server_->nextPendingConnection());
        int socketId = socket->socketDescriptor();
        if(socketId == -1)
            qDebug() << tr("При новом подключении не удалось определить дескриптор сокета. IP адрес: %1").arg(socket->peerAddress().toString());
        else
        {
            sockets_[socketId] = socket;
            connect(socket.get(), &QTcpSocket::readyRead, this, &ServerUtility::slotListenClinet);
        }
    }
}

void ServerUtility::slotDisconnectClients()
{
    for(const std::pair<int, std::shared_ptr<QTcpSocket>> &socket : sockets_) //проходимся по сокетам
    {
        if(socket.second)
            socket.second->close(); //и кидаем оповещение об остановке
    }
}

void ServerUtility::slotListenClinet()
{
    //TODO
}
