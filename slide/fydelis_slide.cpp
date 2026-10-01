#include "fydelis_slide.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QFile>
#include <QColorDialog>
#include <QInputDialog>
#include <QKeyEvent>
#include <QToolBar> 

FydelisSlide::FydelisSlide(QWidget *parent)
    : QMainWindow(parent), caminhoAtual(""), modificado(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle("FydelisSlide — Apresentação sem nome");
    resize(1024, 680);
    setMinimumSize(800, 520);

    // ==========================================
    // ✅ CONTAINER RAIZ — COM BORDA ROXA EXTERNA
    // ==========================================
    QWidget* raiz = new QWidget(this);
    raiz->setObjectName("raiz");
    raiz->setStyleSheet(R"(
        #raiz {
            background: #5E35B1;  /* 💜 COR DA BORDA EXTERNA */
            border-radius: 8px;
        }
    )");

    // Conteúdo interno — com margem = espessura da borda
    QWidget* janela = new QWidget(raiz);
    janela->setObjectName("conteudo");
    janela->setStyleSheet(R"(
        #conteudo {
            background: #FFFFFF;
            border-radius: 5px;
        }
    )");

    QVBoxLayout* layRaiz = new QVBoxLayout(raiz);
    layRaiz->setContentsMargins(3, 3, 3, 3);  // Borda de 3px roxa
    layRaiz->setSpacing(0);
    layRaiz->addWidget(janela);

    setCentralWidget(raiz);
    setMenuBar(nullptr);

    // ==========================================
    // BARRA DE TÍTULO — ROXA GRADIENTE
    // ==========================================
    barraTitulo = new QWidget(janela);
    barraTitulo->setObjectName("barraTitulo");
    barraTitulo->setFixedHeight(50);
    barraTitulo->setStyleSheet(R"(
        #barraTitulo {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #673AB7, stop:1 #4527A0);
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
    iconeApp->setPixmap(QIcon(":/fydelis/icon_slide.png").pixmap(32,32));
    iconeApp->setFixedSize(48,50);
    iconeApp->setAlignment(Qt::AlignCenter);

    lblTitulo = new QLabel(windowTitle(), barraTitulo);
    lblTitulo->setStyleSheet("color: #FFFFFF; font-size: 14px; font-weight:600; padding-top:2px;");

    btnMin = new QPushButton("−", barraTitulo);
    btnMax = new QPushButton("□", barraTitulo);
    btnFechar = new QPushButton("×", barraTitulo);
    btnFechar->setObjectName("btnFechar");

    QHBoxLayout* layBarra = new QHBoxLayout(barraTitulo);
    layBarra->setContentsMargins(0,0,0,0);
    layBarra->setSpacing(0);
    layBarra->addWidget(iconeApp);
    layBarra->addWidget(lblTitulo);
    layBarra->addStretch();
    layBarra->addWidget(btnMin);
    layBarra->addWidget(btnMax);
    layBarra->addWidget(btnFechar);

    // ==========================================
    // BARRA DE MENU
    // ==========================================
    QMenuBar* barraMenu = new QMenuBar(janela);
    barraMenu->setStyleSheet(R"(
        QMenuBar {
            background: #FFFFFF;
            border-bottom: 1px solid #EDE7F6;
            padding: 2px 4px;
        }
        QMenuBar::item {
            padding: 6px 14px;
            color: #1A1A1A;
            border-radius: 3px;
        }
        QMenuBar::item:selected {
            background: #EDE7F6;
            color: #5E35B1;
        }
    )");

    QMenu* arq = barraMenu->addMenu("&Arquivo");
    arq->addAction("&Novo", this, &FydelisSlide::novo, QKeySequence::New);
    arq->addAction("&Abrir...", this, &FydelisSlide::abrir, QKeySequence::Open);
    arq->addAction("&Salvar", this, &FydelisSlide::salvar, QKeySequence::Save);
    arq->addAction("Salvar &como...", this, &FydelisSlide::salvarComo, QKeySequence::SaveAs);
    arq->addSeparator();
    arq->addAction("&Sobre", this, &FydelisSlide::sobre);
    arq->addSeparator();
    arq->addAction("Sair", this, &QWidget::close, QKeySequence::Quit);

    QMenu* apr = barraMenu->addMenu("&Apresentação");
    apr->addAction("&Iniciar", this, &FydelisSlide::iniciarApresentacao, Qt::Key_F5);
    apr->addAction("&Adicionar Slide", this, &FydelisSlide::adicionarSlide);
    apr->addAction("&Remover Slide", this, &FydelisSlide::removerSlide);

    QMenu* ajd = barraMenu->addMenu("&Ajuda");
    ajd->addAction("&Sobre", this, &FydelisSlide::sobre);

    // ==========================================
    // ÁREA CENTRAL
    // ==========================================
    painelCentral = new QWidget(janela);
    QHBoxLayout* layEditor = new QHBoxLayout(painelCentral);
    layEditor->setContentsMargins(8,8,8,8);
    layEditor->setSpacing(8);

    listaSlides = new QListWidget();
    listaSlides->setFixedWidth(220);
    layEditor->addWidget(listaSlides);

    editorConteudo = new QTextEdit();
    editorConteudo->setPlaceholderText("Digite o conteúdo do slide...");
    layEditor->addWidget(editorConteudo);

    connect(editorConteudo, &QTextEdit::textChanged, this, [this](){
        modificado = true;
        atualizarTitulo();
    });
    connect(listaSlides, &QListWidget::currentRowChanged, this, [this](int i){
        if (i >= 0 && i < slides.size()) {
            slideAtual = i;
            atualizarEditor();
        }
    });

    // ==========================================
    // ✅ LAYOUT PRINCIPAL — TUDO DENTRO 💜
    // ==========================================
    QVBoxLayout* layPrincipal = new QVBoxLayout(janela);
    layPrincipal->setContentsMargins(0,0,0,0);
    layPrincipal->setSpacing(0);

    layPrincipal->addWidget(barraTitulo);        // 1. Título
    layPrincipal->addWidget(barraMenu);          // 2. Menu

    criarBarraFerramentas();
    if (ribbon) layPrincipal->addWidget(ribbon); // 3. Ribbon

    layPrincipal->addWidget(painelCentral);       // 4. Conteúdo

    // ✅ BARRA DE STATUS — DENTRO, FECHANDO!
    barraStatus = new QWidget(janela);
    barraStatus->setFixedHeight(24);
    barraStatus->setStyleSheet(R"(
        background: #F3E5F5;
        border-top: 1px solid #CE93D8;
    )");
    QHBoxLayout* layStatus = new QHBoxLayout(barraStatus);
    layStatus->setContentsMargins(8, 2, 8, 2);
    layStatus->setSpacing(0);
    textoStatus = new QLabel("Pronto", barraStatus);
    textoStatus->setStyleSheet("color: #5E35B1; font-size: 11px;");
    layStatus->addWidget(textoStatus);
    layStatus->addStretch();

    layPrincipal->addWidget(barraStatus);        // ✅ 5. Fechamento perfeito!

    // Botões
    connect(btnFechar, &QPushButton::clicked, this, &QWidget::close);
    connect(btnMin, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(btnMax, &QPushButton::clicked, [this](){
        isMaximized() ? showNormal() : showMaximized();
    });

    aplicarEstilo();

    // Slide inicial
    slides.append({"Título", "Conteúdo...", "#FAFAFA", "#212121"});
    atualizarListaSlides();
    atualizarEditor();
}

void FydelisSlide::atualizarTitulo() {
    QString nome = caminhoAtual.isEmpty() ? "Apresentação sem nome" : caminhoAtual.section('/',-1);
    setWindowTitle(QString("%1%2 — FydelisSlide").arg(nome).arg(modificado ? " •" : ""));
    if (lblTitulo) lblTitulo->setText(windowTitle());
}

void FydelisSlide::atualizarListaSlides() {
    listaSlides->clear();
    for (int i=0; i<slides.size(); i++) {
        listaSlides->addItem(QString("Slide %1: %2").arg(i+1).arg(slides[i].titulo));
    }
    listaSlides->setCurrentRow(slideAtual);
}

void FydelisSlide::atualizarEditor() {
    if (slideAtual < 0 || slideAtual >= slides.size()) return;
    editorConteudo->setPlainText(slides[slideAtual].conteudo);
}

void FydelisSlide::criarMenus() {
    QMenu* arq = menuBar()->addMenu("&Arquivo");
    arq->addAction("&Novo", this, &FydelisSlide::novo, QKeySequence::New);
    arq->addAction("&Abrir...", this, &FydelisSlide::abrir, QKeySequence::Open);
    arq->addAction("&Salvar", this, &FydelisSlide::salvar, QKeySequence::Save);
    arq->addAction("Salvar &como...", this, &FydelisSlide::salvarComo, QKeySequence::SaveAs);
    arq->addSeparator();
    arq->addAction("&Sobre", this, &FydelisSlide::sobre);
    arq->addSeparator();
    arq->addAction("Sair", this, &QWidget::close, QKeySequence::Quit);

    QMenu* apr = menuBar()->addMenu("&Apresentação");
    apr->addAction("&Iniciar", this, &FydelisSlide::iniciarApresentacao, Qt::Key_F5);
    apr->addAction("&Adicionar Slide", this, &FydelisSlide::adicionarSlide);
    apr->addAction("&Remover Slide", this, &FydelisSlide::removerSlide);
}

void FydelisSlide::criarBarraFerramentas() {
    for (auto* tb : findChildren<QToolBar*>()) removeToolBar(tb);

    ribbon = new QTabWidget(this);
    ribbon->setObjectName("RibbonTab");
    ribbon->setFixedHeight(115);
    ribbon->setStyleSheet(R"(
        QTabWidget::pane {
            background: #F3E5F5;
            border-top: 1px solid #D1C4E9;
            border-bottom: 1px solid #D1C4E9;
        }
        QTabBar::tab {
            background: transparent;
            color: #444;
            padding: 6px 14px;
            margin: 2px;
            border-top-left-radius: 3px;
            border-top-right-radius: 3px;
            font-size: 12px;
        }
        QTabBar::tab:selected {
            background: #FFFFFF;
            color: #5E35B1;
            font-weight: bold;
            border-bottom: 3px solid #5E35B1;
        }
        QTabBar::tab:hover {
            background: #EDE7F6;
            color: #4527A0;
        }
        QToolButton {
            background: transparent;
            border: 1px solid transparent;
            border-radius: 4px;
            padding: 6px 8px;
            color: #333;
            font-size: 11px;
            min-width: 70px;
        }
        QToolButton:hover {
            background: #EDE7F6;
            border: 1px solid #CE93D8;
        }
        QToolButton:pressed {
            background: #D1C4E9;
        }
    )");

    auto criarPainel = [](const QString& titulo, QHBoxLayout* p) -> QHBoxLayout* {
        QWidget* w = new QWidget();
        QVBoxLayout* v = new QVBoxLayout(w);
        v->setContentsMargins(2,2,2,2); v->setSpacing(2);
        QHBoxLayout* h = new QHBoxLayout(); h->setSpacing(4);
        v->addLayout(h);
        QLabel* l = new QLabel(titulo);
        l->setStyleSheet("color:#666; font-size:10px; font-weight:bold;");
        l->setAlignment(Qt::AlignCenter);
        v->addWidget(l);
        p->addWidget(w);
        return h;
    };

    // ABA 1: PÁGINA INICIAL
    QWidget* tabHome = new QWidget();
    QHBoxLayout* layHome = new QHBoxLayout(tabHome);
    layHome->setContentsMargins(8,6,8,6); layHome->setSpacing(12);

    QHBoxLayout* pArq = criarPainel("Arquivo", layHome);
    QToolButton* btnNovo = new QToolButton();
    btnNovo->setText("Novo"); btnNovo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnNovo, &QToolButton::clicked, this, &FydelisSlide::novo);
    pArq->addWidget(btnNovo);

    QToolButton* btnAbrir = new QToolButton();
    btnAbrir->setText("Abrir"); btnAbrir->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnAbrir, &QToolButton::clicked, this, &FydelisSlide::abrir);
    pArq->addWidget(btnAbrir);

    QToolButton* btnSalvar = new QToolButton();
    btnSalvar->setText("Salvar"); btnSalvar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnSalvar, &QToolButton::clicked, this, &FydelisSlide::salvar);
    pArq->addWidget(btnSalvar);

    QHBoxLayout* pSlide = criarPainel("Slides", layHome);
    QToolButton* btnAdd = new QToolButton();
    btnAdd->setText("Adicionar"); btnAdd->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnAdd, &QToolButton::clicked, this, &FydelisSlide::adicionarSlide);
    pSlide->addWidget(btnAdd);

    QToolButton* btnRem = new QToolButton();
    btnRem->setText("Remover"); btnRem->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnRem, &QToolButton::clicked, this, &FydelisSlide::removerSlide);
    pSlide->addWidget(btnRem);

    layHome->addStretch();
    ribbon->addTab(tabHome, "Página Inicial");

    // ABA 2: APRESENTAÇÃO
    QWidget* tabApr = new QWidget();
    QHBoxLayout* layApr = new QHBoxLayout(tabApr);
    layApr->setContentsMargins(8,6,8,6); layApr->setSpacing(12);

    QHBoxLayout* pCont = criarPainel("Controle", layApr);
    QToolButton* btnIniciar = new QToolButton();
    btnIniciar->setText("Iniciar F5"); btnIniciar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnIniciar->setStyleSheet("font-weight:bold; color:#5E35B1;");
    connect(btnIniciar, &QToolButton::clicked, this, &FydelisSlide::iniciarApresentacao);
    pCont->addWidget(btnIniciar);

    QToolButton* btnAnt = new QToolButton();
    btnAnt->setText("Anterior"); btnAnt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnAnt, &QToolButton::clicked, this, &FydelisSlide::slideAnterior);
    pCont->addWidget(btnAnt);

    QToolButton* btnProx = new QToolButton();
    btnProx->setText("Próximo"); btnProx->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnProx, &QToolButton::clicked, this, &FydelisSlide::proximoSlide);
    pCont->addWidget(btnProx);

    layApr->addStretch();
    ribbon->addTab(tabApr, "Apresentação");

    // ABA 3: AJUDA
    QWidget* tabAjuda = new QWidget();
    QHBoxLayout* layAjuda = new QHBoxLayout(tabAjuda);
    layAjuda->setContentsMargins(8,6,8,6); layAjuda->setSpacing(12);
    QHBoxLayout* pSis = criarPainel("Sistema", layAjuda);
    QToolButton* btnSobre = new QToolButton();
    btnSobre->setText("Sobre"); btnSobre->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnSobre, &QToolButton::clicked, this, &FydelisSlide::sobre);
    pSis->addWidget(btnSobre);
    layAjuda->addStretch();
    ribbon->addTab(tabAjuda, "Ajuda");
}

