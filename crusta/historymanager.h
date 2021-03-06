/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017 Anmol Gautam <anmol@crustabrowser.com>
*
* THIS FILE IS A PART OF CRUSTA
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */

#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include "mainview.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPoint>
#include <QAction>
#include <QComboBox>




class HistoryManager:public QDialog{
    Q_OBJECT
public:
    QTreeWidget* display_area=new QTreeWidget();
    QPushButton* clear_all=new QPushButton(tr("Clear All"));
    QVBoxLayout* vbox=new QVBoxLayout();
    QAction* open=new QAction(tr("Open"));
    QAction* del=new QAction(tr("Remove"));
    QComboBox* date=new QComboBox();
    MainView* mview;
    void createManager();
    void showContextMenu(const QPoint& pos);
    void clearAll();
    void openUrl();
    void clearEntry();
    void setFilterDate();
    HistoryManager(MainView* m);
};




#endif // HISTORYMANAGER_H

