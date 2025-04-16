/*
# ==============================================================================
# Nome do Desenvolvedor: João Victor Madureira de Oliveira Almeida
# Data de Criação: 04/04/2025
# Descrição: Este módulo implementa a função de gravador RFID com dicionário pré estabelecido
# Última Modificação: 05/04/2025
# Versão: 0.2
# ==============================================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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
MenuItem menuAlfabeto[] = {
  {"A", NULL, 0}, {"B", NULL, 0}, {"C", NULL, 0}, {"D", NULL, 0}, {"E", NULL, 0},
  {"F", NULL, 0}, {"G", NULL, 0}, {"H", NULL, 0}, {"I", NULL, 0}, {"J", NULL, 0},
  {"K", NULL, 0}, {"L", NULL, 0}, {"M", NULL, 0}, {"N", NULL, 0}, {"O", NULL, 0},
  {"P", NULL, 0}, {"Q", NULL, 0}, {"R", NULL, 0}, {"S", NULL, 0}, {"T", NULL, 0},
  {"U", NULL, 0}, {"V", NULL, 0}, {"W", NULL, 0}, {"X", NULL, 0}, {"Y", NULL, 0},
  {"Z", NULL, 0}
};

MenuItem menuMusicaEduca[] = {
  {"Do", NULL, 0},
  {"Re", NULL, 0},
  {"Mi", NULL, 0},
  {"Fa", NULL, 0},
  {"Sol", NULL, 0},
  {"La", NULL, 0},
  {"Si", NULL, 0},
  {"Piano", NULL, 0},
  {"Harpa", NULL, 0},
  {"Flauta", NULL, 0}
};

MenuItem menuCoresEduca[] = {
  {"Azul", NULL, 0},
  {"Amarelo", NULL, 0},
  {"Verde", NULL, 0},
  {"Vermelho", NULL, 0},
  {"Marrom", NULL, 0},
  {"Cinza", NULL, 0}
};

MenuItem menuNumerosEduca[] = {
  {"0", NULL, 0},
  {"1", NULL, 0},
  {"2", NULL, 0},
  {"3", NULL, 0},
  {"4", NULL, 0},
  {"5", NULL, 0},
  {"6", NULL, 0},
  {"7", NULL, 0},
  {"8", NULL, 0},
  {"9", NULL, 0}
};

MenuItem menuEmocoesEduca[] = {
  {"Medo", NULL, 0},
  {"Feliz", NULL, 0},
  {"Nojo", NULL, 0},
  {"Surpresa", NULL, 0},
  {"Raiva", NULL, 0},
  {"Triste", NULL, 0}
};

MenuItem menuDirecoesEduca[] = {
  {"Direita", NULL, 0},
  {"Esquerda", NULL, 0},
  {"Cima", NULL, 0},
  {"Baixo", NULL, 0},
  {"Direita 3X", NULL, 0},
  {"Esquerda 3X", NULL, 0},
  {"Cima 3X", NULL, 0},
  {"Baixo 3X", NULL, 0}
};

MenuItem menuEducaNaveSub[] = {
  {"Alfabeto", menuAlfabeto, 26},
  {"Musica", menuMusicaEduca, 10},
  {"Cores", menuCoresEduca, 6},
  {"Numeros", menuNumerosEduca, 10},
  {"Emocoes", menuEmocoesEduca, 6},
  {"Direcoes", menuDirecoesEduca, 8}
};

MenuItem menuEducaNave[] = {
  {"Baralho Completo", NULL, 0},
  {"Carta Individual", menuEducaNaveSub, 6}
};

MenuItem menuColetorIndividual[] = {
  {"Acionar", NULL, 0},
  {"Suspender", NULL, 0},
  {"Aguarde", NULL, 0},
  {"1 segundo", NULL, 0},
  {"3 segundos", NULL, 0},
  {"5 segundos", NULL, 0},
  {"Buzzer", NULL, 0},
  {"LED", NULL, 0},
  {"Maior que", NULL, 0},
  {"Menor que", NULL, 0},
  {"Igual", NULL, 0},
  {"Pressao dif.", NULL, 0},
  {"Bussola", NULL, 0},
  {"Temp/Umidade rel.", NULL, 0},
  {"Temperatura", NULL, 0},
  {"Corrente eletrica", NULL, 0},
  {"Efeito Hall", NULL, 0},
  {"Detector Sonoro", NULL, 0},
  {"Forca", NULL, 0},
  {"Ultrassonico", NULL, 0},
  {"pH", NULL, 0},
  {"Umidade do solo", NULL, 0},
  {"Freq. Cardiaca", NULL, 0},
  {"Prog 1", NULL, 0},
  {"Prog 2", NULL, 0},
  {"Prog 3", NULL, 0}
};

MenuItem menuColetorDados[] = {
  {"Baralho Completo", NULL, 0},
  {"Carta Individual", menuColetorIndividual, 26}
};

MenuItem menuRoboReiIndividual[] = {
  {"Carta inicio", NULL, 0},
  {"Mover p/ frente", NULL, 0},
  {"Mover p/ tras", NULL, 0},
  {"Girar p/ direita", NULL, 0},
  {"Girar p/ esquerda", NULL, 0},
  {"Girar p/ tras", NULL, 0},
  {"Multiplicador 2x", NULL, 0},
  {"Multiplicador 3x", NULL, 0},
  {"Multiplicador de 4x", NULL, 0},
  {"Aguardar 3s", NULL, 0},
  {"Aguardar 5s", NULL, 0},
  {"Tocar musica 1", NULL, 0},
  {"Tocar musica 2", NULL, 0},
  {"Tocar musica 3", NULL, 0},
  {"Tocar musica 4", NULL, 0},
  {"Tocar musica 5", NULL, 0},
  {"Tocar musica 6", NULL, 0},
  {"Som de acerto", NULL, 0},
  {"Som em loop", NULL, 0},
  {"Carta fim", NULL, 0}
};

MenuItem menuRoboRei[] = {
  {"Baralho Completo", NULL, 0},
  {"Carta Individual", menuRoboReiIndividual, 20}
};

MenuItem mainMenu[] = {
  {"Robo Rei", menuRoboRei, 2},
  {"Coletor de Dados", menuColetorDados, 2},
  {"Educa Nave", menuEducaNave, 2}
};


// Pilha de menus
MenuItem* menuStack[5];
int menuSizeStack[5];
int indexStack[5];

// ... resto do código permanece o mesmo ...


void setup() {
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

      if (strcmp(title, "Baralho Completo") == 0) {
        executarBaralhoCompleto();
      }
      else if (strcmp(title, "Carta Individual") == 0 && currentLevel == 1 && strcmp(menuStack[0][indexStack[0]].title, "Educa Nave") == 0) {
        executarCartaEduca();
      }
      else if (strcmp(title, "Alfabeto") == 0) {
        executarAlfabeto();
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Executando:");
      lcd.setCursor(0, 1);
      lcd.print(sanitize(title));
      delay(1000);
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
