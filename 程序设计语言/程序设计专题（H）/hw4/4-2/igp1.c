#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#define TIMER_BLINK500  1     /*500ms��ʱ���¼���־��*/
#define TIMER_BLINK1000 2     /*1000ms��ʱ��ʱ���־��*/

const int mseconds500 = 500;
const int mseconds1000 = 1000;

//����30�У�ÿ�����1000����ĸ
static char text[50][1000]; /*������ַ���������*/
static int textlen[50];/*������ַ�������*/
static double textx, texty; /*��ǰ�ַ�������ʼλ��*/
int block_px = 0, block_py = 0;//��ǰ�������λ��

static bool isBlink = TRUE;   /*�Ƿ���˸��־*/
static bool isDisplayText = FALSE; /*�ַ�����ʾ��־*/
static bool isBlock = FALSE; /*�Ƿ��˹��*/

void KeyboardEventProcess(int key, int event);/*������Ϣ�ص�����*/
void CharEventProcess(char c);/*�ַ���Ϣ�ص�����*/
void TimerEventProcess(int timerID);/*��ʱ����Ϣ�ص�����*/

void drawBlock(bool state) {
	bool erasemode = GetEraseMode();
	SetEraseMode(!state);
	StartFilledRegion(1);
	DrawLine(0, GetFontHeight());
	DrawLine(0.05, 0);
	DrawLine(0, -GetFontHeight());
	DrawLine(-0.05, 0);
	EndFilledRegion();
	isBlock = state;
	SetEraseMode(erasemode);
}

void eraseText() {
	double ox = GetCurrentX(), oy = GetCurrentY();
	MovePen(textx, texty);
	SetEraseMode(TRUE);
	DrawTextString(text[block_py]);
	SetEraseMode(FALSE);
	MovePen(ox, oy);
}

void drawText() {
	double ox = GetCurrentX(), oy = GetCurrentY();
	MovePen(textx, texty);
	DrawTextString(text[block_py]);
	MovePen(ox, oy);
}

void moveBlock() {
	char t = text[block_py][block_px];
	text[block_py][block_px] = 0;
	MovePen(textx + TextStringWidth(text[block_py]), texty);
	text[block_py][block_px] = t;
}

void Main() /*����ʼ��ִ��һ��*/
{
	InitGraphics();

	registerKeyboardEvent(KeyboardEventProcess);/*ע�������Ϣ�ص�����*/
	registerCharEvent(CharEventProcess);/*ע���ַ���Ϣ�ص�����*/
	registerTimerEvent(TimerEventProcess);/*ע�ᶨʱ����Ϣ�ص�����*/

	SetPenColor("Black");
	MovePen(0.2, GetWindowHeight()*0.95);
	textx = 0.2;
	texty = GetWindowHeight()*0.95;
	startTimer(TIMER_BLINK500, mseconds500);/*500ms��ʱ������*/
	startTimer(TIMER_BLINK1000, mseconds1000);/*1000ms��ʱ������*/
}

void KeyboardEventProcess(int key, int event)/*ÿ������������Ϣ����Ҫִ��*/
{
	double oldradius;

	switch (event) {
	case KEY_DOWN:
		switch (key) {
		case VK_UP:/*UP*/
			if (block_py > 0) {
				if (isBlock) {
					drawBlock(FALSE);
				}
				drawText();
				block_py--;
				if (textlen[block_py] < block_px) {
					block_px = textlen[block_py];
				}
				texty += GetFontHeight();
				if (block_px == 0) {
					MovePen(textx, texty);
				}else
					moveBlock();
			}
			break;
		case VK_DOWN:
			if (block_py < 50) {
				if (isBlock) {
					drawBlock(FALSE);
				}
				drawText();
				block_py++;
				if (textlen[block_py] < block_px) {
					block_px = textlen[block_py];
				}
				texty -= GetFontHeight();
				if (block_px == 0) {
					MovePen(textx, texty);
				}
				else
					moveBlock();
			}
			break;
		case VK_LEFT:
			if (block_px > 0) {
				if (isBlock) {
					drawBlock(FALSE);
				}
				drawText();
				--block_px;
				moveBlock();
			}
			break;
		case VK_RIGHT:
			if (block_px < textlen[block_py]) {
				if (isBlock) {
					drawBlock(FALSE);
				}
				drawText();	
				++block_px;
				moveBlock();
			}
			break;
		}
		break;
	case KEY_UP:
		break;
	}
}

void CharEventProcess(char c)
{
	static char str[2] = { 0, 0 };
	switch (c) {
	case '\r':  /* ע�⣺�س������ﷵ�ص��ַ���'\r'������'\n'*/
		if (isBlock) {
			drawBlock(FALSE);
		}
		drawText();
		textx = 0.2;/*�����ַ�������ʼ����*/
		texty = GetCurrentY() - GetFontHeight();
		MovePen(textx, texty);
		block_py++;
		block_px = 0;
		//int ret = MessageBox(NULL, TEXT(text[block_py]), TEXT("�ն˴���"), MB_OK);
		break;
	case 27: /*ESC*/
		break;
	case '\b':
	case 127:
		if (block_px > 0) {
			if (isBlock) {
				drawBlock(FALSE);
			}
			eraseText();
			for (int p1 = block_px; p1 <= textlen[block_py]; p1++)
				text[block_py][p1 - 1] = text[block_py][p1];
			drawText();
			block_px--;
			moveBlock();
			textlen[block_py]--;
		}
		break;
	default:
		str[0] = c;/*�γɵ�ǰ�ַ����ַ���*/
		/*�������β������ǰ�ַ����뵽�����ַ���������*/
		if (block_px == textlen[block_py]) {
			text[block_py][textlen[block_py]++] = c;
			text[block_py][textlen[block_py]] = '\0';
			if (isBlock) {
				drawBlock(FALSE);
			}
			DrawTextString(str);/*�����ǰ�ַ��������λ����Ӧ����*/
		}
		//���������
		else {
			//������д
			if (isBlock) {
				drawBlock(FALSE);
			}
			eraseText();
			//����
			for (int p1 = textlen[block_py]; p1 >= block_px; --p1)
				text[block_py][p1 + 1] = text[block_py][p1];
			text[block_py][block_px] = c;
			textlen[block_py]++;
			drawText();
		}
		block_px++;
		moveBlock();
		break;
	}
}

void TimerEventProcess(int timerID)
{
	bool erasemode;
	switch (timerID) {
	case TIMER_BLINK500: /*500ms�����˸��ʱ��*/
		if (!isBlink) return;
		drawBlock(!isBlock);
		break;
	case TIMER_BLINK1000: /*1000ms�����˸��ʱ��*/
		if (!isBlink) return;
		//drawBlock(TRUE);
		break;
	default:
		break;
	}
}
