#include "fydelis_writer.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QTabWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QCloseEvent>
#include <QFontDialog>
#include <QColorDialog>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QByteArray>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QPushButton> 
#include <QMouseEvent>
#include <QMouseEvent>
#include <QPainter> 
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QSpinBox>

FydelisWriter::FydelisWriter(QWidget *parent)
    : QMainWindow(parent), caminhoAtual(""), modificado(false)
{
	setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle("FydelisWriter — Documento sem nome");
    resize(1024, 680);
    setMinimumSize(800, 520);

    // ==========================================
    // ✅ CONTAINER EXTERNO — Garante a BORDA
    // ==========================================
    QWidget* raiz = new QWidget(this);
    raiz->setObjectName("raiz");
    raiz->setStyleSheet(R"(
        #raiz {
            background: #0066CC;  /* COR DA BORDA — AZUL */
            border-radius: 8px;
        }
    )");

    // ==========================================
    // ✅ INTERNO — Conteúdo com margem = espessura da borda
    // ==========================================
    QWidget* janela = new QWidget(raiz);
    janela->setObjectName("conteudo");
    janela->setStyleSheet(R"(
        #conteudo {
            background: #FFFFFF;
            border-radius: 5px; /* um pouco menor que a raiz */
        }
    )");

    // Layout da RAIZ: só o conteúdo interno com margem
    QVBoxLayout* layRaiz = new QVBoxLayout(raiz);
    layRaiz->setContentsMargins(3, 3, 3, 3);  // ✅ ESTA É A BORDA! 3px azul
    layRaiz->setSpacing(0);
    layRaiz->addWidget(janela);

    setCentralWidget(raiz);
    setMenuBar(nullptr);  // ✅ REMOVE menu nativo DEFINITIVAMENTE

    // ==========================================
    // BARRA DE TÍTULO — AZUL GRADIENTE
    // ==========================================
    barraTitulo = new QWidget(janela);
    barraTitulo->setObjectName("barraTitulo");
    barraTitulo->setFixedHeight(50);
    barraTitulo->setStyleSheet(R"(
        #barraTitulo {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #0066CC, stop:1 #004C99);
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
    iconeApp->setPixmap(QIcon(":/fydelis/icon_writer.png").pixmap(32, 32));
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
    // BARRA DE MENU — MANUAL
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
            background: #E5F1FB;
            color: #0066CC;
        }
    )");

    QMenu* arq = barraMenu->addMenu("&Arquivo");
    arq->addAction("&Novo", this, &FydelisWriter::novo, QKeySequence::New);
    arq->addAction("&Abrir...", this, &FydelisWriter::abrir, QKeySequence::Open);
    arq->addAction("&Salvar", this, &FydelisWriter::salvar, QKeySequence::Save);
    arq->addAction("Salvar &como...", this, &FydelisWriter::salvarComo, QKeySequence::SaveAs);
	arq->addAction("Exportar", this, &FydelisWriter::Exportar);
	arq->addAction("Importar", this, &FydelisWriter::Importar);
	arq->addAction("Imprimir", this, &FydelisWriter::Imprimir, QKeySequence::Print);
    arq->addSeparator();
    arq->addAction("Sair", this, &QWidget::close, QKeySequence::Quit);

    QMenu* edt = barraMenu->addMenu("&Editar");
    edt->addAction("Desfazer", editor, &QTextEdit::undo, QKeySequence::Undo);
    edt->addAction("Refazer", editor, &QTextEdit::redo, QKeySequence::Redo);
    edt->addSeparator();
    edt->addAction("Recortar", editor, &QTextEdit::cut, QKeySequence::Cut);
    edt->addAction("Copiar", editor, &QTextEdit::copy, QKeySequence::Copy);
    edt->addAction("Colar", editor, &QTextEdit::paste, QKeySequence::Paste);

	QMenu* mostrar = barraMenu->addMenu("&Exibir");
	mostrar->addAction("&Configurar Página...", this, &FydelisWriter::mnuConfigurarPagina); // ✅ PRIMEIRO
    mostrar->addSeparator();
    mostrar->addAction("&Layout de Impressão", this, &FydelisWriter::mnuLayoutImpressao);
    mostrar->addAction("Página &Vertical", this, &FydelisWriter::mnuPaginaVertical);
    mostrar->addAction("Página &Lado a Lado", this, &FydelisWriter::mnuPaginaLado);
    mostrar->addSeparator();
    mostrar->addAction("&Macros", this, &FydelisWriter::mnuMacro);
    mostrar->addAction("&Propriedades", this, &FydelisWriter::mnuPropriedade);
	
    QMenu* fmt = barraMenu->addMenu("&Formatar");
    fmt->addAction("Fonte...", this, &FydelisWriter::mudarFonte);
    fmt->addAction("Cor do texto...", this, &FydelisWriter::mudarCorTexto);
	
	QMenu* help = barraMenu->addMenu("&Ajuda");
	help->addAction("Ajuda", this, &FydelisWriter::sobre);

    // ==========================================
    // EDITOR + RÉGUAS
    // ==========================================
    editor = new QTextEdit(janela);
    editor->setAcceptRichText(true);
    editor->setLineWrapMode(QTextEdit::WidgetWidth);
    editor->setFont(QFont("Calibri", 12));
    editor->setStyleSheet(R"(
        QTextEdit {
            background: #ffffff; color: #1a1a1a; border: none;
            padding: 20px 30px;
        }
    )");
    connect(editor, &QTextEdit::textChanged, this, [this](){
        modificado = true;
        atualizarTitulo();
    });

    reguaSuperior = new QWidget();
    reguaSuperior->setFixedHeight(36);
    reguaSuperior->setStyleSheet("background: #F8F9FA; border-bottom: 1px solid #E0E0E0;");
    reguaSuperior->installEventFilter(this);

    reguaLateral = new QWidget();
    reguaLateral->setFixedWidth(36);
    reguaLateral->setStyleSheet("background: #F8F9FA; border-right: 1px solid #E0E0E0;");
    reguaLateral->installEventFilter(this);

    painelEditor = new QWidget();
    QHBoxLayout* layEditor = new QHBoxLayout(painelEditor);
    layEditor->setContentsMargins(0, 0, 0, 0);
    layEditor->setSpacing(0);
    layEditor->addWidget(reguaLateral);
    layEditor->addWidget(editor);

    // ==========================================
    // RIBBON
    // ==========================================
    criarBarraFerramentas();
    ribbon = findChild<QTabWidget*>("RibbonTab");

    // ==========================================
    // ✅ LAYOUT INTERNO — Ordem certa
    // ==========================================
    QVBoxLayout* layPrincipal = new QVBoxLayout(janela);
    layPrincipal->setContentsMargins(0, 0, 0, 0);
    layPrincipal->setSpacing(0);

    layPrincipal->addWidget(barraTitulo);        // 1. Barra título
    layPrincipal->addWidget(barraMenu);          // 2. Menu
    if (ribbon) layPrincipal->addWidget(ribbon); // 3. Ribbon
    layPrincipal->addWidget(reguaSuperior);      // 4. Régua superior
    layPrincipal->addWidget(painelEditor);       // 5. Editor + régua lateral
    //layPrincipal->addWidget(statusBar());        // ✅ 6. Status — DENTRO da borda!
	
	// ✅ NOSSA BARRA DE STATUS — DENTRO da borda!
    barraStatus = new QWidget(janela);
    barraStatus->setFixedHeight(24);
    barraStatus->setStyleSheet(R"(
        background: #F0F6FC;
        border-top: 1px solid #CCE0F5;
    )");
	
    QHBoxLayout* layStatus = new QHBoxLayout(barraStatus);
    layStatus->setContentsMargins(8, 2, 8, 2);
    textoStatus = new QLabel("Pronto", barraStatus);
    textoStatus->setStyleSheet("color: #004C99; font-size: 11px;");
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
	textoStatus->setText("Pronto ✓");

}

