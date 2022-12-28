#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curses.h>

#include "sm.h"
#include "draw.h"

static int _live_score(y_state_t *ys)
{
	int sum_dice = 0;
	memset(ys->live, 0, 13);
	// upper section
	for(int i=0;i<5;i++)
	{
		int8_t score = ys->dice[i];
		if(score<0) score = -score;
		ys->live[score-1]+=score;
		sum_dice += score;
	}
	// lower section
	// three/four of a kind
	for(int i=0;i<6;i++)
	{
		int num = i+1;
		if(ys->live[i] == (num*3))
			// three of a kind
			ys->live[6] = sum_dice;
		if(ys->live[i] == (num<<2))
			// four of a kind
			ys->live[7] = sum_dice;
	}
	// full house
	for(int a=0;a<5;a++)
	{
		int num_a = a+1;
		for(int b=a+1;b<6;b++)
		{
			int num_b = b+1;
			// 2 A 3 B
			if(ys->live[a] == (num_a<<1) && ys->live[b] == (num_b*3))
			{
				ys->live[8] = 25;
				goto _fh_end;
			}
			// 3 A 2 B
			if(ys->live[a] == (num_a*3) && ys->live[b] == (num_b<<1))
			{
				ys->live[8] = 25;
				goto _fh_end;
			}
		}
	}
_fh_end:
	// small straight (1-4, 2-5, 3-6)
	for(int a=0;a<3;a++)
	{
		int b, end = a+4;
		for(b=a; b<end; b++)
		{
			if(!ys->live[b])
				// not there
				break;
		}
		if(b==end)
		{
			// full sequence without break
			ys->live[9] = 30;
			goto _ss_end;
		}
	}
_ss_end:
	// large straight (1-5, 2-6)
	for(int a=0; a<2;a++)
	{
		int b, end = a+5;
		for(b=a;b<end;b++)
		{
			if(!ys->live[b])
				// not there
				break;
		}
		if(b == end)
		{
			// full seq without break
			ys->live[10] == 40;
			goto _ls_end;
		}
	}
_ls_end:
	// yahtzee
	for(int i=0;i<6;i++)
	{
		int num = i+1;
		if(ys->live[i] == (num*5))
		{
			// 5 there
			ys->live[11] = 50;
			break;
		}
	}
	// change
	ys->live[12] = sum_dice;

	return 0;
}

/*
	Player 1 stuff
*/

int sm_p1_roll(y_state_t *ys)
{
	// simply roll the dice and present them
	for(int i=0;i<5;i++)
	{
		if(ys->dice[i] >= 0)
			ys->dice[i] = (random() % 6) + 1;
	}
	_live_score(ys);
	draw_start();
	draw_p1_live(ys);
	if(!ys->sp)
		draw_p2(ys);
	draw_dice(ys);
	draw_info("dice rolled, press any key to continue");
	draw_done();

	if(getch() == ERR)
		return -1;

	if(ys->p2_fd!=-1)
	{
		// TODO implement
	}

	// reset for keep/rule
	ys->choice = -1;

	return 1;
}

int sm_p1_keep(y_state_t *ys)
{
	// select which dice to keep
	draw_start();
	draw_info("keep dice: [arrows] move, [space] select, [enter/r] roll, [s] skip");
	draw_p1_live(ys);
	if(!ys->sp)
		draw_p2(ys);
	if(ys->choice < 0)
		ys->choice = 0;
	draw_dice_selection(ys, ys->choice);
	draw_done();

	int response = -1;
	while(response<0)
	{
		int ch = getch();
		response = 0;
		switch(ch)
		{
		case ERR:
			return -1;
		case KEY_LEFT:
			if(ys->choice > 0) ys->choice--;
			response = 0;
			break;
		case KEY_RIGHT:
			if(ys->choice < 4) ys->choice++;
			response = 0;
			break;
		case ' ':
			ys->dice[ys->choice] = -ys->dice[ys->choice];
			response = 0;
			break;
		case 's':
			// skip rool
			// response = 2; break;
			response ++;
		case 'r':
		case '\n':
			// response = 1;
			response ++;
			// all good
			break;
		default:
			response = -1;
		}
		if(response != -1)
		{
			// cool
			if(ys->p2_fd != -1)
			{
				// TODO implement
			}
			return response;
		}
	}

	// shouldn't reach here
	return -1;
}

