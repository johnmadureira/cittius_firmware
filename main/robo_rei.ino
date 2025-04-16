// Estrutura da carta com valor e nome
struct Carta {
  uint8_t valor;
  const char* nome;
};

// Dicionário: valor a ser gravado => nome da carta (exibido apenas)
const Carta roboRei[] = {
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x02, "Mover frente"},
  {0x06, "Girar tras"},
  {0x06, "Girar tras"},
  {0x06, "Girar tras"},
  {0x03, "Mover tras"},
  {0x03, "Mover tras"},
  {0x03, "Mover tras"},
  {0x03, "Mover tras"},
  {0x03, "Mover tras"},
  {0x04, "Girar direita"},
  {0x04, "Girar direita"},
  {0x04, "Girar direita"},
  {0x04, "Girar direita"},
  {0x04, "Girar direita"},
  {0x05, "Esquerda esquerda"},
  {0x05, "Esquerda esquerda"},
  {0x05, "Esquerda esquerda"},
  {0x05, "Esquerda esquerda"},
  {0x05, "Esquerda esquerda"},
  {0x07, "Acao 2x"},
  {0x07, "Acao 2x"},
  {0x07, "Acao 2x"},
  {0x07, "Acao 2x"},
  {0x07, "Acao 2x"},
  {0x08, "Acao 3x"},
  {0x08, "Acao 3x"},
  {0x08, "Acao 3x"},
  {0x08, "Acao 3x"},
  {0x08, "Acao 3x"},
  {0x09, "Acao 4x"},
  {0x09, "Acao 4x"},
  {0x09, "Acao 4x"},
  {0x09, "Acao 4x"},
  {0x09, "Acao 4x"},
  {0x0A, "3 Segundos"},
  {0x0A, "3 Segundos"},
  {0x0A, "3 Segundos"},
  {0x0B, "5 Segundos"},
  {0x0B, "5 Segundos"},
  {0x0B, "5 Segundos"},
  {0x0B, "5 Segundos"},
  {0x13, "Som loop"},
  {0x13, "Som loop"},
  {0x0C, "Musica 1"},
  {0x0D, "Musica 2"},
  {0x0E, "Musica 3"},
  {0x0F, "Musica 4"},
  {0x10, "Musica 5"},
  {0x11, "Musica 6"},
  {0x12, "Som acerto"},
  {0x12, "Som acerto"},
  {0x12, "Som acerto"},
  {0x12, "Som acerto"},
  {0x12, "Som acerto"},
  {0x12, "Som acerto"},
  {0x01, "Carta Inicio"},
  {0xFE, "Carta fim"}
};

const int totalCartoes = sizeof(roboRei) / sizeof(roboRei[0]);

void gravacao_rei() {
  int i = 0;
  unsigned long backPressStart = 0;

  while (i < totalCartoes) {
    uint8_t valor = roboRei[i].valor;
    const char* nome = roboRei[i].nome;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aproxime o cartao:");
    lcd.setCursor(0, 1);
    lcd.print(nome);

    bool sucesso = false;
    while (!sucesso) {
      // Verifica botoes
      if (digitalRead(38) == LOW) {  // Baixo - avança
        i = min(i + 1, totalCartoes - 1);
        delay(300);
        break;
      }
      if (digitalRead(47) == LOW) {  // Cima - volta
        i = max(i - 1, 0);
        delay(300);
        break;
      }

      // Botão Back pressionado
      if (digitalRead(48) == LOW) {
        if (backPressStart == 0) backPressStart = millis();
        else if (millis() - backPressStart > 2000) {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("Menu principal...");
          delay(1500);
          return;  // Volta ao menu principal
        }
      } else if (backPressStart > 0) {
        // Pressão curta: cancelar gravação
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Gravacao cancelada");
        delay(1500);
        return;
      }

      // Aguarda cartão
      if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        delay(100);
        continue;
      }

      // Prepara dados
      byte buffer[16];
      memset(buffer, 0x00, 16);
      buffer[0] = valor;  // grava apenas 1 byte representando a carta

      // Autentica
      MFRC522::MIFARE_Key key;
      for (byte j = 0; j < 6; j++) key.keyByte[j] = 0xFF;

      MFRC522::StatusCode status = rfid.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A,
        BLOCO_GRAVACAO_REI,
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

      // Grava
      status = rfid.MIFARE_Write(BLOCO_GRAVACAO_REI, buffer, 16);
      if (status != MFRC522::STATUS_OK) {
        lcd.setCursor(0, 3);
        lcd.print("Falha na gravacao ");
        beepErro();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        delay(1500);
        continue;
      }

      // Verifica gravação
      byte leitura[18];
      byte tamanho = 18;
      status = rfid.MIFARE_Read(BLOCO_GRAVACAO_REI, leitura, &tamanho);
      if (status != MFRC522::STATUS_OK) {
        lcd.setCursor(0, 3);
        lcd.print("Erro leitura check ");
        beepErro();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        delay(1500);
        continue;
      }

      if (memcmp(buffer, leitura, 16) == 0) {
        lcd.setCursor(0, 3);
        lcd.print("Gravado com sucesso");
        beepSucesso();
        sucesso = true;
        i++;  // Avança para o próximo valor
      } else {
        lcd.setCursor(0, 3);
        lcd.print("Verificacao falhou ");
        beepErro();
      }

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      delay(2000);
    }
    backPressStart = 0;  // reseta controle do botão back
  }

  // Final
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" Todos gravados! ");
  delay(2000);
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
