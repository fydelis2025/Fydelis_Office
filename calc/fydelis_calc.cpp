#include "fydelis_calc.h"
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QCloseEvent>
#include <QColorDialog>
#include <QFontDialog>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QToolButton>
#include <QMouseEvent>
#include <QInputDialog> 
#include <QClipboard>
#include <QApplication>

FydelisCalc::FydelisCalc(QWidget *parent)
    : QMainWindow(parent), caminhoAtual(""), modificado(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle("FydelisCalc — Planilha sem nome");
    resize(1024, 680);
    setMinimumSize(800, 520);

    // ==========================================
    // CONTAINER EXTERNO — BORDA VERDE 💚
    // ==========================================
    QWidget* raiz = new QWidget(this);
    raiz->setObjectName("raiz");
    raiz->setStyleSheet(R"(
        #raiz {
            background: #2E7D32;  /* 💚 COR DA BORDA — VERDE */
            border-radius: 8px;
        }
    )");

    // Conteúdo interno
    QWidget* janela = new QWidget(raiz);
    janela->setObjectName("conteudo");
    janela->setStyleSheet(R"(
        #conteudo {
            background: #FFFFFF;
            border-radius: 5px;
        }
    )");

    QVBoxLayout* layRaiz = new QVBoxLayout(raiz);
    layRaiz->setContentsMargins(3, 3, 3, 3);  // Borda de 3px
    layRaiz->setSpacing(0);
    layRaiz->addWidget(janela);

    setCentralWidget(raiz);
    setMenuBar(nullptr);

    // ==========================================
    // BARRA DE TÍTULO — GRADIENTE VERDE 💚
    // ==========================================
    barraTitulo = new QWidget(janela);
    barraTitulo->setObjectName("barraTitulo");
    barraTitulo->setFixedHeight(50);
    barraTitulo->setStyleSheet(R"(
        #barraTitulo {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #2E7D32, stop:1 #1B5E20);
            border-top-left-radius: 5px;
            border-top-right-radius: 5px;
        }
        QPushButton {
            background: transparent;
            color: white;
            border: none;
            width: 50px;
            height: 50px;
            font-size: 18px;
        }
        QPushButton:hover { background: rgba(255,255,255,0.2); }
        #btnFechar:hover {
            background: #E53935;
            border-top-right-radius: 5px;
        }
    )");

    QLabel* iconeApp = new QLabel(barraTitulo);
    iconeApp->setPixmap(QIcon(":/fydelis/icon_calc.png").pixmap(32, 32)); // ✅ CALC
    iconeApp->setFixedSize(48, 50);
    iconeApp->setAlignment(Qt::AlignCenter);

    lblTitulo = new QLabel(windowTitle(), barraTitulo);
    lblTitulo->setStyleSheet("color: #FFFFFF; font-size: 14px; font-weight: 600; padding-top: 2px;");

    btnMin = new QPushButton("−", barraTitulo);
    btnMax = new QPushButton("□", barraTitulo);
    btnFechar = new QPushButton("×", barraTitulo);
    btnFechar->setObjectName("btnFechar");

    QHBoxLayout* layBarra = new QHBoxLayout(barraTitulo);
    layBarra->setContentsMargins(0, 0, 0, 0);
    layBarra->setSpacing(0);
    layBarra->addWidget(iconeApp);
    layBarra->addWidget(lblTitulo);
    layBarra->addStretch();
    layBarra->addWidget(btnMin);
    layBarra->addWidget(btnMax);
    layBarra->addWidget(btnFechar);

    // ==========================================
    // BARRA DE MENU — MANUAL 💚
    // ==========================================
    QMenuBar* barraMenu = new QMenuBar(janela);
    barraMenu->setStyleSheet(R"(
        QMenuBar {
            background: #FFFFFF;
            border-bottom: 1px solid #E0E0E0;
            padding: 2px 4px;
        }
        QMenuBar::item {
            padding: 6px 14px;
            color: #1A1A1A;
            border-radius: 3px;
        }
        QMenuBar::item:selected { 
            background: #E8F5E9;
            color: #2E7D32;
        }
    )");

    QMenu* arq = barraMenu->addMenu("&Arquivo");
	arq->addAction("&Novo", this, &FydelisCalc::novoArquivo, QKeySequence::New);
	arq->addAction("&Abrir...", this, &FydelisCalc::abrirArquivo, QKeySequence::Open);
	arq->addAction("&Salvar", this, &FydelisCalc::salvarArquivo, QKeySequence::Save);
	arq->addAction("Salvar &como...", this, &FydelisCalc::salvarComo, QKeySequence::SaveAs);
	arq->addAction("Exportar", this, &FydelisCalc::Exportar);
	arq->addAction("Importar", this, &FydelisCalc::Importar);
	arq->addAction("Imprimir", this, &FydelisCalc::Imprimir, QKeySequence::Print);
	arq->addSeparator();
	arq->addAction("Sair", this, &QWidget::close, QKeySequence::Quit);

	QMenu* edt = barraMenu->addMenu("&Editar");
	edt->addAction("Desfazer", this, &FydelisCalc::desfazer, QKeySequence::Undo);
	edt->addAction("Refazer", this, &FydelisCalc::refazer, QKeySequence::Redo);
	edt->addSeparator();
	edt->addAction("Recortar", this, &FydelisCalc::recortar, QKeySequence::Cut);
	edt->addAction("Copiar", this, &FydelisCalc::copiar, QKeySequence::Copy);
	edt->addAction("Colar", this, &FydelisCalc::colar, QKeySequence::Paste);

	QMenu* fmt = barraMenu->addMenu("&Formatar");
	fmt->addAction("Fonte...", this, &FydelisCalc::alterarFonte);
	fmt->addAction("Cor do texto...", this, &FydelisCalc::alterarCorTexto);
	
	QMenu* help = barraMenu->addMenu("&Ajuda");
	help->addAction("Ajuda", this, &FydelisCalc::sobre);

    // ==========================================
    // ✅ PLANILHA — NÃO é QTextEdit!
    // ==========================================
    planilha = new QTableWidget(50, 26, janela); // 50 linhas × 26 colunas A-Z
    planilha->setHorizontalHeaderLabels(QStringList()
        << "A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"<<"L"<<"M"
        << "N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z");
    planilha->verticalHeader()->setDefaultSectionSize(28);
    planilha->horizontalHeader()->setDefaultSectionSize(100);
    planilha->setStyleSheet(R"(
        QTableWidget {
            background: #ffffff;
            border: none;
            gridline-color: #E0E0E0;
        }
    )");
    connect(planilha, &QTableWidget::cellChanged, this, [this](){
        modificado = true;
        atualizarTitulo();
    });

    // ==========================================
    // RIBBON
    // ==========================================
    criarBarraFerramentas();
    ribbonTab = findChild<QTabWidget*>("RibbonTab");

    // ==========================================
    // LAYOUT PRINCIPAL
    // ==========================================
    QVBoxLayout* layPrincipal = new QVBoxLayout(janela);
    layPrincipal->setContentsMargins(0, 0, 0, 0);
    layPrincipal->setSpacing(0);

    layPrincipal->addWidget(barraTitulo);
    layPrincipal->addWidget(barraMenu);
    if (ribbonTab) layPrincipal->addWidget(ribbonTab);
    layPrincipal->addWidget(planilha);  // ✅ PLANILHA
	//layPrincipal->addWidget(statusBar());         // ✅ 5. Status — DENTRO!
	
	 // ✅ NOSSA BARRA DE STATUS — DENTRO da borda!
    barraStatus = new QWidget(janela);
    barraStatus->setFixedHeight(24);
    barraStatus->setStyleSheet(R"(
        background: #F0F9F2;
        border-top: 1px solid #B3E6C2;
    )");
    QHBoxLayout* layStatus = new QHBoxLayout(barraStatus);
    layStatus->setContentsMargins(8, 2, 8, 2);
    textoStatus = new QLabel("Pronto", barraStatus);
    textoStatus->setStyleSheet("color: #006622; font-size: 11px;");
    layStatus->addWidget(textoStatus);
    layStatus->addStretch(); // Empurra o texto para a esquerda
    
    layPrincipal->addWidget(barraStatus); // ✅ Inclui no layout!

    // Botões
    connect(btnFechar, &QPushButton::clicked, this, &QWidget::close);
    connect(btnMin, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(btnMax, &QPushButton::clicked, [this](){
        isMaximized() ? showNormal() : showMaximized();
    });

    aplicarEstiloOffice();
    //statusBar()->showMessage("Pronto");
}

