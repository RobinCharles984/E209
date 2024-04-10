#define LED1 (1 << PD3)
#define LED2 (1 << PD4)

int main(){
    DDRD |= LED1 | LED2;

    while (1)
    {
        //00
        PORTD &= ~(LED1 & LED2);
        _delay_ms(500);

        //01
        PORTD |= LED2;
        _delay_ms(500);

        //10
        PORTD &= ~(LED2);
        PORTD |= LED1;
        _delay_ms(500);
        
        //11
        PORTD |= LED1 | LED2;
        _delay_ms(500);
    }
    
}