void FydelisWriter::atualizarTitulo() {
    QString nome = caminhoAtual.isEmpty() ? "Documento sem nome" : caminhoAtual.section('/', -1);
    setWindowTitle(QString("%1%2 — FydelisWriter")
        .arg(nome).arg(modificado ? " •" : ""));
}

/*void FydelisWriter::criarMenus() {
    QMenu* arq = menuBar()->addMenu("&Arquivo");
    arq->addAction("&Novo", this, &FydelisWriter::novo, QKeySequence::New);
    arq->addAction("&Abrir...", this, &FydelisWriter::abrir, QKeySequence::Open);
    arq->addAction("&Salvar", this, &FydelisWriter::salvar, QKeySequence::Save);
    arq->addAction("Salvar &como...", this, &FydelisWriter::salvarComo, QKeySequence::SaveAs);
    arq->addSeparator();
    arq->addAction("&Sobre", this, &FydelisWriter::sobre);
    arq->addSeparator();
    arq->addAction("Sair", this, &QWidget::close, QKeySequence::Quit);

    QMenu* edt = menuBar()->addMenu("&Editar");
    edt->addAction("Desfazer", editor, &QTextEdit::undo, QKeySequence::Undo);
    edt->addAction("Refazer", editor, &QTextEdit::redo, QKeySequence::Redo);
    edt->addSeparator();
    edt->addAction("Recortar", editor, &QTextEdit::cut, QKeySequence::Cut);
    edt->addAction("Copiar", editor, &QTextEdit::copy, QKeySequence::Copy);
    edt->addAction("Colar", editor, &QTextEdit::paste, QKeySequence::Paste);
    edt->addSeparator();
    edt->addAction("Selecionar tudo", editor, &QTextEdit::selectAll, QKeySequence::SelectAll);

    QMenu* fmt = menuBar()->addMenu("&Formatar");
    fmt->addAction("Fonte...", this, &FydelisWriter::mudarFonte);
    fmt->addAction("Cor do texto...", this, &FydelisWriter::mudarCorTexto);
    fmt->addSeparator();
    fmt->addAction("Negrito", this, &FydelisWriter::alterarNegrito, QKeySequence::Bold);
    fmt->addAction("Itálico", this, &FydelisWriter::alterarItalico, QKeySequence::Italic);
    fmt->addAction("Sublinhado", this, &FydelisWriter::alterarSublinhado, QKeySequence::Underline);
}*/