FydelisCalc::~FydelisCalc() {}

void FydelisCalc::atualizarTitulo() {
    QString nome = caminhoAtual.isEmpty() ? "Planilha sem nome" : caminhoAtual.section('/', -1);
    QString titulo = QString("%1%2 — FydelisCalc")
        .arg(nome).arg(modificado ? " •" : "");
    setWindowTitle(titulo);
    if (lblTitulo) lblTitulo->setText(titulo);
}

bool FydelisCalc::verificarSalvo() {
    if (!modificado) return true;
    auto r = QMessageBox::warning(this, "FydelisCalc",
        "A planilha foi alterada.\nDeseja salvar?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (r == QMessageBox::Yes) return salvarAtual();
    if (r == QMessageBox::Cancel) return false;
    return true;
}

bool FydelisCalc::salvarAtual() {
    if (caminhoAtual.isEmpty()) {
        caminhoAtual = QFileDialog::getSaveFileName(this, "Salvar como", "",
            "Planilha Fydelis (*.fyd *.csv)");
        if (caminhoAtual.isEmpty()) return false;
    }
    QFile f(caminhoAtual);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar.");
        return false;
    }
    QTextStream out(&f);
    for (int l = 0; l < planilha->rowCount(); l++) {
        QStringList linha;
        for (int c = 0; c < planilha->columnCount(); c++) {
            auto item = planilha->item(l, c);
            linha << (item ? item->text() : "");
        }
        out << linha.join("\t") << "\n";
    }
    f.close();
    modificado = false;
    atualizarTitulo();
    textoStatus->setText("Salvo ✓");
    return true;
}

