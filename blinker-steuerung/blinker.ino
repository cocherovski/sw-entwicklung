
/*********
 * Datei: bh__wb_bns_vorlage
 * Autor, Datum: Reiner Kriesten, 2016-02-25
 * aktualisierung: Christian Gutenkunst, 2016-09-29
 * Beschreibung: Datei, welche die Übung Blinkerhebel/Warnblinker auf den Arduino transferiert
 *
 * Schnittstellen:
 * - Eingänge: 
 * Warnblinker: Digi 4
 * Blinkerhebel: Digi 5, 6  (2 Pins, 1: links ja/nein; 2: rechts ja/nein)
 * Bordnetzspannung: Analog 0 A0 ([0V-5V] -> 10 Bit {0,...,1023})
 * - Ausgänge: 
 * Licht_VL: Digi 7
 * Licht_HL: Digi 2
 * Licht_VR: Digi 8
 * Licht_HR: Digi 3
 * 
 * Programmierkonventionen:
 * Verwendung der folgenden Datentypen:
 * - float, uint8 (unsigned char), uint16 (unsigned int), int (für Einlesen Analogspannung)
 * - Variablen und Funktionen: keinschreibung_und_underscore
 * - Defines: GROSSSCHREIBUNG
 * - Benamung der Pin: Prefix pin_
 * 
 * Debug-Realisierung: wird über die Ausgabe der seriellen Schnittstelle realisiert
 * 
 * 
 * 
 * Funktionen - Arduino-Anpassung:
 * uint read_bh(uint8 BH_links, uint8 BH_rechts): wandelt die beiden Binäreingänge in eine Variable 
 *                                                mit Werten 0(AUS), 1(links), 2(rechts) um
 * float read_bns(int analog): wandelt den 10-Bit Wert zurück in einen Spannungswert [0,5]
 * 
 ***********/


/***************************************************************************************************
 * Ab hier: Defines, Prototypen, Globale Varibalen, Pinbenamungen
 **************************************************************************************************/


/* Defines */
#define AUS 0
#define LINKS 1
#define RECHTS 2
#define BEIDE 3

/* Typedef */
typedef unsigned int uint16;
typedef unsigned char uint8;


/* Funktionsprototyen */
//uint8 read_bh(uint8 BH_links, uint8 BH_rechts);
void read_input(void);
void write_output(void);
void pwm_anzeit_berechnung(float bns, uint8 * an_ticks_1ms);
void lichtansteuerung(uint8 blinkprio, uint8 an_ticks_1ms, uint8 * blink_vl, uint8 *blink_vr, uint8 *blink_hl, uint8 * blink_hr);
void blinker_warnblinker_priorisierung(uint8 wb, uint8 bh, uint8  * blink_prio);
void rising_edge_bh(uint8 in, uint8 * out);
void rising_edge_wb(uint8 in, uint8 * out);
void modell_logik(void);

/* Globale Variablen -  dienen als Schnittstelle für den "Simulink-Modellcode"*/
uint8 wb, bh, licht_vl, licht_hl, licht_vr, licht_hr ;
float bns;


/* Pinbenamung gemäß Arduino */
int pin_wb = 4;
int pin_bh_links = 5;
int pin_bh_rechts = 6;
int pin_bns = A0;
int pin_licht_vl = 7;
int pin_licht_hl = 2;
int pin_licht_vr = 8;
int pin_licht_hr = 3;


/***************************************************************************************************
 * Ab hier Setup + Loop
 ***************************************************************************************************/

void setup() {
  // Setup der Eingänge /Ausgänge
  pinMode(pin_wb, INPUT_PULLUP); // Input_PULLUP
  pinMode(pin_bh_links, INPUT_PULLUP);
  pinMode(pin_bh_rechts, INPUT_PULLUP);
  // Analog-Pin: keine Aktion notwendig
  pinMode(pin_licht_vl, OUTPUT);
  pinMode(pin_licht_hl, OUTPUT);
  pinMode(pin_licht_vr, OUTPUT);
  pinMode(pin_licht_hr, OUTPUT);
  // Setup der seriellen Schnittstelle für Debug
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // Realisierung nach EVA - Eingabe Verarbeitung Ausgabe im 1ms Raster
  unsigned long zeit;
  zeit=millis();
  //Serial.print(zeit); Serial.print(" vor read \n");
  read_input();
  //zeit=millis();
  //Serial.print(zeit); Serial.print(" nach read \n");
  modell_logik();
  //zeit=millis();
  //Serial.print(zeit); Serial.print(" nach Logik \n");
  write_output();
  //zeit=millis();
  //Serial.print(zeit); Serial.print(" nach Write \n");
  delay(1); // 1ms-Wartezeit

  Serial.print("bns: \t"); Serial.print(bns); Serial.print("\tbh:\t");Serial.print(bh);Serial.print("\twb:\t");Serial.println(wb);

  

}

