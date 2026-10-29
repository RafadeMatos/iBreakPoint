# iBreakPoint v0.1.0

CLI em C para diagnóstico de dispositivos seriais e firmware próprio para Raspberry Pi Pico/RP2040. A versão atual inclui menu interativo, Device, DocTor Who e Analyze.
## Host — Fedora Linux

Dependências de build:

```bash
sudo dnf install gcc gcc-c++ cmake make
```

Build e testes:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Execute sem instalar:

```bash
./build/ibreakpoint
./build/ibreakpoint --help
./build/ibreakpoint --version
./build/ibreakpoint --color=never analyze
```

Instalação local opcional:

```bash
sudo cmake --install build
```


## Pico / RP2040

Instale o Raspberry Pi Pico SDK conforme a documentação oficial e exporte `PICO_SDK_PATH` apontando para o diretório do SDK. Em seguida:

```bash
cmake -S firmware -B firmware/build -DPICO_BOARD=pico
cmake --build firmware/build --parallel
```

O UF2 estará em `firmware/build/ibreakpoint_pico.uf2`. Coloque o Pico em modo BOOTSEL e copie o UF2 para a unidade USB que aparecer. O firmware usa USB CDC; depois de reiniciar, descubra o dispositivo em `/dev/serial/by-id/` ou `/dev/ttyACM*`.

Se o usuário não tiver permissão de leitura/escrita, verifique o grupo do dispositivo (`ls -l /dev/ttyACM0`, substituindo pelo caminho correto). No Fedora, pode ser necessário adicionar o usuário ao grupo `dialout` e iniciar uma nova sessão:

```bash
sudo usermod -aG dialout "$USER"
```

Não altere permissões automaticamente; desconecte/reconecte o dispositivo depois de ajustar grupos.

## Comandos

```text
ibreakpoint
ibreakpoint --help
ibreakpoint --version
ibreakpoint about
ibreakpoint device list
ibreakpoint device info [DEVICE]
ibreakpoint doctor inspect [DEVICE]
ibreakpoint doctor check [DEVICE]
ibreakpoint analyze [DEVICE]
```

O caminho serial pode ser informado como último argumento ou pela variável `IBP_DEVICE=/dev/ttyACM0`. O host tenta primeiro `/dev/serial/by-id/*`, depois `/dev/ttyACM*` e `/dev/ttyUSB*`. `Device` lista transportes; isso não prova que o firmware iBreakPoint está instalado. O DocTor Who confirma o protocolo por handshake.

Cores: `--color=auto`, `--color=always`, `--color=never`. `auto` desativa cores quando stdout não é terminal e respeita `NO_COLOR`.

## Limitações conhecidas

- O protocolo é texto ASCII, sem autenticação e destinado ao laboratório local.
- O inventário inicial lista nós seriais Linux; a enumeração de VID/PID via libusb e suporte detalhado a descritores USB ainda não estão incluídos.
- Build do firmware depende do Pico SDK instalado no sistema e não pode ser validado sem esse SDK.
- Testes automatizados não substituem teste físico no Pico.
- `Analyze` verifica ferramentas, permissões, SDK e handshake quando possível; não instala pacotes nem altera o sistema.
- Os campos `Created by` e `PwnBy` permanecem placeholders para não inventar autoria.
