#include "MainWindow.h"
#include "Pane.h"
#include "PreferencesDialog.h"
#include "Properties.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Crossfile")); // necessary since Qt5
    setUnifiedTitleAndToolBarOnMac(true);
    createActionsAndMenus();

    settings = new QSettings("Free Software", "Crossfile");
    splitter = new QSplitter(this);

    fileSystemModel = new QFileSystemModel;
    fileSystemModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
    fileSystemModel->setRootPath("");
    fileSystemModel->setReadOnly(false);

    fileSystemProxyModel = new FileSystemModelFilterProxyModel();
    fileSystemProxyModel->setSourceModel(fileSystemModel);
    fileSystemProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    //fileSystemProxyModel->sort(0);

    directoryTreeView = new QTreeView(splitter);
    directoryTreeView->setModel(fileSystemProxyModel);
    directoryTreeView->setHeaderHidden(true);
    directoryTreeView->setUniformRowHeights(true);
    directoryTreeView->hideColumn(1);
    directoryTreeView->hideColumn(2);
    directoryTreeView->hideColumn(3);
    directoryTreeView->setDragDropMode(QAbstractItemView::DropOnly);
    directoryTreeView->setDefaultDropAction(Qt::MoveAction);
    directoryTreeView->setDropIndicatorShown(true);
    directoryTreeView->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
    directoryTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(directoryTreeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    treeSelectionModel = directoryTreeView->selectionModel();
    connect(treeSelectionModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(treeSelectionChanged(QModelIndex, QModelIndex)));

    leftPane = new Pane(splitter);
    rightPane = new Pane(splitter);

    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), SLOT(focusChangedSlot(QWidget*, QWidget*)));

    splitter->addWidget(directoryTreeView);
    splitter->addWidget(leftPane);
    splitter->addWidget(rightPane);
    splitter->setHandleWidth(3);
    this->setCentralWidget(splitter);
    connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)), this, SLOT(clipboardChanged()));
    restoreState();
}

//MainWindow::~MainWindow()
//{
//}

void MainWindow::focusChangedSlot(QWidget *, QWidget *now)
{
    if (now == rightPane->pathLineEdit || now == rightPane->treeView || now == rightPane->listView)
        setActivePane(rightPane);
    else if (now == leftPane->pathLineEdit || now == leftPane->treeView || now == leftPane->listView)
        setActivePane(leftPane);
}

void MainWindow::setActivePane(Pane* pane)
{
    pane->setActive(true);
    if (pane == leftPane)
        rightPane->setActive(false);
    else
        leftPane->setActive(false);
    activePane = pane;
    updateViewActions();
}

Pane* MainWindow::getActivePane()
{
    return(activePane);
}

void MainWindow::treeSelectionChanged(QModelIndex current, QModelIndex previous)
{
    QFileInfo fileInfo(fileSystemModel->fileInfo(fileSystemProxyModel->mapToSource(current)));
    if(!fileInfo.exists())
        return;
    getActivePane()->moveTo(fileInfo.filePath());
}

void MainWindow::moveTo(const QString path)
{
    QModelIndex index = fileSystemProxyModel->mapFromSource(fileSystemModel->index(path));
    treeSelectionModel->select(index, QItemSelectionModel::Select);
    getActivePane()->moveTo(path);
}

void MainWindow::showContextMenu(const QPoint& position)
{
    contextMenu->exec(directoryTreeView->mapToGlobal(position));
}

void MainWindow::cut()
{
    QModelIndexList selectionList;

    QWidget* focus(focusWidget());
    QAbstractItemView* view;
    if (focus == directoryTreeView || focus == leftPane->treeView || focus == leftPane->listView || focus == rightPane->treeView || focus == rightPane->listView){
        view =  qobject_cast<QAbstractItemView *>(focus);
        selectionList = view->selectionModel()->selectedIndexes();
    }

    if(selectionList.count() == 0)
        return;

    QApplication::clipboard()->setMimeData(fileSystemModel->mimeData(selectionList));
    pasteAction->setData(true);

    view->selectionModel()->clear();
}

