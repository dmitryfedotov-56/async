    #define _CRT_SECURE_NO_WARNINGS

    #include <iostream>
	#include <random>
    #include <future>

	using namespace std;

	/***********************************************************/
    /*                 Multithread Mergge Sort                 */
    /***********************************************************/

    bool mthread = false;

    void merge(int* arr, int l, int m, int r)
    {
        int nl = m - l + 1;
        int nr = r - m;

        int* left = new int[nl];
        int* right = new int[nr];

        for (int i = 0; i < nl; i++)
            left[i] = arr[l + i];
        for (int j = 0; j < nr; j++)
            right[j] = arr[m + 1 + j];

        int i = 0, j = 0;
        int k = l;

        while (i < nl && j < nr) {
            if (left[i] <= right[j]) {
                arr[k] = left[i];
                i++;
            }
            else {
                arr[k] = right[j];
                j++;
            }
            k++;
        };

        while (i < nl) {
            arr[k] = left[i];
            i++;
            k++;
        };

        while (j < nr) {
            arr[k] = right[j];
            j++;
            k++;
        };

        delete[]left;
        delete[]right;
    };

    /***********************************************************/

    int pcounter = 0;

	void mergeSort(int* arr, int l, int r)
	{
		if (l >= r)return;
		int m = (l + r - 1) / 2;
		mergeSort(arr, l, m);                   // left part
	
        if (mthread && (r - m - 1 > 10000)) {   // right part
            auto f = async(launch::async, [&]() { mergeSort(arr, m + 1, r); });
            // the destructor guarantees the completion of the thread
            pcounter++;
        }
        else
            mergeSort(arr, m + 1, r);
                
        merge(arr, l, m, r);                    // merge two parts
	};

    /***********************************************************/
    /*                       Test                              */
    /***********************************************************/

    #include <chrono>
    #include <conio.h>
    void sort_Test(int* array, long size)
    {
        time_t start, end;
        for (long i = 0; i < size; i++) {
            array[i] = rand() % 500000;
        };

        pcounter = 0;

        time(&start);
        mergeSort(array, 0, size - 1);          // sort array
        time(&end);

        for (long i = 0; i < size - 1; i++) {   // check sotring
            if (array[i] > array[i + 1]) {
                cout << "Unsorted" << endl;
                break;
             };
        };

        double seconds = difftime(end, start);
        printf("The time: %f seconds\n", seconds);
        cout << "number of additional threads : " << pcounter << endl;
    };

    /***********************************************************/
    
    int main()
    {
        int cores = std::thread::hardware_concurrency();
        cout << "merge sort test" << endl;
        cout << "number of processor cores : " << cores << endl;

        srand(0);
        long size;
        size = 40000000;
        int* array = new int[size];

        mthread = true;
        cout << "multi-thread test" << endl;
        sort_Test(array, size);
        mthread = false;
        cout << "single-thread test" << endl;
        sort_Test(array, size);
 
        std::cout << "test finished, press any key" << std::endl;
        char c = _getch();
        return 0;
    };

	/***********************************************************/


