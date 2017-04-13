
#define ENABLE_RADIO 1

void setup(void)
{
  setClockPrescaler(0x4);
  // Print preamble
  ADCSRA = 0;
//  ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA

  Serial.begin(getClockDivisionFactor() * 1200, SERIAL_7E1);   // for linky hardware
//  Serial.begin(9600);
  
  Serial.println("Hello from Linky bridge");

#ifdef ENABLE_RADIO
  setup_radio();
#endif
}

//L’ensemble des groupes est émis cycliquement sous la f//    using SoftwareSerial::SoftwareSerial;orme suivante :
//« Start Text » STX (0x02) indique le début de la trame.
//Le corps de la trame est composé de plusieurs groupes d’informations.
//Le caractère « End Text » ETX (0x03) indique la fin de la trame.

//Chaque ligne de donnée est présentée ainsi :
// - LF (0x0A)
// - ETIQUETTE (4 à 8 caractères)
// - SP (0x20)
// - DATA (1 à 12 caractères)
// - SP (0x20)
// - CHECKSUM (caractère de contrôle : Somme de caractère)
// - CR (0x0D)
#define STX 0x02
#define ETX 0x03
#define CR 0x0d
#define LF 0x0a
#define HT 0x09
#define SPACE 0x20
String inStr = "";

void send_linky_packet() {
  
}

typedef struct s_radio_packet {
  byte type;
  byte checksum;
  byte value[4];
} radio_packet;

void parse_line(char *line, byte length) {
  radio_packet packet;
//  Serial.print("parse line '");
//  Serial.print(line);
//  Serial.print("'");
//  Serial.println("");
//  return;
  char *label = line;
  char *value;
  byte check;
  byte i;
  for (i = 0; i < length; i++)
    if (line[i] == SPACE || line[i] == HT) {
      line[i] = 0;
      i++;
      value = &(line[i]);
      break;
    }
//  Serial.print("parse line of ");
//  Serial.print(length);
//  Serial.print(" bytes '");
//  Serial.print(label);
//  Serial.print("' end at ");
//  Serial.print(i);
//  Serial.print(",");
  for (; i < length; i++)
    if (line[i] == SPACE || line[i] == HT) {
      line[i] = 0;
      i++;
      check = line[i];
      break;
    }
//  Serial.print(" value '");
//  Serial.print(value);
//  Serial.print("' end at ");
//  Serial.print(i);
//  Serial.print(",");
//  Serial.print(" check '");
//  Serial.print((char)check);
//  Serial.print("'");
//  Serial.println("");

  if (strcmp(label, "IINST") == 0) {
//    Serial.print("===================");
    inStr += value;
    unsigned long nbr = inStr.toInt();
    inStr = "";
//    Serial.print("=> IINST: ");
//    Serial.print(nbr);
//    Serial.println(" A");
    packet.type = 2;
    memcpy(&packet.value, &nbr, sizeof(unsigned long));
#ifdef ENABLE_RADIO
    bool ok = radio_write((byte*)&packet, sizeof(packet));
#endif
//    Serial.println("IINST");
  }
  else if (strcmp(label, "PAPP") == 0) {
//    Serial.print("===================");
    inStr += value;
    unsigned long nbr = inStr.toInt();
    inStr = "";
//    Serial.print("=> PAPP: ");
//    Serial.print(nbr);
//    Serial.println(" VA");

    packet.type = 3;
    memcpy(&packet.value, &nbr, sizeof(unsigned long));
#ifdef ENABLE_RADIO
    bool ok = radio_write((byte*)&packet, sizeof(packet));
#endif
  }
}

char line[25];
byte position = 0;
byte in_frame = 0;

void loop_linky() {

  if (!Serial.available())
    return;

  byte c = Serial.read();
//  Serial.write(c);
  if (!in_frame) {
    if (c == STX) {
      in_frame = 1;
//      Serial.println("start frame");
    }
    else {
      // skip char out of frame
      return;
    }
  }
  else {
    if (c == ETX) {
      // end of frame, send data
//      Serial.println("end frame");
//      in_frame = 0;
//      delay(5000);
//      send_linky_packet();
    }
    else if (c == CR) {
      line[position] = 0;
//      Serial.print("...[");
//      Serial.print(line);
//      Serial.println("]...");
      parse_line(line, position);
      position = 0;
    }
    else if (c == LF) {
      memset(line, 0, 25);
      position = 0;
    }
    else {
      if (position >= 25) {
        Serial.println("buffer overflow, protocol error");
        in_frame = 0;
        position = 0;
      }
      line[position++] = c;
//      Serial.write(BTSerie.read());
    }
  }
}

long debounce = 0;
byte counter = 0;

void loop(void) {
//  loop_radio();
//  return;

  loop_linky();

//  if (BTSerie.available())
//    Serial.write(BTSerie.read());
    
//  if (Serial.available())
//    Serial.write(Serial.read());

  unsigned long time = millis();
  if (time - debounce > 1000) {
    debounce = time;

//    Serial.print("Hi serial ");
//    Serial.print(time);
//    BTSerie.print("Hi soft");
//    BTSerie.print(time);
//    BTSerie.println(".");
    radio_packet packet;
    packet.type = 1;
    memcpy(&packet.value, &time, sizeof(unsigned long));
#ifdef ENABLE_RADIO
    bool ok = radio_write((byte*)&packet, sizeof(packet));
#endif

//    delay(20);
//    radio_close();
//    delay(5000);
//    radio_open();
//    radio_wakeup();
////    radio_wakeup();
//    Serial.print(" returns: ");
//    Serial.print(ok? "ok": "fail");
//    Serial.println(".");
//    if (counter++ % 10 == 0) {
//      radio_close();
//      delay(5000);
//      radio_open();
//      counter = 0;
//    }
  }
}


