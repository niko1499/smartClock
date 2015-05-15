//includes the needed libraries, note that compile command:
// gcc -o betaTest betaTest.c -lwiringPi -std=c99

#include <stdio.h>
#include <wiringPi.h>
#include <sr595.h>
#include <time.h>

//declares methods
void initializationTest();  //displays documentation and cycles REGs
void write595();            //preforms the digital write to the 595s
void getTime(); 	    //Gets the time	
void clear595();	    //clears the 595s

//declares variales needed in multiple mehtods
int input[4] = {4321};  //string to be displayed
int AM ={1};	        //AM or PM
int h ={88};            //the hour which is used in write595

//the main method
int main (void)
{
	printf ("\nSmartClock: betaTest\n""\n --Nikolas Gamarra (NXG) 2015 \n""\n");

	wiringPiSetup() ;

	//wiringPi pins(Base pin, # of registries, data, clock, latch)
	sr595Setup (100, 32, 0, 1, 2) ;

	initializationTest();

	//makes program start at 0 seconds
	int s = 1;
	printf("\nWaiting until seconds = 0\n");

	do
	{
		time_t epoch_time;
		struct tm *tm_p;
		epoch_time = time( NULL );
		tm_p = localtime( &epoch_time );

		s = tm_p->tm_sec;
		printf("\rseconds = %d", s);
		fflush(stdout);
	}while(s != 0);

	for(;;)
	{
		getTime();
		for(int x=0;x <4; ++x)
		{
			writeDigit(x,input[x]);
		}
		printf("\nEvent: Display update complete! The clock will check the time in 60.006 seconds.\n");
		delay(60006);
	}

	return 0;
}

void initializationTest()
{
	//displays silly text
	printf ("\nEvent: initializationTest has been called\n");
	printf("\nDocumentation\n");
	printf("\n           TPIC6B595                         7-SEG\n");
	printf("       	     (Top)                        ----A----\n");
	printf("          |---------|                    |         |\n");
	printf("       NC |1  ||  20| NC                 F         B\n");
	printf("      Vcc |2      19| GND                |         |\n");
	printf("   SER IN |3      18| SER OUT             --- G --- \n");
	printf("       D0 |4      17| D7                 |         |\n");
	printf("       D1 |5      16| D6                 E         C\n");
	printf("       D2 |6      15| D5                 |         |\n");
	printf("       D3 |7      14| D4                  ----D----   DP\n");
	printf("   *SRCLR |8      13| SRCK\n");
	printf("       *G |9      12| RCK\n");
	printf("      GND |10     11| GND\n");
	printf("          |---------|\n");

	printf("\nUsing the TPIC6B595:\n");
	printf("Connect Vcc to 5V\n");
	printf("Connect the GND pins to the common GND\n");
	printf("Connect Pin 3 to Data in pin on MCU\n");
	printf("Connect Pin 13 to the Clock pin on the MCU\n");
	printf("Connect Pin 12 to the Latch pin on the MCU\n");
	printf("Connect Pin 18 to Pin 3 on the next REG\n");
	printf("Tie Pin 8 to 5V\n");
	printf("Tie Pin 9 to GND\n");

	printf("\nPhysical connections:\n");
	printf("The sClockProgram uses the GPIO pins [0, 1, 2] (Physical pins [11, 12, 13]) for Data, Clock, and Latch respectively. For the parallel connection, the green four connects to the first four registries with the black side connection to registry 1. The red four connections go to the next four registries with the black side on registry 4.\n");
	delay(1500);
	printf("\nBegining Reg Test\n");

	clear595();
	digitalWrite (100, 1);
	printf ("Testing Reg:0");
	delay (1500);	

	//cycles the REGs
	for(int i = 1; i < 31; ++i)
	{
		printf ("\rTesting Reg:%d",i);
		digitalWrite (100 + i, 1);
		digitalWrite (99 + i, 0);
		delay (100);
		fflush(stdout);
	}

	clear595();
	printf ("\n");
}