void FydelisWriter::criarBarraFerramentas() {
    // Remove barras antigas
    for (auto* tb : findChildren<QToolBar*>()) {
        removeToolBar(tb);
    }

    // ========== RIBBON COM ABAS (ESTILO OFFICE) ==========
    QTabWidget *ribbonTab = new QTabWidget(this);
    ribbonTab->setObjectName("RibbonTab");
    ribbonTab->setFixedHeight(115);
    ribbonTab->setStyleSheet(R"(
         QTabWidget::pane {
        background: #F0F7FF;  /* Azul clarinho — igual Calc */
        border-top: 1px solid #CCE0F5;
        border-bottom: 1px solid #CCE0F5;
    }
    QTabBar::tab {
        background: transparent;
        color: #444444;
        padding: 6px 14px;
        margin: 2px;
        border-top-left-radius: 3px;
        border-top-right-radius: 3px;
        font-size: 12px;
    }
    QTabBar::tab:selected {
        background: #FFFFFF;
        color: #0066CC;
        font-weight: bold;
        border-bottom: 3px solid #0066CC;
    }
    QTabBar::tab:hover {
        background: #E5F1FB;
        color: #004C99;
    }
    QToolButton {
        background: transparent;
        border: 1px solid transparent;
        border-radius: 3px;
        padding: 3px 6px;
        color: #333333;
        font-size: 11px;
        min-width: 50px;
        max-height: 65px;
    }
    QToolButton:hover {
        background: #E5F1FB;
        border: 1px solid #B3D1FF;
    }
    QToolButton:pressed {
        background: #C7DFFF;
        border: 1px solid #99C7F4;
    }
    )");

// Função auxiliar para painéis em grade (Área de Transferência)
    /*auto criarPainelGrade = [](const QString &titulo, QHBoxLayout *layoutPai) -> QGridLayout* {
        QWidget *panel = new QWidget();
        QVBoxLayout *vbox = new QVBoxLayout(panel);
        vbox->setContentsMargins(2, 2, 2, 2);
        vbox->setSpacing(2);
        
        QGridLayout *grid = new QGridLayout();
        grid->setContentsMargins(0, 0, 0, 0);
        grid->setHorizontalSpacing(4);
        grid->setVerticalSpacing(2);
        vbox->addLayout(grid);
        
        QLabel *lbl = new QLabel(titulo);
        lbl->setStyleSheet("color: #666666; font-size: 10px; font-weight: bold;");
        lbl->setAlignment(Qt::AlignCenter);
        vbox->addWidget(lbl);
        
        layoutPai->addWidget(panel);
        
        QFrame *sep = new QFrame();
        sep->setFrameShape(QFrame::VLine);
        sep->setFrameShadow(QFrame::Sunken);
        sep->setStyleSheet("color: #d0d0d0;");
        layoutPai->addWidget(sep);
        
        return grid;
    };*/

    // Função auxiliar para painéis lineares normais
    auto criarPainel = [](const QString &titulo, QHBoxLayout *layoutPai) -> QHBoxLayout* {
        QWidget *panel = new QWidget();
        QVBoxLayout *vbox = new QVBoxLayout(panel);
        vbox->setContentsMargins(2, 2, 2, 2);
        vbox->setSpacing(2);
        
        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->setSpacing(4);
        vbox->addLayout(hlayout);
        
        QLabel *lbl = new QLabel(titulo);
        lbl->setStyleSheet("color: #666666; font-size: 10px; font-weight: bold;");
        lbl->setAlignment(Qt::AlignCenter);
        vbox->addWidget(lbl);
        
        layoutPai->addWidget(panel);
        
        QFrame *sep = new QFrame();
        sep->setFrameShape(QFrame::VLine);
        sep->setFrameShadow(QFrame::Sunken);
        sep->setStyleSheet("color: #d0d0d0;");
        layoutPai->addWidget(sep);
        
        return hlayout;
    };

    // ==========================================
    // ABA 1: PÁGINA INICIAL
    // ==========================================
    QWidget *tabHome = new QWidget();
    QHBoxLayout *layHome = new QHBoxLayout(tabHome);
    layHome->setContentsMargins(8, 6, 8, 6);
    layHome->setSpacing(12);

    // Painel: Arquivo
    QHBoxLayout *pArquivo = criarPainel("Arquivo", layHome);
    
	QToolButton *btnCopiar = new QToolButton();
    btnCopiar->setIcon(QIcon(":/fydelis/writer/copiar.png"));
    btnCopiar->setText("Copiar");
    btnCopiar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnCopiar, &QToolButton::clicked, this, &FydelisWriter::copiar);
    pArquivo->addWidget(btnCopiar);
	
	QToolButton *btnColar = new QToolButton();
    btnColar->setIcon(QIcon(":/fydelis/writer/colar.png"));
    btnColar->setText("Colar");
    btnColar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnColar, &QToolButton::clicked, this, &FydelisWriter::colar);
    pArquivo->addWidget(btnColar);
	
	QToolButton *btnCortar = new QToolButton();
    btnCortar->setIcon(QIcon(":/fydelis/writer/cortar.png"));
    btnCortar->setText("Cortar");
    btnCortar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnCortar, &QToolButton::clicked, this, &FydelisWriter::cortar);
    pArquivo->addWidget(btnCortar);
	
    QToolButton *btnNovo = new QToolButton();
    btnNovo->setIcon(QIcon(":/fydelis/writer/novo.png"));
    btnNovo->setText("Novo");
    btnNovo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnNovo, &QToolButton::clicked, this, &FydelisWriter::novo);
    pArquivo->addWidget(btnNovo);

    QToolButton *btnAbrir = new QToolButton();
    btnAbrir->setIcon(QIcon(":/fydelis/writer/abrir.png"));
    btnAbrir->setText("Abrir");
    btnAbrir->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnAbrir, &QToolButton::clicked, this, &FydelisWriter::abrir);
    pArquivo->addWidget(btnAbrir);

    QToolButton *btnSalvar = new QToolButton();
    btnSalvar->setIcon(QIcon(":/fydelis/writer/salvar.png"));
    btnSalvar->setText("Salvar");
    btnSalvar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnSalvar, &QToolButton::clicked, this, &FydelisWriter::salvar);
    pArquivo->addWidget(btnSalvar);

    // Painel: Editar
    QHBoxLayout *pEditar = criarPainel("Editar", layHome);

    QToolButton *btnDesfazer = new QToolButton();
    btnDesfazer->setIcon(QIcon(":/fydelis/writer/desfazer.png"));
    btnDesfazer->setText("Desfazer");
    btnDesfazer->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnDesfazer, &QToolButton::clicked, editor, &QTextEdit::undo);
    pEditar->addWidget(btnDesfazer);

    QToolButton *btnRefazer = new QToolButton();
    btnRefazer->setIcon(QIcon(":/fydelis/writer/refazer.png"));
    btnRefazer->setText("Refazer");
    btnRefazer->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnRefazer, &QToolButton::clicked, editor, &QTextEdit::redo);
    pEditar->addWidget(btnRefazer);

    layHome->addStretch();
    ribbonTab->addTab(tabHome, "Página Inicial");

    // ==========================================
    // ABA 2: FORMATAÇÃO — Fonte + Tamanho + Estilos
    // ==========================================
    QWidget *tabFmt = new QWidget();
    QHBoxLayout *layFmt = new QHBoxLayout(tabFmt);
    layFmt->setContentsMargins(8, 6, 8, 6);
    layFmt->setSpacing(12);

    // Painel: Fonte
    QHBoxLayout *pFonte = criarPainel("Fonte", layFmt);

    QToolButton *btnNegrito = new QToolButton();
    btnNegrito->setIcon(QIcon(":/fydelis/writer/negrito.png"));
    btnNegrito->setText("Negrito");
    btnNegrito->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnNegrito, &QToolButton::clicked, this, &FydelisWriter::alterarNegrito);
    pFonte->addWidget(btnNegrito);

    QToolButton *btnItalico = new QToolButton();
    btnItalico->setIcon(QIcon(":/fydelis/writer/italico.png"));
    btnItalico->setText("Itálico");
    btnItalico->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnItalico, &QToolButton::clicked, this, &FydelisWriter::alterarItalico);
    pFonte->addWidget(btnItalico);

    QToolButton *btnSublinhado = new QToolButton();
    btnSublinhado->setIcon(QIcon(":/fydelis/writer/sublinhado.png"));
    btnSublinhado->setText("Sublinhado");
    btnSublinhado->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnSublinhado, &QToolButton::clicked, this, &FydelisWriter::alterarSublinhado);
    pFonte->addWidget(btnSublinhado);

    // Painel: Tamanho da Fonte
    QHBoxLayout *pTamanho = criarPainel("Tamanho", layFmt);

    QToolButton *btnAumentar = new QToolButton();
    btnAumentar->setIcon(QIcon(":/fydelis/writer/aumentar.png"));
    btnAumentar->setText("Aumentar");
    btnAumentar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnAumentar, &QToolButton::clicked, this, &FydelisWriter::aumentarFonte);
    pTamanho->addWidget(btnAumentar);

    QToolButton *btnDiminuir = new QToolButton();
    btnDiminuir->setIcon(QIcon(":/fydelis/writer/diminuir.png"));
    btnDiminuir->setText("Diminuir");
    btnDiminuir->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnDiminuir, &QToolButton::clicked, this, &FydelisWriter::diminuirFonte);
    pTamanho->addWidget(btnDiminuir);

    // Painel: Cor e Estilo
    QHBoxLayout *pEstilo = criarPainel("Estilo", layFmt);

    QToolButton *btnCorTexto = new QToolButton();
    btnCorTexto->setIcon(QIcon(":/fydelis/writer/cor_texto.png"));
    btnCorTexto->setText("Cor do\nTexto");
    btnCorTexto->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnCorTexto, &QToolButton::clicked, this, &FydelisWriter::alterarCorTexto);
    pEstilo->addWidget(btnCorTexto);

    QToolButton *btnCorFundo = new QToolButton();
    btnCorFundo->setIcon(QIcon(":/fydelis/writer/cor_fundo.png"));
    btnCorFundo->setText("Cor de\nFundo");
    btnCorFundo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnCorFundo, &QToolButton::clicked, this, &FydelisWriter::alterarCorFundo);
    pEstilo->addWidget(btnCorFundo);

    layFmt->addStretch();
    ribbonTab->addTab(tabFmt, "Formatar");

    // ==========================================
    // ABA 3: PARÁGRAFO
    // ==========================================
    QWidget *tabParagrafo = new QWidget();
    QHBoxLayout *layParagrafo = new QHBoxLayout(tabParagrafo);
    layParagrafo->setContentsMargins(8, 6, 8, 6);
    layParagrafo->setSpacing(12);

    QHBoxLayout *pAlinhar = criarPainel("Alinhamento", layParagrafo);

    QToolButton *btnEsq = new QToolButton();
    btnEsq->setIcon(QIcon(":/fydelis/esquerda.png"));
    btnEsq->setText("Esquerda");
    btnEsq->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnEsq, &QToolButton::clicked, this, &FydelisWriter::alinharEsquerda);
    pAlinhar->addWidget(btnEsq);

    QToolButton *btnCentro = new QToolButton();
    btnCentro->setIcon(QIcon(":/fydelis/writer/centro.png"));
    btnCentro->setText("Centralizar");
    btnCentro->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnCentro, &QToolButton::clicked, this, &FydelisWriter::alinharCentro);
    pAlinhar->addWidget(btnCentro);

    QToolButton *btnDir = new QToolButton();
    btnDir->setIcon(QIcon(":/fydelis/writer/direita.png"));
    btnDir->setText("Direita");
    btnDir->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnDir, &QToolButton::clicked, this, &FydelisWriter::alinharDireita);
    pAlinhar->addWidget(btnDir);

    layParagrafo->addStretch();
    ribbonTab->addTab(tabParagrafo, "Parágrafo");

    // ==========================================
    // ABA 4: INSERIR — Tabela, Imagem, Link
    // ==========================================
    QWidget *tabInserir = new QWidget();
    QHBoxLayout *layInserir = new QHBoxLayout(tabInserir);
    layInserir->setContentsMargins(8, 6, 8, 6);
    layInserir->setSpacing(12);

    QHBoxLayout *pTabela = criarPainel("Tabela", layInserir);

    QToolButton *btnTabela = new QToolButton();
    btnTabela->setIcon(QIcon(":/fydelis/writer/tabela.png"));
    btnTabela->setText("Inserir\nTabela");
    btnTabela->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnTabela, &QToolButton::clicked, this, &FydelisWriter::inserirTabela);
    pTabela->addWidget(btnTabela);

    QHBoxLayout *pElementos = criarPainel("Elementos", layInserir);

    QToolButton *btnImagem = new QToolButton();
    btnImagem->setIcon(QIcon(":/fydelis/writer/imagem.png"));
    btnImagem->setText("Imagem");
    btnImagem->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnImagem, &QToolButton::clicked, this, &FydelisWriter::inserirImagem);
    pElementos->addWidget(btnImagem);

    QToolButton *btnLink = new QToolButton();
    btnLink->setIcon(QIcon(":/fydelis/writer/link.png"));
    btnLink->setText("Link");
    btnLink->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnLink, &QToolButton::clicked, this, &FydelisWriter::inserirLink);
    pElementos->addWidget(btnLink);

    layInserir->addStretch();
    ribbonTab->addTab(tabInserir, "Inserir");

    // ==========================================
    // ABA 5: AJUDA — Sobre
    // ==========================================
    QWidget *tabAjuda = new QWidget();
    QHBoxLayout *layAjuda = new QHBoxLayout(tabAjuda);
    layAjuda->setContentsMargins(8, 6, 8, 6);
    layAjuda->setSpacing(12);

    QHBoxLayout *pSistema = criarPainel("Sistema", layAjuda);

    QToolButton *btnSobre = new QToolButton();
    btnSobre->setIcon(QIcon(":/fydelis/writer/sobre.png"));
    btnSobre->setText("Sobre");
    btnSobre->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnSobre, &QToolButton::clicked, [this](){
    QMessageBox::about(this, "Sobre — FydelisWriter",
        "<h2 style='color:#0078d7; margin: 8px 0;'>FydelisWriter v2.2</h2>"
        "<p style='font-size: 13px; color: #333;'>Editor de Texto — FydelisOffice</p>"
        "<p style='font-size: 12px; color: #666;'>Estilo Office · Código Aberto</p>"
        "<hr style='border: none; border-top: 1px solid #e0e0e0; margin: 10px 0;'>"
        "<p style='font-size: 12px; color: #0078d7; font-weight: bold;'>Salvador • Bahia 🇧🇷</p>"
        "<p style='font-size: 11px; color: #888; margin-top: 6px;'>FydelisTech OS</p>");
    });
    pSistema->addWidget(btnSobre);

    layAjuda->addStretch();
    ribbonTab->addTab(tabAjuda, "Ajuda");

    // Define o Ribbon no topo da janela
    //setMenuWidget(ribbonTab);
}

