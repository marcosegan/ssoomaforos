#pragma once
/*
 * En uno de tus archivos .cpp, agregar (RESPETANDO EL ORDEN DE LAS LINEAS):
 *  #define _SSOOMAFOROS_IMPLEMENTACION // Esto va en un solo archivo
 *  #include "ssoomaforos.hpp"          // Esto va en todos los archivos
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <vector>

class Semaforo {
    sem_t s;
    bool inicializado = false;

    public:

    bool esValido() const { return inicializado; }
    Semaforo () = default;
    Semaforo (unsigned value) {
        *this = value;
    }
    void operator = (unsigned value) {
        inicializado = true;
        int error = sem_init(&s, 0, value);
        if (error != 0)
            perror("sem_init");
    }

    sem_t *get_sem() {
        return &s;
    }

    // Inicializa un array
    static void set(Semaforo s[], size_t count, unsigned value) {
        for (size_t i = 0; i < count; i++)
            s[i] = value;
    }
};

void wait(Semaforo &s);
void signal(Semaforo &s);

class Hilos {
    public:
    static useconds_t intervalo_usleep;

    private:
    static std::vector<pthread_t> hilos;

    static void *funcion_generica(void *arg);
    static void *funcion_generica_bucle(void *arg);
    static void _lanzar_funcion(void (*funcion)(), void *(*f_generica)(void*));

    public:
    static void lanzar(void (*funcion)()) {
        _lanzar_funcion(funcion, funcion_generica);
    }
    static void lanzar_bucle(void (*funcion)()) {
        _lanzar_funcion(funcion, funcion_generica_bucle);
    }
    // Me gustaría que esto fuera ~Hilo() pero no funca :_(
    static void join();
};

#ifdef _SSOOMAFOROS_IMPLEMENTACION
#include <iostream>

std::vector<pthread_t> Hilos::hilos;
useconds_t Hilos::intervalo_usleep = 250*1000;

void wait(Semaforo &s) {
    if (!s.esValido())
        throw std::logic_error("wait: semáforo no inicializado");
    int error = sem_wait(s.get_sem());
    if (error != 0)
        perror("sem_wait");
}

void signal(Semaforo &s) {
    if (!s.esValido())
        throw std::logic_error("signal: semáforo no inicializado");
    int error = sem_post(s.get_sem());
    if (error != 0)
        perror("sem_post");
}

void *Hilos::funcion_generica(void *arg) {
    void (*f)() = (void (*)())arg;
    f();
    return NULL;
}
void *Hilos::funcion_generica_bucle(void *arg) {
    void (*f)() = (void (*)())arg;
    while (1) {
        f();
        usleep(intervalo_usleep);
    }
}
void Hilos::_lanzar_funcion(void (*funcion)(), void *(*f_generica)(void*)) {
    pthread_t hilo;
    int error = pthread_create(&hilo, NULL, f_generica, (void*)funcion);
    if (error != 0)
        perror("pthread_create");
    hilos.push_back(hilo);
}
void Hilos::join()
{
    for (auto hilo : hilos) {
        int error = pthread_join(hilo, NULL);
        if (error != 0)
            perror("pthread_join");
    }
}
#endif
