#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct line
{
    size_t lengh;
    char* ptr;
};

long long find_file_size(const char* file_name);

size_t count_lines(char* buffer);

void get_text(struct line*  text, char* buffer);

int lcomp(const void* elem1, const void* elem2);

int rcomp(const void* elem1, const void* elem2);

void bubblesort(void* text, size_t linenum, size_t size, int(* comp)(const void *elem1, const void *elem2));

void print_l_encyclopedia(FILE* stream, struct line* text, size_t linenum);

void print_r_encyclopedia(FILE* stream, struct line* text, size_t linenum);

int main()
{
    long long file_size0 = find_file_size("OneginText1251.txt");

    char* buffer = (char*) calloc(file_size0 + 1, sizeof(char));
    assert(buffer != NULL);

    int Onegin = open("OneginText1251.txt", O_RDONLY);
    assert(Onegin != 0);

    size_t file_size = read (Onegin, buffer, file_size0 + 1);
    assert(file_size != 0);
    buffer[file_size] = '\0';

    close(Onegin);

    size_t linenum = count_lines(buffer);
    assert(linenum > 0);

    struct line* text = (struct line*) calloc(linenum, sizeof(line));
    assert(text != 0);
    get_text(text, buffer);

    FILE* encyclopedia = fopen("EncyclopediaOfRussianLive.txt", "w");
    assert(encyclopedia != 0);

    fprintf(encyclopedia, "                                ENCYCLOPEDIA OF RUSSIAN LIFE\n"
                          "                 (BASED ON THE NOVEL IN VERSE BY A.S. PUSHKIN 'EUGENE ONEGIN')\n"
                          "             approved by poltorashka (DED, you have no chance of not reading this)\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n");

    qsort(text, linenum, sizeof(struct line), lcomp);

    print_l_encyclopedia(encyclopedia, text, linenum);

    qsort(text, linenum, sizeof(struct line), rcomp);

    print_r_encyclopedia(encyclopedia, text, linenum);

    fprintf(encyclopedia, "WRITERS, DON'T TAKE IT PERSONALLY. HERE IS YOUR ETERNAL PUSHKIN, WE HAVE NOT CHANGED ANYTHING.------------\n"
                          "\n"
                          "\n");
    fputs(buffer, encyclopedia);

    fclose(encyclopedia);

    free(text);
    free(buffer);

    return 0;
}




long long find_file_size(const char* file_name)
{
    assert (file_name != 0);

    struct stat file_info = {0};
    stat(file_name, &file_info);

    assert(file_info.st_size != 0);

    return file_info.st_size;
}


size_t count_lines(char* buffer)
{
    assert (buffer != 0);

    size_t linenum = 0;
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            linenum++;
        }
    }

    assert(linenum != 0);

    return linenum;
}


void get_text(struct line* text, char* buffer)
{
    assert (text != 0);
    assert (buffer != 0);

    size_t count = 0, ptr_shift = 0;

    for (size_t letter = 0; buffer[letter] != '\0'; letter++) {
        if (buffer[letter] == '\n') {
            text[count].ptr = buffer + ptr_shift;
            text[count].lengh = (letter + 1) - ptr_shift;
            ptr_shift = letter + 1;
            count++;
        }
    }
}


void bubblesort(void* text, size_t linenum, size_t size, int(* comp)(const void* line1, const void* line2))
{
    assert (text != 0);
    assert (comp != 0);
    assert (text != 0);
    assert (linenum > 0);

    void* save = calloc(1, size);
    assert (save != 0);

    for (size_t dlina_prohoda = linenum-1; dlina_prohoda > 1; dlina_prohoda--) {
        assert (dlina_prohoda<linenum);
        assert (dlina_prohoda > 1);
        for(size_t pos = 1; pos <= dlina_prohoda; pos++) {
            assert (pos <= dlina_prohoda);
            if (comp((char*)text + (pos-1)*size, (char*)text + pos*size) > 0) {
                memcpy(save, (char*)text + (pos-1)*size, size);
                memcpy((char*)text + (pos-1)*size, (char*)text + pos*size, size);
                memcpy((char*)text + pos*size, save, size);
            }
        }
    }
    free(save);
}


