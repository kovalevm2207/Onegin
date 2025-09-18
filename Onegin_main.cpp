// to do : %p; +
// todo передать имя файла +
// todo refactor lcomp +
// todo: прочитай что такое isalpha() +
// todo: препроцессирование в отдельную функцию: +
//       копировать в новый буфер +
//       два указателя (если будешь успевать) - так как мне нужен буфер, в котором ничего не поменялось



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
    size_t lenghfix;
    char* ptr;
    char* ptrfix;
};

long long find_file_size(const char* file_name);

size_t count_lines(char* buffer);

void get_text(struct line*  text, char* fixed_buffer, char* buffer);

void fix_buffer (char* fixed_buffer, const char* buffer);

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

    char* fixed_buffer = (char*) calloc(file_size, sizeof(char));
    assert(fixed_buffer != 0);
    fix_buffer(fixed_buffer, buffer);



    size_t linenum = count_lines(buffer);
    assert(linenum > 0);

    struct line* text = (struct line*) calloc(linenum, sizeof(line));
    assert(text != 0);
    get_text(text, fixed_buffer, buffer);

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

    fclose(encyclopedia);

    free(text);
    free(fixed_buffer);
    free(buffer);
}




long long find_file_size(const char* file_name)
{
    assert (file_name != 0);

    struct stat file_info;
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


void fix_buffer (char* fixed_buffer, const char* buffer)
{
    assert (fixed_buffer != 0);
    assert (buffer != 0);

    size_t fix = 0;
    for (size_t base = 0; buffer[base] != '\0'; base++) {
        char c = buffer[base];
        if (isalpha(c) != 0 || c == '\n') {
            fixed_buffer[fix] = c;
            fix++;
        }
    }
    fixed_buffer[fix] = '\0';
}


void get_text(struct line* text, char* fixed_buffer, char* buffer)
{
    assert (text != 0);
    assert (fixed_buffer != 0);
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

    size_t fixcount = 0, fixptr_shift = 0;

    for (size_t i = 0; fixed_buffer[i] != '\0'; i++) {
        if (fixed_buffer[i] == '\n') {
            text[fixcount].ptrfix = fixed_buffer + fixptr_shift;
            text[fixcount].lenghfix = (i + 1) - fixptr_shift;
            fixptr_shift = i + 1;
            fixcount++;
        }
    }
    assert(fixcount == count);
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

    int pos = 0;
    char *str1 = text[line1].ptrfix, *str2 = text[line2].ptrfix;
    while (str1[pos] != '\n' && str2[pos] != '\n') {
        char c1 = tolower(str1[pos]);
        char c2 = tolower(str2[pos]);

        if (c1 < c2) return 1;
        if (c1 > c2) return -1;

        pos++;
    }

    // Если одна строка короче другой (раньше заканчивается)
    if (str1[pos] == '\n' && str2[pos] != '\n') return 1;
    if (str2[pos] == '\n' && str1[pos] != '\n') return -1;

    // Если строки идентичны
    return 0;

}


int rcomp(struct line* text, size_t line1, size_t line2)
{
    assert(text != 0);

    char *str1 = text[line1].ptrfix, *str2 = text[line2].ptrfix;
    size_t lengh1 = text[line1].lenghfix, lengh2 = text[line2].lenghfix;

    for (size_t pos = 0; pos < (lengh2 - 1) && pos < (lengh2 - 1); pos++) {
        // Берем символы с конца строк
        int c1 = tolower(str1[lengh1 - 2 - pos]);
        int c2 = tolower(str2[lengh2 - 2 - pos]);

        if (c1 < c2) return 1;
        if (c1 > c2) return -1;
    }

    // Если одна строка короче другой
    if (lengh1 < lengh2) return 1;
    if (lengh2 < lengh1) return -1;

    // Если строки идентичны
    return 0;
}


void change(struct line* text, size_t line1, size_t line2)
{
    assert (text != 0);

    char* s_ptr = text[line1].ptr, *s_ptrfix = text[line1].ptrfix;
    size_t s_lengh = text[line1].lengh, s_lenghfix = text[line1].lenghfix;

    text[line1].ptr = text[line2].ptr; text[line1].ptrfix = text[line2].ptrfix;
    text[line1].lengh = text[line2].lengh; text[line1].lenghfix = text[line2].lenghfix;

    text[line2].ptr = s_ptr; text[line2].ptrfix = s_ptrfix;
    text[line2].lengh = s_lengh; text[line2].lenghfix = s_lenghfix;
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
