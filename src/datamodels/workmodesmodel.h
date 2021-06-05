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

#ifndef WORKMODESMODEL_H
#define WORKMODESMODEL_H

#include <QAbstractListModel>
#include <vector>

namespace vol2com
{
    struct WorkModesModelItemData
    {
        WorkModesModelItemData(const QString& _id, const QString& _name,
                               const QString& _description, const QVariant& _image = QVariant()) :
            id(_id),
            name(_name),
            description(_description),
            image(_image)
        {}

        const QString id;
        const QString name;
        const QString description;
        const QVariant image;
    };

    class WorkModesModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum Roles
        {
            IdRole = Qt::UserRole + 1,
            NameRole,
            DescriptionRole,
            ImageRole
        };

        WorkModesModel(QObject *parent = nullptr);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role) const override;

        void add(const QString& id, const QString& name, const QString& description, const QVariant& image = QVariant());
        void clear();

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        std::vector<WorkModesModelItemData> m_data;
    };
}

Q_DECLARE_METATYPE(vol2com::WorkModesModel*)

#endif // WORKMODESMODEL_H
