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

#include <QAbstractListModel>
#include <vector>
#include <QtQml/qqmlregistration.h>

namespace vol2com
{
  class BasicQMLModel : public QAbstractListModel
  {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

  public:
    struct BasicQMLModelItemData
    {
      QVariant text;
      QVariant value;
      QVariant image;
    };

    using value_type = BasicQMLModelItemData;
    using container_type = std::vector<value_type>;

    enum Roles
    {
        TextRole = Qt::UserRole + 1
      , ValueRole
      , ImageRole
    };

    BasicQMLModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex& = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

    Q_INVOKABLE int findIndex(const QVariant& value) const;
    void add(QVariant&& text, QVariant&& value, QVariant&& image = QVariant());
    void clear();
    void SetData(container_type data);

  protected:
    virtual QHash<int, QByteArray> roleNames() const override;

  private:
    std::vector<BasicQMLModelItemData> m_data;
  };
}

Q_DECLARE_METATYPE(vol2com::BasicQMLModel*)
