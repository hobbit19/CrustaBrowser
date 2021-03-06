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

#include "privatetabwindow.h"
#include "privatewebview.h"

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QFile>
#include <QMenu>
#include <QTextStream>
#include <QIODevice>
#include <QString>
#include <QWebEngineProfile>
#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QCompleter>
#include <QDir>



void PrivateTabWindow::viewHome(){
    this->view->returnPrivateView()->load(QUrl(this->view->home_page));
}

void PrivateTabWindow::updateAddrBar(){
    QString url=this->view->returnPrivateView()->url().toString();
    QDir* exec_dir=new QDir(QDir::homePath()+"/.crusta_db");
    exec_dir->cd("speeddial");
    QString forbidden;
    if(exec_dir->absolutePath().startsWith("/"))
        forbidden="file://"+exec_dir->absolutePath()+"/index.html";
    else
        forbidden="file:///"+exec_dir->absolutePath()+"/index.html";
    if(forbidden==url){
        addr_bar->setText("");
        this->addr_bar->siteinfo_btn->setIcon(QIcon(":/res/drawables/normal_site.svg"));
        this->addr_bar->siteinfo_btn->setStyleSheet("border: 1px solid #00b0e3");
        return;
    }
    QDir* viewer_file=new QDir(QCoreApplication::applicationDirPath());
    viewer_file->cd("3rd_party/pdfjs/web");
    QString forbidden_;
    if(viewer_file->absolutePath().startsWith("/"))
        forbidden_="file://"+viewer_file->absolutePath()+"/viewer.html?file";
    else
        forbidden_="file:///"+viewer_file->absolutePath()+"/viewer.html?file";
    if(forbidden_==url.split("=")[0]){
        this->addr_bar->initialize()->setText(url.split("/")[url.split("/").length()-1]);
        this->addr_bar->initialize()->setCursorPosition(0);
        return;
    }
    if(url.startsWith("https://")){
        this->addr_bar->siteinfo_btn->setIcon(QIcon(":/res/drawables/secure_site.svg"));
        this->addr_bar->siteinfo_btn->setStyleSheet("border: 1px solid #00c400");
    } else {
        this->addr_bar->siteinfo_btn->setIcon(QIcon(":/res/drawables/normal_site.svg"));
        this->addr_bar->siteinfo_btn->setStyleSheet("border: 1px solid #00b0e3");
    }
    connect(this->addr_bar->siteinfo_btn,&QPushButton::clicked,this,&PrivateTabWindow::showSiteInfo);
    this->addr_bar->initialize()->setText(url);
    this->addr_bar->initialize()->setText(url);
    this->addr_bar->initialize()->setCursorPosition(0);
}

