#ifndef CONTROL_RULES_H
#define CONTROL_RULES_H
#include <QLineEdit>
#include <QDebug>
#include "arrows.h"
#include <windows.h>
//bool* pressed_ptr;


void lr_pressed(int b,int r);
void littleControlPWM(INPUT& ip);
void key_map(INPUT& ip, int b);
void rule(float x, float y, INPUT& ip, int b);
void simple_tracker_rule(float x, float y, INPUT& ip, int b);
void controlFromUDP(INPUT& ip,int b,int sended);
void controlFromTracker(float x, float y, INPUT& ip);
#endif // CONTROL_RULES_H
