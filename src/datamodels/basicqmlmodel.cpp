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

#include "basicqmlmodel.h"

namespace vol2com
{
  BasicQMLModel::BasicQMLModel(QObject* parent)
    : QAbstractListModel(parent)
  {
  }

  int BasicQMLModel::rowCount(const QModelIndex&) const
  {
    return static_cast<int>(m_data.size());
  }

  QVariant BasicQMLModel::data(const QModelIndex& index, int role) const
  {
    if(const auto row = index.row();
           index.isValid()
        && row >= 0
        && static_cast<container_type::size_type>(index.row()) < m_data.size())
    {
      const auto &rowData = m_data[static_cast<container_type::size_type>(row)];
      switch(role)
      {
      case Roles::TextRole:  return rowData.text;
      case Roles::ValueRole: return rowData.value;
      case Roles::ImageRole: return rowData.image;
      }
    }

    return {};
  }

  int BasicQMLModel::findIndex(const QVariant& value) const
  {
    for(size_t i = 0; i < m_data.size(); ++i)
    {
      if(m_data[i].value == value)
      {
        return static_cast<int>(i);
      }
    }

    return -1;
  }

  void BasicQMLModel::add(QVariant&& text, QVariant&& value, QVariant&& image)
  {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data.emplace_back(value_type
                        {
                          .text = text,
                          .value = value,
                          .image = image
                        });
    endInsertRows();
  }

  void BasicQMLModel::clear()
  {
    beginResetModel();
    m_data.clear();
    endResetModel();
  }

  void BasicQMLModel::SetData(container_type newData)
  {
    beginResetModel();
    m_data = std::move(newData);
    endResetModel();
  }

  QHash<int, QByteArray> BasicQMLModel::roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[TextRole]  = "text";
    roles[ValueRole] = "value";
    roles[ImageRole] = "image";
    return roles;
  }
}
