



//cmake -B build
//cmake --build build
//build/guardia
//gcc main.c -o main
//sudo ./main /dev/ttyACM0
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/ioctl.h>

int verif_instruction(char **instruction, char *buffer_sending){
        for (int i = 0 ; i < 12 ; i++){
            if (strcmp(buffer_sending, instruction[i]) == 0){
                return 1;
                break;
            }   
        }
        return 0;
}

int setup_serial_port(const char *port_name) {
    int fd = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd == -1) {
        perror("Unable to open serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    options.c_iflag = IGNBRK;
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

int write_serial_port(int fd, const char *data, size_t size) {
    return write(fd, data, size);
}

int main(int argc, char **argv) {
    int ret = -1;
    int len = 0;
    int bytes_read = 0;
    struct pollfd fds;
    fds.events = POLLIN;
    char *instruction[] = {
    "LED1 ON",
    "LED1 OFF",
    "LED2 ON",
    "LED2 OFF",
    "LED3 ON",
    "LED3 OFF",
    "CHENILLARD1 ON",
    "CHENILLARD1 OFF",
    "CHENILLARD2 ON",
    "CHENILLARD2 OFF",
    "CHENILLARD3 ON",
    "CHENILLARD3 OFF",
    "CHENILLARD FREQUENCE1",
    "CHENILLARD FREQUENCE2",
    "CHENILLARD FREQUENCE3"
    };

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <serial_port>\n", argv[0]);
        return 1;
    }

    fds.fd = setup_serial_port(argv[1]);
    if (fds.fd == -1) {
        return 1;
    }

    char buffer[256];
    char buffer_sending[256];
    uint8_t ch;

    printf("Instruction possible : \n\r");
    for (int i = 0; i < 15; i++){
        printf("%s\n\r", instruction[i]);
    }
    while (1) 
    {

        printf("Instruction : ");

        

        if (fgets(buffer_sending, sizeof(buffer_sending), stdin) != NULL) {
            buffer_sending[strcspn(buffer_sending, "\n")] = 0;
                if ((buffer_sending[0] == 'q' || buffer_sending[0] == 'Q') && strlen(buffer_sending) == 1){ 
                    printf("fermeture du programme.\n\r"); 
                    break; 
                }
                else if((buffer_sending[0] == 'c' || buffer_sending[0] == 'C') && strlen(buffer_sending) == 1){
                    system("clear");
                    memset(buffer_sending,0,sizeof(buffer_sending));        
                }
                else if((buffer_sending[0] == 'h' || buffer_sending[0] == 'H') && strlen(buffer_sending) == 1){
                    for (int i = 0; i < 15; i++){
                        printf("%s\n\r", instruction[i]);
                    }
                    memset(buffer_sending,0,sizeof(buffer_sending));
                }
                else if(verif_instruction(instruction, buffer_sending) == 1){
                    
                    ssize_t bytes = write(fds.fd, buffer_sending, strlen(buffer_sending));
                    if (bytes == -1){
                        printf("erreur\n\r");
                    }
                    else{
                        printf("envoyée avec succès : %s\n\r",buffer_sending);
                        printf("nombre de bytes envoyés : %ld\n\r", bytes);
                    }
                    memset(buffer_sending,0,sizeof(buffer_sending));
                }
                else{
                    printf("mauvaise instruction\n\r");
                    memset(buffer_sending,0,sizeof(buffer_sending));
                }        
        }   
        
    }

    close(fds.fd);
    return 0;
}

/*ret = poll(&fds, 1, 1000); // 1-second timeout
        if (ret > 0) 
        {
            if (fds.revents & POLLIN) {
                if(ioctl(fds.fd, FIONREAD, &bytes_read) == -1) {
                    printf("Error getting bytes available");
                    continue;;
                }
                printf("Bytes available: %d\n\r", bytes_read);
                len = read(fds.fd, buffer, bytes_read);

                if (len > 0) {
                    buffer[len] = '\0'; // Null-terminate the string
                    printf("Received: %s %d\r\n", buffer, len);
                    memset(buffer, 0, sizeof(buffer)); // Clear buffer
                    len = 0; // Reset length for next read
                } else if (len == 0) {
                    printf("No data received\n\r");
                    len = 0; // Reset length for next read
                    continue;
                } else {
                    printf("Error reading from serial port\n\r");
                    len = 0; // Reset length for next read
                    continue;
                }
            }
        }else if (ret == 0) {
            printf("Timeout waiting for data...\n\r");
            len = 0; // Reset length for next read
        }*/