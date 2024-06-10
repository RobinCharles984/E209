#define LED_VERDE (1 << PB1)
#define LED_AMARELO (1 << PB2) //Estes defines não serão necessários quando PWM for configurado
#define LED_VERMELHO (1 << PB3)

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

  sei();//Ligando interrupções e timers
  while(true){

  }
}

//Trocando o faról
ISR(TIMER0_COMPA_vect){
    countCtc++;

    if(ligado){
      if(countCtc==1){
        PORTB ^= LED_VERMELHO;
      }

      //Troca pro Verde ligado
      if(countCtc==2000){
        PORTB ^= LED_VERMELHO;
        PORTB ^= LED_VERDE;
      }

      //Troca pro Amarelo ligado
      if(countCtc==3500){
        PORTB ^= LED_VERDE;
        PORTB ^= LED_AMARELO;
      }

      //Troca pro Vermelho ligado e zera o contador
      if(countCtc==4000){
        PORTB ^= LED_AMARELO;
        countCtc=0;
      }
    }
}

//Ligando/Desligando todos os Leds por interrupção na PD2
ISR(INT0_vect){
  countInt++;
  countCtc = 0;
  if(countInt % 2 == 0){
    ligado = true;
  }
  else{
    PORTB &= LED_VERDE & LED_AMARELO & LED_VERMELHO;//Será substituído por OCXX quando terminar o PWM
    ligado = false;
  }
}
