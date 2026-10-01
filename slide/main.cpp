#include <QApplication>
#include "fydelis_slide.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    a.setApplicationName("FydelisSlide");
    a.setOrganizationName("FydelisTech");
    a.setApplicationVersion("2.2");
    
    FydelisSlide w;
    w.show();
    
    return a.exec();
}