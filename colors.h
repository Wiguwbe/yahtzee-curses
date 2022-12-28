/*
	to define colors
*/

#ifndef _YAHTZEE_COLORS_H_
#define _YAHTZEE_COLORS_H_

int create_colors(void);

#define D_RULE_SCORED 1
#define D_RULE_SELECTED 2
// start with ACTIVE
// add 1 if inactive
// add 2 if selected
#define D_DICE_ACTIVE 3
#define D_DICE_INACTIVE 4
#define D_DICE_SELECTED 5
#define D_DICE_SELECTED_INACTIVE 6


#endif
