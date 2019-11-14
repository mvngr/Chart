#include <QCoreApplication>
#include <QDebug>

#include "serverutility.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServerUtility::inst()->start();

    return a.exec();
}
