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

using namespace vol2com;

BasicQMLModel::BasicQMLModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int BasicQMLModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(m_data.size());
}

QVariant BasicQMLModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= static_cast<int>(m_data.size()))
        return QVariant();

    const auto &item = m_data[index.row()];
    switch(role)
    {
    case TextRole:
        return item.text;
    case ValueRole:
        return item.value;
    case ImageRole:
        return item.image;
    default:
        return QVariant();
    }
}

int BasicQMLModel::findIndex(const QVariant& value) const
{
    for(size_t i = 0; i < m_data.size(); ++i)
    {
        if(m_data[i].value == value)
            return static_cast<int>(i);
    }

    return -1;
}

void BasicQMLModel::add(QVariant&& text, QVariant&& value, QVariant&& image)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data.emplace_back(text, value, image);
    endInsertRows();
}

void BasicQMLModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

QHash<int, QByteArray> BasicQMLModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[ValueRole] = "value";
    roles[ImageRole] = "image";
    return roles;
}
