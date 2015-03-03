#ifndef SECURITYGUARD_H
#define	SECURITYGUARD_H

#include "AI.H"

#pragma once

class SecurityGuard	:	public AI
{
public:
	SecurityGuard(void);
	~SecurityGuard(void);

	void spawnAI(void);
	void updateAI(void);
	void interactWith(void);
};

enum GuardState
{
	Sitting,
	Standing,
};
#endif