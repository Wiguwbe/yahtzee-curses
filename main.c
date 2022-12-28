#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <curses.h>
#include <locale.h>

#include "state.h"
#include "draw.h"
#include "colors.h"
#include "sm.h"

static void with_attr(int a, int y)
{
	move(y, 0);
	for(int c=0;c<8;c++) {
		addch('A'|a|COLOR_PAIR(c));
	}
}

static void _init_pairs()
{
	for(int i=0;i<8;i++) {
		init_pair(i, i, 7-i);
	}
}

static void test_colors()
{
	_init_pairs();
	int y = 18;
	with_attr(A_NORMAL, y++);
	with_attr(A_STANDOUT, y++);;
	with_attr(A_UNDERLINE, y++);;
	with_attr(A_REVERSE, y++);;
	with_attr(A_BLINK, y++);;
	with_attr(A_DIM, y++);;
	with_attr(A_BOLD, y++);;
	with_attr(A_PROTECT, y++);;
	with_attr(A_INVIS, y++);;
	with_attr(A_ALTCHARSET, y++);;
	with_attr(A_CHARTEXT, y++);;
}

int main(int argc, char **argv)
{
	y_state_t ys;

	// init ys
	ys.p1_score = ys.p2_score = 0;
	memset(ys.p1_rules, -1, 13);
	memset(ys.p2_rules, -1, 13);
	memset(ys.dice, 0, 5);
	ys.phase = 0;
	ys.choice = -1;
	ys.p2_fd = -1;
	ys.turns_left = 26;

	/*
		handle argv[1] choices

		can be either:
		"sp" -> single player
		"mp" -> multi player (local)
		(future)
		"host" -> host online multiplayer, port on argv[2]
		"connect" -> connect to online multiplayer, ip:port on argv[2]
	*/
	if(argc<2)
	{
		fprintf(stderr, "usage: %s sp|mp\n", *argv);
		return 1;
	}
	if(!strcmp(argv[1], "sp"))
	{
		ys.sp = 1;
		ys.turns_left = 13;
	}
	else if(!strcmp(argv[1], "mp"))
		ys.sp = 0;
	// else if(!strcmp(argv[1], "host"))
	// {
	// 	// TODO implement
	// }
	// else if(!strcmp(argv[1], "connect"))
	// {
	// 	// TODO implement
	// }
	else
	{
		fprintf(stderr, "usage: %s sp|mp\n", *argv);
		return 1;
	}

	/* init ncurses */
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);	// special keys
	start_color();
	/* init our stuff */
	create_colors();
	/* other stuff */
	srand(time(NULL));

	// do the statemachine
	while(ys.turns_left)
	{
		int change = sms[ys.phase](&ys);
		if(change < 0)
		{
			// error
			endwin();
			fprintf(stderr, "an error was encountered");
			return 1;
		}
		ys.phase += change;
		if(ys.sp && ys.phase > PHASE_P1_RULE) {
			ys.phase = PHASE_P1_ROLL1;
		}
		// always, why not
		ys.phase %= PHASE_COUNT;
	}

	/* finalize ncurses */
	endwin();

	return 0;
}
