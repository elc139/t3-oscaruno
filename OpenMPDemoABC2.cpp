#include <iostream>
#include <string>
#include <algorithm>
#include <omp.h>

class SharedArray {
  private:
    char *array;
    int index;
    int size;
    bool usemutex;

    void spendSomeTime() {
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 100; j++) {
                // These loops shouldn't be removed by the compiler
            }
        }
    }

  public:
    SharedArray(int n, bool use) {
        size = n;
        index = 0;
        usemutex = use;
        array = new char[size];
        std::fill(array, array + size, '-');
    }
    ~SharedArray() {
        delete[] array;
    }
    void addChar(char c) {
        if (usemutex) {
#pragma omp ordered
            {
                array[index] = c;
                spendSomeTime();
                index++;
            }
        } else {
            array[index] = c;
            spendSomeTime();
            index++;
        }
    }
    void reset(bool invert) {
        this->usemutex = invert ? !usemutex : usemutex;
        std::fill(array, array + size, '-');
        index = 0;
    }
    int countOccurrences(char c) {
        return std::count(array, array + size, c);
    }
    std::string toString() {
        return std::string(array, size);
    }
};

class ArrayFiller {
  private:
    int nThreads, wSize, chunk;
    omp_sched_t sched;
    SharedArray *array;
    bool runtime;

  public:
    ArrayFiller(int nThreads,
                int wSize,
                bool usemutex,
                omp_sched_t sched,
                int chunk = -1, bool run = false) {

        this->nThreads = nThreads;
        this->wSize = wSize;
        this->chunk = chunk;
        this->sched = sched;
        this->runtime = run;
        array = new SharedArray(wSize, usemutex);
    }
    void fillArrayConcurrently() {
        if (!runtime)
            omp_set_schedule(sched, chunk);

#pragma omp parallel for schedule(runtime) shared(array) ordered
        for (int i = 0; i < wSize; i++) {
            array->addChar('A' + omp_get_thread_num());
        }
    }
    void printStats() {
        std::cout << array->toString() << ' ';
        char c = 'A';
        for (int i = 0; i < nThreads; ++i, ++c)
            std::cout << c << "="
                      << array->countOccurrences(c) << " ";
        std::cout << std::endl;
    }
    void invert() {
        array->reset(true);
    }
    ~ArrayFiller() {
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
    ArrayFiller run(nthreads, worksize, true, omp_sched_auto, -1, true);
    run.fillArrayConcurrently();
    std::cout << "runtime mutex nope  ";
    run.printStats();
    run.invert();
    run.fillArrayConcurrently();
    std::cout << "runtime nope  nope  ";
    run.printStats();
    // STATIC COM MUTEX, COM E SEM CHUNK
    ArrayFiller sta_ch(nthreads, worksize, true, omp_sched_static, chunk);
    ArrayFiller sta(nthreads, worksize, true, omp_sched_static);
    sta_ch.fillArrayConcurrently();
    sta.fillArrayConcurrently();
    std::cout << "static  mutex chunk ";
    sta_ch.printStats();
    std::cout << "static  mutex nope  ";
    sta.printStats();
    // DYNAMIC COM MUTEX, COM E SEM CHUNK
    ArrayFiller dyn_ch(nthreads, worksize, true, omp_sched_dynamic, chunk);
    ArrayFiller dyn(nthreads, worksize, true, omp_sched_dynamic);
    dyn_ch.fillArrayConcurrently();
    dyn.fillArrayConcurrently();
    std::cout << "dynamic mutex chunk ";
    dyn_ch.printStats();
    std::cout << "dynamic mutex nope  ";
    dyn.printStats();
    // AUTO COM MUTEX, COM E SEM CHUNK
    ArrayFiller aut(nthreads, worksize, true, omp_sched_auto);
    aut.fillArrayConcurrently();
    std::cout << "auto    mutex nope  ";
    aut.printStats();
    // GUIDED COM MUTEX, COM E SEM CHUNK
    ArrayFiller gui_ch(nthreads, worksize, true, omp_sched_guided, chunk);
    ArrayFiller gui(nthreads, worksize, true, omp_sched_guided);
    gui_ch.fillArrayConcurrently();
    gui.fillArrayConcurrently();
    std::cout << "guided  mutex chunk ";
    gui_ch.printStats();
    std::cout << "guided  mutex nope  ";
    gui.printStats();

    // STATIC SEM MUTEX, COM E SEM CHUNK
    sta_ch.invert();
    sta.invert();
    sta_ch.fillArrayConcurrently();
    sta.fillArrayConcurrently();
    std::cout << "static  nope  chunk ";
    sta_ch.printStats();
    std::cout << "static  nope  nope  ";
    sta.printStats();
    // DYNAMIC SEM MUTEX, COM E SEM CHUNK
    dyn.invert();
    dyn_ch.invert();
    dyn_ch.fillArrayConcurrently();
    dyn.fillArrayConcurrently();
    std::cout << "dynamic nope  chunk ";
    dyn_ch.printStats();
    std::cout << "dynamic nope  nope  ";
    dyn.printStats();
    // AUTO SEM MUTEX
    aut.invert();
    aut.fillArrayConcurrently();
    std::cout << "auto    nope  nope  ";
    aut.printStats();
    // GUIDED SEM MUTEX, COM E SEM CHUNK
    gui.invert();
    gui_ch.invert();
    gui_ch.fillArrayConcurrently();
    gui.fillArrayConcurrently();
    std::cout << "guided  nope  chunk ";
    gui_ch.printStats();
    std::cout << "guided  nope  nope  ";
    gui.printStats();
    return EXIT_SUCCESS;
}
