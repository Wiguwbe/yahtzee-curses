/*
	state definition for game
*/

#ifndef _YAHTZEE_STATE_H_
#define _YAHTZEE_STATE_H_

#include <stdint.h>

#define PHASE_P1_ROLL1	0
#define PHASE_P1_KEEP1	1
#define PHASE_P1_ROLL2	2
#define PHASE_P1_KEEP2	3
#define PHASE_P1_ROLL3	4
#define PHASE_P1_RULE	5
#define PHASE_P2_ROLL1	6
#define PHASE_P2_KEEP1	7
#define PHASE_P2_ROLL2	8
#define PHASE_P2_KEEP2	9
#define PHASE_P2_ROLL3	10
#define PHASE_P2_RULE	11
#define PHASE_COUNT	12

struct y_state {
	// in case of remote connection, write choices to socket
	int p2_fd;
	unsigned p1_score;
	unsigned p2_score;
	// scores for already scored rules
	// -1 if not scored yet
	int8_t p1_rules[13];
	int8_t p2_rules[13];
	int8_t live[13];
	int8_t dice[5];
	int8_t phase;
	int8_t choice;
	int8_t sp;
	int8_t turns_left;
};
typedef struct y_state y_state_t;

#endif