void FydelisWriter::aplicarEstiloOffice() {
    setStyleSheet(R"(
        /* ========== RIBBON — FUNDO AZUL SUAVE (IGUAL CALC) ========== */
        QTabWidget#RibbonTab::pane {
            background: #F0F7FF;  /* ✅ Era #f3f3f3 → agora azul clarinho */
            border-top: 1px solid #CCE0F5;
            border-bottom: 1px solid #CCE0F5;
        }
        
        QTabBar::tab:selected {
            background: #FFFFFF;
            color: #0066CC;
            font-weight: bold;
            border-bottom: 3px solid #0066CC; /* ✅ Linha grossa azul */
        }
        
        QTabBar::tab:hover {
            background: #E5F1FB;
            color: #004C99;
        }
        
        /* ========== BOTÕES DO RIBBON ========== */
        QToolButton {
            background: transparent;
            border: 1px solid transparent;
            border-radius: 4px;
            padding: 6px 8px;
            color: #222222;
            font-size: 11px;
            min-width: 70px;
        }
        QToolButton:hover {
            background: #E5F1FB;
            border: 1px solid #B3D1FF;
        }
        QToolButton:pressed {
            background: #C7DFFF;
            border: 1px solid #99C7F4;
        }
        
        /* ========== RESTO ========== */
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
            background: #E5F1FB;
            color: #0066CC;
        }
        QStatusBar {
            background: #F3F3F3;
            border-top: 1px solid #E0E0E0;
            color: #555555;
        }
    )");
}

