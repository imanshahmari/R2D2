#include <stdio.h>
#include <string.h>

char serialPortFilename[] = "/dev/ttyACM0";

int main()
{
	FILE *serPort = fopen(serialPortFilename, "w");

	if (serPort == NULL)
	{
		printf("ERROR");	
		return 0;
	}

	char writeBuffer[] = {'s'};

	fwrite(writeBuffer, sizeof(char), sizeof(writeBuffer), serPort);
	fclose(serPort);
	return 0;

}