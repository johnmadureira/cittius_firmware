/*
# ==============================================================================
# Nome do Desenvolvedor: João Victor Madureira de Oliveira Almeida
# Data de Criação: 04/04/2025
# Descrição: Este módulo implementa a função de gravador RFID com dicionário pré estabelecido
# Última Modificação: 04/04/2025
# Versão: 0.1
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
#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

#define BLOCO_GRAVACAO 4  // bloco 4 do setor 1 (correto)

// Instanciação dos objetos
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// Dicionário: valor a ser gravado => nome da carta (exibido apenas)
const char* valores[][2] = {
  {"Q", "Do"},
  {"W", "Re"},
  {"E", "Mi"},
  {"R", "Fa"},
  {"T", "Sol"},
  {"Y", "La"},
  {"U", "Si"},
  {"1", "Harpa"},
  {"2", "Piano"},
  {"3", "Flauta"}
};
const int totalCartoes = sizeof(valores) / sizeof(valores[0]);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print(" Gravador de Cartoes ");
  delay(2000);
  lcd.clear();
}

void loop() {
  for (int i = 0; i < totalCartoes; i++) {
    const char* valor = valores[i][0];
    const char* nome = valores[i][1];

    bool sucesso = false;
    while (!sucesso) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Aproxime o cartao:");
      lcd.setCursor(0, 1);
      lcd.print("Nome: ");
      lcd.print(nome);
      lcd.setCursor(0, 2);
      lcd.print("Valor: ");
      lcd.print(valor);

      // Aguarda cartão
      if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        delay(500);
        continue;
      }

      // Prepara dados para gravar (16 bytes)
      byte buffer[16];
      memset(buffer, 0x00, 16);  // Preenche o buffer com espaços   //AQUI ESCOLHE COMO SERA PREENCHIDO OS VALORES VAZIOS
      int len = strlen(valor);
      memcpy(buffer, valor, min(16, len));  // Copia o valor no buffer

      // Autentica setor 1 (bloco 4)
      MFRC522::MIFARE_Key key;
      for (byte j = 0; j < 6; j++) key.keyByte[j] = 0xFF;

      MFRC522::StatusCode status = rfid.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A,
        BLOCO_GRAVACAO,  // bloco 4 do setor 1
        &key,
        &(rfid.uid)
      );

      if (status != MFRC522::STATUS_OK) {
        lcd.setCursor(0, 3);
        lcd.print("Autenticacao falhou");
        beepErro();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        delay(1500);
        continue;
      }

      // Grava os dados
      status = rfid.MIFARE_Write(BLOCO_GRAVACAO, buffer, 16);
      if (status != MFRC522::STATUS_OK) {
        lcd.setCursor(0, 3);
        lcd.print("Falha na gravacao ");
        beepErro();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        delay(1500);
        continue;
      }

      // Verifica a gravação
      byte leitura[18];
      byte tamanho = 18;
      status = rfid.MIFARE_Read(BLOCO_GRAVACAO, leitura, &tamanho);
      if (status != MFRC522::STATUS_OK) {
        lcd.setCursor(0, 3);
        lcd.print("Erro leitura check ");
        beepErro();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        delay(1500);
        continue;
      }

      // Verifica se o valor gravado confere
      if (memcmp(buffer, leitura, 16) == 0) {
        lcd.setCursor(0, 3);
        lcd.print("Gravado com sucesso");
        beepSucesso();
        sucesso = true;
      } else {
        lcd.setCursor(0, 3);
        lcd.print("Verificacao falhou ");
        beepErro();
      }

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      delay(2000);
    }
  }

  // Finalizou todos os cartões
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" Todos gravados! ");
  while (true) delay(1000);
}

void beepSucesso() {
  NewTone(BUZZER_PIN, 1000, 200);  // Som de sucesso
  delay(200);
}

void beepErro() {
  for (int i = 0; i < 2; i++) {
    NewTone(BUZZER_PIN, 400, 150);  // Som de erro
    delay(200);
  }
}