void FydelisSlide::aplicarEstilo() {
    setStyleSheet(R"(
        QMenuBar {
            background: #FFFFFF;
            border-bottom: 1px solid #EDE7F6;
            padding: 2px 4px;
        }
        QMenuBar::item {
            padding: 6px 14px;
            color: #1A1A1A;
            border-radius: 3px;
        }
        QMenuBar::item:selected {
            background: #EDE7F6;
            color: #5E35B1;
        }
        QStatusBar {
            background: #F3F3F3;
            border-top: 1px solid #E0E0E0;
            color: #555;
        }
        QListWidget {
            background: #FAFAFA;
            border: 1px solid #EDE7F6;
            border-radius: 4px;
        }
        QListWidget::item:selected {
            background: #EDE7F6;
            color: #5E35B1;
        }
        QTextEdit {
            background: #FFFFFF;
            border: 1px solid #EDE7F6;
            border-radius: 4px;
            padding: 12px;
            font-size: 13px;
        }
    )");
}

bool FydelisSlide::podeFechar() {
    if (!modificado) return true;
    auto r = QMessageBox::warning(this, "FydelisSlide",
        "A apresentação foi alterada.\nDeseja salvar?",
        QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if (r == QMessageBox::Yes) return salvar();
    if (r == QMessageBox::Cancel) return false;
    return true;
}

void FydelisSlide::novo() {
    if (!podeFechar()) return;
    slides.clear();
    slides.append({"Título", "Conteúdo...", "#FAFAFA", "#212121"});
    slideAtual = 0;
    caminhoAtual.clear();
    modificado = false;
    atualizarListaSlides();
    atualizarEditor();
    atualizarTitulo();
    //statusBar()->showMessage("Nova apresentação", 3000);
	textoStatus->setText("Nova apresentação");

}

void FydelisSlide::abrir() {
    if (!podeFechar()) return;
    QString arq = QFileDialog::getOpenFileName(this, "Abrir Apresentação", "",
        "Apresentações (*.fyds *.json);;Todos (*)");
    if (arq.isEmpty()) return;
    QFile f(arq);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erro", "Não foi possível ler.");
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isArray()) return;
    slides.clear();
    QJsonArray arr = doc.array();
    for (const auto& v : arr) {
        QJsonObject o = v.toObject();
        slides.append({
            o["titulo"].toString(),
            o["conteudo"].toString(),
            o["corFundo"].toString("#FAFAFA"),
            o["corTexto"].toString("#212121")
        });
    }
    slideAtual = 0;
    caminhoAtual = arq;
    modificado = false;
    atualizarListaSlides();
    atualizarEditor();
    atualizarTitulo();
    //statusBar()->showMessage("Aberto ✓", 3000);
	textoStatus->setText("Aberto");

}