void getTime()
{
	printf ("\nEvent: getTime has been called\n");

	time_t epoch_time;
	struct tm *tm_p;
	epoch_time = time( NULL );
	tm_p = localtime( &epoch_time );
	printf("The time is %.2d:%.2d:%.2d\n",tm_p->tm_hour, tm_p->tm_min, tm_p->tm_sec );

	int hh = tm_p->tm_hour;
	int m = tm_p->tm_min;
	h = hh;

	//Changes to 12hr time
	if (h >= 12)
	{
		h = hh-12;
		AM = 0;
	}
	if (h==0)
	{
		h =12;
		AM = 1;
	}

	//clears the input to 0
	for( int i=0; i<4; ++i)
	{
		input[i]=0;
	}

	//places the appropriate time numbers to the cells of input
	if (h < 10)
	{
		input[1]=h;
	}
	else
	{
		int H=(h%10);
		input[1]=H;
		H=((h/10)%10);
		input[0]=H;

	}

	if (m < 10)
	{	
		input[3]=m;
	}
	else
	{
		int M=(m%10);
		input[3]=M;
		M=((m/10)%10);
		input[2]=M;
	}

	printf("The input is");
	for (int i=0; i<4; ++i)
	{
		printf(" %d",input[i]);
	}

	printf("\n");
}

void writeSegment(int x)
{
	digitalWrite(x,1);
}

void writeDigit(int dig, int num)
{

	int shiftSize = dig * 8;

	switch(num)
	{
		case 0:
		/
	if(digi==0)
			{
                        digitalWrite(100 + shiftSize,0);
                        digitalWrite(101 + shiftSize,0);
                        digitalWrite(102 + shiftSize,0);
                        digitalWrite(103 + shiftSize,0);
                        digitalWrite(104 + shiftSize,0);
                        digitalWrite(105 + shiftSize,0);
                        digitalWrite(106 + shiftSize,0);

			}
			if(dig !=0)
			{
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,1);
			digitalWrite(104 + shiftSize,1);
			digitalWrite(105 + shiftSize,1);
			digitalWrite(106 + shiftSize,0);
			}
			break;

		case 1:
			digitalWrite(100 + shiftSize,0);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,0);
			digitalWrite(104 + shiftSize,0);
			digitalWrite(105 + shiftSize,0);
			digitalWrite(106 + shiftSize,0);
			break;

		case 2:
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,0);
			digitalWrite(103 + shiftSize,1);
			digitalWrite(104 + shiftSize,1);
			digitalWrite(105 + shiftSize,0);
			digitalWrite(106 + shiftSize,1);
			break;

		case 3:
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,1);
			digitalWrite(104 + shiftSize,0);
			digitalWrite(105 + shiftSize,0);
			digitalWrite(106 + shiftSize,1);
			break;

		case 4:
			digitalWrite(100 + shiftSize,0);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,0);
			digitalWrite(104 + shiftSize,0);
			digitalWrite(105 + shiftSize,1);
			digitalWrite(106 + shiftSize,1);
			break;

		case 5:
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,0);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,1);
			digitalWrite(104 + shiftSize,0);
			digitalWrite(105 + shiftSize,1);
			digitalWrite(106 + shiftSize,1);
			break;

		case 6:
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,0);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,1);
			digitalWrite(104 + shiftSize,1);
			digitalWrite(105 + shiftSize,1);
			digitalWrite(106 + shiftSize,1);
			break;

		case 7:
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,0);
			digitalWrite(104 + shiftSize,0);
			digitalWrite(105 + shiftSize,0);
			digitalWrite(106 + shiftSize,0);
			break;

		case 8:
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,1);
			digitalWrite(104 + shiftSize,1);
			digitalWrite(105 + shiftSize,1);
			digitalWrite(106 + shiftSize,1);
			break;

		case 9:
			digitalWrite(100 + shiftSize,1);
			digitalWrite(101 + shiftSize,1);
			digitalWrite(102 + shiftSize,1);
			digitalWrite(103 + shiftSize,0);
			digitalWrite(104 + shiftSize,0);
			digitalWrite(105 + shiftSize,1);
			digitalWrite(106 + shiftSize,1);
			break;

		default:
			printf("This should not happen\n");
			break;
	}
}

