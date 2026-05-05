#include <mega16a.h>
#include <delay.h>

#define signal PINA.0
#define fnd1 PORTC.6
#define fnd2 PORTC.7
#define fire PORTB.4
#define headlight PORTD.6

bit sig_start, sig_flag1, sig_flag2;
char sig_1, sig_2, sig_3, sig_4, sig_5, sig_6, gun_mode, gun_mode1, fndc;
int sig_point, mode, mode1, mode2, tick, num1, num10, num100, num1000, buf, number, fire_cnt, 
    mode1_cnt, mode2_cnt, mode3_cnt, mode4_cnt, mode5_cnt, mode6_cnt, mode7_cnt,
    mode8_cnt, mode9_cnt, mode10_cnt, mode11_cnt, mo_cnt, stepmode; 
/****fnd_배열****/
int F[10] = {0x7e, 0x0c, 0xb6, 0x9e, 0xcc, 0xda, 0xf8, 0x0e, 0xfe, 0xce};

/********_fnd는_캐소드_제품*******/
void fnd(){
    /********signal_카운터표시_확인*******/
    fndc++;  
    switch(fndc){
        case 1: PORTA = F[num10]; fnd1 = 0; fnd2 = 1; break;
        case 2: fnd1 = 1; fnd2 = 1; break;
        case 3: PORTA = F[num1]; fnd1 = 1; fnd2 = 0; fndc = 0; break;
    }               
    /**********9999진_**********/
    num1000 = number / 1000;
    buf = number % 1000;
    
    num100 = buf / 100;
    buf = buf % 100;
    
    num10 = buf / 10;
    num1 = buf % 10;       
 
}
/*****동작_*****/