bool FydelisWriter::podeFechar() {
    if (!modificado) return true;
    auto r = QMessageBox::warning(this, "FydelisWriter",
        "O documento foi alterado.\nDeseja salvar antes de fechar?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (r == QMessageBox::Yes) return salvar();
    if (r == QMessageBox::Cancel) return false;
    return true;
}

void FydelisWriter::novo() {
    if (!podeFechar()) return;
    editor->clear();
    caminhoAtual.clear();
    modificado = false;
    atualizarTitulo();
    //statusBar()->showMessage("Documento criado", 3000);
	textoStatus->setText("Documento criado ✓");

}

void FydelisWriter::abrir() {
    if (!podeFechar()) return;
    QString arq = QFileDialog::getOpenFileName(this, "Abrir Documento", "",
        "Documentos (*.txt *.md *.fyd *.html);;Todos os arquivos (*)");
    if (arq.isEmpty()) return;
    QFile f(arq);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo.");
        return;
    }
    editor->setPlainText(f.readAll());
    f.close();
    caminhoAtual = arq;
    modificado = false;
    atualizarTitulo();
    //statusBar()->showMessage(QString("Aberto: %1").arg(arq), 5000);
	textoStatus->setText("Aberto:");

}

bool FydelisWriter::salvar() {
    if (caminhoAtual.isEmpty()) return salvarComo();
    QFile f(caminhoAtual);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar o arquivo.");
        return false;
    }
    f.write(editor->toPlainText().toUtf8());
    f.close();
    modificado = false;
    atualizarTitulo();
    //statusBar()->showMessage("Salvo com sucesso ✓", 3000);
	textoStatus->setText("Salvo com sucesso ✓");

    return true;
}

bool FydelisWriter::salvarComo() {
    QString arq = QFileDialog::getSaveFileName(this, "Salvar como", "",
        "Documento Fydelis (*.fyd *.txt *.html)");
    if (arq.isEmpty()) return false;
    caminhoAtual = arq;
    return salvar();
}

void FydelisWriter::mudarFonte() {
    bool ok;
    QFont fonte = QFontDialog::getFont(&ok, editor->font(), this, "Escolher Fonte");
    if (ok) editor->setFont(fonte);
}

void FydelisWriter::mudarCorTexto() {
    QColor cor = QColorDialog::getColor(Qt::black, this, "Cor do Texto");
    if (cor.isValid()) editor->setTextColor(cor);
}

