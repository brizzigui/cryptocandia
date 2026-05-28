#ifndef ATTACK_H
#define ATTACK_H

#include "representation.h"

float validate_text(const char *locale, const char *method, char* string);
void brute_force(raw data, float threashold);

#endif