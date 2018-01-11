// Author: Benjamin Truong
//  apt-get install libi2c-dev
//
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>

const unsigned char MCP_SLAVE = 0x70;
const char *i2c_deviceName = "/dev/i2c-1";

int file;                                       // Reading/writing from this file is necessary to utilize the i2c APIs
uint8_t buffer[] = {0, 0, 0, 0, 0, 0, 0, 0};    // The first 8 bytes of the device pertain to the 16 LEDs

/* These LED addresses do not work although they are used in the demos provided by Adafruit. Not sure why.
uint8_t ledAddr[16] = { 0x3A, 0x37, 0x35, 0x34,
                        0x28, 0x29, 0x23, 0x24,
                        0x16, 0x1B, 0x11, 0x10,
                        0x0E, 0x0D, 0x0C, 0x02 };
*/

void init(void);
void setLED(uint8_t);
void clearLED(uint8_t);
int buttonPressed(void);
/* This function initializes the Adafruit Trellis board for i2c communication */
void init(void) {

        if ((file = open (i2c_deviceName, O_RDWR)) < 0) 
        {
            printf("Failed to open %s\n", i2c_deviceName);
            exit(1);
        }
        if (ioctl (file, I2C_SLAVE, MCP_SLAVE) < 0) 
        {
            perror("i2c_write: ioctl");
            printf("i2c_write: Failed to acquire bus access: SlaveAddress = 0x%x\n", MCP_SLAVE);
            exit(1);
        }

        i2c_smbus_write_byte(file, 0x21);       // turn on oscillator
        i2c_smbus_write_byte(file, 0x81);       // turn on display

        /* do a neat little light sweep */
        int i;
        for (i = 1; i < 17; i++)
        {
            setLED(i);
            usleep(50000);
        }
        for (i = 1; i < 17; i++)
        {
            clearLED(i);
            usleep(50000);
        }

        printf ("Init Complete\n");
}

/* This function turns on LED number 'n'
 * It is worth noting LEDs 1-4 require setting the bits of other
 * buffer elements in an unintuitive way.
 */ 
void setLED(uint8_t n)
{
    switch(n)
    {
        case 1:
            buffer[1] |= 0x04;
            buffer[7] |= 0x04;
            printf("LED1 set\n");
            break;
        case 2:
            buffer[0] |= 0x80;
            buffer[6] |= 0x80;
            printf("LED2 set\n");
            break;
        case 3:
            buffer[0] |= 0x20;
            buffer[6] |= 0x20;
            printf("LED3 set\n");
            break;
        case 4:
            buffer[0] |= 0x10;
            buffer[6] |= 0x10;
            printf("LED4 set\n");
            break;
        case 5:
            buffer[5] |= 0x01;
            printf("LED5 set\n");
            break;
        case 6:
            buffer[5] |= 0x02;
            printf("LED6 set\n");
            break;
        case 7:
            buffer[4] |= 0x08;
            printf("LED7 set\n");
            break;
        case 8:
            buffer[4] |= 0x10;
            printf("LED8 set\n");
            break;
        case 9:
            buffer[2] |= 0x40;
            printf("LED9 set\n");
            break;
        case 10:
            buffer[3] |= 0x08;
            printf("LED10 set\n");
            break;
        case 11:
            buffer[2] |= 0x02;
            printf("LED11 set\n");
            break;
        case 12:
            buffer[2] |= 0x01;
            printf("LED12 set\n");
            break;
        case 13:
            buffer[1] |= 0x40;
            printf("LED13 set\n");
            break;
        case 14:
            buffer[1] |= 0x20;
            printf("LED14 set\n");
            break;
        case 15:
            buffer[1] |= 0x10;
            printf("LED15 set\n");
            break;
        case 16:
            buffer[0] |= 0x04;
            printf("LED16 set\n");
            break;
    }

    i2c_smbus_write_i2c_block_data(file, 0x00, sizeof(buffer), (uint8_t *)buffer);
}

