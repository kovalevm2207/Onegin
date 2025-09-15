#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
// #include <unistd.h>

size_t count_lines(char* buffer);
size_t find_maxline(char* buffer);
void get_lines(char* Onegin_lines1, size_t maxsize, char* buffer);
void check_print(char* Onegin_lines1,size_t number_of_start, size_t number_of_end, size_t maxsize);

int main()
{
    //--------------ВЫДЕЛЕНИЕ ПАМЯТИ----------------------------------------------------------------------------------------------------

    // сначала найдем размер памяти, которую нам надо выделить для записи наешего текста

    struct stat file_info;
    if (stat("Онегин отрывок.txt", &file_info) == 0) {
        printf("Размер файла в байтах: %lld\n", (long long) file_info.st_size);
    } else {
        printf("Ошибка при выводе размера файла\n");
    }

    // теперь мы можем выделить память под этот текст и нулевой байт в конце с помощью calloc()

    char* buffer = (char*) calloc(file_info.st_size + 1, sizeof(char));
    if (buffer == NULL) {
        printf("Ошибка выделения памяти");
        return 1;
    }
    printf("Адресс начала буффера = %ld\n", buffer);

    //--------------------------------------------ЧТЕНИЕ ФАЙЛА-------------------------------------------------------------------------

    FILE* Onegin = fopen("Онегин отрывок.txt", "rb");
    if(Onegin == NULL) {
        printf("Ошибка при открытии файла файла\n");
        return 1;
    }

    if (fread (buffer, sizeof(char),file_info.st_size, Onegin) == 0) {
        printf("Ошибки в чтении данных с файла");
        return 1;
    }
    buffer[file_info.st_size] = '\0';

    fclose(Onegin);

    //-------------------------------------РАЗБИЕНИЕ ПО СТРОКАМ-------------------------------------------------------------------------------------

    // самая длинная строчка задает размер подмассива, а количество ячеек основного массива совпадает с количеством строк
    // получается нам надо узнать количество строк и длинну самой длинной строки
    size_t number_of_lines = count_lines(buffer), maxsize = find_maxline(buffer);
    // проверка
    printf("В веденном тексте количество строк = %ld\n", number_of_lines);
    printf("Максимальная длинна строки = %ld\n\n", maxsize);

    // теперь можно создать двумерный массив, в котором у нас будет храниться текст построчно
    char* Onegin_list1 = (char*) calloc((maxsize+1)*number_of_lines, sizeof(char));
    // заполним его
    get_lines(Onegin_list1, maxsize, buffer);
    // попробуем распечатать с какой-то по какую-то строчку нашего массива
    check_print(Onegin_list1, 100, 125, maxsize);



    //------------------------------------------------------КОНЕЦ------------------------------------------------------------------------------------
    //  не забудем в конце очистить память
    free(buffer);
    free(Onegin_list1);

    return 0;
}



size_t count_lines(char* buffer)
{
    assert (buffer != 0);

    size_t number_of_lines = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            number_of_lines++;
        }
    }
    return number_of_lines;
}



size_t find_maxline(char* buffer)
{
    assert (buffer != 0);

    size_t maxsize = 0, current_size = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            if (current_size >= maxsize) {
                maxsize = current_size;
            }
            current_size = 0;
        } else {
            current_size++;
        }
    }
    return maxsize;
}




void get_lines(char* Onegin_lines1, size_t maxsize, char* buffer)
{
    assert ((Onegin_lines1 != 0) && (buffer != 0));

    int line = 0, sumbol = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] != '\n') {
            *(char*) ((size_t)Onegin_lines1 + (maxsize*line + sumbol)*sizeof(char)) = buffer[i];
            sumbol++;
        } else {
            *(char*) ((size_t)Onegin_lines1 + (maxsize*line + sumbol)*sizeof(char)) = '\0';
            sumbol = 0;
            line++;
        }
    }
}



void check_print(char* Onegin_lines1,size_t number_of_start, size_t number_of_end, size_t maxsize)
{
    assert (Onegin_lines1 != 0);
    assert (number_of_start <= number_of_end);

    for (size_t line = number_of_start - 1; line < number_of_end; line++) {
        for (size_t sumbol = 0; sumbol < maxsize; sumbol++) {
            printf("%c", *(char*)((size_t)Onegin_lines1 + (line*maxsize + sumbol)*sizeof(char)));
        }
    printf("\n");
    }
}




































/*
    // Потренируемся выводить какойнибудь элемент бууфера для понимания работы

    printf("%c\n", buffer[1]);   // печататет вторую букву
    printf("%c\n", *(unsigned char*)((size_t)buffer + 0));  // печататет первую букву , но тут раскрыто ,что такое []
    //printf("%s\n", buffer);  // печататет весь текст

    int i = 0; while (buffer[i] != '\n') { // печатает первую строку
        printf("%c", buffer[i]);
        i++;
    }
    printf("\n\n");

    i = 0;
    while (buffer[i] != '\0') { // печатает весь текст
        printf("%c", buffer[i]);
        i++;
    }



    printf("\n");
*/
