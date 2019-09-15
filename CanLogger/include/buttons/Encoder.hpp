#ifndef ENCODER_HPP
#define ENCODER_HPP

void initEncoder();
bool hasEncoderPosChanged();
int getRelativeEncoderPos();
bool wasEncoderButtonPressed();
void resetEncoderStates();

#endif