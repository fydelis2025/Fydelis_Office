/********************************************************************************
** Form generated from reading UI file 'fydelis_slide.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FYDELIS_SLIDE_H
#define UI_FYDELIS_SLIDE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FydelisSlide
{
public:
    QAction *acaoNovo;
    QAction *acaoAbrir;
    QAction *acaoSalvar;
    QAction *acaoApresentacao;
    QAction *acaoSair;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QListWidget *listaSlides;
    QWidget *areaSlide;
    QVBoxLayout *layoutSlide;
    QLabel *tituloSlide;
    QLabel *conteudoSlide;
    QMenuBar *menuBar;
    QMenu *menuArquivo;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FydelisSlide)
    {
        if (FydelisSlide->objectName().isEmpty())
            FydelisSlide->setObjectName("FydelisSlide");
        FydelisSlide->resize(1000, 700);
        acaoNovo = new QAction(FydelisSlide);
        acaoNovo->setObjectName("acaoNovo");
        acaoAbrir = new QAction(FydelisSlide);
        acaoAbrir->setObjectName("acaoAbrir");
        acaoSalvar = new QAction(FydelisSlide);
        acaoSalvar->setObjectName("acaoSalvar");
        acaoApresentacao = new QAction(FydelisSlide);
        acaoApresentacao->setObjectName("acaoApresentacao");
        acaoSair = new QAction(FydelisSlide);
        acaoSair->setObjectName("acaoSair");
        centralwidget = new QWidget(FydelisSlide);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        listaSlides = new QListWidget(centralwidget);
        listaSlides->setObjectName("listaSlides");
        listaSlides->setMaximumSize(QSize(180, 16777215));
        listaSlides->setIconSize(QSize(120, 90));

        horizontalLayout->addWidget(listaSlides);

        areaSlide = new QWidget(centralwidget);
        areaSlide->setObjectName("areaSlide");
        areaSlide->setStyleSheet(QString::fromUtf8("background-color: #ffffff;"));
        layoutSlide = new QVBoxLayout(areaSlide);
        layoutSlide->setObjectName("layoutSlide");
        layoutSlide->setContentsMargins(0, 0, 0, 0);
        tituloSlide = new QLabel(areaSlide);
        tituloSlide->setObjectName("tituloSlide");
        tituloSlide->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        tituloSlide->setFont(font);

        layoutSlide->addWidget(tituloSlide);

        conteudoSlide = new QLabel(areaSlide);
        conteudoSlide->setObjectName("conteudoSlide");
        conteudoSlide->setAlignment(Qt::AlignCenter);
        QFont font1;
        font1.setPointSize(14);
        conteudoSlide->setFont(font1);

        layoutSlide->addWidget(conteudoSlide);


        horizontalLayout->addWidget(areaSlide);

        FydelisSlide->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(FydelisSlide);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1000, 24));
        menuArquivo = new QMenu(menuBar);
        menuArquivo->setObjectName("menuArquivo");
        FydelisSlide->setMenuBar(menuBar);
        statusBar = new QStatusBar(FydelisSlide);
        statusBar->setObjectName("statusBar");
        FydelisSlide->setStatusBar(statusBar);

        menuBar->addAction(menuArquivo->menuAction());
        menuArquivo->addAction(acaoNovo);
        menuArquivo->addAction(acaoAbrir);
        menuArquivo->addAction(acaoSalvar);
        menuArquivo->addSeparator();
        menuArquivo->addAction(acaoApresentacao);
        menuArquivo->addSeparator();
        menuArquivo->addAction(acaoSair);

        retranslateUi(FydelisSlide);

        QMetaObject::connectSlotsByName(FydelisSlide);
    } // setupUi

    void retranslateUi(QMainWindow *FydelisSlide)
    {
        FydelisSlide->setWindowTitle(QCoreApplication::translate("FydelisSlide", "FydelisSlide \342\200\224 Apresenta\303\247\303\265es", nullptr));
        acaoNovo->setText(QCoreApplication::translate("FydelisSlide", "Nova Apresenta\303\247\303\243o", nullptr));
        acaoAbrir->setText(QCoreApplication::translate("FydelisSlide", "Abrir...", nullptr));
        acaoSalvar->setText(QCoreApplication::translate("FydelisSlide", "Salvar", nullptr));
        acaoApresentacao->setText(QCoreApplication::translate("FydelisSlide", "Exibir Apresenta\303\247\303\243o", nullptr));
        acaoSair->setText(QCoreApplication::translate("FydelisSlide", "Sair", nullptr));
        tituloSlide->setText(QCoreApplication::translate("FydelisSlide", "T\303\255tulo do Slide", nullptr));
        conteudoSlide->setText(QCoreApplication::translate("FydelisSlide", "Conte\303\272do do slide...", nullptr));
        menuArquivo->setTitle(QCoreApplication::translate("FydelisSlide", "Arquivo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FydelisSlide: public Ui_FydelisSlide {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FYDELIS_SLIDE_H
