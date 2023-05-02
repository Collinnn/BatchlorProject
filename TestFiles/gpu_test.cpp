#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv) {

    size_t N = 1000;
    double* A = new double[N];
    double* B = new double[N];
    double* C = new double[N];

    double* C_device = (double*)omp_target_alloc(N * sizeof(double), omp_get_default_device());

    int num_devices = omp_get_num_devices();
    int default_device = omp_get_default_device();
    int host_device = omp_get_initial_device();

    printf("num_devices: %d, default_device: %d, host_device: %d\n", num_devices, default_device, host_device);

    // Initialize arrays
    for (size_t i = 0; i < N; i++) {
        A[i] = i;
        B[i] = i;
    }

    // Initialize particles

    // PIC
    // - Move particles from forces from the grid/mesh
    // - Update grid from the includes particles
    // - Compute forces using poisson
    // - Update particle forces from the grid/mesh


    // Compute C = A + B
    #pragma omp target teams distribute parallel for map(to: A[0:N], B[0:N]) is_device_ptr(C_device)
    for (size_t i = 0; i < N; i++) {
        C_device[i] = A[i] + B[i];
    }

    // Copy result back to host
    omp_target_memcpy(C, C_device, N * sizeof(double), 0, 0, host_device, default_device);

    // Print the result
    for (size_t i = 0; i < N; i++) {
        printf("%f + %f = %f\n", A[i], B[i], C[i]);
    }

    delete[] A;
    delete[] B;
    delete[] C;
}