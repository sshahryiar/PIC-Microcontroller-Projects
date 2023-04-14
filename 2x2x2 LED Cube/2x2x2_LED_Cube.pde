unsigned char row1=11;
unsigned char row2=12;

unsigned char col1=21;
unsigned char col2=22;
unsigned char col3=23;
unsigned char col4=24;

unsigned char times=0;

unsigned int timeGap=0;   

unsigned int dutyL1=0;
unsigned int dutyL2=0;
unsigned int duty=0;


void all_off()
{
    delay(10);
    digitalWrite(col1, LOW);
    digitalWrite(col2, LOW);
    digitalWrite(col3, LOW);
    digitalWrite(col4, LOW);
    delay(100);
}


void loop_back_and_grow_1()
{
    for(times=0;times<3;times++)
    {
        digitalWrite(col1, HIGH);
        digitalWrite(col2, HIGH);
        digitalWrite(col3, HIGH);
        digitalWrite(col4, HIGH);
        for(duty=0;duty<1023;duty++)
        {
            dutyL1=duty;
            dutyL2=duty;
            analogWrite(row1,dutyL1);
            analogWrite(row2,dutyL2);
            delay(4);
        }
        for(duty=1023;duty>0;duty--)
        {
            dutyL1=duty;
            dutyL2=duty;
            analogWrite(row1,dutyL1);
            analogWrite(row2,dutyL2);
            delay(4);
        }
    }
}


void loop_back_and_grow_2()
{
    for(times=0;times<3;times++)
    {
        digitalWrite(col1, HIGH);
        digitalWrite(col2, HIGH);
        digitalWrite(col3, HIGH);
        digitalWrite(col4, HIGH);
        for(duty=0;duty<1023;duty++)
        {
            dutyL1=(1023-duty);
            dutyL2=duty;
            analogWrite(row1,dutyL1);
            analogWrite(row2,dutyL2);
            delay(4);
        }
        for(duty=1023;duty>0;duty--)
        {
            dutyL1=(1023-duty);
            dutyL2=duty;
            analogWrite(row1,dutyL1);
            analogWrite(row2,dutyL2);
            delay(4);
        }
    }
}


void helical()
{
    for(times=0;times<9;times++)
    {
        for(timeGap=150;timeGap>=60;timeGap-=10)
        {
            analogWrite(row1,1023);
            analogWrite(row2,1023);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, LOW);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, LOW);
            digitalWrite(col4, HIGH);
            delay(timeGap);
            
            analogWrite(row1,0);
            analogWrite(row2,0);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, LOW);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, LOW);
            digitalWrite(col4, HIGH);
            delay(timeGap);
        }
    }
}


void levelMotion()
{
    digitalWrite(col1, HIGH);
    digitalWrite(col2, HIGH);
    digitalWrite(col3, HIGH);
    digitalWrite(col4, HIGH);
    for(times=0;times<3;times++)
    {
        for(timeGap=120;timeGap<180;timeGap+=30)
        {
            analogWrite(row1,1023);
            analogWrite(row2,1023);
            delay(timeGap);
            analogWrite(row1,0);
            analogWrite(row2,0);
            delay(timeGap);
        }
    }
}


void pinpongMotion()
{
    analogWrite(row1,1023);
    analogWrite(row2,0);
    for(times=0;times<3;times++)
    {
        for(timeGap=180;timeGap<230;timeGap+=20)
        {
            digitalWrite(col1, HIGH);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, HIGH);
            delay(timeGap);
        }
    }
}


void cross()
{
    analogWrite(row1,1023);
    analogWrite(row2,0);
    for(times=0;times<3;times++)
    {
        for(timeGap=180;timeGap<230;timeGap+=20)
        {
            digitalWrite(col1, HIGH);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, LOW);
            delay(timeGap);
            digitalWrite(col1, LOW);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, HIGH);
            delay(timeGap);
        }
    }
}


void circulate()
{
    for(times=0;times<3;times++)
    {
        for(timeGap=180;timeGap<230;timeGap+=20)
        {
            analogWrite(row1,1023);
            analogWrite(row2,1023);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            analogWrite(row1,1023);
            analogWrite(row2,1023);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, HIGH);
            delay(timeGap);
            
            analogWrite(row1,0);
            analogWrite(row2,0);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, HIGH);
            delay(timeGap);
            analogWrite(row1,0);
            analogWrite(row2,0);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
        }
    }
}


void crossDiagonal()
{
    for(times=0;times<3;times++)
    {
        for(timeGap=180;timeGap<230;timeGap+=20)
        {
            analogWrite(row1,1023);
            analogWrite(row2,1023);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            analogWrite(row1,0);
            analogWrite(row2,0);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, HIGH);
            delay(timeGap);
             
            analogWrite(row1,0);
            analogWrite(row2,0);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, HIGH);
            delay(timeGap);
            analogWrite(row1,1023);
            analogWrite(row2,1023);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);  

            analogWrite(row1,1023);
            analogWrite(row2,1023);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, HIGH);
            delay(timeGap);
            analogWrite(row1,0);
            analogWrite(row2,0);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);   
            
            analogWrite(row1,0);
            analogWrite(row2,0);
            digitalWrite(col1, HIGH);
            digitalWrite(col2, HIGH);
            digitalWrite(col3, LOW);
            digitalWrite(col4, LOW);
            delay(timeGap);
            analogWrite(row1,1023);
            analogWrite(row2,1023);
            digitalWrite(col1, LOW);
            digitalWrite(col2, LOW);
            digitalWrite(col3, HIGH);
            digitalWrite(col4, HIGH);
            delay(timeGap); 
        }
    }
}


void randomMotion()
{
    boolean s=0;
    for(times=0;times<16;times++)
    {
        analogWrite(row1,analogRead(0));
        analogWrite(row2,analogRead(1));
        s=(analogRead(2)&0x01);
        digitalWrite(col1,s);
        s=(analogRead(2)&0x01);
        digitalWrite(col2,s);
        s=(analogRead(2)&0x01);
        digitalWrite(col3,s);
        s=(analogRead(2)&0x01);
        digitalWrite(col4,s);
        delay(200);
    }
}


void setup() 
{
  pinMode(row1, OUTPUT); 
  pinMode(row2, OUTPUT);
  pinMode(col1, OUTPUT); 
  pinMode(col2, OUTPUT); 
  pinMode(col3, OUTPUT); 
  pinMode(col4, OUTPUT);   
}

void loop() 
{
    all_off();
    loop_back_and_grow_1();
    all_off();
    crossDiagonal();
    all_off();
    randomMotion();
    all_off();
    circulate();
    all_off();
    pinpongMotion();
    all_off();
    cross();
    all_off();
    levelMotion();
    all_off();
    helical();
    all_off();
    loop_back_and_grow_2();
    all_off();
}