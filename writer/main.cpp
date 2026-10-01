#include <QApplication>
#include "fydelis_writer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    a.setApplicationName("FydelisWriter");
    a.setOrganizationName("FydelisTech");
    a.setApplicationVersion("2.2");
    
    FydelisWriter w;
    w.show();
    
    return a.exec();
}