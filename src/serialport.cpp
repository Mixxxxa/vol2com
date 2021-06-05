// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/****************************************************************************
**
** Copyright (C) 2020-2021 Mixxxxa <gelvikhmikhail@gmail.com>.
**
** This file is part of vol2com.
**
** vol2com is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
**
** vol2com is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with vol2com. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "serialport.h"

using namespace vol2com;

SerialPort::SerialPort(const QString& port, QObject* parent) :
    QSerialPort(port, parent)
{
}

SerialPort::~SerialPort()
{
    if(isOpen())
        close();
}

void SerialPort::openPort()
{
    if(this->open(QIODevice::ReadWrite))
        emit connectionSuccess();
    else
        emit connectionFailed();
}

void SerialPort::closePort()
{
    if(isOpen())
        close();
    emit connectionClosed();
}

void SerialPort::reconnect()
{
    if(isOpen())
        close();

    bool check = false;
    for(size_t i = 0; !check && (i < 3); ++i)
    {
        check = this->open(QIODevice::ReadWrite);
    }

    if(check)
        emit connectionSuccess();
    else
        emit connectionFailed();
}

void SerialPort::formatAndSend(const QByteArray &data)
{
    constexpr char DataSep = '-';
    constexpr char DataTail = '#';

    QByteArray dataToSend(data);
    dataToSend.append(DataSep);
    dataToSend.append(DataTail);
    write(dataToSend);
}