bool FydelisSlide::salvar() {
    if (caminhoAtual.isEmpty()) return salvarComo();
    QJsonArray arr;
    for (const auto& s : slides) {
        QJsonObject o;
        o["titulo"] = s.titulo;
        o["conteudo"] = s.conteudo;
        o["corFundo"] = s.corFundo;
        o["corTexto"] = s.corTexto;
        arr.append(o);
    }
    QFile f(caminhoAtual);
    if (!f.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar.");
        return false;
    }
    f.write(QJsonDocument(arr).toJson());
    f.close();
    modificado = false;
    atualizarTitulo();
    //statusBar()->showMessage("Salvo ✓", 3000);
	textoStatus->setText("Salvo ✓");
    return true;
}

bool FydelisSlide::salvarComo() {
    QString arq = QFileDialog::getSaveFileName(this, "Salvar como", "",
        "Apresentação Fydelis (*.fyds)");
    if (arq.isEmpty()) return false;
    caminhoAtual = arq;
    return salvar();
}

void FydelisSlide::adicionarSlide() {
    slides.insert(slideAtual+1, {"Novo Slide", "", "#FAFAFA", "#212121"});
    slideAtual++;
    modificado = true;
    atualizarListaSlides();
    atualizarTitulo();
}

void FydelisSlide::removerSlide() {
    if (slides.size() <= 1) {
        QMessageBox::information(this, "Aviso", "Deve haver pelo menos 1 slide.");
        return;
    }
    slides.removeAt(slideAtual);
    if (slideAtual >= slides.size()) slideAtual = slides.size()-1;
    modificado = true;
    atualizarListaSlides();
    atualizarEditor();
    atualizarTitulo();
}

