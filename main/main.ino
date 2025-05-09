/*
# ==============================================================================
# Nome do Desenvolvedor: João Victor Madureira de Oliveira Almeida
# Data de Criação: 04/04/2025
# Descrição: Este módulo implementa a função de gravador RFID com dicionário pré estabelecido, e teste dos sensores. Exceto os sensores I²C
# Última Modificação: 09/05/2025
# Versão: 1.4
# ==============================================================================
*/

extern "C" void init_buttons(); // Chama o assembly

#include <Wire.h>
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <NewTone.h>
#include <DallasTemperature.h>
#include <MFRC522.h>
#include <dht.h>


// Pinos e definições
#define SS_PIN  42
#define RST_PIN 255 // não utilizado
#define BUZZER_PIN 26


#define BLOCO_GRAVACAO_EDUCA 1  // bloco 4 do setor 1
#define BLOCO_GRAVACAO_COLETOR 1  // bloco 4 do setor 1
#define BLOCO_GRAVACAO_REI 1  // bloco 4 do setor 1

// Instanciação dos objetos
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);


// Pinos dos botões
const byte btnUp = 47;
const byte btnDown = 38;
const byte btnEnter = 49;
const byte btnBack = 48;

const byte sensorpin_1 = 62;
const byte sensorpin_2 = 2;

// Controle de menu
byte currentMenuIndex = 0;
byte currentLevel = 0;
byte cursorPosition = 0;

const byte MAX_ITEMS = 10;
const byte LCD_ROWS = 4;

bool showingWelcome = true;

// Debounce
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;
unsigned long lastDebounceTimeEnter = 0;
unsigned long lastDebounceTimeBack = 0;
const unsigned long debounceDelay = 250;

// Piscar "Press Enter"
unsigned long lastBlinkTime = 0;
bool blinkVisible = true;

// Estrutura de menu
struct MenuItem {
  const char* title;
  MenuItem* subMenu;
  int subMenuSize;
};

// Submenus
MenuItem menuColetorDados[] = {
  {"Fisica (laranja)", NULL, 0},
  {"Matematica (verm.)", NULL, 0},
  {"Quimica (verde)", NULL, 0},
  {"Biologia (azul)", NULL, 0}
};

MenuItem gravacaoCartas[] = {
  {"Coletor de Dados", menuColetorDados, 4},
  {"Educa Nave", NULL, 0},
  {"Robo Rei", NULL, 0}
};

MenuItem testeSensores[] = {
  {"Educa Nave", NULL, 0},
  {"Laser 650nm", NULL, 0},
  {"Sens. Botao", NULL, 0},
  {"Sens. Chuva", NULL, 0},
  {"Sens. Corrente SCT", NULL, 0},
  {"Sens. Decibel", NULL, 0},
  {"Sens. Forca", NULL, 0},
  {"Sens. Gas", NULL, 0},
  {"Sens. Hall", NULL, 0},
  {"Sens. IR", NULL, 0},
  {"Sens. Peltier", NULL, 0},
  {"Sens. PIR", NULL, 0},
  {"Sens. RFID", NULL, 0},
  {"Sens. Temperatura", NULL, 0},
  {"Sens. Umidade", NULL, 0},
  {"Sens. Umi Rel/Temp", NULL, 0},
  {"Sens. Ultrassonico", NULL, 0},
  {"Sens. Vibracao", NULL, 0},
  {"Sens. pH", NULL, 0}
};


MenuItem mainMenu[] = {
  {"Gravador RFID", gravacaoCartas, 3},
  {"Teste Sensores", testeSensores, 20}
};

// Pilha de menus
MenuItem* menuStack[5];
int menuSizeStack[5];
int indexStack[5];

void setup() {
  //Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  init_buttons();  // Chama a função em Assembly
  //pinMode(btnUp, INPUT_PULLUP);
  //pinMode(btnDown, INPUT_PULLUP);
  //pinMode(btnEnter, INPUT_PULLUP);
  //pinMode(btnBack, INPUT_PULLUP);

  // Mostra tela de boas-vindas
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("CITTIUS");
  lcd.setCursor(3, 1);
  lcd.print("Teste & RFID");
  lcd.setCursor(4, 3);
  lcd.print("Press Enter");
}

