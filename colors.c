
#include <curses.h>

#include "colors.h"

#define D_COLOR_DGRAY 32
#define D_COLOR_LGRAY 33

int create_colors()
{
	// init some colors first
	init_color(D_COLOR_DGRAY, 350, 350, 350);
	init_color(D_COLOR_LGRAY, 700, 700, 700);
	// rule scored
	//init_pair(D_RULE_SCORED, COLOR_WHITE , D_COLOR_DGRAY);
	init_pair(D_RULE_SCORED, D_COLOR_DGRAY, COLOR_BLACK);
	// rule selected
	init_pair(D_RULE_SELECTED, COLOR_BLACK, COLOR_CYAN);
	// dice active
	init_pair(D_DICE_ACTIVE, COLOR_BLACK, D_COLOR_LGRAY);
	// dice inactive
	init_pair(D_DICE_INACTIVE, COLOR_BLACK, D_COLOR_DGRAY);
	// dice selected
	init_pair(D_DICE_SELECTED, COLOR_CYAN, D_COLOR_LGRAY);
	// dice selected inactive
	init_pair(D_DICE_SELECTED_INACTIVE, COLOR_CYAN, D_COLOR_DGRAY);
	return 1;
}
