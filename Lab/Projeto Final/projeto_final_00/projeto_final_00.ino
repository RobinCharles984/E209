#define LED_VERDE (1 << PB1) //OC1A
#define LED_AMARELO (1 << PB2) //OC1B
#define LED_VERMELHO (1 << PB3) //OC2A
#define POT (1 << PC2) //Potenciometro

//Variáveis globais
unsigned int countInt = 0;
unsigned int countCtc = 0;
bool ligado = true;
int valorPot = 0;
int adc_result;

//Função Inicializa conversão AD
void ADC_init(void)
{
  // Configurando Vref para VCC = 5V
  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // configuracao do prescaler de 128
}

//Função lê conversão AD
int ADC_read(u8 ch)
{
  char i;
  int ADC_temp = 0; // ADC temporário, para manipular leitura
  int ADC_read = 0; // ADC_read
  ch &= 0x07;
  // Zerar os 3 primeiros bits e manter o resto
  ADMUX = (ADMUX & 0xF8) | ch;
  // ADSC (ADC Start Conversion)
  ADCSRA |= (1 << ADSC); // Faça uma conversão
  // ADIF (ADC Interrupt Flag) é setada quando o ADC pede interrupção
  // e resetada quando o vetor de interrupção
  // é tratado.
  while (!(ADCSRA & (1 << ADIF))); // Aguarde a conversão do sinal

  for (i = 0; i < 8; i++) // Fazendo a conversão 8 vezes para maior precisão
  {
    ADCSRA |= (1 << ADSC); // Faça uma conversão
    while (!(ADCSRA & (1 << ADIF))); // Aguarde a conversão do sinal
    ADC_temp = ADCL; // lê o registro ADCL
    ADC_temp += (ADCH << 8); // lê o registro ADCH
    ADC_read += ADC_temp; // Acumula o resultado (8 amostras) para média
  }
  ADC_read = ADC_read >> 3; // média das 8 amostras ( >> 3 é o mesmo que /8)
  return ADC_read;
}

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

  Serial.begin(9600);
  ADC_init();//Liga conversão AD
  sei();//Ligando interrupções e timers
  
  //Loop
  while(true){
    adc_result = ADC_read(ADC0D); // lê o valor do ADC0 = PC0
    _delay_ms(50); // Tempo para troca de canal

    // Mostra o valor na serial
    Serial.print("ADC0: ");
    Serial.println(adc_result);
    Serial.print("Resistor: ");
    Serial.println(valorPot);
    _delay_ms(100);

    valorPot = (adc_result * 0.25);
  }
}

//Trocando o faról
ISR(TIMER0_COMPA_vect){
    countCtc++;

    if(ligado){
      if(countCtc==1){
        OCR2A = valorPot;  //Vermelho
        OCR1A = 0; //Verde
        OCR1B = 0; //Amarelo
      }

      //Troca pro Verde ligado
      if(countCtc==2000){
        OCR2A = 0;  //Vermelho
        OCR1A = valorPot; //Verde
        OCR1B = 0; //Amarelo
      }

      //Troca pro Amarelo ligado
      if(countCtc==3500){
        OCR2A = 0;  //Vermelho
        OCR1A = 0; //Verde
        OCR1B = valorPot; //Amarelo
      }

      //Troca pro Vermelho ligado e zera o contador
      if(countCtc==4000){
        OCR2A = valorPot;  //Vermelho
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
        OCR1B = valorPot; //Amarelo
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