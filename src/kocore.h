/*
  This file is part of KOrganizer.

  SPDX-FileCopyrightText: 2001, 2003 Cornelius Schumacher <schumacher@kde.org>
  SPDX-FileCopyrightText: 2003-2004 Reinhold Kainhofer <reinhold@kainhofer.com>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#pragma once

#include "korganizer_core_export.h"
#include "part.h"

#include <EventViews/CalendarDecoration>

#include <KService>

namespace KIdentityManagement
{
class IdentityManager;
}

class KORGANIZER_CORE_EXPORT KOCore
{
public:
    ~KOCore();

    static KOCore *self();

    Q_REQUIRED_RESULT KService::List availableCalendarDecorations();

    EventViews::CalendarDecoration::Decoration *loadCalendarDecoration(const KService::Ptr &service);
    EventViews::CalendarDecoration::Decoration::List loadCalendarDecorations();

    void addXMLGUIClient(QWidget *, KXMLGUIClient *guiclient);
    void removeXMLGUIClient(QWidget *);
    KXMLGUIClient *xmlguiClient(QWidget *) const;

    void unloadPlugins();

    void reloadPlugins();

    KIdentityManagement::IdentityManager *identityManager();

protected:
    KOCore();
    KService::List availablePlugins(const QString &type, int pluginInterfaceVersion = -1);

private:
    static KOCore *mSelf;

    EventViews::CalendarDecoration::Decoration::List mCalendarDecorations;
    bool mCalendarDecorationsLoaded = false;

    QMap<QWidget *, KXMLGUIClient *> mXMLGUIClients;
};

