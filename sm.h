/*
	state machine for game states
*/
#ifndef _YAHTZEE_SM_H_
#define _YAHTZEE_SM_H_

#include "state.h"

int sm_p1_roll(y_state_t *);
int sm_p1_keep(y_state_t *);
int sm_p1_rule(y_state_t *);
int sm_p2_roll(y_state_t *);
int sm_p2_keep(y_state_t *);
int sm_p2_rule(y_state_t *);

static int (*sms[13])(y_state_t*) = {
	sm_p1_roll,
	sm_p1_keep,
	sm_p1_roll,
	sm_p1_keep,
	sm_p1_roll,
	sm_p1_rule,
	sm_p2_roll,
	sm_p2_keep,
	sm_p2_roll,
	sm_p2_keep,
	sm_p2_roll,
	sm_p2_rule
};

#endif
