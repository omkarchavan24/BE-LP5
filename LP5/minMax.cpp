#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

// ---------------- Sequential Functions ----------------

int minval_sequential(const vector<int> &arr)
{
    int minval = arr[0];

    for (int val : arr)
    {
        if (val < minval)
            minval = val;
    }

    return minval;
}

int maxval_sequential(const vector<int> &arr)
{
    int maxval = arr[0];

    for (int val : arr)
    {
        if (val > maxval)
            maxval = val;
    }

    return maxval;
}

long long sum_sequential(const vector<int> &arr)
{
    long long sum = 0;

    for (int val : arr)
    {
        sum += val;
    }

    return sum;
}

double average_sequential(const vector<int> &arr)
{
    return (double)sum_sequential(arr) / arr.size();
}

// ---------------- Parallel Functions ----------------

int minval_parallel(const vector<int> &arr)
{
    int minval = arr[0];

    #pragma omp parallel for reduction(min:minval)
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] < minval)
            minval = arr[i];
    }

    return minval;
}

int maxval_parallel(const vector<int> &arr)
{
    int maxval = arr[0];

    #pragma omp parallel for reduction(max:maxval)
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] > maxval)
            maxval = arr[i];
    }

    return maxval;
}

long long sum_parallel(const vector<int> &arr)
{
    long long sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); i++)
    {
        sum += arr[i];
    }

    return sum;
}

double average_parallel(const vector<int> &arr)
{
    return (double)sum_parallel(arr) / arr.size();
}

// ---------------- Main Function ----------------

int main()
{
    int n = 50000000;

    vector<int> arr(n);

    // Initialize array
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 1000;
    }

    double start, end;

    // -------- Sequential Minimum --------
    start = omp_get_wtime();

    int seq_min = minval_sequential(arr);

    end = omp_get_wtime();

    cout << "\nSequential Minimum Value : " << seq_min << endl;
    cout << "Sequential Min Time      : "
         << (end - start) << " seconds\n";

    // -------- Sequential Maximum --------
    start = omp_get_wtime();

    int seq_max = maxval_sequential(arr);

    end = omp_get_wtime();

    cout << "\nSequential Maximum Value : " << seq_max << endl;
    cout << "Sequential Max Time      : "
         << (end - start) << " seconds\n";

    // -------- Sequential Sum --------
    start = omp_get_wtime();

    long long seq_sum = sum_sequential(arr);

    end = omp_get_wtime();

    cout << "\nSequential Sum           : " << seq_sum << endl;
    cout << "Sequential Sum Time      : "
         << (end - start) << " seconds\n";

    // -------- Sequential Average --------
    start = omp_get_wtime();

    double seq_avg = average_sequential(arr);

    end = omp_get_wtime();

    cout << "\nSequential Average       : " << seq_avg << endl;
    cout << "Sequential Avg Time      : "
         << (end - start) << " seconds\n";

    // -------- Parallel Minimum --------
    start = omp_get_wtime();

    int par_min = minval_parallel(arr);

    end = omp_get_wtime();

    cout << "\nParallel Minimum Value   : " << par_min << endl;
    cout << "Parallel Min Time        : "
         << (end - start) << " seconds\n";

    // -------- Parallel Maximum --------
    start = omp_get_wtime();

    int par_max = maxval_parallel(arr);

    end = omp_get_wtime();

    cout << "\nParallel Maximum Value   : " << par_max << endl;
    cout << "Parallel Max Time        : "
         << (end - start) << " seconds\n";

    // -------- Parallel Sum --------
    start = omp_get_wtime();

    long long par_sum = sum_parallel(arr);

    end = omp_get_wtime();

    cout << "\nParallel Sum             : " << par_sum << endl;
    cout << "Parallel Sum Time        : "
         << (end - start) << " seconds\n";

    // -------- Parallel Average --------
    start = omp_get_wtime();

    double par_avg = average_parallel(arr);

    end = omp_get_wtime();

    cout << "\nParallel Average         : " << par_avg << endl;
    cout << "Parallel Avg Time        : "
         << (end - start) << " seconds\n";

    return 0;
}