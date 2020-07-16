/*
  This file is part of KOrganizer.

  Copyright (c) 2000,2001,2002,2003 Cornelius Schumacher <schumacher@kde.org>
  Copyright (C) 2003-2004 Reinhold Kainhofer <reinhold@kainhofer.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

  As a special exception, permission is given to link this program
  with any edition of Qt, and distribute the resulting executable,
  without including the source code for Qt in the source distribution.
*/
#ifndef KORG_KOPREFSDIALOG_H
#define KORG_KOPREFSDIALOG_H

#include "kcm_korganizer_export.h"
#include "kcmdesignerfields.h"
#include <QSet>
#include <KColorButton>

class QPushButton;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QLineEdit;

namespace Ui {
class KOGroupwarePrefsPage;
}

namespace Akonadi {
class CollectionComboBox;
}

class QRadioButton;

class KCM_KORGANIZER_EXPORT KOPrefsDialogMain : public KCModule
{
    Q_OBJECT
public:
    explicit KOPrefsDialogMain(QWidget *parent);

    void save() override;
    void load() override;

protected Q_SLOTS:
    void toggleEmailSettings(bool on);

private:
    QWidget *mUserEmailSettings = nullptr;
    QCheckBox *mEmailControlCenterCheckBox = nullptr;
    QLineEdit *mUserName = nullptr;
    QLineEdit *mUserEmail = nullptr;
    QCheckBox *mConfirmCheckBox = nullptr;
    QCheckBox *mDestinationCheckBox = nullptr;
    QCheckBox *mShowReminderDaemonCheckBox = nullptr;
};

class FontPreviewButton : public QWidget
{
    Q_OBJECT
public:
    explicit FontPreviewButton(const QString &labelStr, QWidget *parent = nullptr);
    void setFont(const QFont &font);
    Q_REQUIRED_RESULT QFont font() const;
    void setPreviewText(const QString &str);
Q_SIGNALS:
    void changed();
private:
    void selectFont();
    QLabel *mPreview = nullptr;
};

class KCM_KORGANIZER_EXPORT KOPrefsDialogColorsAndFonts : public KCModule
{
    Q_OBJECT
public:
    explicit KOPrefsDialogColorsAndFonts(QWidget *parent);

    void save() override;
    void load() override;

private Q_SLOTS:
    void useSystemColorToggle(bool useSystemColor);

protected Q_SLOTS:
    void updateCategories();
    void setCategoryColor();
    void updateCategoryColor();

    void updateResources();
    void setResourceColor();
    void updateResourceColor();

private:
    void slotConfigChanged();
    QComboBox *mCategoryCombo = nullptr;
    KColorButton *mCategoryButton = nullptr;
    QHash<QString, QColor> mCategoryDict;

    QCheckBox *mUseSystemColorCheckBox = nullptr;
    Akonadi::CollectionComboBox *mResourceCombo = nullptr;
    KColorButton *mResourceButton = nullptr;
    QHash<QString, QColor> mResourceDict;
    QList<KColorButton *> mButtonsDisable; // button that are disabled when using system color
    KColorButton *mAgendaBgColorButton = nullptr;
    KColorButton *mViewBgBusyColorButton = nullptr;
    KColorButton *mAgendaGridWorkHoursBackgroundColorButton = nullptr;
    KColorButton *mAgendaMarcusBainsLineLineColorButton = nullptr;
    KColorButton *mAgendaHolidaysBackgroundColorButton = nullptr;
    KColorButton *mTodoDueTodayColorButton = nullptr;
    KColorButton *mTodoOverdueColorButton = nullptr;
    KColorButton *mUnsetCategoryColorButton = nullptr;
    FontPreviewButton *mTimeBarFontButton = nullptr;
    FontPreviewButton *mMonthViewFont = nullptr;
    FontPreviewButton *mAgendaViewFont = nullptr;
    FontPreviewButton *mMarcusBainsFont = nullptr;
};

class KCM_KORGANIZER_EXPORT KOPrefsDialogGroupScheduling : public KCModule
{
    Q_OBJECT
public:
    explicit KOPrefsDialogGroupScheduling(QWidget *parent);

    void save() override;
    void load() override;

private:
    void slotConfigChanged();
    QCheckBox *mUseGroupwareCommunicationCheckBox = nullptr;
    QCheckBox *mBccBox = nullptr;
};

class KOGroupwarePrefsPage;

class KCM_KORGANIZER_EXPORT KOPrefsDialogGroupwareScheduling : public KCModule
{
    Q_OBJECT
public:
    explicit KOPrefsDialogGroupwareScheduling(QWidget *parent);
    ~KOPrefsDialogGroupwareScheduling() override;

    void save() override;
    void load() override;

private:
    void slotConfigChanged();
    Ui::KOGroupwarePrefsPage *mGroupwarePage = nullptr;
};

class KCM_KORGANIZER_EXPORT KOPrefsDialogPlugins : public KCModule
{
    Q_OBJECT
public:
    explicit KOPrefsDialogPlugins(QWidget *parent);
    ~KOPrefsDialogPlugins() override;

    void save() override;
    void load() override;

protected Q_SLOTS:
    void configure();
    void selectionChanged();
    void positioningChanged();

private:
    void slotConfigChanged();
    void buildList();
    QTreeWidget *mTreeWidget = nullptr;
    QLabel *mDescription = nullptr;
    QPushButton *mConfigureButton = nullptr;
    QGroupBox *mPositioningGroupBox = nullptr;

    QRadioButton *mPositionAgendaTop = nullptr;
    QRadioButton *mPositionAgendaBottom = nullptr;
    QTreeWidgetItem *mDecorations = nullptr;
    QTreeWidgetItem *mOthers = nullptr;
    QSet<QString> mDecorationsAtMonthViewTop;
    QSet<QString> mDecorationsAtAgendaViewTop;
    QSet<QString> mDecorationsAtAgendaViewBottom;
};



#endif
