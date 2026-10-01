#ifndef FYDELIS_WRITER_H
#define FYDELIS_WRITER_H

#include "../compartilhado/fydelis_config.h"
#include <QMainWindow>
#include <QPushButton>
#include <QTabWidget>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QTextEdit>
#include <QString>
#include <QLabel>

class FydelisWriter : public QMainWindow {
    Q_OBJECT
public:
    explicit FydelisWriter(QWidget* pai = nullptr);

private slots:
    void novo();
    void abrir();
    bool salvar();
    bool salvarComo();
    void mudarFonte();
    void mudarCorTexto();
    void alterarNegrito();
    void alterarItalico();
    void alterarSublinhado();	
    void sobre();
	
	void mnuLayoutImpressao();
	void mnuPaginaVertical();
	void mnuPaginaLado();
	void mnuMacro();
	void mnuPropriedade();
	void mnuConfigurarPagina();
       
    void aumentarFonte();
    void diminuirFonte();
    void alterarCorTexto();
    void alterarCorFundo();

    // Parágrafo → Alinhamento
    void alinharEsquerda();
    void alinharCentro();
    void alinharDireita();

    // Inserir → Tabela e Elementos
    void inserirTabela();
    void inserirImagem();
    void inserirLink();
	
	void Importar();
	void Exportar();
	void Imprimir();
	
	void copiar();
	void colar();
	void cortar();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
     void criarMenus();
    void criarBarraFerramentas();
    void aplicarEstiloOffice();
    void atualizarTitulo();
	bool verificarSalvo();
    bool salvarAtual();
    bool podeFechar();
    void closeEvent(QCloseEvent* e) override;
    
	QLabel* lblTitulo = nullptr;
    QTextEdit* editor = nullptr;
    QString caminhoAtual;
    bool modificado = false;
    bool textoModificado = false; 
	
    QWidget* barraTitulo = nullptr;
    QTabWidget* ribbon = nullptr;
    QPushButton* btnMin = nullptr;
    QPushButton* btnMax = nullptr;
    QPushButton* btnFechar = nullptr;
    bool arrastandoJanela = false;
    QPoint posicaoArrasto;
	
	// status bar
	QWidget* barraStatus;  // ✅ Nossa barra personalizada
    QLabel*  textoStatus;
	
    // ✅ RÉGUAS — APENAS ESTAS, SEM DUPLICATAS
    QWidget* reguaSuperior = nullptr;
    QWidget* reguaLateral = nullptr;
    QWidget* painelEditor = nullptr;
	
	int margemEsq = 40;
    int margemDir = 40;
    int margemSup = 40;
    int margemInf = 40;
    QString tamanhoPagina = "A4";
    bool retrato = true;
	
	FydelisConfig cfg;
	void aplicarEstiloPagina();

};

#endif