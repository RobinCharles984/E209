//Defines
#define BOTAOA (1 << PD0)
#define BOTAOB (1 << PD1)
#define BOTAOC (1 << PD2)
#define MOTOR1 (1 << PB3)
#define MOTOR2 (1 << PB2)
#define MOTOR3 (1 << PB1)

bool motor1, motor2, motor3;

int main(){
  DDRD = 0;
  DDRB = 255;
  PORTD = 255;
  PORTB = 0;

  motor1 = false;
  motor2 = false;
  motor3 = false;

  while(1){
    //Ligando MOTOR1
    if(!(PIND & (1 << BOTAOA))){
      PORTB |= (MOTOR1);
      motor1 = true;
    }

    //Ligando MOTOR2
    if(!(PIND & (1 << BOTAOB))){
      PORTB |= (MOTOR2);
      motor2 = true;
    }

    //Ligando MOTOR3
    if(!(PIND & (1 << BOTAOC))){
      PORTB |= (MOTOR3);
      motor3 = true;
    }

    ////CHECANDO QUANDO PASSAR DE 90HP

    if(motor1 && motor3){
      PORTB &= ~(MOTOR1);
      motor1 = false;
    }

    if(motor2 && motor3){
      PORTB &= ~(MOTOR2);
      motor2 = false;
    }

    if(motor1 && motor2 && motor3){
      PORTB &= ~(MOTOR1);
      PORTB &= ~(MOTOR2);
      motor1 = false;
      motor2 = false;
    }
  }
}