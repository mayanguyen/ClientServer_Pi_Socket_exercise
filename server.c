#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "bcm2835.h"
#include "common.h"


int portno = 9997;

int main(int argc, char *argv[]){
    int sockfd, newsockfd;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n, i, optval;
    char buffer[256], answer[256];
    pthread_t client1, client2, client3, client4, client5;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("couldn't open socket");
        exit(-1);
    }
    
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0){
        perror("binding failed");
        exit(-1);
    }
    
    listen(sockfd, 5);
    
    clilen = sizeof(cli_addr);
    
    
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0) {
        perror("accept failed");
        exit(-1);
    }
    
    //interpreting user input
    for (;;) {
        bzero(buffer, 256);
        n = read(newsockfd,buffer,255);
        printf("%d bytes read; data: %s\n",n, buffer);
        
        int pin;
        
        if (buffer[3] == 'g') { // get state of pin (button)
            sscanf(buffer, "%d %*s", &pin);
            printf("Pin: %d\n", pin);
            if (getPin(pin) == 1) {
                strcpy(answer, "Pressed.\n");
            }
            else {
                strcpy(answer,"Not pressed.\n");
            }
            printf(answer);
        }
        else if (buffer[3] == 's') { // set sth
            char set[5], what[5];
            int how;
            sscanf(buffer,"%d %s %s %d", &pin, set, what, &how);
            
            setPin(pin, what, how);
            
            strcpy(answer,"Pin is set.\n");
            printf(answer);
        }
        
        sleep(5);
        n = write(newsockfd,answer, n);
    }
    
    close(newsockfd);
    close(sockfd);
    return 0;
}

// return 1 if button pressed, 0 otherwise
int getPin(int pin) {
    if(!bcm2835_init())
        return 1;
    
    int pin2;
    switch (pin) {
        case 0:
            pin2 = RPI_GPIO_P1_03;
            break;
        case 1:
            pin2 = RPI_GPIO_P1_05;
            break;
        case 4:
            pin2 = RPI_GPIO_P1_07;
            break;
        case 7:
            pin2 = RPI_GPIO_P1_26;
            break;
        case 8:
            pin2 = RPI_GPIO_P1_24;
            break;
        case 9:
            pin2 = RPI_GPIO_P1_21;
            break;
        case 10:
            pin2 = RPI_GPIO_P1_19;
            break;
        case 11:
            pin2 = RPI_GPIO_P1_23;
            break;
        case 14:
            pin2 = RPI_GPIO_P1_08;
            break;
        case 15:
            pin2 = RPI_GPIO_P1_10;
            break;
        case 17:
            pin2 = RPI_GPIO_P1_11;
            break;
        case 18:
            pin2 = RPI_GPIO_P1_12;
            break;
        case 21:
            pin2 = RPI_GPIO_P1_13;
            break;
        case 22:
            pin2 = RPI_GPIO_P1_15;
            break;
        case 23:
            pin2 = RPI_GPIO_P1_16;
            break;
        case 24:
            pin2 = RPI_GPIO_P1_18;
            break;
        case 25:
            pin2 = RPI_GPIO_P1_22;
            break;
    }
    
    if (bcm2835_gpio_lev(pin2)) {
        bcm2835_close();
        return 0;
    }
    bcm2835_close();
    return 1;
}

int setPin(int pin, char* what, int how) {
    if (!bcm2835_init())
        return 1;
    int pin2;
    switch (pin) {
        case 0:
            pin2 = RPI_GPIO_P1_03;
            break;
        case 1:
            pin2 = RPI_GPIO_P1_05;
            break;
        case 4:
            pin2 = RPI_GPIO_P1_07;
            break;
        case 7:
            pin2 = RPI_GPIO_P1_26;
            break;
        case 8:
            pin2 = RPI_GPIO_P1_24;
            break;
        case 9:
            pin2 = RPI_GPIO_P1_21;
            break;
        case 10:
            pin2 = RPI_GPIO_P1_19;
            break;
        case 11:
            pin2 = RPI_GPIO_P1_23;
            break;
        case 14:
            pin2 = RPI_GPIO_P1_08;
            break;
        case 15:
            pin2 = RPI_GPIO_P1_10;
            break;
        case 17:
            pin2 = RPI_GPIO_P1_11;
            break;
        case 18:
            pin2 = RPI_GPIO_P1_12;
            break;
        case 21:
            pin2 = RPI_GPIO_P1_13;
            break;
        case 22:
            pin2 = RPI_GPIO_P1_15;
            break;
        case 23:
            pin2 = RPI_GPIO_P1_16;
            break;
        case 24:
            pin2 = RPI_GPIO_P1_18;
            break;
        case 25:
            pin2 = RPI_GPIO_P1_22;
            break;
    }
    if (what[0] == 'i') { // set direction of pin to i/o
        if (how == 1)
            bcm2835_gpio_fsel(pin2, BCM2835_GPIO_FSEL_INPT);
        else if (how == 0)
            bcm2835_gpio_fsel(pin2, BCM2835_GPIO_FSEL_OUTP);
    }
    else if (what[0] == 'o') { // set LED on/off
        if (how == 1)
            bcm2835_gpio_write(pin2, HIGH);
        else if (how == 0)
            bcm2835_gpio_write(pin2, LOW);
    }
    else if (what[0] == 'u') { // set resistor to up/down
        if (how == 1)
            bcm2835_gpio_set_pud(pin2, BCM2835_GPIO_PUD_UP);
        else if (how == 0)
            bcm2835_gpio_set_pud(pin2, BCM2835_GPIO_PUD_DOWN);
    }

    bcm2835_close();
    return 0;
}