void FydelisCalc::criarMenus() {
    QMenu* arq = menuBar()->addMenu("&Arquivo");
    arq->addAction("&Nova", this, &FydelisCalc::novoArquivo, QKeySequence::New);
    arq->addAction("&Abrir...", this, &FydelisCalc::abrirArquivo, QKeySequence::Open);
    arq->addAction("&Salvar", this, &FydelisCalc::salvarArquivo, QKeySequence::Save);
    arq->addAction("Salvar &como...", this, &FydelisCalc::salvarComo, QKeySequence::SaveAs);
    arq->addSeparator();
    arq->addAction("&Sobre", this, &FydelisCalc::sobre);
    arq->addSeparator();
    arq->addAction("Sair", this, &QWidget::close, QKeySequence::Quit);

    QMenu* edt = menuBar()->addMenu("&Editar");
    edt->addAction("Inserir &Linha", this, &FydelisCalc::inserirLinha);
    edt->addAction("Inserir &Coluna", this, &FydelisCalc::inserirColuna);
    edt->addSeparator();
    edt->addAction("&Remover Linha", this, &FydelisCalc::removerLinha);
    edt->addAction("R&emover Coluna", this, &FydelisCalc::removerColuna);
    edt->addSeparator();
    edt->addAction("&Limpar Célula", this, &FydelisCalc::limparCelula, QKeySequence::Delete);

    QMenu* fmt = menuBar()->addMenu("&Formatar");
    fmt->addAction("Cor do &Texto...", this, &FydelisCalc::alterarCorTexto);
}

