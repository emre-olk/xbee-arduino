#include <XBee.h>
#include <SoftwareSerial.h>
XBee xbee = XBee();
SoftwareSerial nss(2, 3);
uint8_t payload[] = { 0, 0 };
boolean isData=false;
boolean controlFinish=false;
XBeeAddress64 addr64_coordinator = XBeeAddress64(0x0013a200, 0x40bd2a14);
ZBTxRequest zbTx = ZBTxRequest(addr64_coordinator, payload, sizeof(payload));
ZBRxResponse rx = ZBRxResponse();
int timer=0;
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  nss.begin(9600);
  delay(5000);
}
void loop() {   
   if(controlFinish==true){
    
   }
   else{
  delay(1000);
  xbee.send(zbTx);
  Ready();
  delay(1000);
  SleepOption();
  timerAyar();
   } 
}
void Ready(){
  while(isData==false){
    xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(rx);
        if(rx.getRemoteAddress64()==addr64_coordinator){
          isData=true;
          timer=(rx.getData(0) << 8) | rx.getData(1);
        }
      }
}
}
}
void SleepOption(){
  Serial.write("+++");
     delay(1020);
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.write("ATSM 4\r");
     delay(1020);
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.write("ATIR 0x7D0\r");
     delay(1020); 
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.write("ATD3 2\r");
     delay(1020); 
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.println(Serial.read());
     Serial.write("ATWR \r");
     delay(1020);
     Serial.println(Serial.read());
     Serial.write("ATCN \r");
     delay(1020); 
}
ISR(TIMER1_COMPA_vect){
timer++;
Serial.println(timer);
}
void timerAyar(){
    cli();
  /* Ayarlamaların yapılabilmesi için öncelikle kesmeler durduruldu */
  /* Timer1 kesmesi saniyede bir çalışacak şekilde ayarlanacaktır (1 Hz)*/
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;
  /* Bir saniye aralıklar için zaman sayıcısı ayarlandı */
  TCCR1B |= (1 << WGM12);
  /* Adımlar arasında geçen süre kristal hızının 1024'e bölümü olarak ayarlandı */
  TCCR1B |= (1 << CS12) | (1 << CS10);
  /* Timer1 kesmesinin çalışabilmesi için tüm kesmeler aktif hale getirildi */
    TIMSK1 |= (1 << OCIE1A);
  /* Timer1 kesmesi aktif hale getirildi */
  sei();
  timer=timer+6;
  controlFinish=true;
}