/* This function does the opposite of setLED */
void clearLED(uint8_t n)
{
    switch(n)
    {
        case 1:
            buffer[1] &= ~0x04;
            buffer[7] &= ~0x04;
            printf("LED1 cleared\n");
            break;
        case 2:
            buffer[0] &= ~0x80;
            buffer[6] &= ~0x80;
            printf("LED2 cleared\n");
            break;
        case 3:
            buffer[0] &= ~0x20;
            buffer[6] &= ~0x20;
            printf("LED3 cleared\n");
            break;
        case 4:
            buffer[0] &= ~0x10;
            buffer[6] &= ~0x10;
            printf("LED4 cleared\n");
            break;
        case 5:
            buffer[5] &= ~0x01;
            printf("LED5 cleared\n");
            break;
        case 6:
            buffer[5] &= ~0x02;
            printf("LED6 cleared\n");
            break;
        case 7:
            buffer[4] &= ~0x08;
            printf("LED7 cleared\n");
            break;
        case 8:
            buffer[4] &= ~0x10;
            printf("LED8 cleared\n");
            break;
        case 9:
            buffer[2] &= ~0x40;
            printf("LED9 cleared\n");
            break;
        case 10:
            buffer[3] &= ~0x08;
            printf("LED10 cleared\n");
            break;
        case 11:
            buffer[2] &= ~0x02;
            printf("LED11 cleared\n");
            break;
        case 12:
            buffer[2] &= ~0x01;
            printf("LED12 cleared\n");
            break;
        case 13:
            buffer[1] &= ~0x40;
            printf("LED13 cleared\n");
            break;
        case 14:
            buffer[1] &= ~0x20;
            printf("LED14 cleared\n");
            break;
        case 15:
            buffer[1] &= ~0x10;
            printf("LED15 cleared\n");
            break;
        case 16:
            buffer[0] &= ~0x04;
            printf("LED16 cleared\n");
            break;
    }
    i2c_smbus_write_i2c_block_data(file, 0x00, sizeof(buffer), (uint8_t *)buffer);
}

/* This function determines whether a button has been pressed */
int buttonPressed(void)
{
    uint8_t readbuf[4];
    i2c_smbus_read_i2c_block_data(file, 0x40, 4, (uint8_t *)readbuf);

    if ((readbuf[0] & 0x80) == 0x80)
        return 1;
    else if ((readbuf[0] & 0x10) == 0x10)
        return 2;
    else if ((readbuf[0] & 0x04) == 0x04)
        return 3;
    else if ((readbuf[0] & 0x20) == 0x20)
        return 5;
    else if ((readbuf[0] & 0x40) == 0x40)
        return 6;
    else if ((readbuf[0] & 0x01) == 0x01)
        return 7;
    else if ((readbuf[0] & 0x02) == 0x02)
        return 8;
    else if ((readbuf[0] & 0x08) == 0x08)
        return 9;
    else if ((readbuf[1] & 0x01) == 0x01)
        return 10;
    else if ((readbuf[1] & 0x08) == 0x08)
        return 13;
    else if ((readbuf[1] & 0x04) == 0x04)
        return 14;
    else if ((readbuf[1] & 0x02) == 0x02)
        return 15;
    else if ((readbuf[2] & 0x04) == 0x04)
        return 4;
    else if ((readbuf[2] & 0x02) == 0x02)
        return 12;
    else if ((readbuf[3] & 0x01) == 0x01)
        return 11;
    else if ((readbuf[3] & 0x02) == 0x02)
        return 16;

    return 0;

}

int main (int argc, char **argv) {
        init();
        printf("\nPress ctr + c to end program\n");
        uint8_t bPressed;              // Tracks if a button was just pressed
        uint8_t setFlag[16];           // Keeps track of whether each LED is set or cleared
        uint8_t wait = 0;              // Waits after initial button press to prevent debounce

        uint8_t i;
        for (i = 0; i < 16; i++)
            setFlag[i] = 0;

        while(1)
        {
            usleep(50000);      // wait a few ms necessary to register
            bPressed = buttonPressed();
            if (bPressed && !wait)
            {
                wait = 1;

                if (!setFlag[bPressed-1])
                {
                    setLED(bPressed);
                    setFlag[bPressed-1] = 1;
                }

                else if (setFlag[bPressed-1])
                {
                    clearLED(bPressed);
                    setFlag[bPressed-1] = 0;
                }
            }

            /* Prevent debounce */
            if (wait)
            {
                if (!bPressed)
                {
                    usleep(20000);
                    wait = 0;
                }
            }
        }
        return 0;
}