// ==========================================
// 📝 FORMATAÇÃO — Fonte e Estilo
// ==========================================
void FydelisWriter::alterarNegrito() {
    QTextCursor cursor = editor->textCursor();
    QTextCharFormat fmt;
    if (cursor.hasSelection()) {
        fmt = cursor.charFormat();
        fmt.setFontWeight(fmt.fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
        cursor.setCharFormat(fmt);
    } else {
        QFont f = editor->font();
        f.setBold(!f.bold());
        editor->setFont(f);
    }
}

void FydelisWriter::alterarItalico() {
    QTextCursor cursor = editor->textCursor();
    QTextCharFormat fmt;
    if (cursor.hasSelection()) {
        fmt = cursor.charFormat();
        fmt.setFontItalic(!fmt.fontItalic());
        cursor.setCharFormat(fmt);
    } else {
        QFont f = editor->font();
        f.setItalic(!f.italic());
        editor->setFont(f);
    }
}

void FydelisWriter::alterarSublinhado() {
    QTextCursor cursor = editor->textCursor();
    QTextCharFormat fmt;
    if (cursor.hasSelection()) {
        fmt = cursor.charFormat();
        fmt.setFontUnderline(!fmt.fontUnderline());
        cursor.setCharFormat(fmt);
    } else {
        QFont f = editor->font();
        f.setUnderline(!f.underline());
        editor->setFont(f);
    }
}

void FydelisWriter::aumentarFonte() {
    QTextCursor cursor = editor->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat fmt = cursor.charFormat();
        int novo = fmt.fontPointSize() + 2;
        if (novo >= 8) {
            fmt.setFontPointSize(novo);
            cursor.setCharFormat(fmt);
        }
    } else {
        QFont f = editor->font();
        int novo = f.pointSize() + 2;
        if (novo >= 8) {
            f.setPointSize(novo);
            editor->setFont(f);
        }
    }
}

void FydelisWriter::diminuirFonte() {
    QTextCursor cursor = editor->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat fmt = cursor.charFormat();
        int atual = fmt.fontPointSize();
        if (atual <= 0) atual = 12;
        int novo = atual - 2;
        if (novo >= 8) {
            fmt.setFontPointSize(novo);
            cursor.setCharFormat(fmt);
        }
    } else {
        QFont f = editor->font();
        int novo = f.pointSize() - 2;
        if (novo >= 8) {
            f.setPointSize(novo);
            editor->setFont(f);
        }
    }
}

void FydelisWriter::alterarCorTexto() {
    QColor cor = QColorDialog::getColor(Qt::black, this, "Cor do Texto");
    if (cor.isValid()) {
        QTextCursor cursor = editor->textCursor();
        QTextCharFormat fmt;
        fmt.setForeground(cor);
        if (cursor.hasSelection())
            cursor.setCharFormat(fmt);
        else
            editor->setTextColor(cor);
    }
}

void FydelisWriter::alterarCorFundo() {
    QColor cor = QColorDialog::getColor(Qt::white, this, "Cor de Fundo");
    if (cor.isValid()) {
        QTextCursor cursor = editor->textCursor();
        QTextCharFormat fmt;
        fmt.setBackground(cor);
        if (cursor.hasSelection())
            cursor.setCharFormat(fmt);
        else
            editor->setTextBackgroundColor(cor);
    }
}

// ==========================================
// 📝 PARÁGRAFO — Alinhamento
// ==========================================
void FydelisWriter::alinharEsquerda() {
    QTextCursor cursor = editor->textCursor();
    QTextBlockFormat fmt;
    fmt.setAlignment(Qt::AlignLeft);
    cursor.setBlockFormat(fmt);
}

void FydelisWriter::alinharCentro() {
    QTextCursor cursor = editor->textCursor();
    QTextBlockFormat fmt;
    fmt.setAlignment(Qt::AlignCenter);
    cursor.setBlockFormat(fmt);
}

void FydelisWriter::alinharDireita() {
    QTextCursor cursor = editor->textCursor();
    QTextBlockFormat fmt;
    fmt.setAlignment(Qt::AlignRight);
    cursor.setBlockFormat(fmt);
}

// ==========================================
// 📝 INSERIR — Tabela, Imagem, Link
// ==========================================
void FydelisWriter::inserirTabela() {
    bool ok;
    int linhas = QInputDialog::getInt(this, "Inserir Tabela", "Número de linhas:", 3, 1, 20, 1, &ok);
    if (!ok) return;
    int cols = QInputDialog::getInt(this, "Inserir Tabela", "Número de colunas:", 3, 1, 20, 1, &ok);
    if (!ok) return;
    QTextCursor cursor = editor->textCursor();
    cursor.insertTable(linhas, cols);
}

void FydelisWriter::inserirImagem() {
    QString arq = QFileDialog::getOpenFileName(
        this, "Inserir Imagem", "",
        "Imagens (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (arq.isEmpty()) return;
    QTextCursor cursor = editor->textCursor();
    cursor.insertImage(arq);
}

void FydelisWriter::inserirLink() {
    bool ok;
    QString url = QInputDialog::getText(this, "Inserir Link", "Endereço (URL):",
                                         QLineEdit::Normal, "https://", &ok);
    if (!ok || url.isEmpty()) return;
    QTextCursor cursor = editor->textCursor();
    QTextCharFormat fmt;
    fmt.setAnchor(true);
    fmt.setAnchorHref(url);
    fmt.setForeground(Qt::blue);
    fmt.setFontUnderline(true);
    cursor.setCharFormat(fmt);
}

// ==========================================
// 📌 AJUDA — Sobre
// ==========================================
void FydelisWriter::sobre() {
    QMessageBox::about(this, "Sobre — FydelisWriter",
        "<h2 style='color:#0078d7; margin: 8px 0;'>FydelisWriter v2.2</h2>"
        "<p style='font-size: 13px; color: #333;'>Editor de Texto — FydelisOffice</p>"
        "<p style='font-size: 12px; color: #666;'>Estilo Office · Código Aberto</p>"
        "<hr style='border: none; border-top: 1px solid #e0e0e0; margin: 10px 0;'>"
        "<p style='font-size: 12px; color: #0078d7; font-weight: bold;'>Salvador • Bahia 🇧🇷</p>"
        "<p style='font-size: 11px; color: #888; margin-top: 6px;'>FydelisTech OS</p>");
}

// ==========================================
// 🚪 FECHAR JANELA — Verificar alterações
// ==========================================
void FydelisWriter::closeEvent(QCloseEvent *event) {
    if (textoModificado) {
        QMessageBox::StandardButton resposta = QMessageBox::question(
            this, "FydelisWriter",
            "O documento foi modificado.\nDeseja salvar antes de fechar?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (resposta == QMessageBox::Yes) {
            salvar();
            event->accept();
        } else if (resposta == QMessageBox::No) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

void FydelisWriter::copiar() { editor->copy(); }
void FydelisWriter::colar() { editor->paste(); }
void FydelisWriter::cortar() { editor->cut(); }

// ==========================================
// ✅ ARRASTAR JANELA — Clique na barra
// ==========================================
void FydelisWriter::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (barraTitulo) {
            QPoint posLocal = barraTitulo->mapFromParent(event->pos());
            if (barraTitulo->rect().contains(posLocal)) {
                arrastandoJanela = true;
                posicaoArrasto = event->globalPosition().toPoint() - frameGeometry().topLeft();
            }
        }
    }
    QMainWindow::mousePressEvent(event);
}

// ==========================================
// ✅ ARRASTAR JANELA — Movimento
// ==========================================
void FydelisWriter::mouseMoveEvent(QMouseEvent* event) {
    if (arrastandoJanela && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - posicaoArrasto);
        return;
    }
    QMainWindow::mouseMoveEvent(event);
}

