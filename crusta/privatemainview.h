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

#ifndef PRIVATEMAINVIEW_H
#define PRIVATEMAINVIEW_H

#include "fullscreennotifier.h"
#include "privatewebview.h"
#include "privateaddresslineedit.h"
#include "privatetabwindow.h"
#include "presentationmodenotifier.h"
#include "downloadwidget.h"
#include "downloadmanager.h"
#include "jseditor.h"

#include <QObject>
#include <QPoint>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QPushButton>
#include <QAction>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QCloseEvent>



class PrivateMainView;



class PWindow:public QWidget{
    Q_OBJECT
    void closeEvent(QCloseEvent *event);
public:
    QMenu* menu=new QMenu();
    PrivateMainView* pview;
    DownloadManager* d_manager=new DownloadManager();
};




class PrivateMainView:public QMainWindow{
    Q_OBJECT
private:
    void quit();
public:
    void closeTab(int);
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void fullScreen();
    void tabBarContext(QPoint);
    void FindText();
    void findFindWidget();
    void hideFindWidget();
    void selectAllText();
    void enterPresentationMode();
    void undoPageAction();
    void redoPageAction();
    void cutPageAction();
    void copyPageAction();
    void pastePageAction();
    void bookmarkTab();
    void bookmarkAllTabs();
    void restoreSession();
    void showPageInfo();
    void changeSpinner(int index);
public:
    PWindow* window=new PWindow();
    QTabWidget* tabWindow=new QTabWidget(this);
    QHBoxLayout* box=new QHBoxLayout();
    QHBoxLayout* side_pane=new QHBoxLayout();
    QMenuBar* menubar=new QMenuBar();
    QMenu* menu=new QMenu();
    QPushButton* add_btn=new QPushButton();
    QMenu* file_menu=new QMenu();
    QMenu* edit_menu=new QMenu();
    QMenu* view_menu=new QMenu();
    QMenu* history_menu=new QMenu();
    QMenu* bookmark_menu=new QMenu();
    QMenu* download_menu=new QMenu();
    QMenu* recently_closed=new QMenu();
    QMenu* tool_menu=new QMenu();
    QMenu* help_menu=new QMenu();
    QMenu* devTools=new QMenu();
    QAction* new_tab_action=new QAction();
    QAction* split_mode_action=new QAction();
    QAction* new_window_action=new QAction();
    QAction* closeCurrentTab=new QAction();
    QAction* open_file=new QAction();
    QAction* save_as_pdf=new QAction();
    QAction* save_page=new QAction();
    QAction* capture_screenshot=new QAction();
    QAction* exit_action=new QAction();
    QAction* undo_action=new QAction();
    QAction* redo_action=new QAction();
    QAction* cut_action=new QAction();
    QAction* copy_action=new QAction();
    QAction* paste_action=new QAction();
    QAction* selectall_action=new QAction();
    QAction* find_action=new QAction();
    QAction* speed_dial=new QAction();
    QAction* toggle_spane_action = new QAction();
    QAction* view_page_source_action=new QAction();
    QAction* zoom_in_action=new QAction();
    QAction* zoom_out_action=new QAction();
    QAction* reset_zoom_action=new QAction();
    QAction* presentation_action=new QAction();
    QAction* show_all_history=new QAction();
    QAction* clearAllHist=new QAction();
    QAction* restore_session=new QAction();
    QAction* bookmark_tab=new QAction();
    QAction* bookmark_all_tabs=new QAction();
    QAction* show_all_bookmarks=new QAction();
    QAction* show_all_downloads=new QAction();
    QAction* web_inspector_action=new QAction();
    QAction* viewSource=new QAction();
    QAction* runJsCode=new QAction();
    QAction* changUA=new QAction();
    QAction* pick_color=new QAction();
    QAction* sitei=new QAction();
    QAction* aboutCr=new QAction();
    PresentationModeNotifier* p_notifier=new PresentationModeNotifier();
    QAction* fullscreen_action=new QAction();
    int start_findwidget;
    QPushButton* close_findwidget=new QPushButton();
    QWidget* findwidget=new QWidget();
    QHBoxLayout* hbox=new QHBoxLayout();
    QCheckBox* match_case_btn=new QCheckBox();
    QLabel* label=new QLabel();
    QLineEdit* text=new QLineEdit();
    DownloadWidget* downloadWidget=new DownloadWidget();
    JsEditor* jsEditor=new JsEditor();
    QPushButton* newtabbtn=new QPushButton();
    QPageLayout currentPageLayout;
    DownloadManager* d_manager=new DownloadManager();
    PrivateMainView* psplitView;
    PrivateMainView();

    void createView();
    void showView();
    void newWindow();
    void createMenuBar();
    void createTabWindow();
    void addNormalTab();
    void viewPageSource();
    void saveAsPdf();
    void savePage();
    void showJsCodeEditor();
    void openLocalFile();
    void screenShot();
    void tabAreaDoubleClicked(int index);
    void addNewTabButton();
    void duplicateTab(QWebEngineView*);
    void reloadAllTabs();
    void closeOtherTabs(int index);
    void restoreTab(QUrl u);
    void help();
    void showBookamrks();
    void showDownloads();
    void changeUAfx();
    void splitModefx();
    void closeWindow();
    void openDebugger();
    void openUrl(QString);
    void pickColor();
};

#endif // PRIVATEMAINVIEW_H
