#include <QApplication>
#include "fydelis_calc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    a.setApplicationName("FydelisCalc");
    a.setOrganizationName("FydelisTech");
    a.setApplicationVersion("2.2");
    
    FydelisCalc w;
    w.show();
    
    return a.exec();
}