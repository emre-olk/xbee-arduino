#include <XBee.h>
#include <SoftwareSerial.h>
int led1=5;
int led2=6;
int led3=7;
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
XBeeAddress64 addr64_node1 = XBeeAddress64(0x0013a200, 0x419b6d1d);
XBeeAddress64 addr64_node2 = XBeeAddress64(0x0013a200, 0x41a780b6);
XBeeAddress64 addr64_node3 = XBeeAddress64(0x0013a200, 0x419b6d26);
uint8_t payload[] = { 0, 0 };
ZBRxResponse rxLoop = ZBRxResponse();
ZBRxResponse rx = ZBRxResponse();
ZBTxRequest zbTx_node1 = ZBTxRequest(addr64_node1, payload, sizeof(payload));
ZBTxRequest zbTx_node2 = ZBTxRequest(addr64_node2, payload, sizeof(payload));
ZBTxRequest zbTx_node3 = ZBTxRequest(addr64_node3, payload, sizeof(payload));
boolean controlFinish=false;
boolean node1=false;
boolean node2=false;
boolean node3=false;
int timer=0;
void setup() {
  Serial.begin(9600);
  xbee.begin(Serial);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
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
   delay(5000);
  
}

void loop() {
  if(controlFinish==true){
     xbee.readPacket();
     if (xbee.getResponse().isAvailable()) {
        xbee.getResponse().getZBRxResponse(rxLoop);
        Serial.print(rxLoop.getData(0));
        Serial.println(rxLoop.getData(1));

        if(rxLoop.getRemoteAddress64()==addr64_node1){
          digitalWrite(led1,HIGH);
          delay(1500);
          digitalWrite(led1,LOW);
        }
        if(rxLoop.getRemoteAddress64()==addr64_node2){
          digitalWrite(led2,HIGH);
          delay(1500);
          digitalWrite(led2,LOW);
        }   
        if(rxLoop.getRemoteAddress64()==addr64_node3){
          digitalWrite(led3,HIGH);
          delay(1500);
          digitalWrite(led3,LOW);
        } 
     }
  }
  else{
   isReady();
   Serial.println("dügümler hazır");
   delay(2000);
   optionTime();
   controlFinish=true;
  }
}

void isReady(){
  while(node1==false || node2==false || node3==false){
     xbee.readPacket();
     if (xbee.getResponse().isAvailable()) {
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(rx);      
        }
        if(node1==false){
         if(rx.getRemoteAddress64()==addr64_node1){
          node1=true;
          Serial.println("node1 hazır");
        } 
        }
        if(node2==false){
          if(rx.getRemoteAddress64()==addr64_node2){
          node2=true;
           Serial.println("node2 hazır");
        }
        }
       if(node3==false){
         if(rx.getRemoteAddress64()==addr64_node3){
        node3=true;
          Serial.println("node3 hazır");
        }
        }
  }
}
}
void optionTime(){
payload[1]=timer & 0xFF;
payload[0]=(timer >>8) & 0xFF;
xbee.send(zbTx_node1);
delay(2000);
payload[1]=timer & 0xFF;
payload[0]=(timer >>8) & 0xFF;
xbee.send(zbTx_node2);
delay(2000);
payload[1]=timer & 0xFF;
payload[0]=(timer >>8) & 0xFF;
xbee.send(zbTx_node3);
delay(2000);
}

ISR(TIMER1_COMPA_vect){
Serial.println(timer);
timer++;
}

