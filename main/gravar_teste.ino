
// Dicionário: valor a ser gravado => nome da carta (exibido apenas)
const char* valores[][2] = {
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
const int totalCartoes = sizeof(valores) / sizeof(valores[0]);


void gravacao () {
  for (int i = 0; i < totalCartoes; i++) {
    const char* valor = valores[i][0];
    const char* nome = valores[i][1];

    bool sucesso = false;
    while (!sucesso) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Aproxime o cartao:");
      lcd.setCursor(0, 1);
      //lcd.print("Nome: ");
      lcd.print(nome);
      //lcd.setCursor(0, 2);
      //lcd.print("Valor: ");
      //lcd.print(valor);

      // Aguarda cartão
      if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        delay(500);
        continue;
      }

      // Prepara dados para gravar (16 bytes)
      byte buffer[16];
      memset(buffer, 0x00, 16);  // Preenche o buffer com espaços   //AQUI ESCOLHE COMO SERA PREENCHIDO OS BYTES VAZIOS
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
  delay(2000);
  return;     //while (true) delay(1000);
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
