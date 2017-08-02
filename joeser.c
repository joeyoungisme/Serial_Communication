#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<sys/stat.h>
#include<fcntl.h>

#include<termios.h>

#include<sys/select.h>

#define SERIAL_NAME         "/dev/ttyUSB0"
#define BAUDRATE            B115200

int main(int argc, char *argv[])
{

    int fd = open(SERIAL_NAME, O_RDWR);

    if(fd <= 0) {
        perror(SERIAL_NAME);
        exit(-1);
    }

    printf("Serial Open Done.\n");

    struct termios oldconf, newconf;
    
    // Save ogregin setting 
    if(tcgetattr(fd, &oldconf)) {
        perror("tcgetattr");
        goto exit;
    }

    memset(&newconf, 0, sizeof(struct termios));

    // Serial Setting Section , Copy from example code

    /* 
     *   BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
     *   CRTSCTS : output hardware flow control (only used if the cable has
     *             all necessary lines. See sect. 7 of Serial-HOWTO)
     *   CS8     : 8n1 (8bit,no parity,1 stopbit)
     *   CLOCAL  : local connection, no modem contol
     *   CREAD   : enable receiving characters
     *          */

    newconf.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    
     
    /*
     *   IGNPAR  : ignore bytes with parity errors
     *   ICRNL   : map CR to NL (otherwise a CR input on the other computer
     *             will not terminate input)
     *   otherwise make device raw (no other input processing)
     *           */
    newconf.c_iflag = IGNPAR | ICRNL;
     
    /*
     *  Raw output.
     *  */
    newconf.c_oflag = 0;
     
    /*
     *   ICANON  : enable canonical input
     *   disable all echo functionality, and don't send signals to calling program
     *     */
    newconf.c_lflag = ICANON;
     
    /* 
     *   initialize all control characters 
     *   default values can be found in /usr/include/termios.h, and are given
     *       in the comments, but we don't need them here
     *       */
    newconf.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
    newconf.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    newconf.c_cc[VERASE]   = 0;     /* del */
    newconf.c_cc[VKILL]    = 0;     /* @ */
    newconf.c_cc[VEOF]     = 4;     /* Ctrl-d */
    newconf.c_cc[VTIME]    = 0;     /* inter-character timer unused */
    newconf.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
    newconf.c_cc[VSWTC]    = 0;     /* '\0' */
    newconf.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
    newconf.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    newconf.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    newconf.c_cc[VEOL]     = 0;     /* '\0' */
    newconf.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    newconf.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    newconf.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    newconf.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    newconf.c_cc[VEOL2]    = 0;     /* '\0' */


    fd_set selset;
    FD_ZERO(&selset);
    // read & write section !!
    while(1) {
        FD_SET(fd, &selset);

        struct timeval timeout = {
            .tv_sec = 0,
            .tv_usec = 0
        };

        if(select(fd + 1, &selset, NULL, NULL, &timeout) == -1) {
            perror("select");
            goto exit;
        }

        //have not catch anymore
        if(FD_ISSET(fd, &selset) == 0)
            continue;

        unsigned char read_word;
        int count = read(fd, &read_word, 1);
        if(count <= 0) {
            perror("read");
            goto exit;
        }

        read_word++;

        count = write(fd, &read_word, 1);
        if(count <= 0) {
            perror("write");
            goto exit;
        }
    }
    
exit:
    close(fd);

    return 0;
}
    