void FydelisCalc::criarBarraFerramentas() {
    //for (auto* tb : findChildren<QToolBar*>()) removeToolBar(tb);

    ribbonTab = new QTabWidget(this);
    ribbonTab->setObjectName("RibbonTab");
    ribbonTab->setFixedHeight(115);
    ribbonTab->setStyleSheet(R"(
        QTabWidget::pane {
            background: #F2F9F4;
            border-top: 1px solid #B3E6C2;
            border-bottom: 1px solid #B3E6C2;
        }
        QTabBar::tab {
            background: transparent;
            color: #333333;
            padding: 6px 14px;
            margin: 2px;
            border-top-left-radius: 3px;
            border-top-right-radius: 3px;
            font-size: 12px;
        }
        QTabBar::tab:selected {
            background: #FFFFFF;
            color: #007A29;
            font-weight: bold;
            border-bottom: 2px solid #009933;
        }
        QTabBar::tab:hover {
            background: #E6F2E9;
            color: #006622;
        }
        QToolButton {
            background: transparent;
            border: 1px solid transparent;
            border-radius: 3px;
            padding: 4px;
            color: #333333;
            font-size: 11px;
            min-width: 60px;
        }
        QToolButton:hover {
            background: #E6F2E9;
            border: 1px solid #B3E6C2;
        }
        QToolButton:pressed {
            background: #B3E6C2;
            border: 1px solid #80CC99;
        }
    )");

    auto criarPainel = [](const QString &titulo, QHBoxLayout *pai) -> QHBoxLayout* {
        QWidget *panel = new QWidget();
        QVBoxLayout *vbox = new QVBoxLayout(panel);
        vbox->setContentsMargins(2, 2, 2, 2);
        vbox->setSpacing(2);
        QHBoxLayout *hlay = new QHBoxLayout();
        hlay->setSpacing(4);
        vbox->addLayout(hlay);
        QLabel *lbl = new QLabel(titulo);
        lbl->setStyleSheet("color: #666666; font-size: 10px; font-weight: bold;");
        lbl->setAlignment(Qt::AlignCenter);
        vbox->addWidget(lbl);
        pai->addWidget(panel);
        QFrame *sep = new QFrame();
        sep->setFrameShape(QFrame::VLine);
        sep->setStyleSheet("color: #D0D0D0;");
        pai->addWidget(sep);
        return hlay;
    };

    // ABA 1: PÁGINA INICIAL
    QWidget *tabHome = new QWidget();
    QHBoxLayout *layHome = new QHBoxLayout(tabHome);
    layHome->setContentsMargins(8, 6, 8, 6);
    layHome->setSpacing(12);

    QHBoxLayout *pArquivo = criarPainel("Arquivo", layHome);
    QToolButton *btnNovo = new QToolButton();
	btnNovo->setIcon(QIcon(":/fydelis/calc/novo.png"));
    btnNovo->setText("Novo");
    btnNovo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnNovo, &QToolButton::clicked, this, &FydelisCalc::novoArquivo);
    pArquivo->addWidget(btnNovo);

    QToolButton *btnAbrir = new QToolButton();
	btnAbrir->setIcon(QIcon(":/fydelis/calc/abrir.png"));
    btnAbrir->setText("Abrir");
    btnAbrir->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnAbrir, &QToolButton::clicked, this, &FydelisCalc::abrirArquivo);
    pArquivo->addWidget(btnAbrir);

    QToolButton *btnSalvar = new QToolButton();
	btnSalvar->setIcon(QIcon(":/fydelis/calc/salvar_planilha.png"));
    btnSalvar->setText("Salvar");
    btnSalvar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnSalvar, &QToolButton::clicked, this, &FydelisCalc::salvarArquivo);
    pArquivo->addWidget(btnSalvar);

    layHome->addStretch();
    ribbonTab->addTab(tabHome, "Página Inicial");

    // ABA 2: ESTRUTURA
    QWidget *tabEditar = new QWidget();
    QHBoxLayout *layEditar = new QHBoxLayout(tabEditar);
    layEditar->setContentsMargins(8, 6, 8, 6);
    layEditar->setSpacing(12);

    QHBoxLayout *pEstrutura = criarPainel("Estrutura", layEditar);
    QToolButton *btnInsLinha = new QToolButton();
    btnInsLinha->setText("Ins. Linha");
    btnInsLinha->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnInsLinha, &QToolButton::clicked, this, &FydelisCalc::inserirLinha);
    pEstrutura->addWidget(btnInsLinha);

    QToolButton *btnInsCol = new QToolButton();
    btnInsCol->setText("Ins. Coluna");
    btnInsCol->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnInsCol, &QToolButton::clicked, this, &FydelisCalc::inserirColuna);
    pEstrutura->addWidget(btnInsCol);

    QToolButton *btnRemLinha = new QToolButton();
    btnRemLinha->setText("Rem. Linha");
    btnRemLinha->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnRemLinha, &QToolButton::clicked, this, &FydelisCalc::removerLinha);
    pEstrutura->addWidget(btnRemLinha);

    QToolButton *btnRemCol = new QToolButton();
    btnRemCol->setText("Rem. Coluna");
    btnRemCol->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnRemCol, &QToolButton::clicked, this, &FydelisCalc::removerColuna);
    pEstrutura->addWidget(btnRemCol);

    QHBoxLayout *pLimpeza = criarPainel("Limpeza", layEditar);
    QToolButton *btnLimpar = new QToolButton();
    btnLimpar->setText("Limpar");
    btnLimpar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnLimpar, &QToolButton::clicked, this, &FydelisCalc::limparCelula);
    pLimpeza->addWidget(btnLimpar);

    layEditar->addStretch();
    ribbonTab->addTab(tabEditar, "Editar");

    //setMenuWidget(ribbonTab);
}

