#include <LiquidCrystal.h> //Biblioteca para fazer funções LCD

const char rs = 8; //register select
const char en = 9; //enable LCD
const char db4 = 4; //pino data numero 4 (data4)
const char db5 = 5; //pino data numero 5(data5)
const char db6 = 6; //pino data numero 6(data6)
const char db7 = 7; //pino data numero 7(data7)


//inicializa o LCD com os terminais definidos
LiquidCrystal lcd(rs, en, db4, db5, db6, db7);

#define BTN_RIGHT 0
#define BTN_UP 1
#define BTN_DOWN 2
#define BTN_LEFT 3
#define BTN_ENTER 4
#define BTN_NONE 5

#define bobina1 13 
#define bobina2 12 
#define bobina3 11 
#define bobina4 3 

//////////////////////PROTOTIPOS/////////////////////////
int lerBotao();
void mostrarNivel1();
void mostrarNivel2();
void mostrarNivel3();
void selExecutarGiro();
void girar();



enum MenuLevel {NIVEL_1, NIVEL_2, NIVEL_3};
MenuLevel menuLevel = NIVEL_1; //menu inicial um

int selecionadoNivel2 = 0; // 0 == Motor no sentido horário, 1 == Motor no sentido anti-horário.
int selecionadoNivel3 = 0; // 0 = Gira 90º, 1= 270º 2 = 810º

const unsigned int tempoLedLigado = 500;
unsigned int  tela[3][3];
int posHorizontal = 0;
int posVertical   = 0;

void setup() //executa comandos base na inicialização
{
  // Definição dos pinos para as bobinas do motor
  pinMode(bobina1, OUTPUT); 
  pinMode(bobina2, OUTPUT);
  pinMode(bobina3, OUTPUT);
  pinMode(bobina4, OUTPUT);
  
  lcd.begin(16,2);           
  lcd.setCursor(0,0);
  lcd.print("MENU MOTOR     ");
  lcd.setCursor(0,1);
  lcd.print("DE PASSO       ");
  delay(3000);
  lcd.clear();
  mostrarNivel1();
  Serial.begin(9600);
  
}

void loop() 
{
  int botao = lerBotao();

  if (botao == BTN_NONE) return;

  switch(menuLevel)
  {
    case NIVEL_1:
    if (botao == BTN_RIGHT)
    {
      menuLevel = NIVEL_2;
      mostrarNivel2();
    }
    break;
    case NIVEL_2:
    if((botao == BTN_UP) || (botao == BTN_DOWN))
    {
      selecionadoNivel2 = 1 - selecionadoNivel2;
      mostrarNivel2();
    }

    else if(botao == BTN_RIGHT)
    {
      menuLevel = NIVEL_3;
      mostrarNivel3();
    }
    else if(botao == BTN_LEFT) 
    {
      menuLevel = NIVEL_1;
      mostrarNivel1();
    }
    break;

    case NIVEL_3:
      if(botao == BTN_UP)
      {
        selecionadoNivel3 = (selecionadoNivel3+2) % 3;
        mostrarNivel3();
      }

    else if (botao == BTN_DOWN)
    {
      selecionadoNivel3 = (selecionadoNivel3+1) % 3;
      mostrarNivel3();
    }
    else if(botao == BTN_ENTER)
    {
      selExecutarGiro();
    }

    else if(botao == BTN_LEFT)
    {
      menuLevel = NIVEL_2;
      mostrarNivel2();
    }
    break;
    
  }
delay(150);
  
}

///////////////////////////////////////////////////FUNÇÕES/////////////////////////////////////

int lerBotao()
{
  int val = analogRead(A0);
  if (val<50) return BTN_RIGHT; // Pressionado botão direita
  else if ((val>=50) && (val<=150)) return BTN_UP; // Pressionado botão cima
  else if ((val>=200) && (val<=340)) return BTN_DOWN; // Pressionado botão baixo
  else if ((val>=350) && (val<=550)) return BTN_LEFT; // Pressionado botão esquerda
  else if ((val>=600) && (val<=760)) return BTN_ENTER;
  else return BTN_NONE;
}

void mostrarNivel1()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" IHM Step Motor ");
  lcd.setCursor(0,1);
  lcd.print("Pressione RIGHT ");
}

void mostrarNivel2()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(selecionadoNivel2 == 0 ? ">MotorHorario" : "MotorHorario");
  lcd.setCursor(0,1);
  lcd.print(selecionadoNivel2 == 1 ? ">MotorAntiHor" : "MotorAntiHor");
}

