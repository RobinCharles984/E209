//Defines
#define BOTAO (1 << PD0)
#define VERMELHO (1 << PC2)
#define AMARELO (1 << PC1)
#define AZUL (1 << PC0)

unsigned int count = 0;

int main(){
  DDRD = 0;
  DDRC = 255;
  PORTD = 255;
  PORTC |= (VERMELHO);

  while(1){
    //Ao pressionar BOTAO
    if(!(PIND & (1 << BOTAO)))
    {
      count++;
      PORTC &= ~(VERMELHO);
      PORTC |= (AMARELO);
      _delay_ms(500);
      PORTC &= ~(AMARELO);
      _delay_ms(500);
      PORTC |= (AMARELO);
      _delay_ms(500);
      PORTC &= ~(AMARELO);
      _delay_ms(500);
      PORTC |= (AMARELO);
      _delay_ms(500);
      PORTC &= ~(AMARELO);
      _delay_ms(500);
      PORTC |= (AZUL);
    }

    //Ao pressionar BOTAO novamente
    if(count % 2 == 0)
    {
      PORTC |= (VERMELHO);
      PORTC &= ~(AZUL);
    }
  }
}