void FydelisCalc::carregarEstilo() {
    setStyleSheet(R"(
        QMainWindow { background: #FAFAFA; }
        QMenuBar {
            background: #FFFFFF;
            border-bottom: 1px solid #E0E0E0;
            padding: 2px 4px;
        }
        QMenuBar::item {
            padding: 6px 14px;
            color: #1A1A1A;
            border-radius: 3px;
        }
        QMenuBar::item:selected { background: #E6F2E9; color: #007A29; }
        QMenu {
            background: #FFFFFF;
            border: 1px solid #D0D0D0;
            padding: 4px 0;
        }
        QMenu::item {
            padding: 7px 28px;
            color: #202020;
        }
        QMenu::item:selected { background: #E6F2E9; color: #007A29; }
        QTableWidget {
            background: #FFFFFF;
            color: #1A1A1A;
            border: none;
            gridline-color: #D9E9DE;
            selection-background-color: #CCEED6;
            selection-color: #003311;
        }
        QHeaderView::section {
            background: #E6F2E9;
            border: 1px solid #B3D9C2;
            padding: 6px;
            font-weight: bold;
            color: #005522;
        }
        QStatusBar {
            background: #F3F3F3;
            border-top: 1px solid #E0E0E0;
            color: #555555;
        }
    )");
}

void FydelisCalc::novoArquivo() {
    if (!verificarSalvo()) return;
    planilha->clearContents();
    caminhoAtual.clear();
    modificado = false;
    atualizarTitulo();
    textoStatus->setText("Planilha Criada ✓");
}

void FydelisCalc::abrirArquivo() {
    if (!verificarSalvo()) return;
    QString arq = QFileDialog::getOpenFileName(this, "Abrir Planilha", "",
        "Planilhas (*.fyd *.csv *.txt);;Todos (*)");
    if (arq.isEmpty()) return;
    QFile f(arq);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível abrir.");
        return;
    }
    planilha->clearContents();
    QTextStream in(&f);
    int linha = 0;
    while (!in.atEnd() && linha < planilha->rowCount()) {
        QStringList cols = in.readLine().split("\t");
        for (int c = 0; c < cols.size() && c < planilha->columnCount(); c++) {
            planilha->setItem(linha, c, new QTableWidgetItem(cols[c]));
        }
        linha++;
    }
    f.close();
    caminhoAtual = arq;
    modificado = false;
    atualizarTitulo();    
	textoStatus->setText("Aberto: %1");
}

void FydelisCalc::salvarArquivo() { salvarAtual(); }
void FydelisCalc::salvarComo() { caminhoAtual.clear(); salvarAtual(); }

void FydelisCalc::sobre() {
    QMessageBox::about(this, "Sobre — FydelisCalc",
        "<h2 style='color:#009933'>FydelisCalc v2.2</h2>"
        "<p>Planilha Eletrônica — FydelisOffice</p>"
        "<p>Estilo Office • Código Aberto</p>"
        "<hr style='border-color:#B3E6C2'>"
        "<p><strong>Salvador • Bahia 🇧🇷</strong></p>"
        "<p>FydelisTech OS</p>");
}

void FydelisCalc::recalcularCelula(int, int) {
    modificado = true;
    atualizarTitulo();
}

void FydelisCalc::inserirLinha() {
    planilha->insertRow(planilha->currentRow() + 1);
    modificado = true; atualizarTitulo();
}

void FydelisCalc::inserirColuna() {
    planilha->insertColumn(planilha->currentColumn() + 1);
    modificado = true; atualizarTitulo();
}

void FydelisCalc::removerLinha() {
    if (planilha->rowCount() <= 1) return;
    planilha->removeRow(planilha->currentRow());
    modificado = true; atualizarTitulo();
}

void FydelisCalc::removerColuna() {
    if (planilha->columnCount() <= 1) return;
    planilha->removeColumn(planilha->currentColumn());
    modificado = true; atualizarTitulo();
}

void FydelisCalc::limparCelula() {
    for (auto* item : planilha->selectedItems())
        item->setText("");
}

void FydelisCalc::closeEvent(QCloseEvent* e) {
    if (verificarSalvo()) e->accept();
    else e->ignore();
}

void FydelisCalc::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && barraTitulo) {
        QPoint pos = barraTitulo->mapFromParent(event->pos());
        if (barraTitulo->rect().contains(pos)) {
            arrastandoJanela = true;
            posicaoArrasto = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
    }
    QMainWindow::mousePressEvent(event);
}

void FydelisCalc::mouseMoveEvent(QMouseEvent* event) {
    if (arrastandoJanela && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - posicaoArrasto);
    }
    QMainWindow::mouseMoveEvent(event);
}

