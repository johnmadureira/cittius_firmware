/* 
// M1
const byte M1_A = 62; // A8
const byte M1_D = 2;
// M2
const byte M2_A = 63; // A9
const byte M2_D = 3;
// M3
const byte M3_A = 64; // A10
const byte M3_D = 18;
// M4
const byte M4_A = 65; // A11
const byte M4_D = 19;


void sensor_MODELO() {
  lcd.clear();
  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado
   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
*/

void sensor_botao() {
  // Configura os pinos como entrada com pull-up
  pinMode(M1_A, INPUT_PULLUP);
  pinMode(M2_A, INPUT_PULLUP);
  pinMode(M3_A, INPUT_PULLUP);
  pinMode(M4_A, INPUT_PULLUP);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Teste de Botoes");

  while (digitalRead(btnBack) == HIGH) {
    lcd.setCursor(0, 1);
    lcd.print("S1:");
    lcd.print(digitalRead(M1_A) == LOW ? "ON " : "OFF");

    lcd.setCursor(7, 1);
    lcd.print("S2:");
    lcd.print(digitalRead(M2_A) == LOW ? "ON " : "OFF");

    lcd.setCursor(0, 2);
    lcd.print("S3:");
    lcd.print(digitalRead(M3_A) == LOW ? "ON " : "OFF");

    lcd.setCursor(7, 2);
    lcd.print("S4:");
    lcd.print(digitalRead(M4_A) == LOW ? "ON " : "OFF");

    delay(200);
  }

  lcd.clear();
  lcd.print("Retornando ao menu");
  delay(500);
}

// 4 sensores
void sensor_forca() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Forca");

  while (digitalRead(btnBack) == HIGH) {  // Enquanto não pressionar o botão Back
    int v1 = analogRead(M1_A);
    int v2 = analogRead(M2_A);
    int v3 = analogRead(M3_A);
    int v4 = analogRead(M4_A);

    lcd.setCursor(0, 2);
    lcd.print("S1:");
    lcd.print(v1);
    lcd.print("     "); // Limpa resquícios
    lcd.setCursor(10, 2);
    lcd.print("S2:");
    lcd.print(v2);
    lcd.print("     "); // Limpa resquícios
    lcd.setCursor(0, 3);
    lcd.print("S3:");
    lcd.print(v3);
    lcd.print("     "); // Limpa resquícios
    lcd.setCursor(10, 3);
    lcd.print("S4:");
    lcd.print(v4);
    lcd.print("     "); // Limpa resquícios

    delay(200);
  }

  lcd.clear();
  lcd.print("Retornando ao menu");
  delay(500);
}

/*
//void sensor_forca_apenasUm() {

  lcd.clear();

  while (digitalRead(btnBack) == HIGH) { // Enquanto o botão NÃO estiver pressionado
    int valor = analogRead(M1_A); // Leitura da força
    int barra = map(valor, 0, 700, 0, 20); // Converte para 0-20 colunas no LCD

    // Define mensagem com base na força
    lcd.setCursor(0, 0);
    //if (valor < 5) {
    lcd.print("Sensor forca FSR50N ");
    //} else if (valor < 350) {
    //  lcd.print("Senti nada aqui     ");
    //} else if (valor < 600) {
    //  lcd.print("Ja abre um pote...  ");
    //} else {
     // lcd.print("Calma, Hulk!        ");
    //}

    // Exibe valor numérico
    lcd.setCursor(0, 2);
    lcd.print("Forca: ");
    lcd.print(valor);
    lcd.print("     ");  // Apaga excesso

    // Exibe barra gráfica
    lcd.setCursor(0, 3);
    for (int i = 0; i < 20; i++) {
      if (i < barra) lcd.print("|");
      else lcd.print(" ");
    }

    delay(20);
  }

  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}*/
/*
void sensor_botao() {
  pinMode(M1_A, INPUT_PULLUP); 
  lcd.clear();

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado
      
    int estadoBotao = digitalRead(M1_A);  // Lê o estado do botão
  

    if (estadoBotao == LOW) {  // Quando o botão for pressionado (LOW devido ao INPUT_PULLUP)
      lcd.setCursor(0, 1);
      lcd.print("PRESSIONADO         ");  // Exibe mensagem no LCD
      tone(BUZZER_PIN, 1000, 200);  // Beep de 200ms a 1000Hz
    } else {
      lcd.setCursor(4, 0);
      lcd.print("Teste Botao");
      lcd.setCursor(0, 1);
      lcd.print("Aperte o Botao");
      }

  delay(200);  // Adiciona um pequeno atraso para evitar leituras rápidas demais
    }
     // Tela de saída
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}*/