void motor(){
    //멈춤
    if(mode == 0){
        PORTC.2 = 0; PORTC.3 = 0; PORTC.4 = 0; PORTC.5 = 0;
    }        
    //전진
    if(mode == 1 && mode1 == 0){
        PORTC.2 = 1; PORTC.3 = 0; PORTC.4 = 1; PORTC.5 = 0;    
    }     
    //후진        
    if(mode == 2 && mode1 == 0){
        PORTC.2 = 0; PORTC.3 = 1; PORTC.4 = 0; PORTC.5 = 1;
    }     
    //좌회전, 우회전_멈춤
    if(mode1 == 0){
        PORTB.5 = 0; PORTB.6 = 0;
    }  
    //좌회전   
    if(mode1 == 1){
        PORTB.5 = 0; PORTB.6 = 1;
    }         
    //우회전
    if(mode1 == 2){
        PORTB.5 = 1; PORTB.6 = 0;
    }  
    /*****PWM을_위해_만든_것*****/
    mo_cnt++; 
    if(mo_cnt >= 100){
        mo_cnt = 0;
    }                             
    ///////////////////////////
    //전진_좌회전
    if(mode == 1 && mode1 == 1){ 
        PORTC.4 = 1; PORTC.5 = 0;
        //_인사이드_바퀴는_느리게_<왼쪽바퀴_느리게>
        //PWM_방식
        if(mo_cnt >= 40){
            PORTC.2 = 1; PORTC.3 = 0;
        }                            
        else{
            PORTC.2 = 0; PORTC.3 = 0;
        }             
    }
    //전진_우회전
    if(mode == 1 && mode1 == 2){  
        PORTC.2 = 1; PORTC.3 = 0;
        //_인사이드_바퀴는_느리게_<오른쪽바퀴_느리게>
        //PWM_방식
        if(mo_cnt >= 40){
            PORTC.4 = 1; PORTC.5 = 0;
        }                            
        else{
            PORTC.4 = 0; PORTC.5 = 0;
        }             
    }      
    
    //후진_좌회전
    if(mode == 2 && mode1 == 1){
        PORTC.4 = 0; PORTC.5 = 1; 
        if(mo_cnt >= 40){
            PORTC.2 = 0; PORTC.3 = 1;
        }                            
        else{
            PORTC.2 = 0; PORTC.3 = 0;
        }             
    } 
    
    //후진_우회전
    if(mode == 2 && mode1 == 2){
        PORTC.2 = 0; PORTC.3 = 1;  
        if(mo_cnt >= 40){
            PORTC.4 = 0; PORTC.5 = 1;
        }                            
        else{
            PORTC.4 = 0; PORTC.5 = 0;
        }             
    }     
}           
/*****드론_카메라******/
/*******_스텝모터_사용******/
void step(){
    if(stepmode == 0){
        PORTB.0 = PORTB.1 = PORTB.2 = PORTB.3 = 0; tick = 0;
    }
    if(stepmode == 1){  //드론카메라_좌회전
        tick++;
        switch(tick){
            case 5: PORTB.0 = 0; PORTB.1 = 0; PORTB.2 = 0; PORTB.3 = 1; break;
            case 10: PORTB.0 = 0; PORTB.1 = 0; PORTB.2 = 1; PORTB.3 = 0; break;
            case 15: PORTB.0 = 0; PORTB.1 = 1; PORTB.2 = 0; PORTB.3 = 0; break;
            case 20: PORTB.0 = 1; PORTB.1 = 0; PORTB.2 = 0; PORTB.3 = 0; tick = 0; break;
        }      
    }     
    if(stepmode == 2){  //드론카메라_우회전
        tick++;
        switch(tick){
            case 5: PORTB.0 = 1; PORTB.1 = 0; PORTB.2 = 0; PORTB.3 = 0; break;
            case 10: PORTB.0 = 0; PORTB.1 = 1; PORTB.2 = 0; PORTB.3 = 0; break;
            case 15: PORTB.0 = 0; PORTB.1 = 0; PORTB.2 = 1; PORTB.3 = 0; break;
            case 20: PORTB.0 = 0; PORTB.1 = 0; PORTB.2 = 0; PORTB.3 = 1; tick = 0; break;       
        }      
    }
}
/*****총기_동작_*****/
void gun(){
    if(gun_mode == 0){
        headlight = 0;
    }                 
    if(gun_mode == 1){
        headlight = 1;
    }              
    /*****단발_*****/
    if(gun_mode == 3){     
        if(gun_mode1 == 1){  
            fire_cnt++;
            if(fire_cnt <= 115){
                fire = 1;           
            }     
            //사격중지
            if(fire_cnt > 115){gun_mode1 = 0; fire_cnt = 0; fire = 0;}  
        }
    }          
    /*****점사_*****/
    if(gun_mode == 4){                         
        if(gun_mode1 == 1){
            fire_cnt++;      
            if(fire_cnt < 330){
                fire = 1;           
            }
            if(fire_cnt >= 330){gun_mode1 = 0; fire_cnt = 0; fire = 0;}
        }
    }                       
    /*****연사_*****/
    if(gun_mode == 5){
        if(gun_mode1 == 1){
            fire = 1;         
        }
        if(gun_mode1 == 0){
            fire = 0;         
        }
    } 
}
void sig_mode(){
    if(number < 4){   
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0;
        mode4_cnt = 0; 
        mode5_cnt = 0;
        mode6_cnt = 0;   
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
    }
    /********전진**********/
    if(number >= 4 && number < 10){  
        mode2_cnt = 0;
        mode3_cnt = 0;
        mode4_cnt = 0; 
        mode5_cnt = 0; 
        mode6_cnt = 0;   
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode1_cnt++;
    }
    if(mode1_cnt >= 50 && number >= 4 && number < 10 && mode == 0){
        number = 0;
        mode = 1;   
        mode1_cnt = 0;     
    } 
    //전진_계속동작
    if(mode1_cnt >= 50 && number >= 4 && number < 10 && mode == 1){
        number = 0;
        mode = 1;   
        mode1_cnt = 0;     
    }
    //후진_계속동작
    if(mode2_cnt >= 50 && number >= 10 && number < 20 && mode == 2){
        number = 0;
        mode = 2;   
        mode2_cnt = 0;     
    }
    /***********후진_멈춤********/
    if(mode1_cnt >= 50 && number >= 4 && number < 10 && mode == 2){
        number = 0;
        mode = 0;   
        mode1_cnt = 0;     
    }
    /********후진**********/
    if(number >= 10 && number < 20){
        mode1_cnt = 0;
        mode3_cnt = 0;
        mode4_cnt = 0;  
        mode5_cnt = 0; 
        mode6_cnt = 0;   
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode2_cnt++;
    }
    if(mode2_cnt >= 50 && number >= 10 && number < 20 && mode == 0){
        number = 0;
        mode = 2;     
        mode2_cnt = 0;
    }
    /*****전진_멈춤_*****/
    if(mode2_cnt >= 50 && number >= 10 && number < 20 && mode == 1){
        number = 0;
        mode = 0;      
        mode2_cnt = 0; 
    }
    
    
//////////////////////////////////    
    /********좌회전**********/
    if(number >= 20 && number < 30){
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode4_cnt = 0; 
        mode5_cnt = 0; 
        mode6_cnt = 0;   
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode3_cnt++;
    }
    if(mode3_cnt >= 80 && number >= 20 && number < 30 && mode1 == 0){
        number = 0;
        mode1 = 1;     
        mode3_cnt = 0;
    }     
    //좌회전_계속동작
    if(mode3_cnt >= 80 && number >= 20 && number < 30 && mode1 == 1){
        number = 0;
        mode1 = 1;   
        mode3_cnt = 0;     
    } 
    //우회전_계속동작
    if(mode4_cnt >= 80 && number >= 30 && number < 40 && mode1 == 2){
        number = 0;
        mode1 = 2;   
        mode4_cnt = 0;     
    }
    /***우회전_차단_***/
    if(mode3_cnt >= 80 && number >= 20 && number < 30 && mode1 == 2){
        number = 0;
        mode1 = 0;     
        mode3_cnt = 0;
    }
    /********우회전**********/
    if(number >= 30 && number < 40){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0;
        mode5_cnt = 0; 
        mode6_cnt = 0;   
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode4_cnt++;
    }
    if(mode4_cnt >= 80 && number >= 30 && number < 40 && mode1 == 0){
        number = 0;
        mode1 = 2;     
        mode4_cnt = 0;
    }   
    /***좌회전_차단_***/
    if(mode4_cnt >= 80 && number >= 30 && number < 40 && mode1 == 1){
        number = 0;
        mode1 = 0;     
        mode4_cnt = 0;
    }   
    /***발사***/
    if(number >= 40 && number < 50){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0; 
        mode4_cnt = 0;  
        mode6_cnt = 0;   
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode5_cnt++;
    }
    //전조등_on      
    if(mode5_cnt >= 80 && number >= 40 && number < 50 && gun_mode == 0){
        number = 0;
        gun_mode = 1;     
        mode5_cnt = 0;
    }          
    //전조등_off
    if(mode5_cnt >= 80 && number >= 40 && number < 50 && gun_mode == 1){
        number = 0;
        gun_mode = 0;     
        mode5_cnt = 0;
    }   
    //단발
    if(mode5_cnt >= 80 && number >= 40 && number < 50 && gun_mode == 3){
        number = 0;
        gun_mode1 = 1;    
        mode5_cnt = 0;
    }
    //점사
    if(mode5_cnt >= 80 && number >= 40 && number < 50 && gun_mode == 4){
        number = 0;
        gun_mode1 = 1;    
        mode5_cnt = 0;
    }
    //연사
    if(mode5_cnt >= 80 && number >= 40 && number < 50 && gun_mode == 5 && gun_mode1 == 0){
        number = 0;
        gun_mode1 = 1;    
        mode5_cnt = 0;
    }
    //연사차단
    if(mode5_cnt >= 80 && number >= 40 && number < 50 && gun_mode == 5 && gun_mode1 == 1){
        number = 0;
        gun_mode1 = 0;    
        mode5_cnt = 0;
    }   
    
    //드론_카메라_좌회전
    if(number >= 50 && number < 60){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0; 
        mode4_cnt = 0;  
        mode5_cnt = 0;  
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode6_cnt++;
    }
    if(mode6_cnt >= 80 && number >= 50 && number < 60 && stepmode == 0){
        number = 0;
        stepmode = 1;    
        mode6_cnt = 0;
    }  
    //드론_카메라_좌회전_계속 
    if(mode6_cnt >= 80 && number >= 50 && number < 60 && stepmode == 1){
        number = 0;
        stepmode = 1;    
        mode6_cnt = 0;
    } 
    //드론_카메라_우회전_계속 
    if(mode7_cnt >= 80 && number >= 60 && number < 70 && stepmode == 2){
        number = 0;
        stepmode = 2;    
        mode7_cnt = 0;
    }              
    //드론_카메라_우회전_차단 
    if(mode6_cnt >= 80 && number >= 50 && number < 60 && stepmode == 2){
        number = 0;
        stepmode = 0;    
        mode6_cnt = 0;
    } 
    //드론_카메라_우회전
    if(number >= 60 && number < 70){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0; 
        mode4_cnt = 0; 
        mode5_cnt = 0;
        mode6_cnt = 0;   
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode7_cnt++;
    }
    if(mode7_cnt >= 80 && number >= 60 && number < 70 && stepmode == 0){
        number = 0;
        stepmode = 2;    
        mode7_cnt = 0;
    }      
    //드론_카메라_좌회전_차단
    if(mode7_cnt >= 80 && number >= 60 && number < 70 && stepmode == 1){
        number = 0;
        stepmode = 0;    
        mode7_cnt = 0;
    }    
    /****안전설정****/  
    if(number >= 70 && number < 80){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0; 
        mode4_cnt = 0;  
        mode5_cnt = 0; 
        mode6_cnt = 0; 
        mode7_cnt = 0; 
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt = 0;
        mode8_cnt++;
        
        
    }   
    if(mode8_cnt >= 80 && number >= 70 && number < 80){
        number = 0;
        gun_mode = 0;
        mode8_cnt = 0;
    }   
    /****단발설정****/
    if(number >= 80 && number < 90){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0; 
        mode4_cnt = 0;  
        mode5_cnt = 0; 
        mode6_cnt = 0; 
        mode7_cnt = 0;
        mode8_cnt = 0; 
        mode10_cnt = 0;
        mode11_cnt = 0;
        
        mode9_cnt++;
        
    }  
    if(mode9_cnt >= 80 && number >= 80 && number < 90){
        number = 0;
        gun_mode = 3;
        mode9_cnt = 0;
    } 
    /****점사설정****/
    if(number >= 90 && number < 100){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0; 
        mode4_cnt = 0;  
        mode5_cnt = 0; 
        mode6_cnt = 0; 
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode11_cnt = 0;
        mode10_cnt++;
        
    }   
    if(mode10_cnt >= 80 && number >= 90 && number < 100){
        number = 0;
        gun_mode = 4;
        mode10_cnt = 0;
    }
    /****연사설정****/
    if(number >= 100){  
        mode1_cnt = 0;
        mode2_cnt = 0;
        mode3_cnt = 0; 
        mode4_cnt = 0;  
        mode5_cnt = 0; 
        mode6_cnt = 0; 
        mode7_cnt = 0;
        mode8_cnt = 0;
        mode9_cnt = 0;
        mode10_cnt = 0;
        mode11_cnt++;
        
    }  
    if(mode11_cnt >= 80 && number >= 100){
        number = 0;
        gun_mode = 5;
        mode11_cnt = 0;
    }
}

interrupt [TIM2_OVF] void timer2_ovf_isr(void){
    TCNT2=0x06;  
    step(); 
    fnd();
    motor();    
    gun();
    sig_mode();

    /*****신호가_없을때는_1이_들어옴_*****/       
    if(signal == 1){
        sig_flag1 = 0;
    }        
}
/******_자율주행모드_초음파센서_준비중*******/
/*
unsigned int read_adc(unsigned char adc_input){
ADMUX=adc_input | ADC_VREF_TYPE;
delay_us(10);
ADCSRA|=(1<<ADSC);
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}     */
void main(void){
DDRA = 0xfe; DDRB = 0xff; DDRC = 0xff; DDRD = 0xff;
/******_자율주행모드_초음파센서_준비중*******/
/*ADMUX=0;*/
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (1<<CS20);
TCNT2=0x06;
OCR2=0x00;
TIMSK=0x41;
#asm("sei")

while (1){ 
        //하강펄스일때 동작
        if(signal == 0 && sig_flag1 == 0){
            sig_flag1 = 1;
            number++;
        } 
        
    }
}