void PrivateTabWindow::createControls(){
    QHBoxLayout* hbox=new QHBoxLayout();
    this->back_btn->setFlat(true);
    this->back_btn->setIcon(QIcon(":/res/drawables/back.svg"));
    connect(this->back_btn,&QPushButton::clicked,this->view->returnPrivateView(),&QWebEngineView::back);
    hbox->addWidget(this->back_btn);
    this->fwd_btn->setFlat(true);
    this->fwd_btn->setIcon(QIcon(":/res/drawables/forward.svg"));
    connect(this->fwd_btn,&QPushButton::clicked,this->view->returnPrivateView(),&QWebEngineView::forward);
    hbox->addWidget(this->fwd_btn);
    this->up_btn->setFlat(true);
    this->up_btn->setIcon(QIcon(":/res/drawables/up.svg"));
    connect(up_btn,&QPushButton::clicked,this,[this]{
        view->page()->runJavaScript("window.scrollTo(0,0)");
    });
    hbox->addWidget(this->up_btn);
    this->load_btn->setFlat(true);
    this->load_btn->setIcon(QIcon(":/res/drawables/reload.svg"));
    connect(this->load_btn,&QPushButton::clicked,this->view->returnPrivateView(),&QWebEngineView::reload);
    hbox->addWidget(this->load_btn);
    hbox->addWidget(this->addr_bar->initialize());
    connect(this->view->returnPrivateView(),&QWebEngineView::urlChanged,this,&PrivateTabWindow::updateAddrBar);
    connect(this->view->returnPrivateView(),&QWebEngineView::loadFinished,this,&PrivateTabWindow::updateStar);
    connect(this->addr_bar->initialize(),&QLineEdit::returnPressed,this,&PrivateTabWindow::loadUrl);
    //hbox->addWidget(this->search_bar->initialize());
    this->time_lbl->setFlat(true);
    connect(time_lbl,&QPushButton::clicked,this,&PrivateTabWindow::showLoadTime);
    hbox->addWidget(this->time_lbl);
    this->home_btn->setFlat(true);
    this->home_btn->setIcon(QIcon(":/res/drawables/home.svg"));
    this->home_btn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->home_btn,&QPushButton::customContextMenuRequested,this,&PrivateTabWindow::homeContext);
    connect(this->home_btn,&QPushButton::clicked,this,&PrivateTabWindow::viewHome);
    hbox->addWidget(this->home_btn);
    this->bookmark_btn->setFlat(true);
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark.svg"));
    connect(this->bookmark_btn,&QPushButton::clicked,this,&PrivateTabWindow::bookmarkPage);
    hbox->addWidget(this->bookmark_btn);
    this->menu_btn->setFlat(true);
    this->menu_btn->setIcon(QIcon(":/res/drawables/menu.svg"));
    hbox->addWidget(menu_btn);
    vbox->addLayout(hbox);
    vbox->addWidget(view);
    link_lbl->setParent(view);
    link_lbl->setFixedHeight(20);
    connect(view->page(),&QWebEnginePage::linkHovered, this,&PrivateTabWindow::showLinkHovered);
    vbox->addWidget(pbar);
    pbar->setMaximum(0);
    pbar->setMaximum(100);
    pbar->hide();
    pbar->setMaximumHeight(5);
    pbar->setTextVisible(false);
    pbar->setStyleSheet("QProgressBar:chunk{background-color: #00b0e3}");
    tab->setLayout(vbox);
    connect(view,&QWebEngineView::loadStarted,this,&PrivateTabWindow::loadBegin);
    connect(view,&QWebEngineView::loadFinished,this,&PrivateTabWindow::loadCompleted);
    connect(view,&QWebEngineView::loadProgress,this,&PrivateTabWindow::pageProgress);
    tab->setLayout(vbox);
    vbox->setSpacing(0);
    back_btn->setFixedSize(30,30);
    fwd_btn->setFixedSize(30,30);
    up_btn->setFixedSize(30,30);
    load_btn->setFixedSize(30,30);
    time_lbl->setFixedSize(50,30);
    home_btn->setFixedSize(30,30);
    bookmark_btn->setFixedSize(30,30);
    menu_btn->setFixedSize(30,30);
    tab->setStyleSheet("QWidget{background-color: #f0f0f0} QLineEdit{background: #fff; selection-background: #00b0e3} QPushButton{background-color: #f0f0f0} QPushButton::hover{background-color: #d0d0d0;}");
    time_lbl->setStyleSheet("color: #00b0e3");
}

QWidget* PrivateTabWindow::returnTab(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    return this->tab;
}

PrivateTabWindow* PrivateTabWindow::returnThis(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    return this;
}

void PrivateTabWindow::setWebView(PrivateWebView* v){
    view=v;
}

void PrivateTabWindow::showLinkHovered(QString url){
    if(url.isEmpty()){
        link_lbl->hide();
    }else{
        link_lbl->raise();
        link_lbl->setText(url);
        link_lbl->move(0,view->height()-20);
        link_lbl->show();
    }
}

QWidget* PrivateTabWindow::returnTab(PrivateWebView* view){
    this->vbox->setContentsMargins(0,0,0,0);
    setWebView(view);
    createControls();
    return this->tab;
}

