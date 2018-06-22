#ifndef CONTROL_RULES_H
#define CONTROL_RULES_H
#include <QLineEdit>
float margin_x=0.05;
float margin_y=0.17;
extern bool write_on;
extern float y_centre;
extern float x_centre;
int state[4]={0,0,0,0};
bool pressed[4]={0,0,0,0};
float sens_div=0.02;
//int gl_cnt=0;
int quantiz=16;


int thresh(int x)
{
    if(x<quantiz)
        return x;
    else
        return quantiz;
}


void key_map(INPUT& ip, int b)
{
    switch(b)
    {
    case 0:
        //AFTER VK_HELP
        ip.ki.wVk = 0x41;
        break;
    case 1:
        ip.ki.wVk = 0x44;//d
        break;
    case 2:
        ip.ki.wVk = 0x57;
        break;
    case 3:
        ip.ki.wVk = 0x53;
    }
}

void rule(float x, float y, INPUT& ip, int b)
{
    key_map(ip,b);
    switch(b)
    {
    case 0:
        //LEFT
        if(x<x_centre-margin_x/2)
        {
            pressed[b]=1;
            state[b]=thresh((0.5-margin_x/2-x)/sens_div);
        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
        break;
    case 1:
        //RIGHT
        if(x>x_centre+margin_x/2)
        {
            pressed[b]=1;
            //            std::cout<<(state[b]=thresh(-(0.5+margin_x/2-x)/sens_div))<<std::endl;
            state[b]=thresh(-(0.5+margin_x/2-x)/sens_div);
            //            state[b]=1;

        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
        break;
    case 2:
        //FORWARD
        if(y<y_centre)
        {
            pressed[b]=1;
            state[b]=thresh((y_centre-y)/(sens_div/2));
        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
        break;
    case 3:
        //BACK
        if(y>y_centre+margin_y/2)
        {
            pressed[b]=1;
            state[b]=quantiz;
        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
    }
}


void control(float x, float y, INPUT& ip)
{

    for(int b=0;b<4;b++)
    {
        rule(x,y,ip,b);
    }

    if(write_on)
        for(int gl_cnt=0;gl_cnt<quantiz;gl_cnt++)
        {
            for(int b=0;b<4;b++)
            {
                key_map(ip,b);
                if((gl_cnt==0)&&(pressed[b]))
                {
                    ip.ki.dwFlags = 0; // KEYEVENTF_KEYDOWN for key release
                    SendInput(1, &ip, sizeof(INPUT));
                }
            }

            for(int b=0;b<4;b++)
            {
                key_map(ip,b);
                if(((gl_cnt)>state[b]))
                {
                    ip.ki.dwFlags =KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
                    SendInput(1, &ip, sizeof(INPUT));
                }
                if(!pressed[b])
                {
                    ip.ki.dwFlags =KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
                    SendInput(1, &ip, sizeof(INPUT));
                }
            }
            Sleep(4);
        }


}
#endif // CONTROL_RULES_H
