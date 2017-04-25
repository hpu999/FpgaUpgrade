#include "spi.h"

#define SIZE 4096

int spi_mode (void) {
    int fd_mtd, fd_tt;
    int res, val;
    static uint8_t mode;
    static uint8_t bits = 8;
    static uint32_t speed = 500000;

    fd_mtd = spi_open("/dev/spidev3.1");
    if (fd_mtd < 0)
    {
        perror ("spi_open");
        return -1;
    }

    res = spi_set_mode(mode);
    if (res < 0)
    {
        perror ("spi_set_mode");
        return -1;
    }

    res = spi_set_bits(bits);
    if (res < 0)
    {
        perror ("spi_set_bits");
        return -1;
    }

    res = spi_set_speed(speed);
    if (res < 0)
    {
        perror ("spi_set_speed");
        return -1;
    }

    fd_tt = spi_cs_open("/dev/gpiodrv");
    if (fd_tt < 0)
    {
        perror ("spi_cs_open");
        return -1;
    }

    ioctl(fd_tt, GPIO99_HIGH, &val);

    return 0;
}

int main (int argc, char *argv[]) {
    int i, j;
    int res;
    int fd_file;
    size_t size = 0;
    size_t tmp = 0;
    char buff[256] = {0};

    spi_mode();

    // RDID
     char id[3] = {0,};
    spi_rdid (id, sizeof (id));
    if (id[0] != 0x20) {
        printf ("No found SPI FLASH. \n");
        return -1;
    }
    printf ("ID:id = %.2x %.2x %.2x \n", id[0], id[1], id[2]);

    // READ
    fd_file = open(argv[1], O_RDWR | O_CREAT);
    if (fd_file < 0)
    {
        perror ("open file");
        return -1;
    }

    char read_data[SIZE] = {0};
    size = 0x01000000;

    while (size / SIZE)
    {
        memset (read_data, 0, SIZE);
        memset (file_data, 0, SIZE);
        res = spi_read(addr, read_data, sizeof (read_data));

        write (fd_file, read_data, res);
        size -= res;
        addr += res;
        if (addr % res)
        {
            printf ("size/SIZE no read SIZE ... \n");
            return -1;
        }
    }
    while (size % SIZE)
    {
        memset (read_data, 0, SIZE);
        memset (file_data, 0, SIZE);
        res = spi_read(addr, read_data, size % SIZE);

        write (fd_file, read_data, res);
        size -= res;
        addr += res;
    }
    printf ("Read from spi ,write to file over ... \n");
    close (fd_file);
    spi_close ();
    spi_cs_close ();

    return 0;
}