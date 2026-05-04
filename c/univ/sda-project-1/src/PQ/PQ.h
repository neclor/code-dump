/* Interface for a (maximum) priority queue. The objects are of type 'void *' and
 * will be compared using the 'compare' function given as argument to the function
 * pqCreate.
 */
#ifndef  PQ_H_DEFINED
#define  PQ_H_DEFINED


#include <stddef.h>
#include <stdbool.h>


/* Opaque structure of a bounded priority queue */
typedef struct PQ_t PQ;


/* ------------------------------------------------------------------------- *
 * Creates a priority queue.
 *
 * PARAMETERS
 * capacity         Maximum number of elements that can be stored in the queue.
 * compare          A function to compare the priorities of two object. The
 *                  function returns 0 if the two priorities are equal, an
 *                  integer greater (resp. lower) than zero if the priority
 *                  of the first argument is greater (resp. lower) than the
 *                  priority of the second argument.
 * NOTE
 * The returned structure should be cleaned with `pqFree` after
 * usage.
 *
 * RETURN
 * pq              The priority queue.
 * ------------------------------------------------------------------------- */
PQ *pqCreate(size_t capacity, int (*compare)(const void *, const void *));

/* ------------------------------------------------------------------------- *
 * Frees the memory allocated for the priority queue.
 *
 * PARAMETERS
 * pq              The priority queue to free
 * ------------------------------------------------------------------------- */
void pqFree(PQ* pq);

/* ------------------------------------------------------------------------- *
 * Inserts an object in the priority queue. If the queue is full,
 * the element is not added and the function returns false.
 *
 *
 * PARAMETERS
 * pq              The priority queue
 * obj             The object to be stored in the queue
 *
 * RETURN
 * opened           True if the object was successfully added, false if the
 *   			    queue is full
 * ------------------------------------------------------------------------- */
bool pqInsert(PQ* pq, void *obj);

/* ------------------------------------------------------------------------- *
 * Returns the object of maximum priority currently stored in the priority queue.
 * The queue must contain at least one element (otherwise calling the function
 * results in an undefined behavior).
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * max              The object of maximum priority
 * ------------------------------------------------------------------------- */
void *pqGetMax(const PQ* pq);

/* ------------------------------------------------------------------------- *
 * Returns the object of aximum priority currently stored in the priority
 * queue and extracts it from the queue. The queue must contain at least
 * one object (otherwise calling the function results in an undefined behavior).
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * max              The object of maximum priority
 * ------------------------------------------------------------------------- */
void *pqExtractMax(PQ* pq);

/* ------------------------------------------------------------------------- *
 * Replaces the object of maximum priority with a new object. The queue
 * must contain at least one object (otherwise calling the function results in
 * an undefined behavior).
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 * obj              The new object
 * ------------------------------------------------------------------------- */
void pqReplaceMax(PQ *pq, void *obj);

/* ------------------------------------------------------------------------- *
 * Returns the size of the queue (i.e. the number of objects currently stored
 * in the queue).
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * size 		    The size of the queue
 * ------------------------------------------------------------------------- */
size_t pqSize(const PQ* pq);

/* ------------------------------------------------------------------------- *
 * Returns the maximum capacity of the queue.
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * capacity 	   The capacity of the queue
 * ------------------------------------------------------------------------- */
size_t pqCapacity(const PQ* pq);


#endif // PQ_H_DEFINED
