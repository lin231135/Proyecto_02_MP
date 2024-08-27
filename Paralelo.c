/*
 * ------------------------------------------------------------
 * Paralelo.c
 * -------------------------------------------------------------
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * Programación de Microprocesadores
 *
 * Proyecto 02 - FASE 02
 * Descripción: Este programa en C simula la fisión nuclear en cadena
 *              utilizando OpenMP para paralelizar las operaciones.
 *              El programa calcula la diferencia de masa, la convierte
 *              a energía utilizando la ecuación E=mc^2, y simula la
 *              reacción en cadena de fisión nuclear de Uranio-235.
 *              Se permite al usuario ingresar la masa de Uranio-235 y
 *              se mide el tiempo de ejecución del programa.
 *
 * Autores:     Marcos Ambrocio - 231140
 *              Daniel Chet - 231177
 *              Javier Linares - 231135
 *              
 * Fecha:       Agosto 27, 2024
 * -------------------------------------------------------------
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h> // Librería de OpenMP

// Constantes
#define C 2.998e+8 // Velocidad de la luz en m/s
#define AVOGADRO 6.02214076e23 // Número de Avogadro

// Prototipos de funciones
double calcular_diferencia_masa(double masa_uranio_mol, double masa_neutron, double masa_Ba, double masa_Kr);
double convertir_masa_a_kg(double delta_masa);
double calcular_energia_por_fision(double delta_masa_kg);
double calcular_numero_atomos(double masa_uranio, double masa_uranio_mol);
void simular_fision_en_cadena(double num_atomos_uranio, double p_fision, int neutrones_por_fision, double energia_por_fision, double *num_atomos_fisionados, double *energia_total);

int main() {
    double masa_uranio; // Masa de Uranio-235 en gramos
    double masa_uranio_mol = 235.0439299; // Masa molar de Uranio-235 en gramos/mol
    double masa_neutron = 1.00866491588; // Masa del neutrón en unidades de masa atómica (u)
    double masa_Ba = 143.922953; // Masa del Bario-144 en unidades de masa atómica (u)
    double masa_Kr = 88.91763; // Masa del Kriptón-89 en unidades de masa atómica (u)
    double p_fision = 0.7; // Probabilidad de que un neutrón cause una fisión
    int neutrones_por_fision = 3; // Neutrones liberados por fisión

    // Solicitar al usuario la masa de Uranio-235
    printf("Ingrese la masa de Uranio-235 en gramos: ");
    scanf("%lf", &masa_uranio);

    // Para medir el tiempo de ejecución del programa
    double start = omp_get_wtime(); // Usar omp_get_wtime para medir el tiempo de ejecución

    // Calcular la diferencia de masa
    double delta_masa = calcular_diferencia_masa(masa_uranio_mol, masa_neutron, masa_Ba, masa_Kr);

    // Convertir la diferencia de masa a kilogramos
    double delta_masa_kg = convertir_masa_a_kg(delta_masa);

    // Calcular el número de átomos de Uranio-235
    double num_atomos_uranio = calcular_numero_atomos(masa_uranio, masa_uranio_mol);

    // Calcular la energía por fisión
    double energia_por_fision = calcular_energia_por_fision(delta_masa_kg);

    // Inicializar variables para simulación
    double num_atomos_fisionados = 0;
    double energia_total = 0.0;

    // Simulación de fisiones en cadena
    simular_fision_en_cadena(num_atomos_uranio, p_fision, neutrones_por_fision, energia_por_fision, &num_atomos_fisionados, &energia_total);

    // Calcular el tiempo de ejecución
    double end = omp_get_wtime();
    double tiempo_ejecucion = end - start;

    // Mostrar resultados
    printf("Numero de atomos de Uranio-235: %.2e\n", num_atomos_uranio);
    printf("Numero de atomos de Uranio-235 fisionados: %.2e\n", num_atomos_fisionados);
    printf("Energia total generada por la fision: %.2e julios\n", energia_total);
    printf("Tiempo de ejecucion del programa: %.2f segundos\n", tiempo_ejecucion);

    return 0;
}

/**
 * Calcular la diferencia de masa entre los reactivos y productos de la fisión nuclear.
 * @param masa_uranio_mol Masa molar de Uranio-235 en gramos/mol.
 * @param masa_neutron Masa del neutrón en unidades de masa atómica (u).
 * @param masa_Ba Masa del Bario-144 en unidades de masa atómica (u).
 * @param masa_Kr Masa del Kriptón-89 en unidades de masa atómica (u).
 * @return Diferencia de masa en unidades de masa atómica (u).
 */
