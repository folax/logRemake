#include <QApplication>
#include <QScopedPointer>
#include "logremake.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QScopedPointer<logRemake> wnd(new logRemake);
    wnd->show();

    return app.exec();
}
