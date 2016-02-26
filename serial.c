/* serial.c
	
   Sends & Receives via the serial port (/dev/ttyS0)

*/

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<netinet/in.h>

void my_isr(int n)
{
	printf("One client is disconnected\n");	
	wait(0);
}

//user define functions prototypes 
//which invoke linux/unix API's(system calls)
int initport(int fd);
int writeport(int fd, char *buff);
int readport(int fd, char *result);
int getbaud(int fd);

int fd;

int initport(int fd) 
{
	struct termios options;

	// Get the current options for the port...
	tcgetattr(fd, &options);

	// Set the baud rates to 9600...
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);

	// Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Set the new options for the port...
	tcsetattr(fd, TCSANOW, &options);
	return 1;
}

int writeport(int fd, char *buff) 
{
	int len = strlen(buff);
	buff[len] = 0x0d; // stick a <CR> after the command
	buff[len+1] = 0x00; // terminate the string properly
	int n = write(fd, buff, strlen(buff));
	if (n < 0) 
        {
		fputs("write failed!\n", stderr);
		return 0;
	}
	return 1;
}

int readport(int fd, char *result) 
{
	int iIn = read(fd, result, 512);
	result[iIn-1] = 0x00;
	if (iIn < 0) 
        {
		if (errno == EAGAIN) 
                {
		  printf("SERIAL EAGAIN ERROR\n");
		  return 0;
		} 
                else 
                {
		  printf("SERIAL read error %d %s\n", errno, strerror(errno));
		  return 0;
		}
	}                    
	return 1;
}

int getbaud(int fd) 
{
	struct termios termAttr;
	int inputSpeed = -1;
	speed_t baudRate;

	tcgetattr(fd, &termAttr);

	/* Get the input speed.                              */
	baudRate = cfgetispeed(&termAttr);
	switch (baudRate) 
        {
		case B0:      inputSpeed = 0; break;
		case B50:     inputSpeed = 50; break;
		case B110:    inputSpeed = 110; break;
		case B134:    inputSpeed = 134; break;
		case B150:    inputSpeed = 150; break;
		case B200:    inputSpeed = 200; break;
		case B300:    inputSpeed = 300; break;
		case B600:    inputSpeed = 600; break;
		case B1200:   inputSpeed = 1200; break;
		case B1800:   inputSpeed = 1800; break;
		case B2400:   inputSpeed = 2400; break;
		case B4800:   inputSpeed = 4800; break;
		case B9600:   inputSpeed = 9600; break;
		case B19200:  inputSpeed = 19200; break;
		case B38400:  inputSpeed = 38400; break;
	}
	return inputSpeed;
}

int main(int argc, char *argv[]) 
{

	int sfd,len,nsfd;
	struct sockaddr_in v,v1;

	if(argc!=2)
	{
		printf("Usage : ./server port_number\n");
		return;
	}

	signal(SIGCHLD,my_isr);

	if((sfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		return;
	}
	perror("socket");

///////////////////////////////////////////////////////////


	v.sin_family = AF_INET;
	v.sin_port = htons(atoi(argv[1]));
	v.sin_addr.s_addr = inet_addr("0.0.0.0");

	len = sizeof(v);

	if(bind(sfd,(struct sockaddr *) &v,len)<0)
	{
		perror("bind");
		return;
	}
	perror("bind");

////////////////////////////////////////////////////////////


	listen(sfd,10);
	perror("listen");

///////////////////////////////////////////////////////////

       	fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1) 
         {
	  perror("open_port: Unable to open /dev/ttyS0 - ");
	  return 1;
         } 
        else 
        {
	  fcntl(fd, F_SETFL, 0);
        }
	
	printf("baud=%d\n", getbaud(fd));
	initport(fd);
	printf("baud=%d\n", getbaud(fd));
/*
	char wbuff[20];
	
        gets(wbuff);
	if (!writeport(fd, wbuff)) 
               {
		printf("write failed\n");
		close(fd);
		return 1;
		}

	printf("written:%s\n", wbuff);
	
	usleep(500000);
*/
	char rbuff[150];
//	fcntl(fd, F_SETFL, FNDELAY); // don't block serial read
        fcntl(fd, F_SETFL,0); // block serial read

//////////////////////////////////////////////////////////

	while(1)
	{

		if((nsfd = accept(sfd,(struct sockaddr *)&v1,&len))<0)
		{
			perror("accept");
			return ;
		}
		perror("accept");

		if(nsfd>=0)
		{
			if(!fork()){
				while(1){
					if (!readport(fd,rbuff)) 
					{
						printf("read failed\n");
						close(fd);
						return 1;
					}
						system("clear");
						printf("%s\n", rbuff);
						write(nsfd,rbuff,32);
						usleep(800);
					}
				}
		}
	}
	close(fd);
	return 0;

}
