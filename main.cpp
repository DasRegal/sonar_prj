#include "main.h"
#include "gpio.h"
#include "sonar.h"
#include "spi.h"

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

void Init(void)
{
	spi.Init(MISO, MOSI, CLK);
	// InitPWM();
	// InitMavlink();
}

int main(int argc, char const *argv[])
{	
	Init();

	int data;
	while(1)
	{
		data = sonar.ReadSonar(SONAR1);
		printf("Нижний сонар: - %d : ", data);
		data = sonar.ReadSonar(L_SONAR);
		printf("Левый сонар: - %d\n", data);
		data = sonar.ReadSonar(R_SONAR);
		printf("Правый сонар: - %d : ", data);
		data = sonar.ReadSonar(F_SONAR);
		printf("Передний сонар: - %d : ", data);
		data = sonar.ReadSonar(B_SONAR);
		printf("Задний сонар: - %d : ", data);
		for (int i = 0; i < 10; ++i)
		{
			usleep(1000);
		}
	}
	return 0;
}

void Delay(int time)
{
	sleep(time);
}