#include <iostream>
#include <string>
#include <algorithm>
#include <omp.h>

class ArrayOMP {
  private:
    char *array;
    int index, nThreads, wSize, chunk;
    bool mutex, runtime;
    omp_sched_t sched;

    void spendSomeTime() {
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 100; j++) {
                // These loops shouldn't be removed by the compiler
            }
        }
    }

  public:
    ArrayOMP(int nThreads,
             int wSize,
             bool mutex,
             omp_sched_t sched,
             int chunk = -1, bool run = false) {
        this->nThreads = nThreads;
        this->wSize = wSize;
        this->chunk = chunk;
        this->sched = sched;
        this->mutex = mutex;
        this->runtime = run;

        index = 0;
        array = new char[wSize];
        std::fill(array, array + wSize, '-');
    }
    void invert() {
        std::fill(array, array + wSize, '-');
        index = 0;
        mutex = !mutex;
    }
    void fillArrayConcurrently() {
        if (!runtime)
            omp_set_schedule(sched, chunk);

        if (mutex) {
#pragma omp parallel for schedule(runtime) shared(array, index)
            for (int i = 0; i < wSize; i++) {
#pragma omp critical
                {
                    array[index] = (char)('A' + omp_get_thread_num());
                    spendSomeTime();
                    index++;
                }
            }
        } else {
#pragma omp parallel for schedule(runtime) shared(array, index)
            for (int i = 0; i < wSize; i++) {
                array[index] = (char)('A' + omp_get_thread_num());
                spendSomeTime();
                index++;
            }
        }
    }
    void printStats() {
        if (!runtime)
            switch (sched) {
            case omp_sched_static:
                std::cout << "static  ";
                break;
            case omp_sched_dynamic:
                std::cout << "dynamic ";
                break;
            case omp_sched_auto:
                std::cout << "auto    ";
                break;
            case omp_sched_guided:
                std::cout << "guided  ";
                break;
            default:
                break;
            }
        else{
            std::cout << "runtime ";
        }
        
        std::cout << (mutex ? "mutex " : "nope  ");
        std::cout << (chunk != -1 ? "chunk " : "nope  ");
        std::string str = std::string(array, wSize);
        std::cout << str << ' ';
        char c = 'A';
        for (int i = 0; i < nThreads; i++, c++)
            std::cout << c << "=" << std::count(str.begin(), str.end(), c) << " ";

        std::cout << std::endl;
    }
    ~ArrayOMP() {
        delete array;
    }
};

int main(int argc, char const *argv[]) {
    int nthreads, worksize, chunk;

    if (argc != 4) {
        std::cout << "Uso:" << argv[0] << " <nthreads> <worksize> <chunk>" << std::endl;
        exit(EXIT_FAILURE);
    }

    nthreads = atoi(argv[1]); // nthreads = total de threads
    worksize = atoi(argv[2]); // worksize = tamanho do array
    chunk = atoi(argv[3]);    // chunk = carga de trabalho

    omp_set_num_threads(nthreads);
    // RUNTIME COM E SEM MUTEX 
    ArrayOMP run(nthreads, worksize, true, omp_sched_auto, -1, true);
    run.fillArrayConcurrently();
    run.printStats();
    run.invert();
    run.fillArrayConcurrently();
    run.printStats();

    // STATIC COM MUTEX, COM E SEM CHUNK
    ArrayOMP sta_ch(nthreads, worksize, true, omp_sched_static, chunk);
    ArrayOMP sta(nthreads, worksize, true, omp_sched_static);
    sta_ch.fillArrayConcurrently();
    sta.fillArrayConcurrently();
    sta_ch.printStats();
    sta.printStats();
    // DYNAMIC COM MUTEX, COM E SEM CHUNK
    ArrayOMP dyn_ch(nthreads, worksize, true, omp_sched_dynamic, chunk);
    ArrayOMP dyn(nthreads, worksize, true, omp_sched_dynamic);
    dyn_ch.fillArrayConcurrently();
    dyn.fillArrayConcurrently();
    dyn_ch.printStats();
    dyn.printStats();
    // AUTO COM MUTEX
    ArrayOMP aut(nthreads, worksize, true, omp_sched_auto);
    aut.fillArrayConcurrently();
    aut.printStats();
    // GUIDED COM MUTEX, COM E SEM CHUNK
    ArrayOMP gui_ch(nthreads, worksize, true, omp_sched_guided, chunk);
    ArrayOMP gui(nthreads, worksize, true, omp_sched_guided);
    gui_ch.fillArrayConcurrently();
    gui.fillArrayConcurrently();
    gui_ch.printStats();
    gui.printStats();
    
    // STATIC SEM MUTEX, COM E SEM CHUNK
    sta_ch.invert();
    sta.invert();
    sta_ch.fillArrayConcurrently();
    sta.fillArrayConcurrently();
    sta_ch.printStats();
    sta.printStats();
    // DYNAMIC SEM MUTEX, COM E SEM CHUNK
    dyn.invert();
    dyn_ch.invert();
    dyn_ch.fillArrayConcurrently();
    dyn.fillArrayConcurrently();
    dyn_ch.printStats();
    dyn.printStats();
    // AUTO SEM MUTEX
    aut.invert();
    aut.fillArrayConcurrently();
    aut.printStats();
    // GUIDED SEM MUTEX, COM E SEM CHUNK
    gui.invert();
    gui_ch.invert();
    gui_ch.fillArrayConcurrently();
    gui.fillArrayConcurrently();
    gui_ch.printStats();
    gui.printStats();

    return EXIT_SUCCESS;
}