void sensor_gas() {

  lcd.clear();

  while (digitalRead(btnBack) == HIGH) { 

    int leitura = analogRead(M1_A);  // Lê valor entre 0-1023
    int barra = map(leitura, 200, 800, 0, 20);  // Mapeia para 0-20 blocos

  // Limpa e atualiza a linha de leitura
    lcd.setCursor(0, 1);
    lcd.print("                    ");  // Limpa a linha inteira
    lcd.setCursor(0, 1);
    lcd.print("Leitura: ");
    lcd.print(leitura);

  // Limpa e atualiza a linha da barra
    lcd.setCursor(0, 2);
    lcd.print("Nivel: ");
    for (int i = 0; i < 20 - 7; i++) {  // 7 caracteres já ocupados por "Nivel: "
      if (i < barra)
        lcd.write(255); // Bloco cheio
      else
        lcd.print(" ");
    }

    delay(500);
  }

   // Tela de saída
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);

   

}
void sensor_ultrassonico() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Ultrassonico");

  pinMode(M1_D, OUTPUT);  // Trig
  pinMode(M1_A, INPUT);   // Echo

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    // Envia pulso para o Trig
    digitalWrite(M1_D, LOW);
    delayMicroseconds(2);
    digitalWrite(M1_D, HIGH);
    delayMicroseconds(10);
    digitalWrite(M1_D, LOW);

    // Mede o tempo do pulso de retorno no Echo
    long duration = pulseIn(M1_A, HIGH);

    // Calcula a distância em cm
    float distance = duration * 0.034 / 2;

    // Exibe no Serial
    //Serial.print("Distancia: ");
    //Serial.print(distance);
    //Serial.println(" cm");

    // Exibe no LCD
    lcd.setCursor(0, 1);
    lcd.print("Distancia: ");
    lcd.print(distance, 1);
    lcd.print(" cm     ");

    delay(500);
  }

  // Tela de saída
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_ph() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Teste Sensor pH");

    #define VREF 5.0    // Tensão de referência
    #define RESOLUCAO 1023.0

    while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

   


    int leituraAnalogica = analogRead(M1_A);
    float tensao = (leituraAnalogica * VREF) / RESOLUCAO;

    // Fórmula baseada na faixa típica de 0-14 pH ≈ 0-3V
    float ph = 7 + ((2.65 - tensao) / 0.18);  // 0.18 ≈ 59.16mV por unidade de pH

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tensao: ");
    lcd.print(tensao, 2);
    lcd.print(" V");

    lcd.setCursor(0, 1);
    lcd.print("pH: ");
    lcd.print(ph, 2);

    delay(1000);
    }
   
   
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_IR() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Infravermelho");
  pinMode(M1_A, INPUT);

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    int leitura = digitalRead(M1_A);

    lcd.setCursor(0, 2);
    if (leitura == LOW) {
      // Obstáculo detectado (pode variar com o modelo)
      lcd.print("Obstaculo detectado ");
    } else {
      lcd.print("Nenhum obstaculo    ");
    }

    delay(200);  // Pequeno atraso para estabilidade

   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_umidadeSolo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Umidade");


  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    int leitura = analogRead(M1_A);
    
    // Convertendo leitura para percentual (ajustar se necessário)
    int umidadePercentual = map(leitura, 1023, 0, 0, 100); // Solo seco: valor alto, solo molhado: valor baixo

    lcd.setCursor(0, 0);
    lcd.print("Umidade Solo:");

    lcd.setCursor(0, 1);
    lcd.print("Nivel: ");
    lcd.print(umidadePercentual);
    lcd.print(" %     "); // Espaços extras para evitar "ghosting"

    //.print("Valor analogico: ");
    //Serial.print(leitura);
    //Serial.print(" -> Umidade: ");
    //Serial.print(umidadePercentual);
    //Serial.println(" %");

    delay(500);

    }
    // Limpa linhas quando sai da função
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Retornando ao menu");
    delay(500);
    
}
void sensor_laser() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Teste Laser 650nm ");

  pinMode(M1_A, OUTPUT);  // Configura o pino do laser como saída

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    lcd.setCursor(0, 2);
    lcd.print("Status: Desligado ");

    // Liga o diodo a laser
    digitalWrite(M1_A, HIGH);
    lcd.setCursor(0, 2);
    lcd.print("Status: Ligado   ");
    delay(3000);  // Laser ligado por 2 segundos
    
    // Desliga o diodo a laser
    digitalWrite(M1_A, LOW);
    lcd.setCursor(0, 2);
    lcd.print("Status: Desligado ");
    delay(1000);  // Laser desligado por 2 segundos
   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_temperatura() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado


    OneWire oneWire(M1_A);
    DallasTemperature sensors(&oneWire);
    sensors.begin();
    sensors.requestTemperatures();

    float tempC = sensors.getTempCByIndex(0);

    lcd.setCursor(0, 1);
    if (tempC == DEVICE_DISCONNECTED_C) {
      lcd.print("Erro no sensor   ");
    } else {
      lcd.print(tempC);
      lcd.print((char)223); // Símbolo de grau
      lcd.print("C");
    }

    delay(1000);
   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_hall() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Hall:");

  pinMode(M1_A, INPUT);

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado
    int estado = digitalRead(M1_A);

    lcd.setCursor(0, 2);  // Linha 3 (índice começa do zero)
    lcd.print("Status:                ");  // Limpa linha

    lcd.setCursor(8, 2);
    if (estado == LOW) {
      lcd.print("MAGNETICO");
    } else {
      lcd.print("SEM CAMPO");
    }

    delay(200);
   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_chuva() {
  lcd.clear();
  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    pinMode(M1_D, INPUT);

    int valorAnalogico = analogRead(M1_A);
    int estadoChuva = digitalRead(M1_D);
    int umidade = map(valorAnalogico, 1023, 0, 0, 100); // Converte para umidade percentual aproximada

    // Exibe os dados no LCD
    lcd.setCursor(0, 0);
    lcd.print("Umidade: ");
    lcd.print(umidade);
    lcd.print(" %     ");
    lcd.setCursor(0, 1);
    lcd.print("Estado: ");
    if (estadoChuva == LOW) lcd.print("Chovendo     ");
    else lcd.print("Seco              ");
    
    // Exibe no Serial Monitor
    //Serial.print("Valor analogico: ");
    //Serial.print(valorAnalogico);
    //Serial.print(" | Umidade: ");
    //Serial.print(umidade);
    //Serial.print("% | Estado Digital: ");
    //Serial.println(estadoChuva == LOW ? "CHUVA" : "SECO");

    delay(500);
   }
    // Limpa linhas quando sai da função
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Retornando ao menu");
    delay(500);
}
void sensor_vibracao() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Vibracao: ");

  unsigned long ultimaVibracao = 0;
  const unsigned long intervalo = 300; // tempo mínimo entre leituras
  bool estadoAnterior = false;
  pinMode(M1_D, INPUT);

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    lcd.setCursor(0, 2);
    lcd.print("Sem vibracao       ");
    delay(20);



    int leitura = digitalRead(M1_D);
    unsigned long agora = millis();

    if (leitura == HIGH && !estadoAnterior && (agora - ultimaVibracao > intervalo)) {
      ultimaVibracao = agora;
      estadoAnterior = true;

      lcd.setCursor(0, 2);
      lcd.print("Vibracao detectada ");
      delay(1000);
    }

    // Quando não há vibração e tempo de silêncio já passou
    if (leitura == LOW && estadoAnterior && (agora - ultimaVibracao > intervalo)) {
      estadoAnterior = false;

      lcd.setCursor(0, 2);
      lcd.print("Sem vibracao       ");
    }

   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_peltier() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Teste Peltier");

  pinMode(M1_D, OUTPUT);

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

      // Liga a pastilha
      digitalWrite(M1_D, HIGH);

      // Contagem regressiva de 10s ligada
      for (int i = 30; i > 0; i--) {
        lcd.setCursor(0, 2);
        lcd.print("Status: ON    ");
        lcd.setCursor(17, 2);
        lcd.print("   ");  // Limpa os caracteres antigos
        lcd.setCursor(17, 2);
        lcd.print(String(i) + "s");
        delay(1000);
      }

      // Desliga a pastilha
      digitalWrite(M1_D, LOW);

      // Contagem regressiva de 2s desligada
      for (int i = 2; i > 0; i--) {
        lcd.setCursor(0, 2);
        lcd.print("Status: OFF ");
        lcd.setCursor(17, 2);
        lcd.print("   ");  // Limpa os caracteres antigos
        lcd.setCursor(17, 2);
        lcd.print(String(i) + "s");
        delay(1000);
      }
    
   }// Limpa linhas quando sai da função
   
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_pir() {

  lcd.clear();
  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    lcd.setCursor(0, 0);
    lcd.print("Sensor movimento:");

    pinMode(M1_A, INPUT);  // A8 usado como entrada digital
    int estado = digitalRead(M1_A);

    if (estado == HIGH) {
      lcd.setCursor(0, 1);
      lcd.print("Movimento detectado ");
      delay(500);

    } else {
      lcd.setCursor(0, 1);
      lcd.print("Sem movimento       ");

    }

    delay(20);

   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_decibel() {

  lcd.clear();
  pinMode(M1_A, INPUT);
  // Caracteres personalizados para barra
  byte barra0[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
  byte barra1[8] = {B10000,B10000,B10000,B10000,B10000,B10000,B10000,B10000};
  byte barra2[8] = {B11000,B11000,B11000,B11000,B11000,B11000,B11000,B11000};
  byte barra3[8] = {B11100,B11100,B11100,B11100,B11100,B11100,B11100,B11100};
  byte barra4[8] = {B11110,B11110,B11110,B11110,B11110,B11110,B11110,B11110};
  byte barra5[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};

  // Criação dos blocos personalizados
  lcd.createChar(0, barra0);
  lcd.createChar(1, barra1);
  lcd.createChar(2, barra2);
  lcd.createChar(3, barra3);
  lcd.createChar(4, barra4);
  lcd.createChar(5, barra5);

  lcd.setCursor(0, 0);
  lcd.print("Som - Intensidade");

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado



      int valorSom = analogRead(M1_A);

      int dbSimulado = map(valorSom, 500, 750, 30, 100);
      dbSimulado = constrain(dbSimulado, 0, 100);

      // Total de blocos: 20 colunas, cada uma com 5 níveis
      int totalBlocos = map(dbSimulado, 30, 100, 0, 100); // escala 0 a 100
      int colunas = totalBlocos / 5;
      int resto = totalBlocos % 5;

      lcd.setCursor(0, 2);
      for (int i = 0; i < 20; i++) {
        if (i < colunas) {
          lcd.write(byte(5));  // cheio
        } else if (i == colunas && resto > 0) {
          lcd.write(byte(resto));  // parcial
        } else {
          lcd.write(byte(0));  // vazio
        }
      }

      lcd.setCursor(0, 1);
      lcd.print("Nivel: ");
      lcd.print(dbSimulado);
      lcd.print(" dB   ");

      //Serial.print("Valor A0: ");
      //Serial.print(valorSom);
      //Serial.print(" => ");
      //Serial.print(dbSimulado);
      //Serial.println(" dB");

      delay(150);

   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_correnteSCT() {
  lcd.clear();
  pinMode(M1_A, INPUT);

  const float referenciaVoltagem = 5.0;
  const int numAmostras = 1000;
  const float tensaoOffset = 2.5;
  const float fatorConversao = 0.185; // A/V

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

      float somaQuadrados = 0.0;

      for (int i = 0; i < numAmostras; i++) {
        int leituraADC = analogRead(M1_A);
        float tensao = (leituraADC / 1023.0) * referenciaVoltagem;
        float diferenca = tensao - tensaoOffset;
        somaQuadrados += diferenca * diferenca;
      }

      float mediaQuadrados = somaQuadrados / numAmostras;
      float tensaoRMS = sqrt(mediaQuadrados);

      float corrente = tensaoRMS / fatorConversao;

      // Exibe no LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tensao RMS:");
      lcd.setCursor(0, 1);
      lcd.print(tensaoRMS, 3);
      lcd.print(" V");

      lcd.setCursor(0, 2);
      lcd.print("Corrente:");
      lcd.setCursor(0, 3);
      lcd.print(corrente, 3);
      lcd.print(" A");

      delay(1000);
    
   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_umidaderel() {

    // Definição do caractere personalizado para "º"
    byte grau[8] = {
      0b00110,
      0b01001,
      0b01001,
      0b00110,
      0b00000,
      0b00000,
      0b00000,
      0b00000
    };

    lcd.createChar(0, grau); // Armazena o caractere na posição 0

    // Instância do sensor
    dht DHT;

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressiona

      DHT.read11(M1_A); // Lê os dados do sensor

      float umidade = DHT.humidity;
      float temperatura = DHT.temperature;

      // Exibe no LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor DHT11");

      lcd.setCursor(0, 1);
      lcd.print("Umidade: ");
      lcd.print(umidade, 0);
      lcd.print(" %");

      lcd.setCursor(0, 2);
      lcd.print("Temp: ");
      lcd.print(temperatura, 0);
      lcd.write(byte(0));  // Escreve o caractere º
      lcd.print("C");

      delay(2000); // Aguarda 2 segundos antes da próxima leitura
   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}
void sensor_RFID() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aproxime o cartao");

  while (digitalRead(btnBack) == HIGH) {  // Loop até o botão Back ser pressionado

    // Espera por novo cartão
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Cartao Detectado ");

      lcd.setCursor(0, 2);
      lcd.print("UID: ");
      for (byte i = 0; i < rfid.uid.size; i++) {
        lcd.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
        lcd.print(rfid.uid.uidByte[i], HEX);
      }

      tone(BUZZER_PIN, 1000, 200);
      delay(300);  // Tempo para o usuário visualizar

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Aproxime o cartao");

      // Aguarda o cartão ser removido antes de continuar lendo
      while (rfid.PICC_IsNewCardPresent() || rfid.PICC_ReadCardSerial()) {
        delay(100);
      }
    }

    delay(20); // Pequeno delay para evitar loop muito rápido
  }

  // Mensagem de saída
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(1000);
}