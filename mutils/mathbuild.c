#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void strjoin(const char* separator, int count, char** strings, char* buffer, size_t buffer_size) {
    if (count <= 0 || buffer_size == 0) {
        if (buffer_size > 0) buffer[0] = '\0';
        return;
    }
    
    size_t sep_len = strlen(separator);
    size_t pos = 0;
    
    for (int i = 1; i < count && pos < buffer_size - 1; i++) {
        size_t len = strlen(strings[i]);
        size_t to_copy = (pos + len < buffer_size - 1) ? len : buffer_size - 1 - pos;
        memcpy(buffer + pos, strings[i], to_copy);
        pos += to_copy;
        
        if (i < count - 1 && pos < buffer_size - 1) {
            size_t sep_to_copy = (pos + sep_len < buffer_size - 1) ? sep_len : buffer_size - 1 - pos;
            memcpy(buffer + pos, separator, sep_to_copy);
            pos += sep_to_copy;
        }
    }
    
    buffer[pos] = '\0';
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
	fprintf(stderr, "\033[1;33m----Mbuild----\033[0m\n");
        fprintf(stderr, "\033[1;31mAn Error occured\033[0m\n");
        fprintf(stderr, "No input files Abort.\n");
        return 1;
    }

    char buff[1024 * 5];
    char clcmds[1024 * 7];

    strjoin(" ", argc, argv, buff, sizeof(buff));
    snprintf(clcmds, sizeof(clcmds), "gcc -O2 -flto -fwhole-program -DMBUILD=1 -Wall -Wextra %s -lm", buff);
    system(clcmds);
    
    return 0;
}
