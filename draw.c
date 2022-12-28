#include <curses.h>
#include <math.h>

// "state.h" already included
#include "draw.h"
#include "colors.h"

struct text {
	char *s;
	unsigned len;
};

static struct text _texts[] = {
	// upper section
	{"aces", 4},
	{"twos", 4},
	{"threes", 6},
	{"fours", 5},
	{"fives", 5},
	{"sixes", 5},
	// lower section
	{"three of a kind", 15},
	{"four of a kind", 14},
	{"full house", 10},
	{"small straight", 14},
	{"large straight", 14},
	{"yahtzee", 7},
	{"chance", 6}
};

//#define B "\xe2\x97\x8f"
#define B "\xe2\x96\xaa"
static char *_dice[6][3] = {
	// 1
	{"     ", "  "B"  ", "     "},
	// 2
	{B "    ", "     ", "    " B},
	// 3
	{B "    ", "  "B"  ", "    "B},
	// 4
	{B"   "B, "     ", B"   "B},
	// 5
	{B"   "B, "  "B"  ", B"   "B},
	// 6
	{B"   "B, B"   "B, B"   "B},
};
#undef B

static inline int _space_taken(int number)
{
	if(number<=0)
		return 1;
	return ((int)log10((double)number))+1;
}

int draw_p1(y_state_t *ys)
{
	// 1 block left padding
	// 1 block upper padding

	// draw score
	wmove(stdscr, 1,1);
	printw("Score: %3d", ys->p1_score);

	// 1 row spacing
	int row = 3;
	for(int i=0; i<13; i++) {
		int scored = ys->p1_rules[i] >= 0;
		if(scored) {
			attron(COLOR_PAIR(D_RULE_SCORED));
			mvprintw(row+i, 1, "%s %d", _texts[i].s, ys->p1_rules[i]);
			attroff(COLOR_PAIR(D_RULE_SCORED));
		} else {
			mvprintw(row+i, 1, "%s", _texts[i]);
		}
	}

	return 0;
}

int draw_p2(y_state_t *ys)
{
	// 1 block right padding
	// 1 block upper padding

	int max_x = getmaxx(stdscr);

	// draw score
	mvprintw(1, max_x - 11, "Score: %3d", ys->p2_score);

	// 1 row spacing
	int row = 3;
	for(int i=0; i<13; i++) {
		int scored = ys->p2_rules[i] >= 0;
		if(scored) {
			attron(COLOR_PAIR(D_RULE_SCORED));
			mvprintw(
				row+i,
				max_x - 2 - _space_taken(ys->p2_rules[i]) - _texts[i].len,
				"%d %s",
				ys->p2_rules[i],
				_texts[i].s
			);
			attroff(COLOR_PAIR(D_RULE_SCORED));
		} else {
			mvprintw(row+i, max_x - 1 - _texts[i].len, "%s", _texts[i].s);
		}
	}

	return 0;
}

int draw_p1_live(y_state_t *ys)
{
	return draw_p1_select(ys, -1);
}

int draw_p1_select(y_state_t *ys, int choice)
{
	// draw score
	mvprintw(1, 1, "Score: %3d", ys->p1_score);

	int row = 3;
	for(int i=0; i<13; i++)
	{
		if(ys->p1_rules[i] >= 0)
		{
			// scored
			attron(COLOR_PAIR(D_RULE_SCORED));
			mvprintw(row+i, 1, "%s %d", _texts[i].s, ys->p1_rules[i]);
			attroff(COLOR_PAIR(D_RULE_SCORED));
		}
		else
		{
			if(choice == i)
				attron(COLOR_PAIR(D_RULE_SELECTED));
			mvprintw(row+i, 1, "%s %d", _texts[i].s, ys->live[i]);
			if(choice == i)
				attroff(COLOR_PAIR(D_RULE_SELECTED));
		}
	}

	return 0;
}

int draw_p2_live(y_state_t *ys)
{
	return draw_p2_select(ys, -1);
}

int draw_p2_select(y_state_t *ys, int choice)
{
	int max_x = getmaxx(stdscr);
	mvprintw(1, max_x - 11, "Score: %3d", ys->p2_score);

	int row = 3;
	for(int i=0;i<13;i++)
	{
		if(ys->p2_rules[i]>=0)
		{
			attron(COLOR_PAIR(D_RULE_SCORED));
			mvprintw(
				row+i,
				max_x - 2 - _space_taken(ys->p2_rules[i]) - _texts[i].len,
				"%d %s",
				ys->p2_rules[i],
				_texts[i].s
			);
			attroff(COLOR_PAIR(D_RULE_SCORED));
		}
		else
		{
			if(choice == i)
				attron(COLOR_PAIR(D_RULE_SELECTED));
			mvprintw(
				row+i,
				max_x - 2 - _space_taken(ys->live[i]) - _texts[i].len,
				"%d %s",
				ys->live[i],
				_texts[i].s
			);
			if(choice == i)
				attroff(COLOR_PAIR(D_RULE_SELECTED));
		}
	}

	return 0;
}

int draw_dice(y_state_t *ys)
{
	return draw_dice_selection(ys, -1);
}

static inline int _dice_at(int x, int value, int attr)
{
	int max_y = getmaxy(stdscr);
	value--;
	attron(attr);
	//mvaddstr(max_y-7, x, "       ");
	for(int i=0,row=max_y-6; i<3; i++,row++)
	{
		//mvaddch(row, x, ' ');
		// for(int j=0;j<5;j++)
		// 	addch(_dice[value][i][j] == 'o' ? '#' : ' ');
		//addch(' ');
		mvaddstr(row, x, _dice[value][i]);
	}
	//mvaddstr(max_y-3, x, "       ");
	attroff(attr);
	return 0;
}

int draw_dice_selection(y_state_t *ys, int choice)
{
	// calculate middle
	int first_x = ((getmaxx(stdscr)+1)>>1) -19;

	for(int i=0; i<5; i++)
	{
		int score = ys->dice[i];
		int color = D_DICE_ACTIVE;
		if(score<0)
		{
			score = -score;
			color += 1;
		}
		if(choice == i)
			color += 2;
		_dice_at(first_x + (i<<3), score, COLOR_PAIR(color));
	}

	return 0;
}

int draw_info(char *text)
{
	mvaddstr(getmaxy(stdscr)-1, 1, text);
	return 0;
}

// could've been a simple define/alias
int draw_done()
{
	return refresh();
}

int draw_start()
{
	return clear();
}
