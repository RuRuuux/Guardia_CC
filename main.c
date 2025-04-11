



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
        for (int i = 0 ; i < 15 ; i++){
            if (strcmp(buffer_sending, instruction[i]) == 0){
                return 1;
                break;
            }   
        }
        return 0;
}

void print_instruction(char **instruction){
    printf("=====================================\n\r");
    printf("Instruction possible : \n\r");
    for (int i = 0; i < 18; i++){
        printf("%s\n\r", instruction[i]);
    }
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


int main(int argc, char **argv) {
    struct pollfd fds;
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
    "CHENILLARD FREQUENCE3",
    "Q/q/quit",
    "C/c/clear",
    "H/h/help"
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

    print_instruction(instruction);
    while (1) 
    {
        
        
        printf("=====================================\n\rInstruction : ");
        


        if (fgets(buffer_sending, sizeof(buffer_sending), stdin) != NULL) {
            buffer_sending[strcspn(buffer_sending, "\n")] = 0;
            buffer_sending[strcspn(buffer_sending, "\r")] = 0;
            buffer_sending[strlen(buffer_sending)]= '\0';
                if (((buffer_sending[0] == 'q' || buffer_sending[0] == 'Q') && strlen(buffer_sending) == 1) || strcmp(buffer_sending,"quit") == 0){ 
                    printf("Fermeture du programme.\n\r"); 
                    break; 
                }
                else if(((buffer_sending[0] == 'c' || buffer_sending[0] == 'C') && strlen(buffer_sending) == 1) || strcmp(buffer_sending,"clear") == 0){
                    system("clear");
                    memset(buffer_sending,0,sizeof(buffer_sending));        
                }
                else if(((buffer_sending[0] == 'h' || buffer_sending[0] == 'H') && strlen(buffer_sending) == 1) || strcmp(buffer_sending,"help") == 0){
                    print_instruction(instruction);
                    memset(buffer_sending,0,sizeof(buffer_sending));
                }
                else if(verif_instruction(instruction, buffer_sending) == 1){
                    
                    ssize_t bytes = write(fds.fd, buffer_sending, strlen(buffer_sending)+1);
                    if (bytes == -1){
                        printf("Erreur lors de l'envoi\n\r");
                    }
                    else{
                        printf("Envoyé avec succès : %s\n\r",buffer_sending);
                        printf("Nombre de bytes envoyés : %ld\n\r", bytes);
                    }
                    memset(buffer_sending,0,sizeof(buffer_sending));
                }
                else{
                    printf("Mauvaise instruction\n\r");
                    memset(buffer_sending,0,sizeof(buffer_sending));
                }        
        } 
        
    }

    close(fds.fd);
    return 0;
}

