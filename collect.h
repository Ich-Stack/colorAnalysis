#ifndef COM_H
#define COM_H

#include "posix_qextserialport.h"

class COLLECT
{
public:
    COLLECT(const QString &ttyPath);
    ~COLLECT();

    void init();
private:
    Posix_QextSerialPort *com;
};

#endif // COM_H