/***************************************************************************************************
 * Ab hier Implementierung der Arduino "Basis-SW" für die Signalpfade
 **************************************************************************************************/

/**********************
Funktion: void read_input(void)
- Ein-/Ausgabeparameter: 
- keine

- Beschreibung: Lese die Inputs von den Pins ein uns speichere die Werte in die globalen Variablen,
                die dem eigentlichen Modell zur Verfügung gestellt werden
- ACHTUNG: NEGATIVE LOGIK, SIEHE SCHALTPLAN
***********************/
void read_input(void)
{
  wb=!digitalRead(pin_wb);
  if((digitalRead(pin_bh_links)==0)&&(digitalRead(pin_bh_rechts)==0)){
    bh=AUS;
  }
  else if(digitalRead(pin_bh_links)==0){
    bh=LINKS;
  }
  else if(digitalRead(pin_bh_rechts)==0){
    bh=RECHTS;
  }
  else{bh=AUS;}
  read_bns(analogRead(A0)); 
}

/**********************
Funktion: void write_output(void)
- Ein-/Ausgabeparameter: 
- keine

- Beschreibung: Schreibe die Werte der globalen Variablen, die den Output des Simulinks-Modells
                beinhalten, auf die Pins heruas
- ACHTUNG: NEGATIVE LOGIK, SIEHE SCHALTPLAN
***********************/
void write_output(void)
{
  digitalWrite(pin_licht_vl,licht_vl);
  digitalWrite(pin_licht_vr,licht_vr);
  digitalWrite(pin_licht_hl,licht_hl);
  digitalWrite(pin_licht_hr,licht_hr);
}


/**********************
Funktion: uint8 read_bh(uint8 BH_links, uint8 BH_rechts)
- Ein-/Ausgabeparameter: 
- Eingänge uint8 BH_links: Links-Wunsch ja/nein
- Eingänge uint8 BH_rechts: Rechts-Wunsch ja/nein
- Ausgang: logischer Ausgang des BH mit Werten 0(AUS), 1 (LINKS), 2 (RECHTS)

- Beschreibung: Error-Handling, falls LINKS und RECHTS aktiv: Ausgabe "AUS" 
***********************/
//uint8 read_bh(uint8 BH_links, uint8 BH_rechts)
//{
//  if((digitalRead(pin_bh_links==0)&&(digitalRead(pin_bh_rechts==0)){
//    bh=AUS;
//  }
//  else if(digitalRead(pin_bh_links==0)){
//    bh=LINKS;
//  }
//  else if(digitalRead(pin_bh_rechts==0)){
//    bh=RECHTS;
//  }
//  else{bh=AUS;}  
//}

/**********************
Funktion: float read_bns(int analog)
- Ein-/Ausgabeparameter: 
- Eingänge int analog: Analogwert {0...1023}
- Ausgang: Bordnetzspannung [0V, 5V]

- Beschreibung: Umrechnung via out/in = 5.0 / 1023
  Berechnung auf Basis von Gleitkommazahlen notwendig 
***********************/
float read_bns(int analog)
{
  bns = analog*5./1023;
}


/***************************************************************************************************
 * Ab hier Implementierung des eigentlichen Modells auf Basis von S-Function Gesamtmodell in Matlab
 ***************************************************************************************************/

void modell_logik(void)
{
  uint8 an_ticks_1ms=0, blink_prio=0;
  pwm_anzeit_berechnung(bns, &an_ticks_1ms);
  blinker_warnblinker_priorisierung(wb, bh, &blink_prio);
  lichtansteuerung(blink_prio, an_ticks_1ms, &licht_vl, &licht_vr, &licht_hl, &licht_hr);
}