void FydelisCalc::mouseReleaseEvent(QMouseEvent*) {
    arrastandoJanela = false;
}

// ==========================================
// 📝 FORMATAÇÃO — Fonte e Estilo
// ==========================================
// ✅ Menu: abre o diálogo de escolha de fonte
void FydelisCalc::alterarFonte() {
    bool ok;
    QFont fonte = QFontDialog::getFont(&ok, QFont("Calibri", 12), this, "Escolher Fonte");
    if (ok && planilha) {
        planilha->setFont(fonte);
    }
}

// ✅ Menu: escolher tamanho via diálogo
void FydelisCalc::alterarTamanhoFonte() {
    bool ok;
    int tamanho = QInputDialog::getInt(this, "Tamanho da Fonte", "Tamanho:", 12, 6, 72, 1, &ok);
    if (ok && planilha) {
        QFont f = planilha->font();
        f.setPointSize(tamanho);
        planilha->setFont(f);
    }
}

// ✅ Menu: alternar negrito
void FydelisCalc::alterarNegrito() {
    if (!planilha) return;
    QFont f = planilha->font();
    f.setBold(!f.bold());
    planilha->setFont(f);
}

// ✅ Menu: escolher cor
void FydelisCalc::alterarCorTexto() {
    QColor cor = QColorDialog::getColor(Qt::black, this, "Cor do Texto");
    if (!cor.isValid()) return;
    auto sel = planilha->selectedItems();
    if (sel.isEmpty()) {
        planilha->setStyleSheet(QString("QTableWidget { color: %1; }").arg(cor.name()));
        return;
    }
    for (auto* item : sel) item->setForeground(QBrush(cor));
}

void FydelisCalc::desfazer() {    
	textoStatus->setText("Desfazer — ainda não implementado");
}

void FydelisCalc::refazer() {
	textoStatus->setText("Refazer — ainda não implementado");
}

void FydelisCalc::recortar() {
    copiar();
    limparCelula();
	textoStatus->setText("Recortado ✓");

}

void FydelisCalc::copiar() {
    auto sel = planilha->selectedItems();
    if (sel.isEmpty()) return;
    QString texto;
    for (int l = 0; l < planilha->rowCount(); l++) {
        QStringList linha;
        for (int c = 0; c < planilha->columnCount(); c++) {
            auto item = planilha->item(l, c);
            linha << (item ? item->text() : "");
        }
        if (!linha.join("").isEmpty())
            texto += linha.join("\t") + "\n";
    }
    QApplication::clipboard()->setText(texto);
	textoStatus->setText("Copiado");
}