void MainWindow::copy()
{
    QModelIndexList selectionList;

    QWidget* focus(focusWidget());
    QAbstractItemView* view;
    if (focus == directoryTreeView || focus == leftPane->treeView || focus == leftPane->listView || focus == rightPane->treeView || focus == rightPane->listView){
        view =  qobject_cast<QAbstractItemView *>(focus);
        selectionList = view->selectionModel()->selectedIndexes();
    }

    if(selectionList.count() == 0)
        return;



    QApplication::clipboard()->setMimeData(fileSystemModel->mimeData(selectionList));
    pasteAction->setData(false);
}

void MainWindow::paste()
{
    QWidget* focus(focusWidget());
    Qt::DropAction cutOrCopy(pasteAction->data().toBool() ? Qt::MoveAction : Qt::CopyAction);

    if (focus == leftPane->treeView || focus == leftPane->listView || focus == rightPane->treeView || focus == rightPane->listView){
        fileSystemModel->dropMimeData(QApplication::clipboard()->mimeData(), cutOrCopy, 0, 0, qobject_cast<QAbstractItemView *>(focus)->rootIndex());
    } else if (focus == directoryTreeView){
        fileSystemModel->dropMimeData(QApplication::clipboard()->mimeData(), cutOrCopy, 0, 0,  fileSystemProxyModel->mapToSource(directoryTreeView->currentIndex()));
    }
}

void MainWindow::del()
{
    QModelIndexList selectionList;
    bool yesToAll = false;
    bool ok = false;
    bool confirm = true;

    QWidget* focus(focusWidget());
    QAbstractItemView* view;
    if (focus == directoryTreeView || focus == leftPane->treeView || focus == leftPane->listView || focus == rightPane->treeView || focus == rightPane->listView){
        view = qobject_cast<QAbstractItemView *>(focus);
        selectionList = view->selectionModel()->selectedIndexes();
    }

    for(int i = 0; i < selectionList.count(); ++i)
    {
        QFileInfo file(fileSystemModel->filePath(selectionList.at(i)));
        if(file.isWritable())
        {
            if(file.isSymLink()) ok = QFile::remove(file.filePath());
            else
            {
                if(!yesToAll)
                {
                    if(confirm)
                    {
                        int answer = QMessageBox::information(this, tr("Delete file"), "Are you sure you want to delete <p><b>\"" + file.filePath() + "</b>?",QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll);
                        if(answer == QMessageBox::YesToAll)
                            yesToAll = true;
                        if(answer == QMessageBox::No)
                            return;
                    }
                }
                ok = fileSystemModel->remove(selectionList.at(i));
            }
        }
        else if(file.isSymLink())
            ok = QFile::remove(file.filePath());
    }

    if(!ok)
        QMessageBox::information(this, tr("Delete Failed"), tr("Some files could not be deleted."));
}

void MainWindow::newFolder()
{
    QAbstractItemView* currentView = qobject_cast<QAbstractItemView *>(getActivePane()->stackedWidget->currentWidget());
    QModelIndex newDir = fileSystemModel->mkdir(currentView->rootIndex(), QString("New folder"));

    if (newDir.isValid()) {
        currentView->selectionModel()->setCurrentIndex(newDir, QItemSelectionModel::ClearAndSelect);
        currentView->edit(newDir);
    }
}

void MainWindow::view()
{
    QModelIndexList selectionList;

    QWidget* focus(focusWidget());
    QString str;
    QFileSystemModel path;
    QAbstractItemView* views;
    if (focus == directoryTreeView || focus == leftPane->treeView || focus == leftPane->listView || focus == rightPane->treeView || focus == rightPane->listView){
        views =  qobject_cast<QAbstractItemView *>(focus);
        selectionList = views->selectionModel()->selectedIndexes();
    }

    if(selectionList.count() == 0)
        return;

    str = path.filePath(views->currentIndex());

    QImage image(str);
    if(image.isNull())
    {
        QMessageBox::information(this,"Image Viewer","Error Displaying image");
        return;
    }
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    view->show();
}

void MainWindow::clipboardChanged()
{
    if(QApplication::clipboard()->mimeData()->hasUrls())
        pasteAction->setEnabled(true);
    else
    {
        pasteAction->setEnabled(false);
    }
}

void MainWindow::toggleToDetailView()
{
    getActivePane()->setViewTo(Pane::TreeViewMode);
}

