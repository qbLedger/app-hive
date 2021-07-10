#pragma once

void rng_rfc6979(unsigned char *rnd,
                 unsigned char *h1,
                 unsigned char *x,
                 unsigned int x_len,
                 const unsigned char *q,
                 unsigned int q_len,
                 unsigned char *V,
                 unsigned char *K);