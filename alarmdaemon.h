/* 	$Id$	 */

#ifndef _ALARMDAEMON_H
#define _ALARMDAEMON_H

#include <qlist.h>
#include <ksystemtray.h>
#include <kpopupmenu.h>
#include <dcopobject.h>

#include "calendar.h"
#include "event.h"
#include "todo.h"

class AlarmDialog;

using namespace KCal;

class AlarmDockWindow : public KSystemTray
{
    Q_OBJECT
  public:
    AlarmDockWindow(QWidget *parent = 0, const char *name = 0);
    virtual ~AlarmDockWindow();

    bool alarmsOn() { return contextMenu()->isItemChecked(itemId); }

    void addToolTip(const QString &);

  protected:
    void mousePressEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *e);

  public slots:
    void toggleAlarmsEnabled() 
    {
      contextMenu()->setItemChecked(itemId,
				  !contextMenu()->isItemChecked(itemId));
      setPixmap(contextMenu()->isItemChecked(itemId) ? dPixmap1 : dPixmap2);
    }
 
  protected:
    QPixmap dPixmap1, dPixmap2;
    int itemId;
};


class AlarmDaemon : public QObject, DCOPObject {
    Q_OBJECT
  public:
    AlarmDaemon(QObject *parent = 0, const char *name = 0);
    virtual ~AlarmDaemon();

    bool process(const QCString &fun, const QByteArray &data,
	         QCString &replyType, QByteArray &replyData);

  public slots:
    void showAlarms(QList<Event> &alarmEvents);
    void showTodoAlarms(QList<Todo> &alarmTodos);
    void reloadCal();
    void suspend(int duration);
    void showDialog();
    
  
  signals:
    void alarmSignal(QList<Event> &);
    void alarmTSignal(QList<Todo> &);
  
  private:
    AlarmDockWindow *mDocker;
    Calendar *mCalendar;
    AlarmDialog *mAlarmDialog;
    QTimer *mAlarmTimer;
    QTimer *mSuspendTimer;
};

#endif
