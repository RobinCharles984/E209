//Defines
#define LIGA (1 << PD0)
#define DESLIGA (1 << PD1)
#define SENSOR (1 << PD2)
#define MOTOR (1 << PB5)
#define ALARME (1 << PB4)

bool alarme = false;

int main(){
  DDRB |= (1 << MOTOR) | (1 << ALARME);
  DDRD = 0;
  PORTB = 0;
  PORTD = 255;

  while(1){
    //Ao pressionar LIGA e SENSOR estiver desligado
    if(!(PIND & (1 << LIGA) & !(1 << SENSOR)))
    {
      alarme = false;
      PORTB |= (MOTOR);
      PORTB &= ~(ALARME);
    }

    //Ao pressionar DESLIGA
    if(!(PIND & (1 << DESLIGA)))
      PORTB &= ~(MOTOR);

    //Ao detectar SENSOR
    if(!(PIND & (1 << SENSOR)))
    {
      alarme = true;
    }

    //Função ALARME
    if(alarme){
      PORTB &= ~(MOTOR);
      PORTB |= (ALARME);
      _delay_ms(500);
      PORTB &= ~(ALARME);
      _delay_ms(500);
      PORTB |= (ALARME);
    }
  }
}