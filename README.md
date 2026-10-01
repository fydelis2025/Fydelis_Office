<!-- ==============================================
   FydelisOffice — Suíte de Produtividade Nativa
   Estilo GitHub Responsivo · v2.2
   Salvador • Bahia 🇧🇷
   ============================================== -->

<p align="center">
  <img src="https://raw.githubusercontent.com/fydelis2025/Fydelis_Office/main/resources/logo_menu.png" 
       alt="FydelisOffice" width="180" style="border-radius: 12px;" />
</p>

<h1 align="center">FydelisOffice</h1>
<p align="center">
  <em>Suíte de produtividade leve, nativa e independente — feita na Bahia 💙💚💜</em>
</p>

<p align="center">
  <a href="#recursos">Recursos</a> ·
  <a href="#tecnologias">Tecnologias</a> ·
  <a href="#instalacao">Instalação</a> ·
  <a href="#documentacao">Documentação</a> ·
  <a href="#contribuir">Contribuir</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Plataforma-Windows%20%7C%20Linux%20%7C%20Qt6-0066CC?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Licença-MIT-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Em%20Desenvolvimento-yellow?style=for-the-badge" />
</p>

---

## ✨ Apresentação

O **FydelisOffice** é uma suíte de aplicativos de escritório desenvolvida de forma independente, com foco em **leveza, velocidade e compatibilidade total** com o sistema operacional FydelisTechOS e também com Windows e Linux.

Nada de dependências pesadas. Tudo nativo. Tudo rápido. 💙

---

## 📦 Componentes

| Aplicativo | Descrição | Tema | Status |
|---|---|---|---|
| **FydelisWriter** | Editor de texto completo com réguas, formatação e configuração de página | 🔵 Azul | ✅ Funcional |
| **FydelisCalc** | Planilha eletrônica com fórmulas e exportação | 🟢 Verde | 🚧 Em desenvolvimento |
| **FydelisSlide** | Editor de apresentações com transições e tela cheia | 🟣 Roxo | 🚧 Em desenvolvimento |

---

## 🚀 Recursos

- 🎨 **Interface estilo Ribbon** — inspirada nos suítes profissionais, leve e responsiva
- 📐 **Configuração de página** — margens, tamanho de papel e orientação salvas automaticamente
- 💾 **Formato próprio `.fydoc` / `.fysheet`** — baseado em JSON, aberto e legível
- 🔄 **Salvamento automático** e detecção de alterações não salvas
- 🖥️ **Multiplataforma** — Qt6 nativo: Windows, Linux e FydelisTechOS
- 🧩 **Arquitetura modular** — código organizado, fácil de manter e expandir
- 🇧🇷 **Interface em português** — nativa, sem traduções genéricas

---

## 🛠️ Tecnologias

- **Qt 6.11+** — Widgets nativos, C++17/20
- **Compiladores**: Clang / MinGW / GCC
- **Formato**: JSON para configurações e documentos
- **Padrão**: Código limpo, orientado a objetos, compatível com o FydelisTechOS v2.2

---

## ⬇️ Instalação

### Pré-requisitos
- Qt 6.11 ou superior
- MinGW 64-bit / Clang / GCC
- `qmake` + `mingw32-make` ou `cmake`

### Compilar
```bash
# Clonar o repositório
git clone https://github.com/fydelis/fydelis-office.git
cd fydelis-office

# Compilar cada componente
cd writer
qmake fydelis_writer.pro
mingw32-make

cd ../calc
qmake fydelis_calc.pro
mingw32-make
