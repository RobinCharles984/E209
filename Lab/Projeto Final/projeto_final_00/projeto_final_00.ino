#define LED_VERDE (1 << PB1) //OC1A
#define LED_AMARELO (1 << PB2) //OC1B
#define LED_VERMELHO (1 << PB3) //OC2A
#define POT (1 << PC2) //Potenciometro

//Variáveis globais
unsigned int countInt = 0;
unsigned int countCtc = 0;
bool ligado = true;

int main(){
  //Configurando GPIO
  DDRB = 0;
  DDRB |= LED_VERDE | LED_AMARELO | LED_VERMELHO;
  DDRD = 0;
  PORTD = 255; //Ativando resistor

  //Configurando Interrupção
  EICRA = (1 << ISC01);
  EIMSK = (1 << INT0);

  //Configurando Timer CTC
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS02);
  OCR0A = 193;
  TIMSK0 |= (1 << OCIE0A);

  //Configurando PWM
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);
  TCCR1B |= (1 << CS10);
  TCCR2A |= (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B |= (1 << CS20);

  //Configurando Conversor AD
  DDRC=0; //TODOS OS PINOS DO PORTC COMO ENTRADAS
  ADMUX = 0;
  ADMUX |= (1<<MUX1); //SELECIONA A ENTRADA PC2
  ADMUX |= (1<<REFS0); //TENSÃO DE REFERÊNCIA
  ADCSRA |= (1<<ADEN); //LIGA O CONVERSOR AD
  ADCSRA |= (1<<ADPS0) | (1<<ADPS1); //PRESCALER = 8
  ADCSRA |= (1<<ADSC); //INICIA A CONVERSÃO

  sei();//Ligando interrupções e timers
  while(true){
    //while (ADCSRA&(1<<ADSC));
    //Serial.println (ADC);
    //OCR2A = map (ADC,0,1023,0,255);
    //ADCSRA |= (1<<ADSC);
  }
}

//Trocando o faról
ISR(TIMER0_COMPA_vect){
    countCtc++;

    if(ligado){
      if(countCtc==1){
        OCR2A = 255;  //Vermelho
        OCR1A = 0; //Verde
        OCR1B = 0; //Amarelo
      }

      //Troca pro Verde ligado
      if(countCtc==2000){
        OCR2A = 0;  //Vermelho
        OCR1A = 255; //Verde
        OCR1B = 0; //Amarelo
      }

      //Troca pro Amarelo ligado
      if(countCtc==3500){
        OCR2A = 0;  //Vermelho
        OCR1A = 0; //Verde
        OCR1B = 255; //Amarelo
      }

      //Troca pro Vermelho ligado e zera o contador
      if(countCtc==4000){
        OCR2A = 255;  //Vermelho
        OCR1A = 0; //Verde
        OCR1B = 0; //Amarelo
        countCtc = 0;
      }
    }

    //Pisca LED amarelo caso desligue o semáforo
    else{
      if(countCtc==100){
        OCR2A = 0;  //Vermelho
        OCR1A = 0; //Verde
        OCR1B = 255; //Amarelo
      }

      if(countCtc==200){
        OCR2A = 0;  //Vermelho
        OCR1A = 0; //Verde
        OCR1B = 0; //Amarelo
        countCtc = 0;
      }
    }
}

//Ligando/Desligando todos os Leds por interrupção na PD2
ISR(INT0_vect){
  countInt++;
  countCtc = 0;
  OCR2A = 0;
  OCR1A = 0;
  OCR1B = 0;
  if(countInt % 2 == 0){
    ligado = true;
  }
  else{
    OCR2A = 0;
    OCR1A = 0;
    OCR1B = 0;
    ligado = false;
  }
}