/*
    This file is part of KOrganizer.

    Copyright (c) 2001,2002,2003 Cornelius Schumacher <schumacher@kde.org>
    Copyright (C) 2003-2004 Reinhold Kainhofer <reinhold@kainhofer.com>

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/

#include <kdebug.h>
#include <klocale.h>

#include "koglobals.h"
#include "navigatorbar.h"
#include "kdatenavigator.h"

#include <kcalendarsystem.h>
#include <kdialog.h>

#include "datenavigatorcontainer.h"

#include <qwhatsthis.h>

DateNavigatorContainer::DateNavigatorContainer( QWidget *parent,
                                                const char *name )
  : QFrame( parent, name ), mCalendar( 0 ),
    mHorizontalCount( 1 ), mVerticalCount( 1 )
{
  mExtraViews.setAutoDelete( true );
  setFrameStyle( QFrame::Sunken | QFrame::StyledPanel );

  mNavigatorView = new KDateNavigator( this, name );
  QWhatsThis::add( mNavigatorView,
                   i18n( "<qt><p>Select the dates you want to "
                         "display in KOrganizer's main view here. Hold the "
                         "mouse button to select more than one day.</p>"
                         "<p>Press the top buttons to browse to the next "
                         "/ previous months or years.</p>"
                         "<p>Each line shows a week. The number in the left "
                         "column is the number of the week in the year. "
                         "Press it to select the whole week.</p>"
                         "</qt>" ) );

  connectNavigatorView( mNavigatorView );
}

DateNavigatorContainer::~DateNavigatorContainer()
{
}

void DateNavigatorContainer::connectNavigatorView( KDateNavigator *v )
{
  connect( v, SIGNAL( datesSelected( const KCal::DateList & ) ),
           SIGNAL( datesSelected( const KCal::DateList & ) ) );
  connect( v, SIGNAL( incidenceDropped( Incidence *, const QDate & ) ),
           SIGNAL( incidenceDropped( Incidence *, const QDate & ) ) );
  connect( v, SIGNAL( incidenceDroppedMove( Incidence *, const QDate & ) ),
           SIGNAL( incidenceDroppedMove( Incidence *, const QDate & ) ) );
  connect( v, SIGNAL( weekClicked( const QDate & ) ),
           SIGNAL( weekClicked( const QDate & ) ) );

  connect( v, SIGNAL( goPrevious() ), SIGNAL( goPrevious() ) );
  connect( v, SIGNAL( goNext() ), SIGNAL( goNext() ) );

  connect( v, SIGNAL( goNextMonth() ), SIGNAL( goNextMonth() ) );
  connect( v, SIGNAL( goPrevMonth() ), SIGNAL( goPrevMonth() ) );
  connect( v, SIGNAL( goNextYear() ), SIGNAL( goNextYear() ) );
  connect( v, SIGNAL( goPrevYear() ), SIGNAL( goPrevYear() ) );

  connect( v, SIGNAL( goMonth( int ) ), SIGNAL( goMonth( int ) ) );
}

void DateNavigatorContainer::setCalendar( Calendar *cal )
{
  mCalendar = cal;
  mNavigatorView->setCalendar( cal );
  KDateNavigator *n;
  for( n = mExtraViews.first(); n; n = mExtraViews.next() ) {
    n->setCalendar( cal );
  }
}

// TODO_Recurrence: let the navigators update just once, and tell them that
// if data has changed or just the selection (because then the list of dayss
// with events doesn't have to be updated if the month stayed the same
void DateNavigatorContainer::updateDayMatrix()
{
  mNavigatorView->updateDayMatrix();
  KDateNavigator *n;
  for( n = mExtraViews.first(); n; n = mExtraViews.next() ) {
    n->updateDayMatrix();
  }
}

void DateNavigatorContainer::updateToday()
{
  mNavigatorView->updateToday();
  KDateNavigator *n;
  for( n = mExtraViews.first(); n; n = mExtraViews.next() ) {
    n->updateToday();
  }
}

void DateNavigatorContainer::updateView()
{
  mNavigatorView->updateView();
  KDateNavigator *n;
  for( n = mExtraViews.first(); n; n = mExtraViews.next() ) {
    n->updateView();
  }
}

void DateNavigatorContainer::updateConfig()
{
  mNavigatorView->updateConfig();
  KDateNavigator *n;
  for( n = mExtraViews.first(); n; n = mExtraViews.next() ) {
    n->updateConfig();
  }
}

void DateNavigatorContainer::selectDates( const DateList &dateList )
{
  mNavigatorView->selectDates( dateList );
  setBaseDates();
}

void DateNavigatorContainer::setBaseDates()
{
  KCal::DateList dateList = mNavigatorView->selectedDates();
  if ( dateList.isEmpty() ) {
    kdError() << "DateNavigatorContainer::selectDates() empty list." << endl;
  }
  QDate baseDate = dateList.first();
  KDateNavigator *n;
  for( n = mExtraViews.first(); n; n = mExtraViews.next() ) {
    baseDate = KOGlobals::self()->calendarSystem()->addMonths( baseDate, 1 );
    n->setBaseDate( baseDate );
  }
}

void DateNavigatorContainer::resizeEvent( QResizeEvent * )
{
#if 0
  kdDebug(5850) << "DateNavigatorContainer::resizeEvent()" << endl;
  kdDebug(5850) << "  CURRENT SIZE: " << size() << endl;
  kdDebug(5850) << "  MINIMUM SIZEHINT: " << minimumSizeHint() << endl;
  kdDebug(5850) << "  SIZEHINT: " << sizeHint() << endl;
  kdDebug(5850) << "  MINIMUM SIZE: " << minimumSize() << endl;
#endif

  QSize minSize = mNavigatorView->minimumSizeHint();

//  kdDebug(5850) << "  NAVIGATORVIEW minimumSizeHint: " << minSize << endl;

  int margin = KDialog::spacingHint();
  int verticalCount = ( size().height() - margin*2 ) / minSize.height();
  int horizontalCount = ( size().width() - margin*2 ) / minSize.width();

  if ( horizontalCount != mHorizontalCount ||
       verticalCount != mVerticalCount ) {
    uint count = horizontalCount * verticalCount;
    if ( count == 0 ) return;

    while ( count > ( mExtraViews.count() + 1 ) ) {
      KDateNavigator *n = new KDateNavigator( this );
      mExtraViews.append( n );
      n->setCalendar( mCalendar );
      setBaseDates();
      connectNavigatorView( n );
      n->show();
    }

    while ( count < ( mExtraViews.count() + 1 ) ) {
      mExtraViews.removeLast();
    }

    mHorizontalCount = horizontalCount;
    mVerticalCount = verticalCount;
  }

  int height = (size().height() - margin*2) / verticalCount;
  int width = (size().width() - margin*2) / horizontalCount;

  NavigatorBar *bar = mNavigatorView->navigatorBar();
  if ( horizontalCount > 1 ) bar->showButtons( true, false );
  else bar->showButtons( true, true );

  mNavigatorView->setGeometry(
      ( ( (KOGlobals::self()->reverseLayout())?(horizontalCount-1):0) * width ) + margin,
        margin, width, height );
  for( uint i = 0; i < mExtraViews.count(); ++i ) {
    int x = ( i + 1 ) % horizontalCount;
    int y = ( i + 1 ) / horizontalCount;

    KDateNavigator *view = mExtraViews.at( i );
    bar = view->navigatorBar();
    if ( y > 0 ) bar->showButtons( false, false );
    else {
        if ( x + 1 == horizontalCount ) bar->showButtons( false, true );
        else bar->showButtons( false, false );
    }
    view->setGeometry(
        ( ( (KOGlobals::self()->reverseLayout())?(horizontalCount-1-x):x) * width ) + margin,
          ( y * height ) + margin, width, height );
  }
}

QSize DateNavigatorContainer::minimumSizeHint() const
{
  int margin = KDialog::spacingHint() * 2;
  return mNavigatorView->minimumSizeHint() + QSize( margin, margin );
}

QSize DateNavigatorContainer::sizeHint() const
{
  int margin = KDialog::spacingHint() * 2;
  return mNavigatorView->sizeHint() + QSize( margin, margin );
}

#include "datenavigatorcontainer.moc"