void PrivateTabWindow::loadUrl(){
    QString text=this->addr_bar->initialize()->text();
    QStringList textList=text.split(" ");
    if(text.startsWith("javascript:")){
        QString script=text.split(":")[1];
        this->view->returnPrivateView()->page()->runJavaScript(script);
    }
    else if(textList.length()==1){
        if(text.startsWith("crusta://")||text.startsWith("file://")||(text.startsWith("view-source:"))){
            this->view->returnPrivateView()->load(QUrl(text));
        }
        else if(text.startsWith("localhost:")||text=="localhost"){
            this->view->returnPrivateView()->load(QUrl("http://"+text));
        }
        else if(text.startsWith("http://localhost")||text.startsWith("http://localhost:")){
            this->view->returnPrivateView()->load(QUrl(text));
        }
        else if(text.split('.').length()==1){
            QString searchStr=this->addr_bar->defaultSearch+QUrl().toPercentEncoding(text);
            this->view->returnPrivateView()->load(QUrl(searchStr));
        }
        else{
            if(!(text.startsWith("http://")||text.startsWith("https://"))){
                text="http://"+text;
            }
            this->view->returnPrivateView()->load(QUrl(text));
        }
    }
    else if(text.startsWith("http://")||text.startsWith("https://")){
        this->view->returnPrivateView()->load(QUrl(text));
    }
    else{
        QString searchStr=this->addr_bar->defaultSearch+QUrl().toPercentEncoding(text);
        this->view->returnPrivateView()->load(QUrl(searchStr));
    }
}

void PrivateTabWindow::bookmarkPage(){
    QDialog* bkmrk_ppup=new QDialog();
    QVBoxLayout* vbox_bkmrk=new QVBoxLayout();
    bkmrk_ppup->setLayout(vbox_bkmrk);
    QLabel* bkmrk_ppup_title=new QLabel(tr("Bookmark"));
    QString s=this->addr_bar->text();
    QFile input(QDir::homePath()+"/.crusta_db/bookmarks.txt");
    bool already_bkmrk=false;
    if (input.open(QIODevice::ReadOnly))
    {
       QTextStream in(&input);
       in.setCodec("UTF-8");
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data.count()==1)continue;
          if(data.count()==2)data.append("");
          if(data[1]==s){
              already_bkmrk=true;
              break;
          }
       }
       input.close();
    }
    bkmrk_ppup_title->setStyleSheet("font-size: 16px; color: #0f0f0f");
    bkmrk_ppup_title->setFixedHeight(30);
    vbox_bkmrk->addWidget(bkmrk_ppup_title);
    QLabel* name_lbl=new QLabel(tr("Name"));
    name_lbl->setFixedWidth(60);
    QHBoxLayout* p0hbox=new QHBoxLayout();
    p0hbox->addWidget(name_lbl);
    QLineEdit* name_edt=new QLineEdit();
    name_edt->setText(view->title());
    name_edt->setCursorPosition(0);
    p0hbox->addWidget(name_edt);
    vbox_bkmrk->addLayout(p0hbox);
    QLineEdit* description=new QLineEdit();
    description->setPlaceholderText(tr("Description"));
    vbox_bkmrk->addWidget(description);
    QPushButton* remove_btn=new QPushButton(tr("Remove"));
    QPushButton* done_btn=new QPushButton(tr("Done"));
    done_btn->setDefault(true);
    connect(done_btn,&QPushButton::clicked,bkmrk_ppup,&QDialog::accept);
    QHBoxLayout* p2hbox=new QHBoxLayout();
    p2hbox->addWidget(remove_btn);
    p2hbox->addWidget(done_btn);
    vbox_bkmrk->addLayout(p2hbox);
    bkmrk_ppup->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);
    bkmrk_ppup->setStyleSheet("QDialog{border: 1px solid #00b0e3}");
    bkmrk_ppup->setFixedSize(250,170);
    connect(remove_btn,&QPushButton::clicked,this,[this,bkmrk_ppup,description]{
        this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark.svg"));
        QString forbidden=this->view->returnPrivateView()->title().toUtf8()+">>>>>"+this->view->returnPrivateView()->url().toString().toUtf8()+">>>>>"+description->text().toUtf8();
        QFile f(QDir::homePath()+"/.crusta_db/bookmarks.txt");
        if(f.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QString s;
            QTextStream t(&f);
            t.setCodec("UTF-8");
            while(!t.atEnd())
            {
                QString line = t.readLine();
                if(line!=forbidden)
                    s.append(line + "\n");
            }
            f.resize(0);
            t << s;
            f.close();
        }
        bkmrk_ppup->reject();
        return;
    });
    if(!already_bkmrk){
        bkmrk_ppup_title->setText(tr("Bookmark Added"));
        this->bookmark_btn->setIcon(QIcon(":/res/drawables/star.svg"));
    }else{
        if (input.open(QIODevice::ReadOnly))
        {
           QTextStream in(&input);
           in.setCodec("UTF-8");
           while (!in.atEnd())
           {
              QString line = in.readLine();
              QStringList data=line.split(">>>>>");
              if(data.count()==1)continue;
              if(data.count()==2)data.append("");
              if(data[1]==s){
                  description->setText(data[2]);
                  break;
              }
           }
           input.close();
        }
    }
    bkmrk_ppup->move(this->tab->mapToGlobal(QPoint(bookmark_btn->x()-220,bookmark_btn->y()+30)));
    description->setFocus();
    bkmrk_ppup->exec();
    if(!already_bkmrk){
        QFile file(QDir::homePath()+"/.crusta_db/bookmarks.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << this->view->returnPrivateView()->title().toUtf8()+">>>>>"+this->view->returnPrivateView()->url().toString().toUtf8()+">>>>>"+description->text().toUtf8()+"\n";
        file.close();
    }
}

