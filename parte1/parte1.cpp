#include <Arduino.h>
constexpr int ADC_PIN = 34;
constexpr int LED_XOR_TRUE = 2;
constexpr int LED_XOR_FALSE = 4;
constexpr int FS_HZ = 1000;
constexpr int WIN_MS = 200;
constexpr int N = FS_HZ * WIN_MS / 1000;
static float buf[N];

float adc_to_unit(int raw) { return raw / 4095.0f; }
void calc_feats(const float *x, int n, float &rms, float &ptp) {
  float xmin = x[0], xmax = x[0];
  double sumsq = 0.0;
  for(int i=0;i<n;++i){
    float v = x[i];
    sumsq += v*v;
    if(v<xmin) xmin=v;
    if(v>xmax) xmax=v;
  }
  rms = sqrt(sumsq / n);
  ptp = xmax - xmin;
}

float TH_RMS = 0.5f;
float TH_PTP = 0.2f;

void setup(){
  Serial.begin(115200);
  pinMode(LED_XOR_TRUE, OUTPUT);
  pinMode(LED_XOR_FALSE, OUTPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  Serial.println("# seq,rms,ptp,A,B,Y");
}
void loop(){
  const uint32_t Ts = 1000000UL/FS_HZ;
  static uint32_t seq=0;
  for(int i=0;i<N;++i){
    buf[i] = adc_to_unit(analogRead(ADC_PIN));
    delayMicroseconds(Ts);
  }
  float rms, ptp;
  calc_feats(buf, N, rms, ptp);
  bool A = (rms >= TH_RMS);
  bool B = (ptp >= TH_PTP);
  bool Y = (A ^ B);
  digitalWrite(LED_XOR_TRUE, Y ? HIGH : LOW);
  digitalWrite(LED_XOR_FALSE, Y ? LOW : HIGH);
  Serial.print(seq++); Serial.print(',');
  Serial.print(rms,3); Serial.print(',');
  Serial.print(ptp,3); Serial.print(',');
  Serial.print(A); Serial.print(',');
  Serial.print(B); Serial.print(',');
  Serial.println(Y);
}
