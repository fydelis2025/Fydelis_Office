<p align="center">
  <img src="https://raw.githubusercontent.com/fydelis2025/Fydelis_Office/main/assets/logo_menu.png" 
       alt="FydelisOffice" width="180" style="border-radius: 12px;" />
</p>

<h1 align="center">FydelisOffice</h1>
<p align="center">
  <em>Suíte de produtividade leve, nativa e independente — feita na Bahia 💙💚💜</em>
</p>

<p align="center">
  <a href="#recursos">Recursos</a> ·
  <a href="#componentes">Componentes</a> ·
  <a href="#tecnologias">Tecnologias</a> ·
  <a href="#instalacao">Instalação</a> ·
  <a href="#contribuir">Contribuir</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Plataforma-Windows%20%7C%20Linux%20%7C%20FydelisTechOS-0066CC?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Qt-6.11+-41CD52?style=for-the-badge" />
  <img src="https://img.shields.io/badge/C++-17%2F20-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Licença-MIT-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Desenvolvimento-yellow?style=for-the-badge" />
</p>


---

## ✨ Apresentação

O **FydelisOffice** é uma suíte de aplicativos de escritório **construída do zero**, com foco em **leveza, velocidade e compatibilidade total** com o sistema operacional **FydelisTechOS v2.2**, além de funcionar nativamente em Windows e Linux.

> Nada de dependências pesadas. Tudo nativo. Tudo rápido. 💙

---

## 📦 Componentes

| Aplicativo | Descrição | Tema | Status |
|---|---|---|---|
| **FydelisWriter** | Editor de texto com réguas, formatação completa, configuração de página e formato `.fydoc` | 🔵 Azul | ✅ Funcional |
| **FydelisCalc** | Planilha eletrônica com tabela, fórmulas e formato `.fysheet` | 🟢 Verde | 🚧 Em desenvolvimento |
| **FydelisSlide** | Editor de apresentações com transições e modo tela cheia | 🟣 Roxo | 🚧 Em desenvolvimento |

---

## 🚀 Recursos

- 🎨 **Interface estilo Ribbon** — inspirada em suítes profissionais, leve e responsiva
- 📐 **Configuração de página** — margens, tamanho de papel e orientação salvos automaticamente
- 💾 **Formatos abertos** — `.fydoc` e `.fysheet` baseados em JSON, legíveis e independentes
- 🔄 **Detecção de alterações** — avisa ao fechar documentos não salvos
- 🖥️ **Multiplataforma** — Qt6 nativo: Windows, Linux e FydelisTechOS
- 🧩 **Arquitetura modular** — código organizado, fácil de manter e expandir
- 🇧🇷 **Interface em português** — nativa, sem traduções genéricas
- 🔒 **Sem telemetria** — seus dados ficam só com você

---

## 🛠️ Tecnologias

- **Qt 6.11+** — Widgets nativos, C++17/20
- **Compiladores**: Clang / MinGW 64-bit / GCC
- **Formato**: JSON para configurações e documentos
- **Padrão**: Código limpo, orientado a objetos, compatível com FydelisTechOS v2.2

---

## ⬇️ Instalação

### Pré-requisitos
- Qt 6.11 ou superior
- MinGW 64-bit / Clang / GCC
- `qmake` + `mingw32-make` ou `make`

### Compilar
```bash
# Clonar o repositório
git clone https://github.com/fydelis2025/Fydelis_Office.git
cd Fydelis_Office

# FydelisWriter
cd writer
qmake fydelis_writer.pro
mingw32-make

# FydelisCalc (em breve)
cd ../calc
qmake fydelis_calc.pro
mingw32-make
