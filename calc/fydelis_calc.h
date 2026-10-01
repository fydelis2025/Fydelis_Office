#ifndef FYDELIS_CALC_H
#define FYDELIS_CALC_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPoint>
#include <QMouseEvent>

class QPushButton;
class QLabel;
class QFontComboBox;
class QSpinBox;

class FydelisCalc : public QMainWindow {
    Q_OBJECT
public:
    explicit FydelisCalc(QWidget *parent = nullptr);
    ~FydelisCalc();

private slots:
    void novoArquivo();
    void abrirArquivo();
    void salvarArquivo();
    void salvarComo();
    void sobre();
    
    void desfazer();
    void refazer();
    void recortar();
    void copiar();
    void colar();
    
    // ✅ Trocado: SEM parâmetros
    void alterarFonte();
    void alterarTamanhoFonte();
    void alterarNegrito();
    void alterarCorTexto();
	
	void Exportar();
	void Importar();
	void Imprimir();
    
    void recalcularCelula(int linha, int coluna);
    void inserirLinha();
    void inserirColuna();
    void removerLinha();
    void removerColuna();
    void limparCelula();
	

private:
    // ==========================================
    // COMPONENTES DA INTERFACE
    // ==========================================
    QWidget*        barraTitulo    = nullptr;
    QLabel*         lblTitulo      = nullptr;
    QPushButton*    btnMin         = nullptr;
    QPushButton*    btnMax         = nullptr;
    QPushButton*    btnFechar      = nullptr;
    
    QTabWidget*     ribbonTab      = nullptr;
    QTableWidget*   planilha       = nullptr;
    
    QFontComboBox*  cmbFonte       = nullptr;
    QSpinBox*       spnTamanho     = nullptr;

    // ==========================================
    // ESTADO DA JANELA
    // ==========================================
    QString         caminhoAtual;
    bool            modificado     = false;
    bool            arrastandoJanela = false;
    QPoint          posicaoArrasto;
	
	// status bar
	QWidget* barraStatus;  // ✅ Nossa barra personalizada
    QLabel*  textoStatus;

    // ==========================================
    // MÉTODOS INTERNOS
    // ==========================================
    void criarMenus();
    void criarBarraFerramentas();
    void carregarEstilo();
    void atualizarTitulo();
    bool verificarSalvo();
    bool salvarAtual();
    
    // Eventos da janela
    void closeEvent(QCloseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
	
	void aplicarEstiloOffice();
};

#endif // FYDELIS_CALC_H