// ==========================================
// ✅ ARRASTAR JANELA — Soltar clique
// ==========================================
void FydelisWriter::mouseReleaseEvent(QMouseEvent* event) {
    arrastandoJanela = false;
    QMainWindow::mouseReleaseEvent(event);
}

// ==========================================
// ✅ DESENHO DAS RÉGUAS
// ==========================================
bool FydelisWriter::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Paint) {
        const int margemEsq = 32;
        const int pxPorCm = 36;

        // RÉGUA SUPERIOR
        if (obj == reguaSuperior && reguaSuperior) {
            QPainter p(reguaSuperior);
            p.fillRect(reguaSuperior->rect(), QColor(248, 249, 250));
            p.setPen(QColor(80, 80, 80));
            p.setFont(QFont("Segoe UI", 8));

            int larg = reguaSuperior->width();
            for (int cm = 0; cm <= 24; cm++) {
                int x = margemEsq + cm * pxPorCm;
                if (x > larg) break;
                if (cm % 2 == 0) {
                    p.drawText(x + 2, 14, QString::number(cm));
                    p.drawLine(x, 20, x, 32);
                } else {
                    p.drawLine(x, 24, x, 32);
                }
                p.drawLine(x + pxPorCm/2, 28, x + pxPorCm/2, 32);
            }
            p.setPen(QPen(QColor(0, 102, 204), 2));
            p.drawLine(margemEsq, 0, margemEsq, 36);
            return true;
        }

        // RÉGUA LATERAL
        if (obj == reguaLateral && reguaLateral) {
            QPainter p(reguaLateral);
            p.fillRect(reguaLateral->rect(), QColor(248, 249, 250));
            p.setPen(QColor(80, 80, 80));
            p.setFont(QFont("Segoe UI", 8));

            int alt = reguaLateral->height();
            for (int cm = 0; cm <= 30; cm++) {
                int y = margemEsq + cm * pxPorCm;
                if (y > alt) break;
                if (cm % 2 == 0) {
                    p.drawText(4, y + 5, QString::number(cm));
                    p.drawLine(20, y, 32, y);
                } else {
                    p.drawLine(24, y, 32, y);
                }
                p.drawLine(28, y + pxPorCm/2, 32, y + pxPorCm/2);
            }
            p.setPen(QPen(QColor(0, 102, 204), 2));
            p.drawLine(0, margemEsq, 36, margemEsq);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void FydelisWriter::Exportar() {
    QString caminho = QFileDialog::getSaveFileName(
        this, "Exportar Documento", "",
        "Arquivo de Texto (*.txt);;Markdown (*.md);;HTML (*.html)");
    if (caminho.isEmpty()) return;

    QFile f(caminho);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível exportar.");
        return;
    }
    QTextStream out(&f);
    if (caminho.endsWith(".html", Qt::CaseInsensitive))
        out << editor->toHtml();
    else
        out << editor->toPlainText();
    f.close();
    //statusBar()->showMessage("Exportado ✓", 3000);
	textoStatus->setText("Exportado ✓");

}

void FydelisWriter::Importar() {
    if (!verificarSalvo()) return;
    QString caminho = QFileDialog::getOpenFileName(
        this, "Importar Arquivo", "",
        "Arquivos Suportados (*.txt *.md *.html *.fyd);;Todos (*)");
    if (caminho.isEmpty()) return;

    QFile f(caminho);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível ler o arquivo.");
        return;
    }
    QTextStream in(&f);
    QString conteudo = in.readAll();
    f.close();

    if (caminho.endsWith(".html", Qt::CaseInsensitive))
        editor->setHtml(conteudo);
    else
        editor->setPlainText(conteudo);

    caminhoAtual.clear();
    modificado = true;
    atualizarTitulo();
    //statusBar()->showMessage("Importado ✓", 3000);
	textoStatus->setText("Importado ✓");

}

void FydelisWriter::Imprimir() {
    QMessageBox::information(this, "Imprimir",
        "📄 Função de Impressão\n\n"
        "Em desenvolvimento — em breve disponível!");
}

bool FydelisWriter::verificarSalvo() {
    if (!modificado) return true;
    auto r = QMessageBox::warning(this, "FydelisWriter",
        "O documento foi alterado.\nDeseja salvar?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (r == QMessageBox::Yes) return salvarAtual();
    if (r == QMessageBox::Cancel) return false;
    return true;
}

bool FydelisWriter::salvarAtual() {
    if (caminhoAtual.isEmpty()) {
        caminhoAtual = QFileDialog::getSaveFileName(this, "Salvar como", "",
            "Documento Fydelis (*.fyd *.txt *.html)");
        if (caminhoAtual.isEmpty()) return false;
    }
    QFile f(caminhoAtual);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar.");
        return false;
    }
    QTextStream out(&f);
    if (caminhoAtual.endsWith(".html", Qt::CaseInsensitive))
        out << editor->toHtml();
    else
        out << editor->toPlainText();
    f.close();
    modificado = false;
    atualizarTitulo();
    //statusBar()->showMessage("Salvo ✓", 3000);
	textoStatus->setText("Salvo ✓");
    return true;
}

