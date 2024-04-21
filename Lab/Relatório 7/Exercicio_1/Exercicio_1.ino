#define LED_LOOP (1 << PD5)
#define LED_INT (1 << PD4)

int main(){
  DDRD = 1;
  PORTD = 0;

  EIMSK = (1 << INT0) + (1 << INT1);
  EICRA = (1 << ISC01) + (1 << ISC00);
  sei();

  while(1){
    PORTD |= (LED_LOOP);
    _delay_ms(500);
    PORTD &= ~(LED_LOOP);
    _delay_ms(500);
  }
}

ISR(INT0_vect){
  PORTD |= (LED_INT);
  _delay_ms(1000);
  PORTD |= (LED_INT);
}

ISR(INT0_vect){
  EIMSK |= ~(1 << INT0);
}