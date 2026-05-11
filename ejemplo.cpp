#define _SSOOMAFOROS_IMPLEMENTACION
#include "ssoomaforos.hpp"

Semaforo
    s1 = 1,
    s2 = 0;

int main() {
    Hilos::intervalo_usleep = 100*1000;
    // Desactivar el line buffering (largo de explicar, pero así se ve el ABABAB... en pantalla)
    setvbuf(stdout, NULL, _IONBF, 0);

    Semaforo array_semaforos_1[10];
    Semaforo::set(array_semaforos_1, 10, 0);
    Semaforo array_semaforos_2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    (void)array_semaforos_2;

    Hilos::lanzar(
        // Se pueden usar lambdas o punteros a funciones :P
        [](){
            puts("Hola, esto se muestra una sola vez :D");
        }
    );

    void (*hilos[])() = {
        [](){
            wait(s1);
            printf("A");
            signal(s2);
        },
        [](){
            wait(s2);
            printf("B");
            signal(s1);
        },
    };
    for (auto h : hilos)
        Hilos::lanzar_bucle(h);

    Hilos::join();
}