void mostrarNivel3()
{
  const char* opcoes[3] = {
    "Gira 90 Graus   ",
    "Gira 270 Graus  ",
    "Gira 810 Graus  "};
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sentido:");
  lcd.print(selecionadoNivel2 == 0 ? "horario" : "antiHor");
  lcd.setCursor(0,1);
  lcd.print (opcoes[selecionadoNivel3]); 
  
}

void selExecutarGiro()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Executando giro");
  lcd.setCursor(0,1);
    if(selecionadoNivel2 == 0)
    {
      switch (selecionadoNivel3)
        {
          case 0: lcd.print("H_90 graus        "); girar(0,0); break;
          case 1: lcd.print("H_270 graus       "); girar(0,1); break;
          case 2: lcd.print("H_Gira810graus    "); girar(0,2); break;
        }
    }
    else
    {
      switch(selecionadoNivel3)
        {
          case 0: lcd.print("AH_90 graus        "); girar(1,0); break;
          case 1: lcd.print("AH_270 graus       "); girar(1,1); break;
          case 2: lcd.print("AH_Gira810graus    "); girar(1,2); break;
        }
        }
  delay(2000); // Aguarda 2 segundos para a leitura da mensagem
  menuLevel = NIVEL_1; // Retorna ao nível 1 do menu
  mostrarNivel1(); // Atualiza display para a tela inicial
        
}

// Função que simula o giro do motor conforme parâmetros recebidos
void girar (char H_AH, char modo)
{
  if ((H_AH == 0) && (modo == 0))//gira no sentido horario no modo 0 - 90 graus
  {
     for (int x=1; x<13; x++) // x < 13, pois o motor dá 50 passos para girar 90 graus
     {	
        //Serial.println(x);
     	acionarBobinasHorario(true, true, true, true, 19);
     }
  }
  else if ((H_AH == 0) && (modo == 1))//gira no sentido horario no modo 0 - 270 graus
  {
     for (int x=1; x<39; x++) 
     {
     	acionarBobinasHorario(true, true, true, true, 19);
     }
  }
  else if ((H_AH == 0) && (modo == 2))//gira no sentido horario no modo 0 - 810 graus
  {
     for (int x=1; x<116; x++) 
     {
     	acionarBobinasHorario(true, true, true, true, 19);
     }
  }
  else if ((H_AH == 1) && (modo == 0))//gira no sentido anti - horario no modo 0 - 90 graus
  {
     for (int x=1; x<13; x++) // x < 13, pois o motor dá 50 passos para girar 90 graus
     {
     	acionarBobinasAntiHorario(true, true, true, true, 19);
     }
  }
  else if ((H_AH == 1) && (modo == 1))//gira no sentido anti - horario no modo 0 - 270 graus
  {
     for (int x=1; x<39; x++) 
     {
     	acionarBobinasAntiHorario(true, true, true, true, 19);
     }
  }
  else if ((H_AH == 1) && (modo == 2))//gira no sentido anti - horario no modo 0 - 810 graus
  {
     for (int x=1; x<116; x++) 
     {
     	acionarBobinasAntiHorario(true, true, true, true, 19);
     }
  }
}

void acionarBobinasAntiHorario (boolean bob1, boolean bob2, boolean bob3, boolean bob4, long delayTime) {
	
  	digitalWrite(bobina1, bob1);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina4, !bob4);
    delay(delayTime);

    digitalWrite(bobina1, !bob1);
  	digitalWrite(bobina2, bob2);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina4, !bob4);
  	delay(delayTime);

    digitalWrite(bobina1, !bob1);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina3, bob3);
  	digitalWrite(bobina4, !bob4);
    delay(delayTime);

    digitalWrite(bobina1, !bob1);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina4, bob4);
    delay(delayTime);
  
    digitalWrite(bobina1, !bob1);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina4, !bob4);
    delay(delayTime);
}

void acionarBobinasHorario (boolean bob1, boolean bob2, boolean bob3, boolean bob4, long delayTime) {
	
  	digitalWrite(bobina4, bob4);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina1, !bob1);
    delay(delayTime);

    digitalWrite(bobina4, !bob4);
  	digitalWrite(bobina3, bob3);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina1, !bob1);
  	delay(delayTime);

    digitalWrite(bobina4, !bob4);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina2, bob2);
  	digitalWrite(bobina1, !bob1);
    delay(delayTime);

    digitalWrite(bobina4, !bob4);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina1, bob1);
    delay(delayTime);
  
    digitalWrite(bobina4, !bob4);
  	digitalWrite(bobina3, !bob3);
  	digitalWrite(bobina2, !bob2);
  	digitalWrite(bobina1, !bob1);
    delay(delayTime);
}

