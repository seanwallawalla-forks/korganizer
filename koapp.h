/*
    This file is part of KOrganizer.
    Copyright (c) 1999 Preston Brown
    Copyright (c) 2000,2001 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef _KOAPP_H
#define _KOAPP_H
// $Id$

#include <kuniqueapp.h>

class KOrganizerApp : public KUniqueApplication
{
  Q_OBJECT
public:
  KOrganizerApp();
  ~KOrganizerApp();

  /** Create new instance of KOrganizer. If there is already running a
    KOrganizer only an additional main window is opened. */
  int newInstance();

private:
  /** Print events for numDays days from calendar loaded from file to screen.*/
  void displayImminent(const QString & file,int numdays);
  /** Start alarm daemon from KDE binary directory */
  void startAlarmDaemon();
  /** process calendar from file. If numDays is 0, open a new KOrganizer window,
    if is is greater print events from corresponding number of dates to the
    screen. If active is true the file is the active calendar. */
  void processCalendar(const QString & file,int numDays,bool active=false);
};

#endif
