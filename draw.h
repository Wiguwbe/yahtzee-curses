/*
	functions to draw on screen
*/
#ifndef _YAHTZEE_DRAW_H_
#define _YAHTZEE_DRAW_H_

#include "state.h"


/*
	draw p1 scores (static)
*/
int draw_p1(y_state_t *ys);

/*
	draw p2 scores (static)
*/
int draw_p2(y_state_t *ys);

/*
	draw p1 scores, with live score

	`live` should have 13 values
*/
int draw_p1_live(y_state_t *ys);

/*
	draw p2 scores, with live score
*/
int draw_p2_live(y_state_t *ys);

/*
	draw p1 scores, with selection
*/
int draw_p1_select(y_state_t *ys, int choice);

/*
	draw p2 scores, with selection
*/
int draw_p2_select(y_state_t *ys, int choice);


/*
	draw dice

	`dice` should be an array of 5 integers,
	with negative value if to be disabled
*/
int draw_dice(y_state_t *ys);

/*
	draw dice, with selection

	`dice` should be the same as above
*/
int draw_dice_selection(y_state_t *ys, int selection);

/*
	write info text on bottom row
*/
int draw_info(char *text);

/*
	finish drawing, show screen
*/
int draw_done(void);

/*
	clear screen to start draw
*/
int draw_start(void);

#endif
