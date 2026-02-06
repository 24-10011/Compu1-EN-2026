///
/// arreglos
///

#include <cstdio>
#include <cmath>

// vector de origen en 4 dimensiones
double o[] = { 0.0, 0.0, 0.0, 0.0 };

double a[] = { 1.0, 1.0, 1.0, 1.0 };
double b[] = { 2.2, 0.0, 1.1, 3.3 };

double x[] = { 1.0, 2.1, 3.0, 5.1, 2.0, 0.0, 6.2, 7.3 };

const int No = sizeof(o) / sizeof(o[0]);
const int Nx = sizeof(x) / sizeof(x[0]);

// 'distance': computa la distancia euclidiana de dos vectores de 'n' dimensiones
double distance(int n, double v[], double w[]) {
    double sum_sq = 0.0;
    for (int i = 0; i < n; ++i) {
        double d = v[i] - w[i];
        sum_sq += d * d;
    }
    double result = std::sqrt(sum_sq);
    return result;
}

// 'vec_sum': calcula la suma de dos vectores de 'n' dimensiones
void vec_sum(int n, double v[], double w[], double r[]) {
    for (int i = 0; i < n; ++i) {
        r[i] = v[i] + w[i];
    }
}

// 'show' muestra un vector 'v', de 'n' dimensiones, en la pantalla
void show(int n, double v[], const char *sep) {
    fprintf(stdout, "[ ");
    for (int i = 0; i < n; ++i) {
        fprintf(stdout, "%f%s", v[i], sep);
    }
    fprintf(stdout, " ]\n");
}

// 'min' retorna el índice del menor valor en un subrango de un arreglo
int min(int i, int n, double a[]) {
    int k = i; // índice del mínimo actual
    for (int j = i + 1; j < n; ++j) {
        if (a[j] < a[k]) {
            k = j;
        }
    }
    return k;
}

// 'copy_array': copia un arreglo a otro
void copy_array(int n, double a[], double r[]) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i];
    }
}

// 'sort': ordena un arreglo 'a', de tamaño 'n' dejando el resultado en otro arreglo
void sort(const int n, double a[], double r[]) {
    // Primero copiamos 'a' a 'r' (para no modificar el original)
    copy_array(n, a, r);

    // Selection sort usando min(i, n, r)
    for (int i = 0; i < n; ++i) {
        int k = min(i, n, r);     // índice del mínimo en el rango [i, n)
        // intercambiar r[i] con r[k]
        double tmp = r[i];
        r[i] = r[k];
        r[k] = tmp;
    }
}

// Lo mismo que `sort`, solo que usando inteligencia artificial.
void sort_AI(const int n, double a[], double r[]) {
    // IA usada: ChatGPT (OpenAI).
    // Insight: implementar selection sort copiando primero el arreglo para no mutar el original
    // y reutilizar min(i,n,r) para hacer el código más claro.

    copy_array(n, a, r);

    for (int i = 0; i < n; ++i) {
        int k = min(i, n, r);
        double tmp = r[i];
        r[i] = r[k];
        r[k] = tmp;
    }
}

// recomendamos que escriban sus propias pruebas
void test_distance() {
    fprintf(stdout, "test_distance\n");

    double ao = distance(No, a, o);
    fprintf(stdout, "a <-> o ==> %f\n", ao);

    double bo = distance(No, b, o);
    fprintf(stdout, "b <-> o ==> %f\n", bo);

    double ab = distance(No, a, b);
    fprintf(stdout, "a <-> b ==> %f\n", ab);

    double ba = distance(No, b, a);
    fprintf(stdout, "b <-> a ==> %f\n", ba);
}

void test_vec_sum() {
    fprintf(stdout, "test_vec_sum\n");
    double r[No];

    vec_sum(No, a, b, r); show(No, r, " ");
    vec_sum(No, a, o, r); show(No, r, " ");
    vec_sum(No, o, b, r); show(No, r, " ");
    vec_sum(No, b, a, r); show(No, r, " ");
}

void test_sort() {
    fprintf(stdout, "test_sort\n");
    double r[Nx];
    sort(Nx, x, r);
    show(Nx, x, " ");
    show(Nx, r, " ");
}

int main(int argc, const char *argv[]) {
    test_distance();
    test_vec_sum();
    test_sort();
    return 0;
}
