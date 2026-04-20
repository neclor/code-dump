/**
 * \file prefixe_suffixe.h
 * \brief Header for calculating the length of the longest subarray at the prefix and suffix.
 * \author Pavlov Aleksandr
 * \date 14.08.2025
 *
 * Implements p-compatibility between T1 and T2
*/

#ifndef _PREFIXE_SUFFIXE_H
#define _PREFIXE_SUFFIXE_H

/**
 * Сalсulating the length of the longest subarray at the prefix and suffix.
 *
 * Сomplexity: O(n^2)
 *
 * @param T: input array.
 * @param N: length of array T.
 *
 * @pre: T != NULL, 0 < N
 * @post: T = T_0, N = N_0
 *
 * @return: length of the longest subarray at the prefix and suffix
 */
int prefixe_suffixe(int *T, unsigned int N);

#endif // _PREFIXE_SUFFIXE_H
