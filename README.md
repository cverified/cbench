Here is a challenge for people who are interested in practical C program verification using formal methods.  Show us how to verify the correctness of the twenty-five programs in the paper [A benchmark for C program verification](https://arxiv.org/abs/1904.01009).

If you are interested in this in any way, feel free to send mail (and preferably verification proofs of these programs) to freek@cs.ru.nl.

Points scored on the benchmark thus far (that we are aware of):

|             | [VeriFast](https://github.com/verifast/verifast) | [VST](https://vst.cs.princeton.edu) |
|-------------|----:|----:|
| **Total**   |   4 |  50 |
| `fac1.c`    |   4 |   4 |
| `fac2.c`    |     |   4 |
| `fac3.c`    |     |   4 |
| `fac4.c`    |     |   4 |
| `fac6.c`    |     |   4 |
| `cat1.c`    |     |   4 |
| `cat2.c`    |     |   4 |
| `malloc1.c` |     |   4 |
| `qsort1.c`  |     |   4 |
| `qsort3.c`  |     |   4 |
| `qsort4.c`  |     |   3 |
| `sqrt1.c`   |     |   4 |
| `sqrt3.c`   |     |   3 |

The verifications are in the repositories [cbench-verifast](https://github.com/cverified/cbench-verifast) and [cbench-vst](https://github.com/cverified/cbench-vst).
