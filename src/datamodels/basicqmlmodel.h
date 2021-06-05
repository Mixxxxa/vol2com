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

#ifndef BASICQMLMODEL_H
#define BASICQMLMODEL_H

#include <QAbstractListModel>
#include <vector>

namespace vol2com
{
    struct BasicQMLModelItemData
    {
        BasicQMLModelItemData(const QVariant& _text, const QVariant& _value, const QVariant& _image = QVariant()) :
            text(_text),
            value(_value),
            image(_image)
        {}

        QVariant text;
        QVariant value;
        QVariant image;
    };

    class BasicQMLModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum Roles
        {
            TextRole = Qt::UserRole + 1,
            ValueRole,
            ImageRole
        };

        BasicQMLModel(QObject *parent = nullptr);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role) const override;

        Q_INVOKABLE int findIndex(const QVariant& value) const;
        void add(QVariant&& text, QVariant&& value, QVariant&& image = QVariant());
        void clear();

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        std::vector<BasicQMLModelItemData> m_data;
    };
}

Q_DECLARE_METATYPE(vol2com::BasicQMLModel*)

#endif // BASICQMLMODEL_H