void FydelisSlide::iniciarApresentacao() {
    if (slides.isEmpty()) return;
    slideAtual = listaSlides->currentRow();
    if (slideAtual < 0) slideAtual = 0;

    telaApresentacao = new QWidget();
    telaApresentacao->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    telaApresentacao->showFullScreen();

    auto atualizarTela = [&](){
        if (slideAtual < 0 || slideAtual >= slides.size()) {
            telaApresentacao->close();
            return;
        }
        qDeleteAll(telaApresentacao->children());
        QVBoxLayout* lay = new QVBoxLayout(telaApresentacao);
        lay->setContentsMargins(80,80,80,80);
        const Slide& s = slides[slideAtual];
        telaApresentacao->setStyleSheet(QString("background: %1;").arg(s.corFundo));

        QLabel* tit = new QLabel(s.titulo);
        tit->setStyleSheet(QString("font-size:36px; font-weight:bold; color:%1;").arg(s.corTexto));
        tit->setAlignment(Qt::AlignCenter);
        lay->addWidget(tit);

        QLabel* cnt = new QLabel(s.conteudo);
        cnt->setStyleSheet(QString("font-size:20px; color:%1;").arg(s.corTexto));
        cnt->setAlignment(Qt::AlignCenter);
        cnt->setWordWrap(true);
        lay->addWidget(cnt);
    };

    atualizarTela();

    // Eventos de teclado na apresentação
    telaApresentacao->installEventFilter(this);
    connect(telaApresentacao, &QWidget::destroyed, [](){ /* limpo */ });
}

