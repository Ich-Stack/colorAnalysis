#include "collect.h"

COLLECT::COLLECT(const QString &ttyPath)
{
    com = new Posix_QextSerialPort(ttyPath, QextSerialBase::Polling);
}

COLLECT::~COLLECT()
{
    delete com;
}

void COLLECT::init()
{
    com->setBaudRate(BAUD9600);
    com->setDataBits(DATA_8);
    com->setParity(PAR_NONE);
    com->setStopBits(STOP_1);
    com->setFlowControl(FLOW_OFF);
    com->open(QIODevice::ReadWrite);
}
