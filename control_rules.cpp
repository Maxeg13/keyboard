#include "control_rules.h"
float margin_x=0.04;
float margin_y=0.17;
extern bool emulate_from_tracker_on;
extern float y_centre;
extern float x_centre;
extern bool lr_checked;
extern bool kb_layout_checked;
extern bool downup_checked;
int state[4]={0,0,0,0};
bool pressed[4]={0,0,0,0};
float sens_div=0.016;
int UDP_cnt[5];
//int gl_cnt=0;
int quantiz=16;
extern Arrows* ars;


int thresh(int x)
{
    if(x<quantiz)
        return x;
    else
        return quantiz;
}

void key_map(INPUT& ip, int b)
{
    if(kb_layout_checked)
        switch(b)
        {
        case 0:
            //AFTER VK_HELP
            ip.ki.wVk = 0x41;//a
            break;
        case 1:
            ip.ki.wVk = 0x44;//d
            break;
        case 2:
            ip.ki.wVk = 0x57;//w
            break;
        case 3:
            ip.ki.wVk = 0x53;//s
            break;
        case 4:
            ip.ki.wVk = VK_SPACE;
        }
    else
        switch(b)
        {
        case 0:
            //AFTER VK_HELP
            ip.ki.wVk = VK_LEFT;//a
            break;
        case 1:
            ip.ki.wVk = VK_RIGHT;//d
            break;
        case 2:
            ip.ki.wVk = VK_UP;//w
            break;
        case 3:
            ip.ki.wVk = VK_DOWN;//s
            break;
        case 4:
            ip.ki.wVk = VK_SPACE;

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
    ars->checked[b]=pressed[b];
}

void simple_tracker_rule(float x, float y, INPUT& ip, int b)
{
    //    pressed_ptr=pressed;
    key_map(ip,b);

    int h=QPoint::dotProduct(ars->centre[b]-QPoint(x*ars->width(),y*ars->height()),
                             ars->centre[b]-QPoint(x*ars->width(),y*ars->height()));
    //    if(b==0)
    //        qDebug()<<h;
    int dist=120;
    if((((b==0)||(b==1))&&lr_checked)||(((b==2)||(b==3))&&downup_checked))
        if(h<dist*dist)
        {
            pressed[b]=1;
            state[b]=quantiz;
        }
        else
        {
            pressed[b]=0;
        }


    ars->checked[b]=pressed[b];
}

//void UDP_rule(float x, float y, INPUT& ip, int b)
//{
//    key_map(ip,b);
//}

void controlFromUDP(INPUT& ip,int b1,int sended)
{

    int time_steps=30;
    int b;
    switch(b1)
    {
    case 1:
        b=4;break;
    case 2:
        b=0;break;
    case 3:
        b=1;break;
    case 4:
        b=2;break;
    case 5:
        b=3;
    }
    key_map(ip,b);
    if(sended)
    {
        qDebug()<<b1;
        pressed[b]=1;
        if((((b==0)||(b==1))&&!lr_checked)||(((b==2)||(b==3))&&!downup_checked))
        {
            ip.ki.dwFlags =0;
            SendInput(1, &ip, sizeof(INPUT));
        }
        UDP_cnt[b]=0;
    }

    if(!sended)
    {

        UDP_cnt[b]++;
        if(UDP_cnt[b]>time_steps)UDP_cnt[b]=time_steps+1;

    }

    if(UDP_cnt[b]==time_steps)
    {
        pressed[b]=0;
        if((((b==0)||(b==1))&&!lr_checked)||(((b==2)||(b==3))&&!downup_checked))
        {
            ip.ki.dwFlags =KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            SendInput(1, &ip, sizeof(INPUT));
        }
    }
    ars->checked[b]=pressed[b];
}

void controlFromTracker(float x, float y, INPUT& ip)
{

    for(int b=0;b<4;b++)
    {
        simple_tracker_rule(x,y,ip,b);
        //        rule(x,y,ip,b);
    }

    if(emulate_from_tracker_on)
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
                if(((gl_cnt)>=state[b]))
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
