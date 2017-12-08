#include <state.h>
#include <parser.h>

int main(int argc, char* argv[])
{
    int fd = open(argv[1], O_RDONLY);
    setupParser(fd);
    program();
    close(fd);
    return 0;
}
