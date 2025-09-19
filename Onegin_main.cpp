#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

struct line
{
    size_t lengh;
    char* ptr;
};

long long find_file_size(const char* file_name);

size_t count_lines(char* buffer);

void get_text(struct line*  text, char* buffer);

int lcomp(struct line* text, size_t line1, size_t line2);

int rcomp(struct line* text, size_t line1, size_t line2);

void change(struct line* text, size_t line1, size_t line2);

void bubblesort(struct line* text, size_t linenum, int(* comp)(struct line* text, size_t line1, size_t line2));

void print_text(FILE* file, struct line* text, size_t linenum);

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

    bubblesort(text, linenum, lcomp);

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
                          "\n"
                          "THE LEFT ENCYCLOPEDIA-------------------------------------------------------------------------------------\n"
                          "\n"
                          "\n");

    print_text(encyclopedia, text, linenum);

    fprintf(encyclopedia, "----------------------------------------------------------------------------------------------------------\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "==========================================================================================================\n"
                          "==========================================================================================================\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "THE RIGHT ENCYCLOPEDIA-------------------------------------------------------------------------------------\n"
                          "\n"
                          "\n");

    bubblesort(text, linenum, rcomp);

    print_text(encyclopedia, text, linenum);

    fprintf(encyclopedia, "----------------------------------------------------------------------------------------------------------\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "==========================================================================================================\n"
                          "==========================================================================================================\n"
                          "\n"
                          "\n"
                          "\n"
                          "\n"
                          "WRITERS, DON'T TAKE IT PERSONALLY. HERE IS YOUR ETERNAL PUSHKIN, WE HAVE NOT CHANGED ANYTHING.------------\n"
                          "\n"
                          "\n");
    fputs(buffer, encyclopedia);

    fclose(encyclopedia);

    free(text);
    free(buffer);
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


void bubblesort(struct line* text, size_t linenum, int(* comp)(struct line* text, size_t line1, size_t line2))
{
    assert (text != 0);
    assert (comp != 0);
    assert (text != 0);
    assert (linenum > 0);

    for (size_t dlina_prohoda = linenum-1; dlina_prohoda > 1; dlina_prohoda--) {
        assert (dlina_prohoda<linenum);
        assert (dlina_prohoda > 1);
        for(size_t pos = 1; pos <= dlina_prohoda; pos++) {
            assert (pos <= dlina_prohoda);
            if (comp(text, pos-1, pos) < 0) {
                change(text, pos-1, pos);
            }
        }
    }
}


int lcomp(struct line* text, size_t line1, size_t line2)
{
    assert(text != 0);

    int pos1 = 0, pos2 = 0;
    char *str1 = text[line1].ptr, *str2 = text[line2].ptr;

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
            return c2 - c1;
        }

        pos1++;
        pos2++;
    }

    return text[line2].lengh - text[line1].lengh;
}



int rcomp(struct line* text, size_t line1, size_t line2)
{
    assert(text != 0);

    char *str1 = text[line1].ptr;
    char *str2 = text[line2].ptr;
    int len1 = text[line1].lengh;
    int len2 = text[line2].lengh;

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
            return c2 - c1;
        }

        pos1--;
        pos2--;
    }

    return len2 - len1;
}


void change(struct line* text, size_t line1, size_t line2)
{
    assert (text != 0);

    char* s_ptr = text[line1].ptr;
    size_t s_lengh = text[line1].lengh;

    text[line1].ptr = text[line2].ptr;
    text[line1].lengh = text[line2].lengh;

    text[line2].ptr = s_ptr;
    text[line2].lengh = s_lengh;
}


void print_text(FILE* file, struct line* text, size_t linenum)
{
    assert (file != 0);
    assert (text != 0);
    assert (linenum != 0);

    for (size_t line = 0; line < linenum; line++) {
        for (size_t let = 0; let < text[line].lengh; let++)
            fprintf(file, "%c", *(text[line].ptr + let));
    }
}


