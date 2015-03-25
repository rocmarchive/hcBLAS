#include <iostream>
#include "ampblaslib.h"

using namespace std;

int main()
{
    Ampblaslibrary amp;
    const float alpha = 5;
    amp.ampblas_saxpy(5, &alpha, NULL, 0, NULL, 0);
    cout<<alpha;
    return 1;
}
