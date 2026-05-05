#include <mega16a.h>
#include <delay.h>

#define safety_key PIND.2       //안전버튼
#define semiauto_key PIND.3     //단발버튼
#define brust_key PIND.4        //점사버튼
#define auto_key PIND.5         //연사버튼
#define joy_sw PINC.2
#define fire_key PINB.5         //발사버튼
#define drone_left_key PINB.6   //드론_카메라_좌회전
#define drone_right_key PINB.7  //드론_카메라_우회전
#define right PINA.2            //차체_우회전
#define front PINA.3            //차체_전진
#define left PINA.4             //차체_좌회전
#define back PINA.5             //차체_후진
#define signal PORTA.0          //IR_led_신호출력


#define safety_led PORTB.3      //안전_led표시
#define semiauto_led PORTB.2    //단발_led표시
#define brust_led PORTB.1       //점사_led표시
#define auto_led PORTB.0        //연사_led표시


bit mode1_flag, mode2_flag, mode3_flag, mode4_flag, mode5_flag, mode6_flag,
    mode7_flag, mode8_flag, mode9_flag, mode10_flag, mode11_flag;
unsigned int gun_mode = 1, joy_stick_cnt;
int sig_point, mode, adc_value, joy_y, joy_x;

void gun_key();
void key_flag();              
void key_move();
void ir_sig();

unsigned int read_adc(){
    switch(joy_stick_cnt){
        case 0: ADMUX=1; break;
        case 1: ADMUX=6; break;
    }
    delay_us(10);
    ADCSRA|=(1<<ADSC);
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}
interrupt [TIM2_OVF] void timer2_ovf_isr(void){
    TCNT2=0x06;  
    ir_sig();
    key_flag();         
    joy_stick_cnt++;  
    if(joy_stick_cnt == 2){
       joy_stick_cnt = 0; 
    }
}
                
