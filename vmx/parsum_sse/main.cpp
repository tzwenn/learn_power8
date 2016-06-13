#include <pthread.h> 
#include <fstream>

#include "uint128_t.h"
#include "sumBlock256.h"

typedef struct {
    pthread_t thread;
	uint64_t start;
	uint64_t end;
	uint128_t result;
} ThreadData;

////////////////////////////////////////////////////////////////////////////////

inline void sum(uint64_t &start, uint64_t &end, uint128_t &result)
{
    static const int blockSize = 256;

    // Sum up blocks of 256 numbers
    for (int i = (end - start) / blockSize; i > 0; i--, start += blockSize)
        sumBlock256(start, result);
    
    // Sum up the rest in the normal way
    for (; start <= end; start++)
        result += start;
}

////////////////////////////////////////////////////////////////////////////////

void * worker(void *data)
{
    sum(((ThreadData *) data)->start,
        ((ThreadData *) data)->end,
        ((ThreadData *) data)->result);

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    if (argc != 4)
	{
		printf("Usage: %s threads start end\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    const int n = atoi(argv[1]);
    const uint64_t start = atol(argv[2]);
    const uint64_t end = atol(argv[3]);
    const uint64_t sectionSize = (end - start) / n;

    ThreadData *data = new ThreadData[n];

    pthread_attr_t pthread_custom_attr;
    pthread_attr_init(&pthread_custom_attr);

    // Spawn worker threads
    int i = 0;
    
    for (; i < n - 1; i++)
	{
		data[i].start = start + i * sectionSize;
		data[i].end = data[i].start + sectionSize - 1;
		data[i].result = 0;

		pthread_create(&data[i].thread, 
		    &pthread_custom_attr, 
            worker,
		    (void *) (data + i));
	}
	
	// The last one will get the rest
	data[i].start = start + i * sectionSize;
	data[i].end = end;
	data[i].result = 0;

	pthread_create(&data[i].thread, 
	    &pthread_custom_attr, 
	    worker, 
	    (void *) (data + i));
	
    // Wait for worker threads to complete and sum up the results
	pthread_join(data[0].thread, NULL);
	
	for (i = 1; i < n; i++)
	{
		pthread_join(data[i].thread, NULL);
		
		data[0].result += data[i].result;
	}

    // Print the result
    std::ofstream outputFile;
    outputFile.open ("output.txt");
    outputFile << data[0].result;
    outputFile.close();
	
	// Clean-up
	free(data);
	
	return EXIT_SUCCESS;
}
