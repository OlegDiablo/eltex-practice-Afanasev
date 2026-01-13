#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define OUTPUT_FILE "counter.log"
#define _XOPEN_SOURCE 700

FILE *file = NULL;
volatile sig_atomic_t counter = 0;
volatile sig_atomic_t sigint_count = 0;
volatile sig_atomic_t should_exit = 0;

// Функция-обработчик сигналов
void signal_handler(int sig) {
    
    if (file != NULL) {
        if (sig == SIGINT) {
            fprintf(file, "Получен и обработан сигнал SIGINT (%d/3)\n", 
                     sigint_count + 1);
            fflush(file);
            sigint_count++;
            if (sigint_count >= 3) {
                should_exit = 1;
            }
        } else if (sig == SIGQUIT) {
            fprintf(file, "Получен и обработан сигнал SIGQUIT\n");
            fflush(file);
        }
    }
}

int main() {
    struct sigaction sa;
    
    // Настройка обработчика сигналов
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Установка обработчиков
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction SIGINT");
        return 1;
    }
    
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction SIGQUIT");
        return 1;
    }
    
    // Открытие файла
    file = fopen(OUTPUT_FILE, "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    
    printf("Программа запущена. PID: %d\n", getpid());
    printf("Файл вывода: %s\n", OUTPUT_FILE);
    printf("Для завершения отправьте 3 сигнала SIGINT (Ctrl+C)\n");
    
    // Основной цикл
    while (!should_exit) {
        counter++;
        
        if (file != NULL) {
            fprintf(file, "%d\n", counter);
            fflush(file); // Принудительная запись в файл
        }
        
        sleep(1);
    }
    
    // Завершение работы
    if (file != NULL) {
        fprintf(file, "Программа завершена после 3 сигналов SIGINT\n");
        fclose(file);
        file = NULL;
    }
    
    printf("Программа завершена\n");
    return 0;
}