void PrivateTabWindow::updateStar(){
    QString s=this->addr_bar->text();
    QFile input(QDir::homePath()+"bookmarks.txt");
    if (input.open(QIODevice::ReadOnly))
    {
       QTextStream in(&input);
       in.setCodec("UTF-8");
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data.count()==1)continue;
          if(data.count()==2)data.append("");
          if(data[1]==s && s.size()!=0){
              this->bookmark_btn->setIcon(QIcon(":/res/drawables/star.svg"));
              input.close();
              return;
          }
       }
       input.close();
    }
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark.svg"));
}

void PrivateTabWindow::homeContext(const QPoint& pos){
    QMenu* menu=new QMenu();
    QAction* go_home=new QAction(tr("Home"));
    QAction* set_home=new QAction(tr("Set Home page"));
    connect(go_home,&QAction::triggered,this->view->returnPrivateView(),&PrivateWebView::home);
    connect(set_home,&QAction::triggered,this,&PrivateTabWindow::setHomePage);
    menu->addAction(go_home);
    menu->addAction(set_home);
    menu->exec(this->home_btn->mapToGlobal(pos));
}

void PrivateTabWindow::setHomePage(){
    QDialog* w=new QDialog();
    QLabel* lbl=new QLabel(tr("Home page URL"));
    QLineEdit* url=new QLineEdit();
    url->setText(this->view->returnPrivateView()->home_page);
    connect(url,&QLineEdit::returnPressed,w,&QDialog::accept);

    QCompleter* c=new QCompleter();
    QStringListModel* m=new QStringListModel();
    QStringList l;
    l.append("https://google.com");
    l.append("https://duckduckgo.com");
    l.append("https://bing.com");
    l.append("https://qwant.com");
    l.append("https://www.yandex.com");
    l.append("https://www.ecosia.org");
    l.append("https://www.baidu.com");
    m->setStringList(l);
    c->setModel(m);
    c->setFilterMode(Qt::MatchContains);
    url->setCompleter(c);

    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(lbl);
    hbox->addWidget(url);
    QHBoxLayout* h1box=new QHBoxLayout();
    QPushButton* cncl=new QPushButton(tr("Cancel"));
    QPushButton* ok=new QPushButton(tr("Save"));
    h1box->addWidget(new QLabel());
    h1box->addWidget(cncl);
    h1box->addWidget(ok);
    cncl->setFixedWidth(100);
    ok->setFixedWidth(100);
    QVBoxLayout* vbox=new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addLayout(h1box);
    w->setLayout(vbox);
    w->setFixedWidth(500);
    w->setWindowTitle("Crusta : Set Home page");
    //w->setStyleSheet("QWidget{background-color:white;color:black} QLabel{color:black} QLineEdit{border:0.5px solid black;border-radius:3px;margin-bottom: 2px;margin-top: 2px;} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    connect(cncl,&QPushButton::clicked,w,&QDialog::reject);
    connect(ok,&QPushButton::clicked,w,&QDialog::accept);
    if(w->exec()!=QDialog::Accepted){
        return;
    }
    QString new_string=url->text();
    if(url->text()=="" || url->text()=="default"){
        QDir* exec_dir=new QDir(QDir::homePath()+"/.crusta_db");
        exec_dir->cd("speeddial");
        QString forbidden;
        if(exec_dir->absolutePath().startsWith("/"))
            forbidden="file://"+exec_dir->absolutePath()+"/index.html";
        else
            forbidden="file:///"+exec_dir->absolutePath()+"/index.html";
        new_string=forbidden;
    }
    QFile f(QDir::homePath()+"/.crusta_db/settings.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        t.setCodec("UTF-8");
        while(!t.atEnd())
        {
            QString line = t.readLine();
            QStringList data=line.split(">>>>>");
            if(data[0]=="Incognito Home page")
                s.append(data[0]+">>>>>"+new_string + "\n");
            else
                s.append(line+"\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
    this->view->returnPrivateView()->home_page=new_string;
}

void PrivateTabWindow::pageProgress(int p){
    pbar->setValue(p);
}

void PrivateTabWindow::loadBegin(){
    ((PrivateWebPage*)view->page())->alertCount=0;
    ((PrivateWebPage*)view->page())->preventAlert=false;
    loadStartTime=QTime::currentTime();
    time_lbl->setText("...");
    pbar->show();
    this->load_btn->setIcon(QIcon(":/res/drawables/close.svg"));
    load_btn->disconnect();
    connect(load_btn,&QPushButton::clicked,view,&QWebEngineView::stop);
}

void PrivateTabWindow::loadCompleted(){
    int millis=QTime::currentTime().msecsTo(loadStartTime);
    millis/=10;
    time_lbl->setText(QString::number(-1*millis/100.0)+"s");
    pbar->hide();
    load_btn->disconnect();
    this->load_btn->setIcon(QIcon(":/res/drawables/reload.svg"));
    connect(load_btn,&QPushButton::clicked,view,&QWebEngineView::reload);
}

void PrivateTabWindow::showSiteInfo(){
    if(addr_bar->text().startsWith("https://")){
        QDialog* dlg=new QDialog();
        dlg->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);
        dlg->setFixedSize(250,100);
        QVBoxLayout* dvbox=new QVBoxLayout();
        dlg->setLayout(dvbox);
        QLabel* site_lbl_0=new QLabel(tr("Secure Connection"));
        site_lbl_0->setStyleSheet("font-size: 14px");
        dvbox->addWidget(site_lbl_0);
        QLabel* site_lbl_1=new QLabel(tr("Information you send or get through the site is private."));
        site_lbl_1->setWordWrap(true);
        dvbox->addWidget(site_lbl_1);
        dlg->setStyleSheet("QDialog{border: 1px solid #00b0e3}");
        dlg->move(addr_bar->siteinfo_btn->mapToGlobal(QPoint(addr_bar->siteinfo_btn->x()-30,addr_bar->siteinfo_btn->y()+20)));
        dlg->exec();
    } else {
        QDialog* dlg=new QDialog();
        dlg->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);
        dlg->setFixedSize(250,150);
        QVBoxLayout* dvbox=new QVBoxLayout();
        dlg->setLayout(dvbox);
        QLabel* site_lbl_0=new QLabel(tr("Insecure Connection"));
        site_lbl_0->setStyleSheet("font-size: 14px");
        dvbox->addWidget(site_lbl_0);
        QLabel* site_lbl_1=new QLabel(tr("The site isn't using a private connection. Someone might be able to see or change the information you send or get through this site."));
        site_lbl_1->setWordWrap(true);
        dvbox->addWidget(site_lbl_1);
        dlg->setStyleSheet("QDialog{border: 1px solid #00b0e3}");
        dlg->move(addr_bar->siteinfo_btn->mapToGlobal(QPoint(addr_bar->siteinfo_btn->x()-30,addr_bar->siteinfo_btn->y()+20)));
        dlg->exec();
    }
}

void PrivateTabWindow::showLoadTime(){
    QDialog* dlg=new QDialog();
    dlg->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);
    dlg->setFixedSize(250,110);
    QVBoxLayout* dvbox=new QVBoxLayout();
    dlg->setLayout(dvbox);
    QLabel* site_lbl_0=new QLabel(time_lbl->text());
    site_lbl_0->setStyleSheet("font-size: 14px");
    dvbox->addWidget(site_lbl_0);
    QLabel* site_lbl_1=new QLabel("the duration from the start time for the first network request, to the response time for the final request made by the website.");
    site_lbl_1->setWordWrap(true);
    dvbox->addWidget(site_lbl_1);
    dlg->setStyleSheet("QDialog{border: 1px solid #00b0e3}");
    dlg->move(time_lbl->mapToGlobal(QPoint(-140,30)));
    dlg->exec();
}
