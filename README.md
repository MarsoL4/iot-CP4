# CP4: Classificador e Lógica XOR com ESP32

## Integrantes
- Nome: Enzo Marsola RM556310, Cauan da Cruz RM558238, Igor Barrocal RM555217

---

## Descrição do Projeto

Este projeto utiliza um **ESP32** para extrair duas features (RMS e Pico-a-Pico) de um sinal analógico (potenciômetro ou sensor), implementa uma lógica XOR usando essas features para controlar dois LEDs, e treina classificadores (Decision Tree e SVM) em Python, exportando-os para código C.

---

## Materiais Utilizados

- ESP32 DevKit
- 1 potenciômetro ou sensor analógico (ex: LDR, NTC)
- 2 LEDs + 2 resistores de 220 Ω
- Protoboard, jumpers

---

## Montagem do Circuito

- **Sensor/POT:**  
  - Pino central → GPIO34 (ADC)  
  - Laterais → 3V3 e GND
- **LED_XOR_TRUE:** GPIO2 → resistor → LED → GND
- **LED_XOR_FALSE:** GPIO4 → resistor → LED → GND

> ![FOTO DO CIRCUITO AQUI](parte2/Image.jpg)

---

## Parte 1: Lógica XOR

### Funcionamento

- O ESP32 lê 200 amostras do ADC (200 ms a 1 kHz).
- Calcula as features **RMS** e **Pico-a-Pico (PTP)** do sinal normalizado.
- As features são comparadas com thresholds e convertidas em booleanos:
  - `A = (RMS >= TH_RMS)`
  - `B = (PTP >= TH_PTP)`
- Realiza o XOR: `Y = A XOR B`
- Acende os LEDs conforme o resultado:
  - Y = 1: LED_XOR_TRUE = ON, LED_XOR_FALSE = OFF
  - Y = 0: LED_XOR_TRUE = OFF, LED_XOR_FALSE = ON

### Explicação das Features

> Escolhemos as features RMS e Pico-a-Pico porque juntas permitem distinguir entre variações rápidas e lentas do sinal. O RMS indica energia média (amplitude geral), enquanto o Pico-a-Pico mostra o quanto o sinal oscilou durante a janela. Assim, conseguimos gerar duas condições lógicas para o XOR a partir de apenas um sensor.

---

## Parte 2: Classificador

### Coleta de Dados

- **Script do ESP32** foi adaptado para calcular e registrar as features: **mean, std, rms, ptp**.
- Os dados foram coletados em janelas de 200 amostras, exportados via Serial e rotulados manualmente (`normal`, `variacao`).
- O arquivo utilizado está em:  
  - `./parte2/sensor_dados.csv`

### Treinamento e Exportação

- Utilizamos **PyCaret** e **m2cgen** para treinar dois classificadores:
  - Decision Tree (DT)
  - Linear SVM (SVM)

- O script de treino está em:  
  - `./parte2/script.py`
  - ou `./parte2/script.ipynb`

## Instruções para Execução

### Parte 1 — Lógica XOR

1. **Monte o circuito** conforme o diagrama e a foto.
2. **Carregue o código** do ESP32 usando Arduino IDE/PlatformIO.
3. **Abra o Monitor Serial** em 115200 baud para ver os logs.
4. **Gire o potenciômetro** ou varie o sensor e observe os LEDs alternando.

### Parte 2 — Coleta, Treinamento e Exportação

1. **Colete os dados** pelo terminal serial e salve em CSV.
2. **Rode o script Python** para treinar e exportar os modelos.
   - `python treino_modelo.py`
3. **Consulte os arquivos gerados** para análise e relatório.

---

## Vídeo do Protótipo

> ![VÍDEO DO PROTÓTIPO EM FUNCIONAMENTO](parte2/MicrosoftTeams-video-ezgif.com-video-to-gif-converter.gif)
