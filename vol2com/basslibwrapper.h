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

#pragma once

#include <QObject>
#include <array>
#include <memory>
#include <Windows.h>
#include <limits>

namespace vol2com
{
  class BasicQMLModel;

  class BassLibWrapper : public QObject
  {
    Q_OBJECT

  public:
    using value_type = uint8_t;

    static constexpr int  AvailableBands      = 20;
    static constexpr int  DefaultInputDevice  = -5;
    static constexpr int  DefaultOutputDevice = -6;
    static constexpr int  UnknownDevice = -7;
    static constexpr auto MaxValue = std::numeric_limits<value_type>::max();

    using BassLibData = std::array<value_type, AvailableBands>;

    enum class State
    {
      Idle,
      Active,
      Error
    };
    Q_ENUM(State)

    explicit BassLibWrapper();
    ~BassLibWrapper();

    static std::vector<std::pair<QString, int> > availableDevices();
    static QString getDeviceName(int deviceID);
    static int defaultInputDeviceID();
    static int defaultOutputDeviceID();

    bool start();
    void stop();

    int device() const { return m_device; }
    State state() const { return m_state; }
    std::shared_ptr<BasicQMLModel> devicesModel() const { return m_devicesModel; }

  public slots:
    void updateDevicesModel();
    void setDevice(int device);
    void load();
    void save();
    BassLibData allBands();
    uint8_t band(int band);
    void onBassEvent(const DWORD& event, const DWORD& device);

  signals:
    void deviceChanged(int device);
    void stateChanged(State state);
    void showNotification(const QString& title, const QString& message);
    //void errorOccured(int code, QString text);

  private:
    void init();
    int getRealDevice() const;
    QString getErrorText(int errorCode) const;
    void setState(const State& state);

    void onDeviceChanged();
    void onDeviceFailed(int device);

    State m_state;
    int m_device;
    float *m_fft;
    std::shared_ptr<BasicQMLModel> m_devicesModel;
  };
}

Q_DECLARE_METATYPE(DWORD)
