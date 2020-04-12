# Polynomial Multiplication using POSIX Threads

We perform multiplication of two polynomials using 2 methods :

- ## Naive method
We perform the `n*n` number of operations using `p` threads by splitting the coefficient array into `p` parts of length `n/p` who perform `n` operations each and later we add up the results to obtain the final answer.

- ## Karatsuba method
We perform the multiplication by creating 3 threads who perform 3 independent multiplications using a recursive subroutine and combine the results to obtain the final answer. This method is not scalable as the maximum number of threads required is `O(n)` and hence, on the machine tested, we cannot go beyond `11000` threads.

![](final_plot.png)

