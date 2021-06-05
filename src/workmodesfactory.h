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

#ifndef WORKMODESFACTORY_H
#define WORKMODESFACTORY_H

#include <map>
#include <memory>
#include <QString>
#include "workmodes/workmodebase.h"
#include "datamodels/workmodesmodel.h"

namespace vol2com
{
    class WorkModeCreatorBase
    {
    public:
        WorkModeCreatorBase() {}
        virtual std::unique_ptr<WorkModeBase> create() const = 0;
    };

    template <class C>
    class WorkModeCreator : public WorkModeCreatorBase
    {
    public:
        virtual std::unique_ptr<WorkModeBase> create() const
        {
            return std::make_unique<C>();
        }
    };

    class WorkModesFactory
    {
    protected:
        typedef std::map<QString, std::unique_ptr<WorkModeCreatorBase> > FactoryMap;
        FactoryMap m_factory;
        std::shared_ptr<WorkModesModel> m_model;

    public:
        WorkModesFactory();
        virtual ~WorkModesFactory() {}

        template <class C>
        void add(const QString& id, const QString& name, const QString& description, const QString& image)
        {
            auto it = m_factory.find(id);
            if (it == m_factory.end())
            {
                m_factory[id] = std::make_unique<WorkModeCreator<C> >();
                m_model->add(id, name, description, image);
            }
        }

        std::unique_ptr<WorkModeBase> create(const QString& id)
        {
            auto it = m_factory.find(id);
            if (it != m_factory.end())
                return it->second->create();
            return std::unique_ptr<WorkModeBase>(nullptr);
        }

        std::shared_ptr<WorkModesModel> model() const;
    };
}

#endif // WORKMODESFACTORY_H
