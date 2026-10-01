#ifndef FORMATOS_H
#define FORMATOS_H
#include <QString>
#include <QStringList>

// ==========================================
// FORMATOS DE ARQUIVO — FydelisOffice
// ==========================================
#define FYDOC_ASSINATURA   "FYDOC"
#define FYDOC_VERSAO       0x0202
#define FYDOC_EXTENSAO     ".fydoc"

#define FYSHEET_ASSINATURA "FYSHT"
#define FYSHEET_VERSAO     0x0202
#define FYSHEET_EXTENSAO   ".fysheet"

#define FYSLIDE_ASSINATURA "FYSLD"
#define FYSLIDE_VERSAO     0x0202
#define FYSLIDE_EXTENSAO   ".fyslide"

struct FormatoArquivo {
    QString nome;
    QStringList extensoes;
    QString filtro;
};

inline FormatoArquivo formatoFydoc() {
    return {
        "Documento Fydelis",
        {"*.fydoc"},
        "Documento Fydelis (*.fydoc)"
    };
}

inline FormatoArquivo formatoFysheet() {
    return {
        "Planilha Fydelis",
        {"*.fysheet"},
        "Planilha Fydelis (*.fysheet)"
    };
}

inline FormatoArquivo formatoFyslide() {
    return {
        "Apresentação Fydelis",
        {"*.fyslide"},
        "Apresentação Fydelis (*.fyslide)"
    };
}

inline QString filtroExportacao() {
    return "Todos os Arquivos (*.*);;"
           "Documento Fydelis (*.fydoc);;"
           "Planilha Fydelis (*.fysheet);;"
           "Apresentação Fydelis (*.fyslide);;"
           "Texto Puro (*.txt)";
}

#endif // FORMATOS_H