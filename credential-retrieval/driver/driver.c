/* FairPlayIOKit driver-call helper. */
#include <IOKit/IOKitLib.h>
#include <mach/vm_map.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum { PREFIX = 0x160, SIZE = 552 };

static void expired(int signum) {
    (void)signum;
    const char text[] = "driver: alarm expired inside IOKit call\n";
    (void)write(STDERR_FILENO, text, sizeof(text) - 1);
    _exit(124);
}

static int read_exact(const char *path, uint8_t *out, size_t size) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    size_t at = 0;
    while (at < size) {
        ssize_t n = read(fd, out + at, size - at);
        if (n <= 0) {
            close(fd);
            return -1;
        }

        at += (size_t)n;
    }

    uint8_t extra;
    ssize_t n = read(fd, &extra, 1);
    close(fd);
    return n == 0 ? 0 : -1;
}

static int write_exact_new(const char *path, const void *bytes, size_t size) {
    int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd < 0) {
        return -1;
    }

    size_t at = 0;
    while (at < size) {
        ssize_t n = write(fd, (const uint8_t *)bytes + at, size - at);
        if (n <= 0) {
            close(fd);
            return -1;
        }

        at += (size_t)n;
    }

    int okay = fsync(fd) == 0 && close(fd) == 0;
    return okay ? 0 : -1;
}

int main(int argc, char **argv) {
    if (argc != 6 || (strcmp(argv[5], "9") && strcmp(argv[5], "21"))) {
        fprintf(stderr, "usage: %s REQUEST.bin OUTPUT.bin SOURCE16.bin SLOT_ADDRESS SELECTOR\n", argv[0]);
        return 2;
    }

    uint8_t region[PREFIX + SIZE] __attribute__((aligned(16)));
    memset(region, 0x5a, sizeof(region));
    if (read_exact(argv[1], region + PREFIX, SIZE) != 0) {
        fprintf(stderr, "request missing or not exactly 552 bytes\n");
        return 2;
    }

    io_service_t service = IOServiceGetMatchingService(MACH_PORT_NULL, IOServiceMatching("com_apple_driver_FairPlayIOKit"));
    if (service == IO_OBJECT_NULL) {
        return 3;
    }

    io_connect_t connection = IO_OBJECT_NULL;
    kern_return_t status = IOServiceOpen(service, mach_task_self(), 0, &connection);
    IOObjectRelease(service);
    if (status != KERN_SUCCESS) {
        return 4;
    }

    vm_address_t mapped_page = 0;
    vm_size_t mapped_size = 0;
    uint8_t *known_slot = NULL;
    uint8_t side_result[16] = {0};
    char *end = NULL;
    errno = 0;
    unsigned long long parsed = strtoull(argv[4], &end, 0);
    if (errno || end == argv[4] || *end || parsed < 0x100000000ULL || parsed >= (1ULL << 40) || (parsed & 0xf)) {
        fprintf(stderr, "invalid slot address\n");
        IOServiceClose(connection);
        return 2;
    }

    const vm_address_t target = (vm_address_t)parsed;
    uint8_t source[16];
    if (read_exact(argv[3], source, sizeof(source)) != 0) {
        fprintf(stderr, "source file missing or not exactly 16 bytes\n");
        IOServiceClose(connection);
        return 2;
    }

    mapped_size = (vm_size_t)getpagesize();
    if ((target & (mapped_size - 1)) + 0x80 > mapped_size) {
        fprintf(stderr, "slot address crosses a VM page\n");
        IOServiceClose(connection);
        return 2;
    }

    mapped_page = target & ~(mapped_size - 1);
    kern_return_t allocation = vm_allocate(mach_task_self(), &mapped_page, mapped_size, 0);
    if (allocation != KERN_SUCCESS || mapped_page != (target & ~(mapped_size - 1))) {
        IOServiceClose(connection);
        return 8;
    }

    known_slot = (uint8_t *)(uintptr_t)target;
    memset(known_slot, 0x5a, 0x80);
    memcpy(known_slot + 0x41, source, sizeof(source));

    signal(SIGALRM, expired);
    alarm(8);
    size_t output_size = SIZE;
    status = IOConnectCallStructMethod(connection, (uint32_t)strtoul(argv[5], NULL, 10), region + PREFIX, SIZE, region + PREFIX, &output_size);
    alarm(0);
    memcpy(side_result, known_slot, sizeof(side_result));
    vm_deallocate(mach_task_self(), mapped_page, mapped_size);

    IOServiceClose(connection);
    uint8_t record[sizeof(region) + sizeof(side_result)];
    memcpy(record, region, sizeof(region));
    memcpy(record + sizeof(region), side_result, sizeof(side_result));

    if (write_exact_new(argv[2], record, sizeof(record)) != 0) {
        fprintf(stderr, "cannot create unique output: %s\n", strerror(errno));
        return 5;
    }

    return status == KERN_SUCCESS ? 0 : 6;
}
