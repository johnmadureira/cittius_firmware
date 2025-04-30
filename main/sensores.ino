// sensorpin_1 = 62 ou A8
// sensorpin_2 = 2
/*
void sensor_MODELO() {
  lcd.clear();
  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado
   }
  // Limpa linhas quando sai da função
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retornando ao menu");
  delay(500);
}*/
void sensor_forca() {

  lcd.clear();

  while (digitalRead(btnBack) == HIGH) { // Enquanto o botão NÃO estiver pressionado
    int valor = analogRead(sensorpin_1); // Leitura da força
    int barra = map(valor, 0, 700, 0, 20); // Converte para 0-20 colunas no LCD

    // Define mensagem com base na força
    lcd.setCursor(0, 0);
    if (valor < 5) {
      lcd.print("Sensor forca FSR50N ");
    } else if (valor < 350) {
      lcd.print("Senti nada aqui     ");
    } else if (valor < 600) {
      lcd.print("Ja abre um pote...  ");
    } else {
      lcd.print("Calma, Hulk!        ");
    }

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
}
void sensor_botao() {
  pinMode(sensorpin_1, INPUT_PULLUP); 
  lcd.clear();

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado
      
    int estadoBotao = digitalRead(sensorpin_1);  // Lê o estado do botão
  

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
}
void sensor_gas() {

  lcd.clear();

  while (digitalRead(btnBack) == HIGH) { 

    int leitura = analogRead(sensorpin_1);  // Lê valor entre 0-1023
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

  pinMode(sensorpin_2, OUTPUT);  // Trig
  pinMode(sensorpin_1, INPUT);   // Echo

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    // Envia pulso para o Trig
    digitalWrite(sensorpin_2, LOW);
    delayMicroseconds(2);
    digitalWrite(sensorpin_2, HIGH);
    delayMicroseconds(10);
    digitalWrite(sensorpin_2, LOW);

    // Mede o tempo do pulso de retorno no Echo
    long duration = pulseIn(sensorpin_1, HIGH);

    // Calcula a distância em cm
    float distance = duration * 0.034 / 2;

    // Exibe no Serial
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");

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

   


    int leituraAnalogica = analogRead(sensorpin_1);
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
  pinMode(sensorpin_1, INPUT);

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    int leitura = digitalRead(sensorpin_1);

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
void sensor_umidade() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Umidade");


  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    int leitura = analogRead(sensorpin_1);
    
    // Convertendo leitura para percentual (ajustar se necessário)
    int umidadePercentual = map(leitura, 1023, 0, 0, 100); // Solo seco: valor alto, solo molhado: valor baixo

    lcd.setCursor(0, 0);
    lcd.print("Umidade Solo:");

    lcd.setCursor(0, 1);
    lcd.print("Nivel: ");
    lcd.print(umidadePercentual);
    lcd.print(" %     "); // Espaços extras para evitar "ghosting"

    Serial.print("Valor analogico: ");
    Serial.print(leitura);
    Serial.print(" -> Umidade: ");
    Serial.print(umidadePercentual);
    Serial.println(" %");

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

  pinMode(sensorpin_1, OUTPUT);  // Configura o pino do laser como saída

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado

    lcd.setCursor(0, 2);
    lcd.print("Status: Desligado ");

    // Liga o diodo a laser
    digitalWrite(sensorpin_1, HIGH);
    lcd.setCursor(0, 2);
    lcd.print("Status: Ligado   ");
    delay(3000);  // Laser ligado por 2 segundos
    
    // Desliga o diodo a laser
    digitalWrite(sensorpin_1, LOW);
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


    OneWire oneWire(sensorpin_1);
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

  pinMode(sensorpin_1, INPUT);

  while (digitalRead(btnBack) == HIGH) {  // Enquanto o botão NÃO estiver pressionado
    int estado = digitalRead(sensorpin_1);

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



