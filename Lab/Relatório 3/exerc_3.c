#define BOTAO (1 << PB0)
#define LED1 (1 << PD3)
#define LED2 (1 << PD4)

int main(){
    DDRD |= LED1 | LED2;
    PORTB = 255;

    bool check = false;;

    while (1)
    {
        while (!check)
        {
            if(!(PINB & (1 << BOTAO)))
                check = true;
        }

        //01
        PORTD |= LED2;
        _delay_ms(500);

        //10
        PORTD &= ~(LED2);
        PORTD |= LED1;
        _delay_ms(500);

        //00
        PORTD &= ~(LED1 & LED2);
        _delay_ms(500);
    }
}