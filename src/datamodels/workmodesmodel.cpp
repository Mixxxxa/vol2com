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

#include "workmodesmodel.h"

using namespace vol2com;

WorkModesModel::WorkModesModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int WorkModesModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return static_cast<int>(m_data.size());
}

QVariant WorkModesModel::data(const QModelIndex& index, int role) const
{
  if (const auto row = index.row();
         index.isValid()
      && row >= 0
      && row < static_cast<int>(m_data.size()))
  {
    const auto &item = m_data[static_cast<decltype(m_data)::size_type>(row)];
    switch(role)
    {
    case IdRole:          return item.id;
    case NameRole:        return item.name;
    case DescriptionRole: return item.description;
    case ImageRole:       return item.image;
    }
  }

  return {};
}

void WorkModesModel::add(const QString &id, const QString& name, const QString& description, const QVariant& image)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_data.emplace_back(id, name, description, image);
  endInsertRows();
}

void WorkModesModel::clear()
{
  beginResetModel();
  m_data.clear();
  endResetModel();
}

QHash<int, QByteArray> WorkModesModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IdRole] = "id";
  roles[NameRole] = "name";
  roles[DescriptionRole] = "description";
  roles[ImageRole] = "image";
  return roles;
}
