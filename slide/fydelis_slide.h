#ifndef FYDELIS_SLIDE_H
#define FYDELIS_SLIDE_H
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QListWidget>
#include <QTextEdit>
#include <QColor>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QToolBar> 

struct Slide {
    QString titulo;
    QString conteudo;
    QString corFundo;
    QString corTexto;
};

class FydelisSlide : public QMainWindow {
    Q_OBJECT
public:
    explicit FydelisSlide(QWidget* pai = nullptr);
    ~FydelisSlide() = default;

private slots:
    void novo();
    void abrir();
    bool salvar();
    bool salvarComo();
    void iniciarApresentacao();
    void proximoSlide();
    void slideAnterior();
    void adicionarSlide();
    void removerSlide();
    void sobre();

private:
    // === WIDGETS ===
    QWidget* barraTitulo = nullptr;
    QLabel* lblTitulo = nullptr;
    QPushButton* btnMin = nullptr;
    QPushButton* btnMax = nullptr;
    QPushButton* btnFechar = nullptr;
    QTabWidget* ribbon = nullptr;
    QListWidget* listaSlides = nullptr;
    QTextEdit* editorConteudo = nullptr;
    QWidget* painelCentral = nullptr;
    QWidget* telaApresentacao = nullptr;
	
		// status bar
	QWidget* barraStatus;  // ✅ Nossa barra personalizada
    QLabel*  textoStatus;

    // === DADOS ===
    QList<Slide> slides;
    int slideAtual = 0;
    QString caminhoAtual;
    bool modificado = false;
    bool arrastandoJanela = false;
    QPoint posicaoArrasto;

    // === MÉTODOS ===
    void atualizarTitulo();
    void atualizarListaSlides();
    void criarMenus();
    void criarBarraFerramentas();
    void aplicarEstilo();
    void atualizarEditor();
    bool podeFechar();

    // === EVENTOS ===
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
};

#endif