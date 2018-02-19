/*
	Serial reading class
	
	 * Arduino-serial
 * --------------
 * 
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC) 
 *
 *
 * Compile with something like:
 * gcc -o arduino-serial arduino-serial.c
 *
 * Created 5 December 2006
 * Copyleft (c) 2006, Tod E. Kurt, tod@todbot.com
 * http://todbot.com/blog/
 *
 * 
 * Updated 8 December 2006: 
 *  Justin McBride discoevered B14400 & B28800 aren't in Linux's termios.h.
 *  I've included his patch, but commented out for now.  One really needs a
 *  real make system when doing cross-platform C and I wanted to avoid that
 *  for this little program. Those baudrates aren't used much anyway. :)
 *
 * Updated 26 December 2007:
 *  Added ability to specify a delay (so you can wait for Arduino Diecimila)
 *  Added ability to send a binary byte number
 *
 * Update 31 August 2008:
 *  Added patch to clean up odd baudrates from Andy at hexapodia.org
 *
 
 ls /dev/(tty*) to check serial ports
 portname ie) "/dev/ttyACM"
 bottom right on pi is /dev/ttyUSB0
 
*/

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

class Serial
{
	public:
	
		Serial(const char* serialport, int baud)
		{
			//_fd is -1 if this failed
			_fd = Init(serialport, baud);
		}
		
		int WriteByte( uint8_t b )
		{
			int n = write(_fd,&b,1);
			if( n!=1)
				return -1;
			return 0;
		}

		int Write(const char* str)
		{
			int len = strlen(str);
			int n = write(_fd, str, len);
			if( n!=len ) 
				return -1;
			return 0;
		}

		int ReadUntil(char until)
		{
			//memset(_buffer, 0, 256);
			char b[1];
			int i=0;
			do { 
				int n = read(_fd, b, 1);  // read a char at a time
				if( n==-1) return -1;    // couldn't read
				if( n==0 ) {
					usleep( 10 * 1000 ); // wait 10 msec try again
					continue;
				}
				_buffer[i] = b[0]; i++;
			} while( b[0] != until );

			_buffer[i] = 0;  // null terminate the string
			return 0;
		}
		
		//to try and prevent lots of junk
		int ReadMsg(char start, char until)
		{
			memset(_buffer, 0, 256);
			char b[1];
			int i=0;
			do { 
				int n = read(_fd, b, 1);
				if(n == 0) 
				{
				//	usleep(10 * 100);
					continue;
				}
			}while( b[0] != start );
			
			do { 
				int n = read(_fd, b, 1);  // read a char at a time
				if( n==-1) return -1;    // couldn't read
				if( n==0 ) {
					//usleep( 10 * 100 ); // wait 10 msec try again
					continue;
				}
				_buffer[i] = b[0]; i++;
			} while( b[0] != until );
			_buffer[i] = 0;  // null terminate the string
			return 0;
		}
		
		char* GetMsg()
		{
			return _buffer;	
		}
		
		bool Open()
		{
			return !(_fd == -1);
		}

		// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
		// and a baud rate (bps) and connects to that port at that speed and 8N1.
		// opens the port in fully raw mode so you can send binary data.
		// returns valid fd, or -1 on error
		int Init(const char* serialport, int baud)
		{
			struct termios toptions;
			int fd;
			
			//fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
			//        serialport,baud);

			fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
			if (fd == -1)  {
				perror("init_serialport: Unable to open port ");
				return -1;
			}
			
			if (tcgetattr(fd, &toptions) < 0) {
				perror("init_serialport: Couldn't get term attributes");
				return -1;
			}
			speed_t brate = baud; // let you override switch below if needed
			switch(baud) {
			case 4800:   brate=B4800;   break;
			case 9600:   brate=B9600;   break;
		#ifdef B14400
			case 14400:  brate=B14400;  break;
		#endif
			case 19200:  brate=B19200;  break;
		#ifdef B28800
			case 28800:  brate=B28800;  break;
		#endif
			case 38400:  brate=B38400;  break;
			case 57600:  brate=B57600;  break;
			case 115200: brate=B115200; break;
			}
			cfsetispeed(&toptions, brate);
			cfsetospeed(&toptions, brate);

			// 8N1
			toptions.c_cflag &= ~PARENB;
			toptions.c_cflag &= ~CSTOPB;
			toptions.c_cflag &= ~CSIZE;
			toptions.c_cflag |= CS8;
			// no flow control
			toptions.c_cflag &= ~CRTSCTS;

			toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
			toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

			toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
			toptions.c_oflag &= ~OPOST; // make raw

			// see: http://unixwiz.net/techtips/termios-vmin-vtime.html
			toptions.c_cc[VMIN]  = 0;
			toptions.c_cc[VTIME] = 20;
			
			if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
				perror("init_serialport: Couldn't set term attributes");
				return -1;
			}

			return fd;
		}
	
	private:
		int _fd;
		char _buffer[256];
		int baud;
};
