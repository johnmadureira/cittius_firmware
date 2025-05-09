# Gravador RFID e Teste de Sensores

**Desenvolvedor:** João Victor Madureira de Oliveira Almeida

**Versão:** 1.4

**Última modificação:** 09/05/2025

**Criação:** 04/04/2025

## Descrição

Este projeto implementa um sistema interativo para placa customizada com microcontrolador ATmega2560 com interface LCD e botões físicos, que permite:

* **Gravação de dados RFID** com dicionário pré-definido para diferentes categorias como "Educa Nave", "Robo Rei" e "Coletor de Dados".
* **Teste de sensores analógicos e digitais** (exceto sensores I²C), organizados em um menu hierárquico acessado via botões.
* Interface gráfica com **display LCD 20x4** e navegação por menu com **quatro botões** (Up, Down, Enter, Back).
* Chamadas de funções escritas em **Assembly externo** para inicialização dos botões.

## Funcionalidades

### Módulos disponíveis no menu:

* **Gravador RFID**

  * Coletor de Dados (Física, Matemática, Química, Biologia)
  * Educa Nave
  * Robo Rei
* **Teste de Sensores**

  * Sensores como: Botão, Gás, Ultrassônico, Força, pH, Hall, PIR, Decibelímetro, etc.
  * Cada sensor possui uma função dedicada de teste.



## Requisitos de Hardware

* Placa Controladora customizada
* Leitor RFID RC522
* Display LCD 20x4 com módulo I2C (endereço 0x27)
* Buzzer (pino 26)
* Botões (Up: 47, Down: 38, Enter: 49, Back: 48)
* Diversos sensores (exceto I²C até a ultima versão 1.4)

## Organização do Código

* `setup()`: inicializa os periféricos, mostra tela de boas-vindas.
* `loop()`: gerencia a navegação no menu e chama as funções específicas para gravação RFID ou teste de sensores.
* `showMenu()`: renderiza o menu atual no LCD.
* `isButtonPressed()`: verifica se um botão foi pressionado, com debounce.
* Funções como `sensor_*()` e `gravacao_*()` são chamadas conforme a seleção do menu.
* `init_buttons()`: função externa escrita em **Assembly**, chamada no `setup()`.

### Bibliotecas Necessárias

* `Wire.h`
* `SPI.h`
* `MFRC522.h`
* `LiquidCrystal_I2C.h`
* `NewTone.h`
* `DallasTemperature.h`
* `OneWire.h`
* `dht.h`

## Estrutura Modular

O código é organizado de forma modular para facilitar manutenção e expansão, com separação clara entre:

* Interface (LCD e botões)
* Lógica de navegação
* Funções de sensores
* Funções de gravação RFID

---
