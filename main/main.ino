/*
# ==============================================================================
# Nome do Desenvolvedor: João Victor Madureira de Oliveira Almeida
# Data de Criação: 04/04/2025
# Descrição: Este módulo implementa a função de gravador RFID com dicionário pré estabelecido
# Última Modificação: 05/04/2025
# Versão: 0.3
# ==============================================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <NewTone.h>


// Pinos e definições
#define SS_PIN  42
#define RST_PIN 255 // não utilizado
#define BUZZER_PIN 26


#define BLOCO_GRAVACAO_EDUCA 1  // bloco 4 do setor 1 (correto)
#define BLOCO_GRAVACAO_COLETOR 1  // bloco 4 do setor 1 (correto)
#define BLOCO_GRAVACAO_REI 1  // bloco 4 do setor 1 (correto)

// Instanciação dos objetos
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);


// Pinos dos botões
const int btnUp = 47;
const int btnDown = 38;
const int btnEnter = 49;
const int btnBack = 48;

// Controle de menu
int currentMenuIndex = 0;
int currentLevel = 0;
int cursorPosition = 0;

const int MAX_ITEMS = 10;
const int LCD_ROWS = 4;
bool showingWelcome = true;

// Debounce
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;
unsigned long lastDebounceTimeEnter = 0;
unsigned long lastDebounceTimeBack = 0;
const unsigned long debounceDelay = 300;

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
  {"Robo Rei", NULL, 0},
  {"Coletor de Dados", menuColetorDados, 4},
  {"Educa Nave", NULL, 0}
};

MenuItem mainMenu[] = {
  {"Gravador RFID", gravacaoCartas, 3},
  {"Teste Sensores", NULL, 0}
};

// Pilha de menus
MenuItem* menuStack[5];
int menuSizeStack[5];
int indexStack[5];


void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnEnter, INPUT_PULLUP);
  pinMode(btnBack, INPUT_PULLUP);

  // Mostra tela de boas-vindas
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("CITTIUS");
  lcd.setCursor(3, 1);
  lcd.print("Gravador RFID");
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
      menuSizeStack[0] = 3;
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

      if (strcmp(title, "Robo Rei") == 0) {
        gravacao_rei();
      }
      else if (strcmp(title, "Educa Nave") == 0) {
        gravacao_coletor();
      }
      else if (strcmp(title, "Educa Nave") == 0) {
        gravacao_educanave();
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      //lcd.print("Executando:");
      //lcd.setCursor(0, 1);
      //lcd.print(sanitize(title));
      //delay(1000);
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
      lcd.print(sanitize(menuStack[currentLevel][itemIndex].title));
    }
  }
}

String sanitize(const char* input) {
  String s = input;
  s.replace("á", "a"); s.replace("à", "a"); s.replace("ã", "a"); s.replace("â", "a");
  s.replace("é", "e"); s.replace("ê", "e");
  s.replace("í", "i");
  s.replace("ó", "o"); s.replace("õ", "o"); s.replace("ô", "o");
  s.replace("ú", "u");
  s.replace("ç", "c");
  return s;
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

void executarBaralhoCompleto() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gravando baralho");
  lcd.setCursor(0, 1);
  lcd.print("completo...");
  delay(2000);
}

void executarCartaEduca() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gravando carta");
  lcd.setCursor(0, 1);
  lcd.print("Educa Nave...");
  delay(2000);
}

void executarAlfabeto() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gravando letra:");
  lcd.setCursor(0, 1);
  lcd.print("A");
  delay(2000);
}