void MainWindow::toggleToIconView()
{
    getActivePane()->setViewTo(Pane::ListViewMode);
}

void MainWindow::toggleHidden()
{
    if(hiddenAction->isChecked())
        fileSystemModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System | QDir::Hidden);
    else
        fileSystemModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveState();
    event->accept();
}

void MainWindow::createActionsAndMenus()
// Construct actions, menus and toolbars
{
    viewAction = new QAction(QIcon::fromTheme("edit-veiw", QIcon(":/Images/View.png")), tr("View"), this);
    viewAction->setStatusTip(tr("View file"));
    //viewAction->setShortcut(QKeySequence::ZoomIn);
    viewAction->setShortcut(QKeySequence(Qt::Key_Space));
    connect(viewAction, SIGNAL(triggered()), this, SLOT(view()));

    deleteAction = new QAction(QIcon::fromTheme("edit-delete", QIcon(":/Images/Delete.ico")), tr("Delete"), this );
    deleteAction->setStatusTip(tr("Delete file"));
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));

    newFolderAction = new QAction(QIcon::fromTheme("edit-new", QIcon(":/Images/NewFolder.ico")), tr("New Folder"), this );
    newFolderAction->setStatusTip(tr("Create New Folder"));
    newFolderAction->setShortcut(QKeySequence::New);
    connect(newFolderAction, SIGNAL(triggered()), this, SLOT(newFolder()));

    preferencesAction = new QAction(QIcon::fromTheme("preferences-other", QIcon(":/Images/Preferences.ico")), tr("&Preferences..."), this );
    preferencesAction->setStatusTip(tr("Set Preferences"));
    preferencesAction->setShortcut(QKeySequence::Preferences);
    connect(preferencesAction, SIGNAL(triggered()), this, SLOT(showPreferences()));

    exitAction = new QAction(QIcon::fromTheme("application-exit", QIcon(":/Images/Exit.png")), tr("&Exit"), this );
    exitAction->setMenuRole(QAction::QuitRole);
    exitAction->setStatusTip(tr("Quit Synopson"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(QIcon::fromTheme("edit-cut", QIcon(":/Images/Cut.png")), tr("Cut"), this );
    cutAction->setStatusTip(tr("Cut file"));
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/Images/Copy.png")), tr("Copy"), this );
    copyAction->setStatusTip(tr("Copy file"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(QIcon::fromTheme("edit-paste", QIcon(":/Images/Paste.png")), tr("Paste"), this );
    pasteAction->setStatusTip(tr("Paste file"));
    pasteAction->setEnabled(false);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    detailViewAction = new QAction(QIcon::fromTheme("view-list-details", QIcon(":/Images/DetailView.ico")), tr("Detail View"), this );
    detailViewAction->setStatusTip(tr("Show list in detail"));
    detailViewAction->setCheckable(true);
    connect(detailViewAction, SIGNAL(triggered()), this, SLOT(toggleToDetailView()));

    iconViewAction = new QAction(QIcon::fromTheme("view-list-icons", QIcon(":/Images/IconView.ico")), tr("Icon View"), this );
    iconViewAction->setStatusTip(tr("Show list as icons"));
    iconViewAction->setCheckable(true);
    connect(iconViewAction, SIGNAL(triggered()), this, SLOT(toggleToIconView()));

    hiddenAction = new QAction(QIcon::fromTheme("folder-saved-search"), tr("Show Hidden"), this );
    hiddenAction->setStatusTip(tr("Show hidden items"));
    hiddenAction->setCheckable(true);
    connect(hiddenAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));

    viewActionGroup = new QActionGroup(this);
    viewActionGroup->addAction(detailViewAction);
    viewActionGroup->addAction(iconViewAction);

    aboutAction = new QAction(QIcon::fromTheme("help-about", QIcon(":/Images/About.ico")), tr("&About"), this );
    aboutAction->setStatusTip(tr("About Synopson"));
    aboutAction->setMenuRole (QAction::AboutRole);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));

    propertiesAction = new QAction(QIcon::fromTheme("document-properties", QIcon(":/Images/Properties.ico")), tr("P&roperties"), this );
    propertiesAction->setStatusTip(tr("Properties"));
    propertiesAction->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(propertiesAction, SIGNAL(triggered()), this, SLOT(showProperties()));

    menuBar = new QMenuBar(0);

    fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(newFolderAction);
    fileMenu->addAction(deleteAction);
    fileMenu->addAction(viewAction);
    fileMenu->addAction(preferencesAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    viewMenu = menuBar->addMenu(tr("&View"));
    viewMenu->addAction(detailViewAction);
    viewMenu->addAction(iconViewAction);
    viewMenu->addAction(hiddenAction);

    helpMenu = menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);

    setMenuBar(menuBar);

    toolBar = addToolBar(tr("Main"));

    contextMenu = new QMenu(this);
    contextMenu->addAction(detailViewAction);
    contextMenu->addAction(iconViewAction);
    contextMenu->addAction(propertiesAction);
    contextMenu->addSeparator();
    contextMenu->addAction(cutAction);
    contextMenu->addAction(copyAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(deleteAction);

    toolBar->addAction(detailViewAction);
    toolBar->addAction(iconViewAction);
}

void MainWindow::saveState()
{
    settings->setValue("Geometry", saveGeometry());
    settings->setValue("ShowStatusBar", statusBar()->isVisible());
    settings->setValue("ShowToolBar", toolBar->isVisible());

    settings->setValue("MainSplitterSizes", splitter->saveState());
    settings->setValue("LeftPaneActive", leftPane->isActive());

    settings->setValue("LeftPanePath", leftPane->pathLineEdit->text());
    settings->setValue("LeftPaneFileListHeader", leftPane->treeView->header()->saveState());
    settings->setValue("LeftPaneViewMode", leftPane->stackedWidget->currentIndex());

    settings->setValue("RightPanePath", rightPane->pathLineEdit->text());
    settings->setValue("RightPaneFileListHeader", rightPane->treeView->header()->saveState());
    settings->setValue("RightPaneViewMode", rightPane->stackedWidget->currentIndex());
    settings->setValue("ShowHidden", hiddenAction->isChecked());
}

void MainWindow::restoreState()
{
    restoreGeometry(settings->value("Geometry").toByteArray());
    toolBar->setVisible(settings->value("ShowToolBar", QVariant(true)).toBool());
    statusBar()->setVisible(settings->value("ShowStatusBar", QVariant(false)).toBool());
    splitter->restoreState(settings->value("MainSplitterSizes").toByteArray());
    setActivePane(settings->value("LeftPaneActive", 1).toBool() ? leftPane : rightPane);
    leftPane->treeView->header()->restoreState(settings->value("LeftPaneFileListHeader").toByteArray());
    leftPane->moveTo(settings->value("LeftPanePath", "").toString());
    leftPane->stackedWidget->setCurrentIndex(settings->value("LeftPaneViewMode", 0).toInt());
    rightPane->treeView->header()->restoreState(settings->value("RightPaneFileListHeader").toByteArray());
    rightPane->moveTo(settings->value("RightPanePath", "").toString());
    rightPane->stackedWidget->setCurrentIndex(settings->value("RightPaneViewMode", 0).toInt());
    hiddenAction->setChecked(settings->value("ShowHidden", false).toBool());
    toggleHidden();
}

void MainWindow::updateViewActions()
{
    switch (activePane->stackedWidget->currentIndex())
    {
    case Pane::TreeViewMode:
        detailViewAction->setChecked(true);
        break;
    case Pane::ListViewMode:
        iconViewAction->setChecked(true);
        break;
    }
}

void MainWindow::showAboutBox()
{
    QMessageBox::about(this, tr("About Crossfile"),
                       tr("<h2>Crossfile</h2>"
                          "<p><em>Version 1.0.0</em>"
                          "<p>File Manager<br>"
                          "Created by Victor Buryak<br>"));
}

void MainWindow::showPreferences()
{
    PreferencesDialog preferences(this);
    preferences.exec();
}

void MainWindow::showProperties()
{
    Properties properties(this);
    properties.exec();
}


//-----------------------

bool FileSystemModelFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QFileSystemModel* fileSystemModel = qobject_cast<QFileSystemModel*>(sourceModel());
    if (fileSystemModel->isDir(index0) && fileSystemModel->fileName(index0).compare("..") != 0)
        return true;
    else
        return false;
}