/**********************
Funktion: void blinker_warnblinker_priorisierung(uint8 wb, uint8 bh, uint8  * blink_prio)
- Ein-/Ausgabeparameter: 
  - Eingang uint8 wb: Warnblinker-Request: 0 (AUS), 1(EIN)
  - Eingang uint8 bh: Blinkerhebel-Request, Werte: AUS(0)/ LINKS (1)/ RECHTS (2) / BEIDE (3)
  - Ausgang uint8 * blink_prio: Ausgangspriorisierung, Werte:AUS(0)/LINKS (1)/RECHTS (2)/BEIDE(3)
- Semantik: Realisierung der Blinkerhebel-/Warnblinklogik gemäß Simulink-Modellblock
- Randbedingung: Direkter Übergang links <-> rechts in Request wird als nicht existent angenommen
- Implementierung: 
  - gemäß Simulink-Modellblock unter Verwendung von Rising-Edge-Blöcken
***********************/
void blinker_warnblinker_priorisierung(uint8 wb, uint8 bh, uint8  * blink_prio)
{ 
  static bool werHatPrio=0;
  uint8 wbFlanke, bhFlanke;

  rising_edge_wb(wb,&wbFlanke);
  rising_edge_bh(bh,&bhFlanke);

  if(werHatPrio==0)
    {
      if(wbFlanke==1)
      {werHatPrio=1;}
      else if(bh == LINKS)
      {*blink_prio = LINKS;}
      else if(bh == RECHTS)
      {*blink_prio = RECHTS;}
      else if(wb == 1)
      {*blink_prio = BEIDE;}
      else {*blink_prio = AUS;}
    }
    else
    {
      if(bhFlanke==1)
      {werHatPrio=0;}
      else if(wb == 1)
      {*blink_prio = BEIDE;}
      else if(bh == LINKS)
      {*blink_prio = LINKS;}
      else if(bh == RECHTS)
      {*blink_prio = RECHTS;}
      else {*blink_prio = AUS;}
    }
}

/**********************
Funktion: void pwm_anzeit_berechnung(float bns, uint8 * an_ticks_1ms)
- Semantik: Berechnung der PWM-Zeit auf Basis der BNS gemäß Simulink-Modell
- Ein-/Ausgabeparameter: 
  - Eingang float bns: Bordnetzspannung [0V, 5V]
  - Ausgang uint8 * an_ticks_1ms: Anzahl Ausgangsticks (zwischen 0 und 100)
- Implementierung: gemäß Simulink-Modell
***********************/
void pwm_anzeit_berechnung(float bns, uint8 * an_ticks_1ms)
{
  if(bns>5){*an_ticks_1ms=50;}
  else if(bns<2.5){*an_ticks_1ms=100;}
  else{*an_ticks_1ms=-20*bns+150;}
}


/**********************
Funktion: void rising_edge_bh(uint8 in, uint8 * out)
- Ein-/Ausgabeparameter:
  - Eingang uint8 in: boolescher Eingang
  - Ausgang uint8 * out: boolescher Ausgang 
- Semantik: Hilfsfunktion, welche von boolescher Variable den Trigger nach oben detektiert
***********************/
void rising_edge_bh(uint8 in, uint8 * out)
{
  static uint8 inAlt=0;
  if(inAlt < in){*out = 1;}
  else {*out = 0;}
  inAlt = in; 
}

/**********************
Funktion: void rising_edge_wb(uint8 in, uint8 * out)
- Ein-/Ausgabeparameter:
  - Eingang uint8 in: boolescher Eingang
  - Ausgang uint8 * out: boolescher Ausgang 
- Semantik: Hilfsfunktion, welche von boolescher Variable den Trigger nach oben detektiert
***********************/
void rising_edge_wb(uint8 in, uint8 * out)
{
  static uint8 inAlt=0;
  if(inAlt < in){*out = 1;}
  else {*out = 0;}
  inAlt = in;
}



