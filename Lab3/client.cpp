#include <mpi.h>
#include <fstream>
#include <iostream>

#define FILENAME "file_to_send.txt"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Client code
        std::ifstream file(FILENAME, std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for reading." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        file.seekg(0, std::ios::end);
        int file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        char* buffer = new char[file_size];
        file.read(buffer, file_size);

        MPI_Bcast(&file_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(buffer, file_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        file.close();
        delete[] buffer;

        std::cout << "File transfer successful." << std::endl;
    } else {
        // Non-client processes
        int file_size = 0;
        MPI_Bcast(&file_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        char* buffer = new char[file_size];
        MPI_Bcast(buffer, file_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        std::ofstream file(FILENAME, std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for writing." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        file.write(buffer, file_size);
        file.close();

        delete[] buffer;
    }

    MPI_Finalize();
    return 0;
}