int sm_p1_rule(y_state_t *ys)
{
	draw_start();
	draw_info("select rule: [arrows] move, [enter/space] select");
	// select first available one
	if(ys->choice < 0)
	{
		for(int i=0;i<13;i++)
		{
			if(ys->p1_rules[i]<0)
			{
				ys->choice = i;
				break;
			}
			// there should always be one available
		}
	}

	draw_p1_select(ys, ys->choice);
	if(!ys->sp)
		draw_p2(ys);
	draw_dice(ys);
	draw_done();

	int response = -1;
	while(response < 0)
	{
		int ch = getch();
		response = 0;
		switch(ch)
		{
		case ERR:
			return -1;
		case KEY_UP:
			// find available one
			for(int i=ys->choice-1; i>=0; i--)
			{
				if(ys->p1_rules[i]<0)
				{
					ys->choice = i;
					break;
				}
			}
			response = 0;
			break;
		case KEY_DOWN:
			// find available one
			for(int i=ys->choice+1; i<13; i++)
			{
				if(ys->p1_rules[i]<0)
				{
					ys->choice = i;
					break;
				}
			}
			response = 0;
			break;
		case KEY_ENTER:
		case ' ':
		case '\n':
			// select
			ys->p1_rules[ys->choice] = ys->live[ys->choice];
			// set new score
			ys->p1_score += ys->p1_rules[ys->choice];
			// clear dice
			memset(ys->dice, 0, 5);
			// update turns
			ys->turns_left--;
			// clear choice
			ys->choice = -1;
			response = 1;
			break;
		default:
			response = -1;
		}

		if(response != -1)
		{
			if(ys->p2_fd != -1)
			{
				// TODO implement
			}
			return response;
		}
	}

	// shouldn't reach here
	return -1;
}

/*
	Player 2 stuff
*/

int sm_p2_roll(y_state_t *ys)
{
	draw_start();
	// simply roll the dice and present them
	int remote = ys->p2_fd != 1;
	if(remote)
	{
		// TODO implement
	}
	else
	{
		for(int i=0;i<5;i++)
		{
			if(ys->dice[i] >= 0)
				ys->dice[i] = (random() % 6) + 1;
		}
	}
	_live_score(ys);
	draw_p2_live(ys);
	draw_p1(ys);
	draw_dice(ys);
	if(remote)
		draw_info("waiting for remote player");
	else
		draw_info("dice rolled, press any key to continue");
	draw_done();

	if(remote)
	{
		// TODO implement
	}
	else
	{
		if(getch() == ERR)
			return -1;
	}
	ys->choice = -1;

	return 1;
}

int sm_p2_keep(y_state_t *ys)
{
	draw_start();
	int remote = ys->p2_fd != -1;
	// select which dice to keep
	if(remote)
		draw_info("remote player choosing");
	else
		draw_info("keep dice: [arrows] move, [space] select, [enter/r] roll, [s] skip");
	if(ys->choice < 0)
		ys->choice = 0;
	draw_p2_live(ys);
	draw_p1(ys);
	draw_dice_selection(ys, ys->choice);
	draw_done();

	while(1)
	{
		int ch;
		if(remote)
		{
			// TODO implement
		}
		else
			ch = getch();

		switch(ch)
		{
		case ERR:
			return -1;
		case KEY_LEFT:
			if(ys->choice > 0) ys->choice--;
			return 0;
		case KEY_RIGHT:
			if(ys->choice < 4) ys->choice++;
			return 0;
		case ' ':
			ys->dice[ys->choice] = -ys->dice[ys->choice];
			return 0;
		case 's':
			// skip rool
			return 2;
		case 'r':
		case '\n':
			// all good
			return 1;
		}
	}

	// shouldn't reach here
	return -1;
}

int sm_p2_rule(y_state_t *ys)
{
	draw_start();
	int remote = ys->p2_fd != -1;
	if(remote)
		draw_info("remote player choosing rule");
	else
		draw_info("select rule: [arrows] move, [enter/space] select");
	// select first available one
	if(ys->choice < 0)
	{
		for(int i=0;i<13;i++)
		{
			if(ys->p2_rules[i]<0)
			{
				ys->choice = i;
				break;
			}
			// there should always be one available
		}
	}

	draw_p2_select(ys, ys->choice);
	draw_p1(ys);
	draw_dice(ys);
	draw_done();

	while(1)
	{
		int ch;
		if(remote)
		{
			// TODO implement
		}
		else
			ch = getch();
		switch(ch)
		{
		case ERR:
			return -1;
		case KEY_UP:
			// find available one
			for(int i=ys->choice-1; i>=0; i--)
			{
				if(ys->p2_rules[i]<0)
				{
					ys->choice = i;
					break;
				}
			}
			return 0;
		case KEY_DOWN:
			// find available one
			for(int i=ys->choice+1; i<13; i++)
			{
				if(ys->p2_rules[i]<0)
				{
					ys->choice = i;
					break;
				}
			}
			return 0;
		case KEY_ENTER:
		case ' ':
		case '\n':
			// select
			ys->p2_rules[ys->choice] = ys->live[ys->choice];
			// set new score
			ys->p2_score += ys->p2_rules[ys->choice];
			// clear dice
			memset(ys->dice, 0, 5);
			// update turns
			ys->turns_left--;
			// clear choice
			ys->choice = -1;
			return 1;
		}
	}

	// shouldn't reach here
	return -1;
}