// ==========================================
// ✅ CONFIGURAR PÁGINA
// ==========================================
void FydelisWriter::mnuConfigurarPagina() {
    QDialog dlg(this);
    dlg.setWindowTitle("Configurar Página");
    dlg.resize(340, 320);

    QVBoxLayout* lay = new QVBoxLayout(&dlg);
    lay->setSpacing(12);
    lay->setContentsMargins(20, 20, 20, 20);

    // Tamanho
    QComboBox* cbTam = new QComboBox();
    cbTam->addItems({"A4", "A5", "Carta", "Ofício"});
    cbTam->setCurrentText(cfg.tamanhoPagina);
    lay->addWidget(new QLabel("Tamanho do papel:"));
    lay->addWidget(cbTam);

    // Orientação
    QRadioButton* rbRetrato = new QRadioButton("Retrato");
    QRadioButton* rbPaisagem = new QRadioButton("Paisagem");
    rbRetrato->setChecked(cfg.retrato);
    rbPaisagem->setChecked(!cfg.retrato);
    QVBoxLayout* layOr = new QVBoxLayout();
    layOr->addWidget(rbRetrato);
    layOr->addWidget(rbPaisagem);
    QGroupBox* grpOr = new QGroupBox("Orientação");
    grpOr->setLayout(layOr);
    lay->addWidget(grpOr);

    // Margens
    QSpinBox* spEsq = new QSpinBox(); spEsq->setRange(0, 200); spEsq->setValue(cfg.margemEsq);
    QSpinBox* spDir = new QSpinBox(); spDir->setRange(0, 200); spDir->setValue(cfg.margemDir);
    QSpinBox* spSup = new QSpinBox(); spSup->setRange(0, 200); spSup->setValue(cfg.margemSup);
    QSpinBox* spInf = new QSpinBox(); spInf->setRange(0, 200); spInf->setValue(cfg.margemInf);
    QFormLayout* layM = new QFormLayout();
    layM->addRow("Esquerda:", spEsq);
    layM->addRow("Direita:", spDir);
    layM->addRow("Superior:", spSup);
    layM->addRow("Inferior:", spInf);
    QGroupBox* grpM = new QGroupBox("Margens (px)");
    grpM->setLayout(layM);
    lay->addWidget(grpM);

    QDialogButtonBox* bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    lay->addWidget(bbox);
    connect(bbox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(bbox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        // ✅ Atualiza objeto
        cfg.tamanhoPagina = cbTam->currentText();
        cfg.retrato = rbRetrato->isChecked();
        cfg.margemEsq = spEsq->value();
        cfg.margemDir = spDir->value();
        cfg.margemSup = spSup->value();
        cfg.margemInf = spInf->value();

        // ✅ Salva no JSON automaticamente
        cfg.salvar();

        aplicarEstiloPagina();
        textoStatus->setText("Página: " + cfg.tamanhoPagina + (cfg.retrato ? " · Retrato" : " · Paisagem"));
    }
}

// ==========================================
// ✅ APLICAR ESTILO DAS MARGENS
// ==========================================
void FydelisWriter::aplicarEstiloPagina() {
    editor->setStyleSheet(QString(
        "QTextEdit {"
        "  background: #ffffff;"
        "  color: #1a1a1a;"
        "  border: none;"
        "  padding: %1px %2px %3px %4px;"
        "}"
    ).arg(cfg.margemSup).arg(cfg.margemDir).arg(cfg.margemInf).arg(cfg.margemEsq));
}

// ==========================================
// ✅ LAYOUT DE IMPRESSÃO — Página real
// ==========================================
void FydelisWriter::mnuLayoutImpressao() {
    editor->setLineWrapMode(QTextEdit::FixedPixelWidth);
    
    int larguraPag = (cfg.tamanhoPagina == "A4" ? 595 : 500) - cfg.margemEsq - cfg.margemDir;
    int alturaPag = (cfg.tamanhoPagina == "A4" ? 842 : 700) - cfg.margemSup - cfg.margemInf;
    
    if (!cfg.retrato) {
        int temp = larguraPag;
        larguraPag = alturaPag;
        alturaPag = temp;
    }
    
    editor->setLineWrapColumnOrWidth(larguraPag);
    aplicarEstiloPagina();
    textoStatus->setText("Modo: Layout de Impressão · " + cfg.tamanhoPagina);
}

// ==========================================
// ✅ PÁGINA VERTICAL — Rolo contínuo
// ==========================================
void FydelisWriter::mnuPaginaVertical() {
    editor->setLineWrapMode(QTextEdit::WidgetWidth);    
    aplicarEstiloPagina();
    textoStatus->setText("Modo: Página Vertical");
}

// ==========================================
// ✅ LADO A LADO
// ==========================================
void FydelisWriter::mnuPaginaLado() {
    QMessageBox::information(this, "Visualização Lado a Lado",
        "Exibição de múltiplas páginas lado a lado\nestará disponível em versões futuras!");
    textoStatus->setText("Lado a Lado — em desenvolvimento");
}

// ==========================================
// ✅ MACROS
// ==========================================
void FydelisWriter::mnuMacro() {
    QMessageBox::information(this, "Macros",
        "Gravação e execução de macros\nem desenvolvimento!");
}

// ==========================================
// ✅ PROPRIEDADES
// ==========================================
void FydelisWriter::mnuPropriedade() {
    int chars = editor->toPlainText().length();
    int words = editor->toPlainText().split(" ", Qt::SkipEmptyParts).count();
    QString info = QString(
        "📄 Propriedades do Documento\n\n"
        "Arquivo: %1\n"
        "Modificado: %2\n"
        "Tamanho: %3 caracteres, %4 palavras\n"
        "Papel: %5 · %6\n"
        "Margens: E:%7 D:%8 S:%9 I:%10"
    ).arg(caminhoAtual.isEmpty() ? "Não salvo" : caminhoAtual)
     .arg(modificado ? "Sim ⚠️" : "Não ✓")
     .arg(chars).arg(words)
     .arg(tamanhoPagina)
     .arg(retrato ? "Retrato" : "Paisagem")
     .arg(margemEsq).arg(margemDir).arg(margemSup).arg(margemInf);

    QMessageBox::about(this, "Propriedades", info);
}