/**********************
Funktion: void lichtansteuerung(uint8 blinkprio, uint8 an_ticks_1ms, uint8 * blink_vl, uint8 *blink_vr, uint8 *blink_hl, uint8 * blink_hr)
- Ein-/Ausgabeparameter:
  - Eingang uint8 blinkprio: Auswertewunsch, welche Blinkrichtung aktiviert wird: 
                             AUS(0)/ LINKS (1)/ RECHTS (2) / BEIDE (3)
  - Eingang uint8 an_ticks_1ms: Anzahl der AN-Ticks für unterlegte PWM mit 100 ms Periodendauer
  - Ausgang uint8 * blink_vl: boolescher Ausgang, Aktivierung Blinker vl
  - Ausgang uint8 * blink_vl: boolescher Ausgang, Aktivierung Blinker vr
  - Ausgang uint8 * blink_hl: boolescher Ausgang, Aktivierung Blinker hl
  - Ausgang uint8 * blink_hr: boolescher Ausgang, Aktivierung Blinker hr
- Semantik: Schaltet die Lichter auf Basis des Einschaltwunsches und der vorgegebenen PWM-Helligkeit
- Implementierung: gemäß Simulink-Modell
***********************/
void lichtansteuerung(uint8 blinkprio, uint8 an_ticks_1ms, uint8 * blink_vl, uint8 *blink_vr, uint8 *blink_hl, uint8 * blink_hr)
{
  static uint8 blink_zustand=AUS;
  static uint8 counter1ms=0;
  static unsigned long counter100ms=0;
  static unsigned long anzeit=0;

  if(blink_zustand==AUS){
    *blink_vr=1; *blink_hr=1; *blink_vl=1; *blink_hl=1;
    counter100ms=0;
    counter1ms=0;
    if(blinkprio==LINKS){
      anzeit=an_ticks_1ms;
      blink_zustand=LINKS;
    }else if(blinkprio==RECHTS)
    {
      anzeit=an_ticks_1ms;
      blink_zustand=RECHTS;
    }else if(blinkprio==BEIDE){
      anzeit=an_ticks_1ms;
      blink_zustand=BEIDE;
    }else{;}

  }else if(blink_zustand==LINKS){
    counter1ms=(counter1ms+1)%100;
    if(counter1ms==0 && blinkprio==LINKS){
    anzeit=an_ticks_1ms;
    counter100ms++;
    }
    else if(counter1ms==0 && blinkprio==BEIDE){
      anzeit=an_ticks_1ms;
      counter100ms=0;
      blink_zustand=BEIDE;
    }
    else if(counter1ms==0 && blinkprio==RECHTS){
      anzeit=an_ticks_1ms;
      counter100ms=0;
      blink_zustand=RECHTS;
    }
    else if(counter1ms==0){
      anzeit=an_ticks_1ms;
            blink_zustand=AUS;
    }
    else if(counter1ms<=anzeit && ((counter100ms%10)<5)){
      *blink_vr=1; *blink_hr=1; *blink_vl=0; *blink_hl=0;
    }
    else{
      *blink_vr=1; 
      *blink_hr=1;
      *blink_vl=1;
      *blink_hl=1;
    }
  }
  else if(blink_zustand==RECHTS){
    counter1ms=(counter1ms+1)%100;
    if(counter1ms==0 && blinkprio==RECHTS){
      anzeit=an_ticks_1ms;
      counter100ms++;
    }
    else if(counter1ms==0 && blinkprio==BEIDE){
      anzeit=an_ticks_1ms;
      counter100ms=0;
      blink_zustand=BEIDE;
    }
    else if(counter1ms==0 && blinkprio==LINKS){
      anzeit=an_ticks_1ms;
      counter100ms=0;
      blink_zustand=LINKS;
    }
    else if(counter1ms==0){
      anzeit=an_ticks_1ms;
            blink_zustand=AUS;
    }
    else if(counter1ms<=anzeit && ((counter100ms%10)<5)){
      *blink_vr=0;*blink_hr=0; *blink_vl=1; *blink_hl=1;
    }
    else{
      *blink_vr=1; *blink_hr=1; *blink_vl=1; *blink_hl=1;
    }
  }
  else{
    counter1ms=(counter1ms+1)%100;
    if(counter1ms==0 && blinkprio==BEIDE){
      anzeit=an_ticks_1ms;
      counter100ms++;
    }
    else if(counter1ms==0 && blinkprio==LINKS)
    {
      anzeit= an_ticks_1ms;
      counter100ms=0;
      blink_zustand=LINKS;
    }
    else if(counter1ms==0 && blinkprio==RECHTS){
      anzeit=an_ticks_1ms;
      counter100ms=0;
      blink_zustand=RECHTS;
    }
    else if(counter1ms==0){
      anzeit=an_ticks_1ms;
            blink_zustand=AUS;
    }
    else if(counter1ms<=anzeit && ((counter100ms%10)<5)){
      *blink_vr=0; *blink_hr=0; *blink_vl=0; *blink_hl=0;
    }
    else{
      *blink_vr=1; *blink_hr=1; *blink_vl=1; *blink_hl=1;
    }
  }
}


