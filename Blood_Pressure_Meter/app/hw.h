/*
 * hw.h
 *
 *  Created on: Oct 26, 2024
 *      Author: stephan
 */

#ifndef HW_H_
#define HW_H_

float analogRead(void);
bool buttonState(void);
void rotateMotor(void);
void rotateMotor(void);
void stopMotor(void);
void valveActivate(void);
void valveDeactivate(void);
float computePressure(float pressureVoltage);


#endif /* HW_H_ */