void write595()
{
	printf ("\nEvent: write595 has been called\n");

	int dig = 0;          //the current digit
	int shiftSize = 0;    //shifts to the next reg for the next dig
	int toWrite = 0;      //the value of the digit	

	clear595();

	//adds :
	//digitalWrite(107,1);
	//digitalWrite(115,1);

	//skips dig0 when dig0=0
	if(h < 10)
	{
		dig = 1;	
	}

	for(dig; dig < 4; ++dig)
	{
		printf("Preforming operations on dig %d\n",dig);

		//sets size of shift for the specific dig
		switch(dig)
		{
			case 0:
				shiftSize = 0;
				break;

			case 1:
				shiftSize = 8;
				break;

			case 2:
				shiftSize = 16;
				break;

			case 3:
				shiftSize = 24;
				break;

			default:
				printf("This should not happen\n");
				break;

		}
		shiftSize = dig * 8;
		//sets the value to be written
		toWrite = input[dig];

		//writes to the 595, note that 100-106 are A-G respectively
		switch(toWrite)
		{
			case 0:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,1);
				digitalWrite(104 + shiftSize,1);
				digitalWrite(105 + shiftSize,1);
				digitalWrite(106 + shiftSize,0);


				break;

			case 1:
				digitalWrite(100 + shiftSize,0);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,0);
				digitalWrite(104 + shiftSize,0);
				digitalWrite(105 + shiftSize,0);
				digitalWrite(106 + shiftSize,0);
				break;

			case 2:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,0);
				digitalWrite(103 + shiftSize,1);
				digitalWrite(104 + shiftSize,1);
				digitalWrite(105 + shiftSize,0);
				digitalWrite(106 + shiftSize,1);
				break;

			case 3:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,1);
				digitalWrite(104 + shiftSize,0);
				digitalWrite(105 + shiftSize,0);
				digitalWrite(106 + shiftSize,1);
				break;

			case 4:
				digitalWrite(100 + shiftSize,0);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,0);
				digitalWrite(104 + shiftSize,0);
				digitalWrite(105 + shiftSize,1);
				digitalWrite(106 + shiftSize,1);
				break;

			case 5:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,0);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,1);
				digitalWrite(104 + shiftSize,0);
				digitalWrite(105 + shiftSize,1);
				digitalWrite(106 + shiftSize,1);
				break;

			case 6:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,0);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,1);
				digitalWrite(104 + shiftSize,1);
				digitalWrite(105 + shiftSize,1);
				digitalWrite(106 + shiftSize,1);
				break;

			case 7:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,0);
				digitalWrite(104 + shiftSize,0);
				digitalWrite(105 + shiftSize,0);
				digitalWrite(106 + shiftSize,0);
				break;

			case 8:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,1);
				digitalWrite(104 + shiftSize,1);
				digitalWrite(105 + shiftSize,1);
				digitalWrite(106 + shiftSize,1);
				break;

			case 9:
				digitalWrite(100 + shiftSize,1);
				digitalWrite(101 + shiftSize,1);
				digitalWrite(102 + shiftSize,1);
				digitalWrite(103 + shiftSize,0);
				digitalWrite(104 + shiftSize,0);
				digitalWrite(105 + shiftSize,1);
				digitalWrite(106 + shiftSize,1);
				break;

			default:
				printf("This should not happen\n");
				break;
		}
	}
}

void clear595()
{
	for(int i = 0; i < 31; ++i)
	{
		//     digitalWrite (100 + i, 0);
	}

}