double calcular_diferencia_masa(double masa_uranio_mol, double masa_neutron, double masa_Ba, double masa_Kr) {
    // Calcular la masa total de los reactivos y productos
    double masa_reactivos = masa_uranio_mol + masa_neutron; // Masa del Uranio-235 y un neutrón
    double masa_productos = masa_Ba + masa_Kr + 3 * masa_neutron; // Masa de los productos

    // Calcular la diferencia de masa (en unidades de masa atómica, u)
    return masa_reactivos - masa_productos;
}

/**
 * Convertir la diferencia de masa de unidades de masa atómica (u) a kilogramos (kg).
 * @param delta_masa Diferencia de masa en unidades de masa atómica (u).
 * @return Diferencia de masa en kilogramos (kg).
 */
double convertir_masa_a_kg(double delta_masa) {
    return delta_masa / 6.022e+26; // Conversión de u a kg
}

/**
 * Calcular la energía liberada por fisión nuclear utilizando la ecuación E=mc^2.
 * @param delta_masa_kg Diferencia de masa en kilogramos (kg).
 * @return Energía liberada por fisión en julios (J).
 */
double calcular_energia_por_fision(double delta_masa_kg) {
    return delta_masa_kg * pow(C, 2); // Energía por fisión en julios
}

/**
 * Calcular el número de átomos de Uranio-235 en una muestra dada su masa.
 * @param masa_uranio Masa de Uranio-235 en gramos.S
 * @param masa_uranio_mol Masa molar de Uranio-235 en gramos/mol.
 * @return Número de átomos de Uranio-235.
 */
double calcular_numero_atomos(double masa_uranio, double masa_uranio_mol) {
    double moles_uranio = masa_uranio / masa_uranio_mol; // Calcular el número de moles de Uranio-235
    return moles_uranio * AVOGADRO; // Calcular el número de átomos de Uranio-235
}

/**
 * Simular la fisión nuclear en cadena.
 * @param num_atomos_uranio Número de átomos de Uranio-235.
 * @param p_fision Probabilidad de que un neutrón cause una fisión.
 * @param neutrones_por_fision Número de neutrones liberados por fisión.
 * @param energia_por_fision Energía liberada por fisión en julios (J).
 * @param num_atomos_fisionados Puntero al número total de átomos fision.
 * @adosparam energia_total Puntero a la energía total generada por la fisión.
 */
void simular_fision_en_cadena(double num_atomos_uranio, double p_fision, int neutrones_por_fision, 
                        double energia_por_fision, double *num_atomos_fisionados, double *energia_total) {
    int neutrones_libres = 1; // Empezamos con un neutrón que inicia la primera fisión
    srand(time(NULL)); // Inicializar la semilla para números aleatorios

    while (neutrones_libres > 0 && *num_atomos_fisionados < num_atomos_uranio) {
        int nuevos_neutrones_totales = 0;  // Variable para reducción global
        double num_atomos_fisionados_local = 0; // Variable local para reducción

        // Paralelización del bucle for con asignación dinámica
        #pragma omp parallel for reduction(+:num_atomos_fisionados_local, nuevos_neutrones_totales) schedule(dynamic)
        for (int i = 0; i < neutrones_libres; i++) {
            // Verificar si la fisión ocurre y actualizar las variables locales
            if ((*num_atomos_fisionados + num_atomos_fisionados_local) < num_atomos_uranio && ((double)rand() / RAND_MAX) < p_fision) {
                num_atomos_fisionados_local++; // Incrementar el número de átomos fisionados localmente
                nuevos_neutrones_totales += neutrones_por_fision; // Incrementar el número de neutrones liberados
            }
        }

        // Actualizar el número total de átomos fisionados de forma segura
        *num_atomos_fisionados += num_atomos_fisionados_local;

        // Actualizar el número de neutrones libres
        neutrones_libres = nuevos_neutrones_totales - neutrones_libres;
    }

    // Calcular la energía total generada
    *energia_total = energia_por_fision * (*num_atomos_fisionados);
}