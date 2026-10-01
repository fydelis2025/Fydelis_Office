#ifndef FYDELIS_CONFIG_H
#define FYDELIS_CONFIG_H

#include <QString>
#include <QJsonObject>

class FydelisConfig {
public:
    FydelisConfig();

    // 📄 Carregar / Salvar
    bool carregar();
    bool salvar();

    // 📄 Página
    QString tamanhoPagina;     // "A4", "A5", "Carta", "Ofício"
    bool retrato;              // true = Retrato, false = Paisagem
    int margemEsq;
    int margemDir;
    int margemSup;
    int margemInf;

    // 🖨️ Impressão
    bool mostrarQuebraPagina;
    bool modoImpressaoVisivel;
    int escalaImpressao;       // 100 = 100%

    // 🎨 Aparência
    QString tema;              // "azul", "verde", "roxo", "claro", "escuro"
    int tamanhoFontePadrao;
    QString fontePadrao;

    // 🔄 Valores Padrão
    void restaurarPadroes();

private:
    QString caminhoArquivo;
    QJsonObject paraObjeto() const;
    void doObjeto(const QJsonObject& obj);
};

#endif // FYDELIS_CONFIG_H