int lcomp(const void* elem1, const void* elem2)
{
    assert(elem1 != 0);
    assert(elem2 != 0);

    const struct line *line1 = (const struct line*) elem1;
    const struct line *line2 = (const struct line*) elem2;

    int pos1 = 0, pos2 = 0;
    char *str1 = line1->ptr, *str2 = line2->ptr;

    while (str1[pos1] != '\n' || str2[pos2] != '\n') {
        while (str1[pos1] != '\n' && !isalpha((unsigned char)str1[pos1])) {
            pos1++;
        }

        while (str2[pos2] != '\n' && !isalpha((unsigned char)str2[pos2])) {
            pos2++;
        }

        if (str1[pos1] == '\n' || str2[pos2] == '\n') {
            break;
        }

        char c1 = tolower((unsigned char)str1[pos1]);
        char c2 = tolower((unsigned char)str2[pos2]);

        if (c1 != c2) {
            return c1 - c2;
        }

        pos1++;
        pos2++;
    }

    return line1->lengh - line2->lengh;
}



int rcomp(const void* elem1, const void* elem2)
{
    assert(elem1 != 0);
    assert(elem2 != 0);

    const struct line *line1 = (const struct line *)elem1;
    const struct line *line2 = (const struct line *)elem2;

    char *str1 = line1->ptr;
    char *str2 = line2->ptr;

    int len1 = line1->lengh;
    int len2 = line2->lengh;

    int pos1 = len1 - 2;
    int pos2 = len2 - 2;

    while (pos1 >= 0 || pos2 >= 0) {
        while (pos1 >= 0 && !isalpha((unsigned char)str1[pos1])) {
            pos1--;
        }

        while (pos2 >= 0 && !isalpha((unsigned char)str2[pos2])) {
            pos2--;
        }

        if (pos1 < 0 || pos2 < 0) {
            break;
        }

        char c1 = tolower((unsigned char)str1[pos1]);
        char c2 = tolower((unsigned char)str2[pos2]);

        if (c1 != c2) {
            return c1 - c2;
        }

        pos1--;
        pos2--;
    }

    return len1 - len2;
}




void print_l_encyclopedia(FILE* stream, struct line* text, size_t linenum)
{
    assert (stream != 0);
    assert (text != 0);
    assert (linenum != 0);

    char current_sym = 0;

    fprintf(stream, "THE LEFT ENCYCLOPEDIA-------------------------------------------------------------------------------------\n");

    for (size_t line = 0; line < linenum; line++) {
        size_t sym = 0;
        while (!isalpha((*(text[line].ptr + sym))) && sym < text[line].lengh) {
            sym++;
        }
        if (toupper(*(text[line].ptr + sym)) != current_sym) {
            current_sym = toupper(*(text[line].ptr + sym));
            fprintf(stream, "\n"
                            "\n"
                            "                                       %c\n"
                            "\n"
                            "\n",
                            current_sym);
        }
        for (size_t let = 0; let < text[line].lengh; let++) {
            fprintf(stream, "%c", *(text[line].ptr + let));
        }
    }

    fprintf(stream, "----------------------------------------------------------------------------------------------------------\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "==========================================================================================================\n"
                          "==========================================================================================================\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n");
}


void print_r_encyclopedia(FILE* stream, struct line* text, size_t linenum)
{
    assert (stream != 0);
    assert (text != 0);
    assert (linenum != 0);

    char current_sym = 0;

    fprintf(stream, "THE RIGHT ENCYCLOPEDIA-------------------------------------------------------------------------------------\n");

    for (size_t line = 0; line < linenum; line++) {
        size_t sym = 0;
        while (!isalpha((*(text[line].ptr + text[line].lengh - 2 - sym))) && sym < text[line].lengh - 1) {
            sym++;
        }
        if (toupper(*(text[line].ptr + text[line].lengh - 2 - sym)) != current_sym) {
            current_sym = toupper(*(text[line].ptr + text[line].lengh - 2 - sym));
            fprintf(stream, "\n"
                            "\n"
                            "                                       %c\n"
                            "\n"
                            "\n",
                            current_sym);
        }
        for (size_t let = 0; let < text[line].lengh; let++) {
            fprintf(stream, "%c", *(text[line].ptr + let));
        }
    }

    fprintf(stream, "----------------------------------------------------------------------------------------------------------\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "==========================================================================================================\n"
                          "==========================================================================================================\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n");
}
