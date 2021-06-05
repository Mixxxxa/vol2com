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

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>

namespace vol2com
{
    class SerialPort : public QSerialPort
    {
        Q_OBJECT

    public:
        SerialPort(const QString& port, QObject* parent = nullptr);
        virtual ~SerialPort();

        void openPort();
        void closePort();
        void reconnect();

    public slots:
        void formatAndSend(const QByteArray& data);

    signals:
        void connectionSuccess();
        void connectionFailed();
        void connectionClosed();
    };
}

#endif // SERIALPORT_H
