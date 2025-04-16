// Dicionário: valor a ser gravado => nome da carta (exibido apenas)
const char* coletor_dados[][2] = {
  {"A", "A"},               
  {"B", "B"},
  {"C", "C"},
  {"D", "D"},
  {"E", "E"},
  {"F", "F"},
  {"G", "G"},
  {"H", "H"},
  {"I", "I"},
  {"J", "J"},
  {"K", "K"},
  {"L", "L"},
  {"M", "M"}, 
  {"N", "N"},
  {"O", "O"},
  {"P", "P"},
  {"Q", "Q"},
  {"R", "R"},
  {"S", "S"},
  {"T", "T"},
  {"U", "U"},
  {"V", "V"},
  {"W", "W"},
  {"X", "X"},
  {"Y", "Y"},
  {"Z", "Z"},
  {"0", "0"},
  {"1", "1"},
  {"2", "2"},
  {"3", "3"},
  {"4", "4"},
  {"5", "5"},
  {"6", "6"},
  {"7", "7"},
  {"8", "8"},
  {"9", "9"},
  {"D", "Direita"},
  {"A", "Esquerda"},
  {"W", "Cima"},
  {"S", "Baixo"},
  {"DGDGDG", "Direita 3x"},
  {"AGAGAG", "Esquerda 3x"},
  {"WGWGWG", "Cima 3x"},
  {"SGSGSG", "Baixo 3x"},
  {"Q", "Do"},
  {"W", "Re"},
  {"E", "Mi"},
  {"R", "Fa"},
  {"T", "Sol"},
  {"Y", "La"},
  {"U", "Si"},
  {"1", "Piano"},
  {"2", "Harpa"},
  {"3", "Flauta"},
  {"E", "Azul"},
  {"Q", "Amarelo"},
  {"R", "Vermelho"},
  {"T", "Verde"},
  {"W", "Marrom"},
  {"Y", "Cinza"},
  {"M", "Medo"},
  {"A", "Feliz"},
  {"N", "Nojo"},
  {"R", "Raiva"},
  {"T", "Triste"},
  {"S", "Surpresa"}
};
const int totalCartoes_dados = sizeof(coletor_dados) / sizeof(coletor_dados[0]);


void gravacao_coletor() {
  int i = 0;
  unsigned long backPressStart = 0;

  while (i < totalCartoes_dados) {
    const char* valor = coletor_dados[i][0];
    const char* nome = coletor_dados[i][1];

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aproxime o cartao:");
    lcd.setCursor(0, 1);
    lcd.print(nome);

    bool sucesso = false;
    while (!sucesso) {
      // Verifica botoes
      if (digitalRead(38) == LOW) {  // Baixo - avança
        i = min(i + 1, totalCartoes_dados - 1);
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
      int len = strlen(valor);
      memcpy(buffer, valor, min(16, len));

      // Autentica
      MFRC522::MIFARE_Key key;
      for (byte j = 0; j < 6; j++) key.keyByte[j] = 0xFF;

      MFRC522::StatusCode status = rfid.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A,
        BLOCO_GRAVACAO_COLETOR,
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
      status = rfid.MIFARE_Write(BLOCO_GRAVACAO_COLETOR, buffer, 16);
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
      status = rfid.MIFARE_Read(BLOCO_GRAVACAO_COLETOR, leitura, &tamanho);
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



