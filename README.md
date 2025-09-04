# Projeto: Classificador e Lógica XOR com ESP32

## Integrantes
- Nome: Enzo Marsola, Cauan da Cruz, Igor Barrocal
- RM: RM556310, RM558238, RM555217

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

> ![FOTO DO CIRCUITO AQUI](caminho/para/sua_foto.jpg)  

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

#### Trecho principal do código:
```cpp
// (Insira aqui o trecho relevante do código usado para lógica XOR e cálculo das features)
```

### Demonstração (vídeo/GIF)
- *(Insira aqui o vídeo ou GIF curto mostrando os LEDs alternando de acordo com o XOR, ou coloque um link para o arquivo de vídeo quando estiver pronto.)*

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
- Os códigos dos modelos exportados estão em:
  - `./parte2/model_dt.c`
  - `./parte2/model_svm.c`
- O script de treino está em:  
  - `./parte2/treino_modelo.py`

### Métricas e Justificativa

- Após validação cruzada, os modelos apresentaram as seguintes métricas:

  | Modelo      | Accuracy | F1-score |
  |-------------|----------|----------|
  | DecisionTree|   xx%    |   xx     |
  | SVM         |   xx%    |   xx     |

- **Escolha:**  
  *(Indique aqui qual modelo foi escolhido e inclua uma justificativa de 3–5 linhas. Ex:)*  
  > O modelo Decision Tree foi escolhido para embarcar pois apresentou melhor performance na validação e é mais eficiente em recursos para o ESP32. O SVM foi mantido para comparação no relatório.

---

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

## Fotos do Protótipo

> ![FOTO DO PROTÓTIPO 1](caminho/para/foto1.jpg)

---

## Observações Finais

- **Ordem das features** no vetor C deve ser igual à do treino.
- **Todos os arquivos** do projeto estão organizados nas pastas `/parte1/` e `/parte2/`.
- **Dúvidas ou problemas:** consulte este README ou entre em contato com os integrantes.

---
