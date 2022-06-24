#include <stdio.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>


#define CHAR_DEVICE_NODE "/dev/demo_char"
#define BUF_SIZE 256


int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    
    in = getchar();
    
    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}

char nonblocking_input(void)
{
    char input_ch;
    struct termios newt, oldt;
    
    int tty = open("/dev/tty", O_RDONLY);    //Open control terminal
    
    tcgetattr(tty, &oldt);            //Get terminal properties
    newt = oldt;
    
    //Set characters are not buffered(~ICANON) and do not echo(~ECHO).
    //You can also choose only one of them.
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr(tty, TCSANOW, &newt);
    
    read(tty, &input_ch, 1);
    tcsetattr(tty, TCSANOW, &oldt);        //Restore terminal properties

    return input_ch;
}

int main(){
    int fd;
    int retval = 0;
    char data_buf[BUF_SIZE] = {0};

    fd = open(CHAR_DEVICE_NODE,O_RDWR);
    if(retval < 0){
        printf("error\n");
        return -1;
    }
    printf("User Control Program Start\n");
    while(1){
        int ascii = nonblocking_input();
        //printf("Press Key:%s",ascii = scanKeyboard());
    	switch(ascii){
            case '1':{ printf("Count_1 Start!\n");      break; }    	
            case '2':{ printf("Count_1 Continue!\n");   break; }    	
            case '3':{ printf("Count_1 Suspend!\n");    break; }    	
            case '4':{ printf("Count_1 Stop!\n");       break; }    	
            
            case '5':{ printf("Count_2 Start!\n");      break; }    	
            case '6':{ printf("Count_2 Continue!\n");   break; }    	
            case '7':{ printf("Count_2 Suspend!\n");    break; }    	
            case '8':{ printf("Count_2 Stop!\n");       break; }    	
            
            case 'a':{ printf("All Start!\n");          break; }    	
            case 's':{ printf("All Continue!\n");       break; }    	
            case 'd':{ printf("All Suspend!\n");        break; }    	
            case 'f':{ printf("All Stop!\n");           break; }    	
    	}
    	retval = ioctl(fd,ascii,0);
    	if(retval != 0){
    	    printf("Ioctl Error!\n");
    	}
    	if(ascii=='q') break;
    }
    printf("User Exit\n");
    return 0;
}

