#include <Arduino.h>
constexpr int ADC_PIN = 34;
constexpr int LED_XOR_TRUE = 2;
constexpr int LED_XOR_FALSE = 4;
constexpr int FS_HZ = 1000;
constexpr int WIN_MS = 200;
constexpr int N = FS_HZ * WIN_MS / 1000;
static float buf[N];

// Função para normalizar o valor lido do ADC (0..4095) para 0..1
float adc_to_unit(int raw) { 
  return raw / 4095.0f; 
}

// Função para calcular mean, std, rms e ptp
void calc_feats(const float *x, int n, float &mean, float &std, float &rms, float &ptp) {
  float xmin = x[0], xmax = x[0];
  double sum = 0.0, sumsq = 0.0;
  for(int i = 0; i < n; ++i){
    float v = x[i];
    sum += v;
    sumsq += v * v;
    if(v < xmin) xmin = v;
    if(v > xmax) xmax = v;
  }
  mean = sum / n;
  rms = sqrt(sumsq / n);
  ptp = xmax - xmin;
  // calcular std
  double var = 0.0;
  for(int i = 0; i < n; ++i){
    double diff = x[i] - mean;
    var += diff * diff;
  }
  std = sqrt(var / n);
}

float TH_RMS = 0.5f;
float TH_PTP = 0.2f;

void setup() {
  Serial.begin(115200);
  pinMode(LED_XOR_TRUE, OUTPUT);
  pinMode(LED_XOR_FALSE, OUTPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  Serial.println("# seq,mean,std,rms,ptp,A,B,Y");
}

void loop() {
  const uint32_t Ts = 1000000UL / FS_HZ;
  static uint32_t seq = 0;
  // 1) Captura janela
  for(int i = 0; i < N; ++i){
    buf[i] = adc_to_unit(analogRead(ADC_PIN));
    delayMicroseconds(Ts);
  }
  // 2) Features
  float mean, std, rms, ptp;
  calc_feats(buf, N, mean, std, rms, ptp);

  // 3) Booleans
  bool A = (rms >= TH_RMS);
  bool B = (ptp >= TH_PTP);

  // 4) XOR e LEDs
  bool Y = (A ^ B);
  digitalWrite(LED_XOR_TRUE, Y ? HIGH : LOW);
  digitalWrite(LED_XOR_FALSE, Y ? LOW : HIGH);

  // 5) Log para CSV: mean,std,rms,ptp,A,B,Y
  Serial.print(seq++); Serial.print(',');
  Serial.print(mean, 3); Serial.print(',');
  Serial.print(std, 3); Serial.print(',');
  Serial.print(rms, 3); Serial.print(',');
  Serial.print(ptp, 3); Serial.print(',');
  Serial.print(A); Serial.print(',');
  Serial.print(B); Serial.print(',');
  Serial.println(Y);

  // Pequeno delay para facilitar leitura no monitor serial (opcional)
  // delay(10);
}