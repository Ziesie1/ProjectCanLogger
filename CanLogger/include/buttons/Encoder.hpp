#ifndef ENCODER_HPP
#define ENCODER_HPP

void initEncoder();
bool hasEncoderPosChanged();
int getEncoderPos();
bool wasEncoderButtonPressed();
void setEncoderPos(int pos);
#endif