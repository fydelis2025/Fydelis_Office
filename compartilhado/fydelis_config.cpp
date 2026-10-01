#include "fydelis_config.h"
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>

FydelisConfig::FydelisConfig() {
    restaurarPadroes();
    // Salva em pasta do usuário — não perde ao reinstalar
    caminhoArquivo = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                     + "/configuracao.json";
    QDir().mkpath(QFileInfo(caminhoArquivo).absolutePath());
}

void FydelisConfig::restaurarPadroes() {
    tamanhoPagina = "A4";
    retrato = true;
    margemEsq = 40;
    margemDir = 40;
    margemSup = 40;
    margemInf = 40;

    mostrarQuebraPagina = false;
    modoImpressaoVisivel = false;
    escalaImpressao = 100;

    tema = "azul";
    tamanhoFontePadrao = 12;
    fontePadrao = "Calibri";
}

QJsonObject FydelisConfig::paraObjeto() const {
    QJsonObject obj;
    obj["tamanhoPagina"] = tamanhoPagina;
    obj["retrato"] = retrato;
    obj["margemEsq"] = margemEsq;
    obj["margemDir"] = margemDir;
    obj["margemSup"] = margemSup;
    obj["margemInf"] = margemInf;

    obj["mostrarQuebraPagina"] = mostrarQuebraPagina;
    obj["modoImpressaoVisivel"] = modoImpressaoVisivel;
    obj["escalaImpressao"] = escalaImpressao;

    obj["tema"] = tema;
    obj["tamanhoFontePadrao"] = tamanhoFontePadrao;
    obj["fontePadrao"] = fontePadrao;
    return obj;
}

void FydelisConfig::doObjeto(const QJsonObject& obj) {
    tamanhoPagina = obj["tamanhoPagina"].toString("A4");
    retrato = obj["retrato"].toBool(true);
    margemEsq = obj["margemEsq"].toInt(40);
    margemDir = obj["margemDir"].toInt(40);
    margemSup = obj["margemSup"].toInt(40);
    margemInf = obj["margemInf"].toInt(40);

    mostrarQuebraPagina = obj["mostrarQuebraPagina"].toBool(false);
    modoImpressaoVisivel = obj["modoImpressaoVisivel"].toBool(false);
    escalaImpressao = obj["escalaImpressao"].toInt(100);

    tema = obj["tema"].toString("azul");
    tamanhoFontePadrao = obj["tamanhoFontePadrao"].toInt(12);
    fontePadrao = obj["fontePadrao"].toString("Calibri");
}

bool FydelisConfig::carregar() {
    QFile arq(caminhoArquivo);
    if (!arq.open(QIODevice::ReadOnly)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(arq.readAll());
    if (!doc.isObject()) return false;
    doObjeto(doc.object());
    return true;
}

bool FydelisConfig::salvar() {
    QFile arq(caminhoArquivo);
    if (!arq.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    QJsonDocument doc(paraObjeto());
    arq.write(doc.toJson(QJsonDocument::Indented));
    return true;
}