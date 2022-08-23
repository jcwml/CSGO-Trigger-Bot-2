/*
--------------------------------------------------
    James William Fletcher (github.com/mrbid)
        AUGUST 2022

    Improved by Jim C. Williams. (github.com/jcwml)
--------------------------------------------------

    WINDOWS & LINUX

    Compile: clang main.c -Ofast -mavx -mfma -o aim

*/

#include <unistd.h>
#include <stdint.h>
#include <locale.h>

//#define FIXED_SEED 1947939716
//#define DO_VALIATION_TESTS

#ifdef __linux__
    //#define HIGH_ENTROPY_RANDOM // you cannot supply a seed to the high entropy random
    #define HIGH_ENTROPY_SEED
#endif

#if defined(HIGH_ENTROPY_RANDOM) || defined(HIGH_ENTROPY_SEED)
    #include <sys/file.h>
#endif

#ifdef __linux__
    #include <signal.h>
#endif
#define forceinline __attribute__((always_inline)) inline

//#define UNIFORM_GLOROT
#define ADA8
#define OPTIM_NAG
#define LEARNING_RATE 0.0001f
#define GAIN          0.0065f
#define SIGMOID_OUTPUT
#include "TBVGG3.h"
#define NORMALISE_INPUTS

#define TARGET_SAMPLES 7683
#define NONTARGET_SAMPLES 5179
#define LOSS_TARGET 0.3f
#define EPOCHS 33

#ifdef HIGH_ENTROPY_RANDOM
    #define TARGET_SAMPLES_FLOAT1 7682.f
    #define NONTARGET_SAMPLES_FLOAT1 5178.f
#endif

#define SAMPLE_SIZE 2352
const uint TOTAL_SAMPLES = TARGET_SAMPLES + NONTARGET_SAMPLES;
uint seed = 0;
time_t tt = 0;
uint ei = 0;
float ael = 0.f;

float targets[TARGET_SAMPLES][3][28][28];
float nontargets[NONTARGET_SAMPLES][3][28][28];
TBVGG3_Network net;

///

#ifdef HIGH_ENTROPY_RANDOM
static inline float urandf() // 0 to 1
{
    static const float FLOAT_UINT64_MAX = 1.f/(float)UINT64_MAX;
    int f = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
    uint64_t s = 0;
    ssize_t result = read(f, &s, sizeof(uint64_t));
    close(f);
    return (((float)s)+1e-7f) * FLOAT_UINT64_MAX;
}

static forceinline uint qRand(const float min, const float max)
{
    return urandf() * (max-min) + min;
}
#endif

static forceinline uint uRand(const uint min, const uint max)
{
    static float rndmax = 1.f/(float)RAND_MAX;
    return (((float)rand()) * rndmax) * (max-min) + min;
}

void generate_output(int sig_num)
{
#ifdef DO_VALIATION_TESTS
    // test targets
    float s = 0.f;
    for(int i = 0; i < TARGET_SAMPLES; i++)
    {
        s += TBVGG3_Process(&net, targets[i], NO_LEARN);
    }
    printf("targets: %f / %u\n", s, TARGET_SAMPLES);

    // test nontargets
    s = 0.f;
    for(int i = 0; i < NONTARGET_SAMPLES; i++)
    {
        s += TBVGG3_Process(&net, nontargets[i], NO_LEARN);
    }
    printf("nontargets: %f / %u\n", s, NONTARGET_SAMPLES);
#endif

    // save network
    TBVGG3_SaveNetwork(&net, "network.save");

    // print seed again & time taken for execution
    printf("Random Seed: %'u\n", seed);
    printf("Time Taken: %'lu\n", time(0)-tt);

    // dump results to file for github markdown table
    // should flock()
    if(ael < LOSS_TARGET)
    {
        FILE* f = fopen("results.txt", "a");
        if(f != NULL)
        {
            fprintf(f, "| %'u | %u | %'lu sec (%.2f mins) | %f |\n", seed, ei, time(0)-tt, (((float)(time(0)-tt))/60.f), ael);
            fclose(f);
        }
    }

    // done
    exit(0);
}

