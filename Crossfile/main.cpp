#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Crossfile");
    app.setWindowIcon(QIcon(":/Images/App.ico"));
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
