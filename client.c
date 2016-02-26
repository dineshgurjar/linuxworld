// To make a client program 

#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
int op;
void my_isr(int n)
{
	printf("Enter Your Choice :\n1] Read Voltage\n2] Read LDR\n3] Read Time\n9] Exit\n");
	scanf("%d",&op);
}

main(int argc,char *argv[])
{

	if(argc!=3)
	{
		printf("Usage : ./client port_number ip_address\n");
		return;
	}

	signal(SIGINT,my_isr);

	int sfd,len,i=0,j;
	char buff[96]={},vbuff[8],ldrbuff[8],tbuff[16];
	struct sockaddr_in v;

////////////////////////////////////////////////

	if((sfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{	
		perror("socket");
		return;
	}	
	perror("socket");

/////////////////////////////////////////////////

	v.sin_family = AF_INET;
	v.sin_port = htons(atoi(argv[1]));
	v.sin_addr.s_addr = inet_addr(argv[2]);

len = sizeof(v);

	if(connect(sfd,(struct sockaddr *) &v,len)<0)
	{
		perror("connect");
		return;
	}
	perror("connect");

	printf("You are connected to server!\n");
/////////////////////////////////////////////////

/* Main Coading */
	printf("Enter Your Choice :\n1] Read Voltage\n2] Read LDR\n3] Read Time\n9] Exit\n");
	scanf("%d",&op);
while(1){
	switch(op)
	{
		case 1: 
			while(1){
				if(op!=1) break;
				// Reading Value from server
				read(sfd,buff,sizeof(buff));

				/* Seprate out voltage from buff */
				for(j=2;j<7;j++)
					vbuff[i++]=buff[j];		
				vbuff[i]=0;
				i=0;

				system("clear");
				printf("Voltage : %s\n",vbuff);
				usleep(500);
				}
		case 2: 
			while(1){
				if(op!=2) break;
				// Reading Value from server
				read(sfd,buff,sizeof(buff));
					  /* Seprate out LDR value from buff */
					  for(j=9;j<14;j++)
						  ldrbuff[i++]=buff[j];
					  ldrbuff[i]=0;
					  i=0;
				system("clear");
				printf("LDR Value : %s\n",ldrbuff);
				usleep(500);
				}
		case 3: 
			while(1){
				if(op!=3) break;
				// Reading Value from server
				read(sfd,buff,sizeof(buff));
					  /* Seprate out time from buff */
					  for(j=19;buff[j];j++)
						  tbuff[i++]=buff[j];
					  tbuff[i]=0;
					  i=0;
				system("clear");
				printf("Time : %s\n",tbuff);
				usleep(500);
				}
		case 9: return;
		default : printf("Enter Valid Option\n"); break;
	}
}
}