int main()
{
    // time taken (start time) for entire execution
    tt = time(0);

#ifdef __linux__
    // ctrl+c callback
    signal(SIGINT, generate_output);
#endif

    // seed random
#ifdef FIXED_SEED
    seed = FIXED_SEED;
#else
    #ifdef HIGH_ENTROPY_SEED
        int f = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
        ssize_t result = read(f, &seed, sizeof(unsigned int));
        close(f);
    #else
        seed = time(0);
    #endif
#endif
    srand(seed);
    setlocale(LC_NUMERIC, "");
    printf("Random Seed: %'u\n\n", seed);

    // load targets
    printf("loading target data\n");
    for(int i = 0; i < TARGET_SAMPLES; i++)
    {
        char fn[256];
        sprintf(fn, "target/%i.ppm", i+1);
        FILE* f = fopen(fn, "rb");
        if(f)
        {
            // notify load
            printf("%s\n", fn);

            // seek past ppm header
            fseek(f, 13, SEEK_SET);

            // read bytes into temp buffer
            unsigned char tb[SAMPLE_SIZE];
            if(fread(&tb, 1, SAMPLE_SIZE, f) !=  SAMPLE_SIZE)
            {
                printf("read error\n");
                return 0;
            }

            // cast byte array to floats & normalise -1 to 1
            for(int y = 0; y < 28; y++)
            {
                for(int x = 0; x < 28; x++)
                {
                    const float r = (float)tb[(((28*y)+x)*3)];
                    const float g = (float)tb[(((28*y)+x)*3)+1];
                    const float b = (float)tb[(((28*y)+x)*3)+2];
#ifdef NORMALISE_INPUTS
                    targets[i][0][y][x] = (r-128.f)*0.0078125f;
                    targets[i][1][y][x] = (g-128.f)*0.0078125f;
                    targets[i][2][y][x] = (b-128.f)*0.0078125f;
#else
                    targets[i][0][y][x] = r;
                    targets[i][1][y][x] = g;
                    targets[i][2][y][x] = b;
#endif
                }
            }

            // done
            fclose(f);
        }
    }

    // load nontargets
    printf("\nloading nontarget data\n");
    for(int i = 0; i < TARGET_SAMPLES; i++)
    {
        char fn[256];
        sprintf(fn, "nontarget/%i.ppm", i+1);
        FILE* f = fopen(fn, "rb");
        if(f)
        {
            // notify load
            printf("%s\n", fn);

            // seek past ppm header
            fseek(f, 13, SEEK_SET);

            // read bytes into temp buffer
            unsigned char tb[SAMPLE_SIZE];
            if(fread(&tb, 1, SAMPLE_SIZE, f) !=  SAMPLE_SIZE)
            {
                printf("read error\n");
                return 0;
            }

            // cast byte array to floats & normalise -1 to 1
            for(int y = 0; y < 28; y++)
            {
                for(int x = 0; x < 28; x++)
                {
                    const float r = (float)tb[(((28*y)+x)*3)];
                    const float g = (float)tb[(((28*y)+x)*3)+1];
                    const float b = (float)tb[(((28*y)+x)*3)+2];
#ifdef NORMALISE_INPUTS
                    nontargets[i][0][y][x] = (r-128.f)*0.0078125f;
                    nontargets[i][1][y][x] = (g-128.f)*0.0078125f;
                    nontargets[i][2][y][x] = (b-128.f)*0.0078125f;
#else
                    nontargets[i][0][y][x] = r;
                    nontargets[i][1][y][x] = g;
                    nontargets[i][2][y][x] = b;
#endif
                }
            }

            // done
            fclose(f);
        }
    }

    // train
    printf("\ntraining network\n\n");
    TBVGG3_Reset(&net, seed);
    for(ei = 0; ei < EPOCHS; ei++)
    {
        float epoch_loss = 0.f;
        time_t st = time(0);
        for(int j = 0; j < TOTAL_SAMPLES; j++)
        {
#ifdef HIGH_ENTROPY_RANDOM
            float r = 0;
            if(qRand(0.f, 100.f) < 50)
                r = 1.f - TBVGG3_Process(&net, targets[qRand(0.f, TARGET_SAMPLES_FLOAT1)], LEARN_MAX);
            else
                r = TBVGG3_Process(&net, nontargets[qRand(0.f, NONTARGET_SAMPLES_FLOAT1)], LEARN_MIN);       
#else
            float r = 0;
            if(uRand(0, 100) < 50)
                r = 1.f - TBVGG3_Process(&net, targets[uRand(0, TARGET_SAMPLES-1)], LEARN_MAX);
            else
                r = TBVGG3_Process(&net, nontargets[uRand(0, NONTARGET_SAMPLES-1)], LEARN_MIN);
#endif
            epoch_loss += r;
            //printf("[%i] loss: %f\n", j, r);
        }
        printf("[%i] epoch loss: %f\n", ei, epoch_loss);
        ael = epoch_loss/TOTAL_SAMPLES;
        printf("[%i] avg epoch loss: %f\n", ei, ael);
        TBVGG3_Debug(&net);
        printf("[%i] SPS: %.2f\n\n", ei, ((float)TOTAL_SAMPLES)/((float)(time(0)-st))); // samples per second
        if(ael < LOSS_TARGET){break;} // break at loss target
    }

    // done
    generate_output(0);
    return 0;
}
