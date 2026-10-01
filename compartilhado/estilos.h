#ifndef ESTILOS_H
#define ESTILOS_H
#include <QString>
#include <QFile>
#include <QApplication>

// ==========================================
// 🎨 SISTEMA DE ESTILOS — FydelisOffice
// Tema Oficial FydelisTech · v2.2
// Salvador • Bahia 🇧🇷
// ==========================================
class Estilos {
public:
    // ==========================================
    // 🔵 WRITER — Tema Azul
    // ==========================================
    struct Writer {
        static QString corPrincipal()   { return "#0066CC"; }
        static QString corEscuro()      { return "#004C99"; }
        static QString corGradienteFim(){ return "#005A9E"; }
    };

    // ==========================================
    // 🟢 CALC — Tema Verde
    // ==========================================
    struct Calc {
        static QString corPrincipal()   { return "#009933"; }
        static QString corEscuro()      { return "#007A29"; }
        static QString corGradienteFim(){ return "#00802B"; }
    };

    // ==========================================
    // 🟣 SLIDE — Tema Roxo
    // ==========================================
    struct Slide {
        static QString corPrincipal()   { return "#5E35B1"; }
        static QString corEscuro()      { return "#4527A0"; }
        static QString corGradienteFim(){ return "#512DA8"; }
    };

    // ==========================================
    // 📄 Cores Comuns
    // ==========================================
    struct Geral {
        static QString corFundoJanela() { return "#FFFFFF"; }
        static QString corFundoClaro()  { return "#F9F9F9"; }
        static QString corFundoRibbon(){ return "#F3F3F3"; }
        static QString corTexto()       { return "#1A1A1A"; }
        static QString corTextoSuave()  { return "#666666"; }
        static QString corTextoClaro()  { return "#FFFFFF"; }
        static QString corBorda()       { return "#D0D0D0"; }
        static QString corBordaSuave()  { return "#E0E0E0"; }
        static QString corBordaForte()  { return "#0066CC"; }
        static QString corHover()       { return "#E6F0FA"; }
        static QString corSelecionado() { return "#0078D4"; }
    };

    // ==========================================
    // 📐 Dimensões
    // ==========================================
    struct Tamanho {
        static constexpr int BARRA_TITULO = 50;
        static constexpr int BARRA_STATUS = 24;
        static constexpr int RIBBON_ALTURA = 115;
        static constexpr int BORDA_JANELA = 3;
        static constexpr int BORDA_RAIO = 8;
        static constexpr int MARGEM_PADRAO = 20;
    };

    // ==========================================
    // 🔤 Fontes
    // ==========================================
    struct Fonte {
        static QString padrao()     { return "Calibri, 'Segoe UI', Arial, sans-serif"; }
        static QString mono()       { return "Consolas, 'Fira Code', monospace"; }
        static int     normal()     { return 12; }
        static int     media()      { return 13; }
        static int     grande()     { return 14; }
        static int     titulo()     { return 14; }
    };

    // ==========================================
    // 🛠️ Funções
    // ==========================================
    static QString carregarEstiloCompleto() {
        QString caminho = ":/compartilhado/estilos.qss";
        QFile arq(caminho);
        if (arq.open(QFile::ReadOnly | QFile::Text)) {
            return QString::fromUtf8(arq.readAll());
        }
        return estiloPadrao();
    }

    static void aplicarEstiloGlobal(QWidget* janela) {
        janela->setStyleSheet(carregarEstiloCompleto());
    }

private:
    static QString estiloPadrao() {
        return R"(
            QMainWindow {
                background: #FFFFFF;
            }
            QWidget#barraTitulo {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #0066CC, stop:1 #005A9E);
                color: #FFFFFF;
            }
            QTabWidget#RibbonTab::pane {
                background: #F3F3F3;
                border-top: 1px solid #D0D0D0;
                border-bottom: 1px solid #D0D0D0;
            }
            QTabBar::tab {
                padding: 6px 16px;
                background: #EDEDED;
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
                margin-right: 2px;
            }
            QTabBar::tab:selected {
                background: #FFFFFF;
                border-bottom: 2px solid #0066CC;
            }
            QStatusBar {
                background: #F9F9F9;
                border-top: 1px solid #E0E0E0;
                color: #666666;
            }
        )";
    }

    Estilos() = delete;
};

#endif // ESTILOS_H