void FydelisSlide::proximoSlide() {
    if (slideAtual < slides.size()-1) {
        slideAtual++;
        atualizarListaSlides();
        atualizarEditor();
    }
}

void FydelisSlide::slideAnterior() {
    if (slideAtual > 0) {
        slideAtual--;
        atualizarListaSlides();
        atualizarEditor();
    }
}

void FydelisSlide::sobre() {
    QMessageBox::about(this, "Sobre — FydelisSlide",
        "<h2 style='color:#5E35B1; margin:8px 0;'>FydelisSlide v2.2</h2>"
        "<p style='font-size:13px; color:#333;'>Apresentações — FydelisOffice</p>"
        "<p style='font-size:12px; color:#666;'>Estilo Office · Código Aberto</p>"
        "<hr style='border:none; border-top:1px solid #EDE7F6; margin:10px 0;'>"
        "<p style='font-size:12px; color:#5E35B1; font-weight:bold;'>Salvador • Bahia 🇧🇷</p>");
}

// ==========================================
// ARRASTAR JANELA
// ==========================================
void FydelisSlide::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton && barraTitulo) {
        QPoint local = barraTitulo->mapFromParent(e->pos());
        if (barraTitulo->rect().contains(local)) {
            arrastandoJanela = true;
            posicaoArrasto = e->globalPosition().toPoint() - frameGeometry().topLeft();
        }
    }
    QMainWindow::mousePressEvent(e);
}
void FydelisSlide::mouseMoveEvent(QMouseEvent* e) {
    if (arrastandoJanela && (e->buttons() & Qt::LeftButton)) {
        move(e->globalPosition().toPoint() - posicaoArrasto);
        return;
    }
    QMainWindow::mouseMoveEvent(e);
}
void FydelisSlide::mouseReleaseEvent(QMouseEvent*) {
    arrastandoJanela = false;
}

void FydelisSlide::closeEvent(QCloseEvent* e) {
    if (podeFechar()) e->accept();
    else e->ignore();
}