void main(void){
DDRA = 0x01; DDRB = 0x0f; DDRC = 0; DDRD = 0;
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (1<<CS20);
TCNT2=0x06;
OCR2=0x00;
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;
switch(joy_stick_cnt){
    case 0: ADMUX=1; break;
    case 1: ADMUX=6; break;
}
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

TIMSK = 0x41;

#asm("sei")
while (1){
        gun_key();   
        key_move();  
        adc_value = read_adc();  
        
        if(adc_value >= 1000 && ADMUX == 1){joy_y = 1;} 
        else if(adc_value <= 100 && ADMUX == 1){joy_y = 2;}
        else{joy_y = 0;} 
        
        if(adc_value >= 800 && ADMUX == 6){joy_x = 2;}
        else if(adc_value <= 100 && ADMUX == 6){joy_x = 1;}
        else{joy_x = 0;}      
    }
}
void gun_key(){
    /*******안전_단발_점사_연사_버튼_gun_mode<1~4>*******/
    /**안전모드**/
    if(safety_key == 0 || UDR == '1'){gun_mode = 1;}        
    /**단발모드**/           
    else if(semiauto_key == 0 || UDR == '2'){gun_mode = 2;}         
    /**점사모드**/
    else if(brust_key == 0 || UDR == '3'){gun_mode = 3;}          
    /**연사모드**/
    else if(auto_key == 0 || UDR == '4'){gun_mode = 4;}       
    /*******리모컨에_안전_단발_점사_연사_led표시_*******/
    switch(gun_mode){     
        case 1: safety_led = 1; semiauto_led = brust_led = auto_led = 0; break; //안전led_표시   
        case 2: semiauto_led = 1; safety_led = brust_led = auto_led = 0; break; //단발led_표시
        case 3: brust_led = 1; semiauto_led = safety_led = auto_led = 0; break; //점사led_표시
        case 4: auto_led = 1; semiauto_led = brust_led = safety_led = 0; break; //연사led_표시
    }                                                       
}      
void key_flag(){ //키를_눌렀을때_상승펄스에서_동작한다.
    /********단일동작_********/
    if(front == 1 || UDR == 'w' || joy_y == 1){mode1_flag = 0;}
    if(back == 1 || UDR == 's' || joy_y == 2){mode2_flag = 0;}
    if(left == 1 || UDR == 'a' || joy_x == 1){mode3_flag = 0;}
    if(right == 1 || UDR == 'd' || joy_x == 2){mode4_flag = 0;}
    ///////////////    
    if(joy_sw == 1 || UDR == 'f'){mode5_flag = 0;}                          
    if(drone_left_key == 1 || UDR == 'q'){mode6_flag = 0;}
    if(drone_right_key == 1 || UDR == 'e'){mode7_flag = 0;}                       
    ///////////////
    if(safety_key == 1 || UDR == '1'){mode8_flag = 0;}
    if(semiauto_key == 1 || UDR == '2'){mode9_flag = 0;}
    if(brust_key == 1 || UDR == '3'){mode10_flag = 0;}
    if(auto_key == 1 || UDR == '4'){mode11_flag = 0;}                                        
}
void key_move(){
    if(joy_y == 1 && mode1_flag == 0 || front == 0 && mode1_flag == 0 || UDR == 'w' && mode1_flag == 0){//전진_모드
        mode1_flag = 1;  
        mode = 1;
    }
    if(joy_y == 2 && mode2_flag == 0 || back == 0 && mode2_flag == 0 || UDR == 's' && mode2_flag == 0){//후진모드
        mode2_flag = 1;
        mode = 2;
    }      
    if(joy_x == 1 && mode3_flag == 0 || left == 0 && mode3_flag == 0 || UDR == 'a' && mode3_flag == 0){//좌회전모드
        mode3_flag = 1;
        mode = 3;
    }
    if(joy_x == 2 && mode4_flag == 0 || right == 0 && mode4_flag == 0 || UDR == 'd' && mode4_flag == 0){//우회전모드
        mode4_flag = 1;
        mode = 4;
    }  
    if(joy_sw == 0 && mode5_flag == 0 || UDR == 'f' && mode5_flag == 0){//발사모드
        mode5_flag = 1;
        mode = 5;
    }        
    if(drone_left_key == 0 && mode6_flag == 0 || UDR == 'q' && mode6_flag == 0){//드론_카메라_좌회전
        mode6_flag = 1;
        mode = 6;
    }
    if(drone_right_key == 0 && mode7_flag == 0 || UDR == 'e' && mode7_flag == 0){//드론_카메라_우회전
        mode7_flag = 1;
        mode = 7;
    }    
    if(safety_key == 0 && mode8_flag == 0 || UDR == '1' && mode8_flag == 0){ //안전모드
        mode8_flag = 1; 
        mode = 8;
    }
    if(semiauto_key == 0 && mode9_flag == 0 || UDR == '2' && mode9_flag == 0){//단발모드
        mode9_flag = 1;
        mode = 9;
    }           
    if(brust_key == 0 && mode10_flag == 0 || UDR == '3' && mode10_flag == 0){  //점사모드
        mode10_flag = 1;
        mode = 10;
    }
    if(auto_key == 0 && mode11_flag == 0 || UDR == '4' && mode11_flag == 0){   //연사모드
        mode11_flag = 1;
        mode = 11;
    }    
}
/****IR_led_신호_출력_****/
void ir_sig(){
    //전진
    if(mode == 1){
        sig_point++;
        switch(sig_point){      //0<x<10
            case 1: signal = 1; break;      
            case 180: signal = 0; break;       
        }      
        if(sig_point >= 200){
            sig_point = 0;
            mode = 0;    
        }
    }           
    //후진
    if(mode == 2){
        sig_point++;
        switch(sig_point){      //10<x<20
            case 1: signal = 1; break;      
            case 195: signal = 0; break;       
        }      
        if(sig_point >= 215){
            sig_point = 0;
            mode = 0;    
        }
    }   
    //좌회전 
    if(mode == 3){
        sig_point++;
        switch(sig_point){      //20<x<30
            case 1: signal = 1; break;      
            case 220: signal = 0; break;       
        }      
        if(sig_point >= 230){
            sig_point = 0;
            mode = 0;    
        }
    }
    //우회전
    if(mode == 4){
        sig_point++;
        switch(sig_point){      //30<x<40
            case 1: signal = 1; break;      
            case 290: signal = 0; break;       
        }      
        if(sig_point >= 300){
            sig_point = 0;
            mode = 0;    
        }
    }  
    //발사
    if(mode == 5){
        sig_point++;
        switch(sig_point){      //40<x<50
            case 1: signal = 1; break;      
            case 310: signal = 0; break;       
        }      
        if(sig_point >= 315){
            sig_point = 0;
            mode = 0;    
        }
    }   
    //드론_카메라_좌회전
    if(mode == 6){
        sig_point++;
        switch(sig_point){      //50<x<60
            case 1: signal = 1; break;      
            case 330: signal = 0; break;       
        }      
        if(sig_point >= 335){
            sig_point = 0;
            mode = 0;    
        }
    }
    //드론_카메라_우회전
    if(mode == 7){
        sig_point++;
        switch(sig_point){      //60<x<70
            case 1: signal = 1; break;      
            case 400: signal = 0; break;       
        }      
        if(sig_point >= 405){
            sig_point = 0;
            mode = 0;    
        }
    }
    //안전설정
    if(mode == 8){
        sig_point++;
        switch(sig_point){      //70<x<80
            case 1: signal = 1; break;      
            case 420: signal = 0; break;       
        }      
        if(sig_point >= 425){
            sig_point = 0;
            mode = 0;    
        }
    }
    //단발설정
    if(mode == 9){
        sig_point++;
        switch(sig_point){      //80<x<90
            case 1: signal = 1; break;      
            case 440: signal = 0; break;       
        }      
        if(sig_point >= 445){
            sig_point = 0;
            mode = 0;    
        }
    } 
    //점사설정
    if(mode == 10){
        sig_point++;
        switch(sig_point){      //90<x<100
            case 1: signal = 1; break;      
            case 518: signal = 0; break;       
        }      
        if(sig_point >= 523){
            sig_point = 0;
            mode = 0;    
        }
    } 
    //연사설정
    if(mode == 11){
        sig_point++;
        switch(sig_point){      //100<x<110
            case 1: signal = 1; break;      
            case 540: signal = 0; break;       
        }      
        if(sig_point >= 545){
            sig_point = 0;
            mode = 0;    
        }
    }
}