void FydelisCalc::colar() {
    QString texto = QApplication::clipboard()->text();
    if (texto.isEmpty()) return;
    int linhaIni = planilha->currentRow();
    int colIni = planilha->currentColumn();
    QStringList linhas = texto.split("\n", Qt::SkipEmptyParts);
    for (int l = 0; l < linhas.size(); l++) {
        QStringList cols = linhas[l].split("\t");
        for (int c = 0; c < cols.size(); c++) {
            int r = linhaIni + l;
            int k = colIni + c;
            if (r < planilha->rowCount() && k < planilha->columnCount()) {
                if (!planilha->item(r, k))
                    planilha->setItem(r, k, new QTableWidgetItem(cols[c]));
                else
                    planilha->item(r, k)->setText(cols[c]);
            }
        }
    }
    modificado = true;
    atualizarTitulo();    
	textoStatus->setText("Colado");
}

void FydelisCalc::aplicarEstiloOffice() {
    // Aplica o estilo visual — você pode expandir depois
    setStyleSheet("");
    carregarEstilo(); // ✅ Reutiliza o estilo que já está pronto!
}

void FydelisCalc::Exportar() {
    QString caminho = QFileDialog::getSaveFileName(
        this, "Exportar Planilha", "",
        "CSV Separado por Vírgula (*.csv);;Texto Tabulado (*.txt)");
    if (caminho.isEmpty()) return;

    QFile f(caminho);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível exportar.");
        return;
    }
    QTextStream out(&f);
    // Detecta formato pela extensão
    bool ehCSV = caminho.endsWith(".csv");
    QString separador = ehCSV ? "," : "\t";

    for (int l = 0; l < planilha->rowCount(); l++) {
        QStringList linha;
        for (int c = 0; c < planilha->columnCount(); c++) {
            auto item = planilha->item(l, c);
            QString val = item ? item->text() : "";
            if (ehCSV && val.contains(separador))
                val = "\"" + val.replace("\"", "\"\"") + "\"";
            linha << val;
        }
        out << linha.join(separador) << "\n";
    }
    f.close();
	textoStatus->setText("Exportado");
}

void FydelisCalc::Importar() {
    if (!verificarSalvo()) return;
    QString caminho = QFileDialog::getOpenFileName(
        this, "Importar Arquivo", "",
        "Arquivos Suportados (*.csv *.txt *.fyd);;Todos (*)");
    if (caminho.isEmpty()) return;

    QFile f(caminho);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível ler o arquivo.");
        return;
    }
    planilha->clearContents();
    QTextStream in(&f);
    bool ehCSV = caminho.endsWith(".csv");
    QString separador = ehCSV ? "," : "\t";

    int linha = 0;
    while (!in.atEnd() && linha < planilha->rowCount()) {
        QString textoLinha = in.readLine();
        QStringList cols;
        if (ehCSV) {
            // Parser simples de CSV com aspas
            QString val;
            bool entreAspas = false;
            for (QChar ch : textoLinha) {
                if (ch == '"') entreAspas = !entreAspas;
                else if (ch == separador && !entreAspas) {
                    cols << val; val.clear();
                } else val += ch;
            }
            cols << val;
        } else {
            cols = textoLinha.split(separador);
        }
        for (int c = 0; c < cols.size() && c < planilha->columnCount(); c++) {
            if (!planilha->item(linha, c))
                planilha->setItem(linha, c, new QTableWidgetItem(cols[c]));
            else
                planilha->item(linha, c)->setText(cols[c]);
        }
        linha++;
    }
    f.close();
    caminhoAtual.clear();
    modificado = true;
    atualizarTitulo();
	textoStatus->setText("Importado");
}

void FydelisCalc::Imprimir() {
    QMessageBox::information(this, "Imprimir",
        "📄 Função de Impressão\n\n"
        "Em desenvolvimento — em breve disponível!");
}