void loop() {
  if (showingWelcome) {
    unsigned long now = millis();
    if (now - lastBlinkTime > 500) {
      lastBlinkTime = now;
      blinkVisible = !blinkVisible;
      lcd.setCursor(4, 3);
      if (blinkVisible) {
        lcd.print("Press Enter");
      } else {
        lcd.print("             ");
      }
    }

    if (isButtonPressed(btnEnter, lastDebounceTimeEnter)) {
      showingWelcome = false;
      menuStack[0] = mainMenu;
      menuSizeStack[0] = 2;
      indexStack[0] = 0;
      cursorPosition = 0;
      showMenu();
    }
    return;
  }

  if (isButtonPressed(btnUp, lastDebounceTimeUp)) {
    if (indexStack[currentLevel] > 0) {
      indexStack[currentLevel]--;
      if (cursorPosition > 0) {
        cursorPosition--;
      } else {
        showMenu();
      }
    }
    showMenu();
  }

  if (isButtonPressed(btnDown, lastDebounceTimeDown)) {
    if (indexStack[currentLevel] < menuSizeStack[currentLevel] - 1) {
      indexStack[currentLevel]++;
      if (cursorPosition < LCD_ROWS - 1 && cursorPosition < menuSizeStack[currentLevel] - 1) {
        cursorPosition++;
      } else {
        showMenu();
      }
    }
    showMenu();
  }

  if (isButtonPressed(btnEnter, lastDebounceTimeEnter)) {
    MenuItem selected = menuStack[currentLevel][indexStack[currentLevel]];
    if (selected.subMenu != NULL) {
      currentLevel++;
      menuStack[currentLevel] = selected.subMenu;
      menuSizeStack[currentLevel] = selected.subMenuSize;
      indexStack[currentLevel] = 0;
      cursorPosition = 0;
      showMenu();
    } else {
      const char* title = selected.title;

      if (strcmp(title, "Robo Rei") == 0) gravacao_rei();

      else if (strcmp(title, "Fisica (laranja)") == 0) gravacao_coletor_fis();
      else if (strcmp(title, "Matematica (verm.)") == 0) gravacao_coletor_mat();
      else if (strcmp(title, "Quimica (verde)") == 0) gravacao_coletor_quim();
      else if (strcmp(title, "Biologia (azul)") == 0) gravacao_coletor_bio();
      else if (strcmp(title, "Educa Nave") == 0) gravacao_educanave();
      else if (strcmp(title, "Sens. Forca") == 0) sensor_forca();
      else if (strcmp(title, "Sens. Botao") == 0) sensor_botao();
      else if (strcmp(title, "Sens. Gas") == 0) sensor_gas();
      else if (strcmp(title, "Sens. Ultrassonico") == 0) sensor_ultrassonico();
      else if (strcmp(title, "Sens. pH") == 0) sensor_ph();
      else if (strcmp(title, "Sens. IR") == 0) sensor_IR();
      else if (strcmp(title, "Sens. Umidade Solo") == 0) sensor_umidadeSolo();
      else if (strcmp(title, "Laser 650nm") == 0) sensor_laser();
      else if (strcmp(title, "Sens. Temperatura") == 0) sensor_temperatura(); 
      else if (strcmp(title, "Sens. Hall") == 0) sensor_hall(); 
      else if (strcmp(title, "Sens. Chuva") == 0) sensor_chuva();
    //else if (strcmp(title, "Sens. RFID") == 0) sensor_rfid();
      else if (strcmp(title, "Sens. Vibracao") == 0) sensor_vibracao();
      else if (strcmp(title, "Sens. Peltier") == 0) sensor_peltier();
      else if (strcmp(title, "Sens. PIR") == 0) sensor_pir();
      else if (strcmp(title, "Sens. Decibel") == 0) sensor_decibel();
      else if (strcmp(title, "Sens. Corrente SCT") == 0) sensor_correnteSCT();
      else if (strcmp(title, "Sens. Umi Rel/Temp") == 0) sensor_umidaderel();
      else if (strcmp(title, "Sens. RFID") == 0) sensor_RFID();

      lcd.clear();
      showMenu();
    }
  }

  if (isButtonPressed(btnBack, lastDebounceTimeBack)) {
    if (currentLevel > 0) {
      currentLevel--;
      cursorPosition = 0;
      showMenu();
    }
  }
}

void showMenu() {
 
  lcd.clear();
  for (int i = 0; i < LCD_ROWS; i++) {
    int itemIndex = indexStack[currentLevel] - cursorPosition + i;
    if (itemIndex >= 0 && itemIndex < menuSizeStack[currentLevel]) {
      lcd.setCursor(0, i);
      lcd.print((i == cursorPosition) ? "> " : "  ");
      lcd.print(menuStack[currentLevel][itemIndex].title);
    }
  }
}

void __attribute__((noinline)) sair() {   // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}

bool isButtonPressed(int pin, unsigned long &lastTime) {
  if (digitalRead(pin) == LOW) {
    unsigned long current = millis();
    if (current - lastTime > debounceDelay) {
      lastTime = current;
      return true;
    }
  }
  return false;
}