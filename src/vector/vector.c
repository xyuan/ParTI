/*
    This file is part of ParTI!.

    ParTI! is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    ParTI! is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with ParTI!.
    If not, see <http://www.gnu.org/licenses/>.
*/

#include <ParTI.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"

/**
 * Initialize a new value vector
 *
 * @param vec a valid pointer to an uninitialized sptVector variable,
 * @param len number of values to create
 * @param cap total number of values to reserve
 *
 * Vector is a type of one-dimentional array with dynamic length
 */
int sptNewVector(sptVector *vec, size_t len, size_t cap) {
    if(cap < len) {
        cap = len;
    }
    if(cap < 2) {
        cap = 2;
    }
    vec->len = len;
    vec->cap = cap;
    vec->data = malloc(cap * sizeof *vec->data);
    spt_CheckOSError(!vec->data, "Vec New");
    return 0;
}


/**
 * Fill an existed dense value vector with a specified constant
 *
 * @param vec   a valid pointer to an existed sptVector variable,
 * @param val   a given value constant
 *
 * Vector is a type of one-dimentional array with dynamic length
 */
int sptConstantVector(sptVector * const vec, sptScalar const val) {
    for(size_t i=0; i<vec->len; ++i)
        vec->data[i] = val;
    return 0;
}

/**
 * Copy a value vector to an uninitialized value vector
 *
 * @param dest a pointer to an uninitialized value vector
 * @param src  a pointer to an existing valid value vector
 *
 * The contents of `src` will be copied to `dest`.
 */
int sptCopyVector(sptVector *dest, const sptVector *src) {
    int result = sptNewVector(dest, src->len, src->len);
    spt_CheckError(result, "Vec Copy", NULL);
    memcpy(dest->data, src->data, src->len * sizeof *src->data);
    return 0;
}

/**
 * Add a value to the end of a value vector
 *
 * @param vec   a pointer to a valid value vector
 * @param value the value to be appended
 *
 * The length of the value vector will be changed to contain the new value.
 */
int sptAppendVector(sptVector *vec, sptScalar value) {
    if(vec->cap <= vec->len) {
#ifndef MEMCHECK_MODE
        size_t newcap = vec->cap + vec->cap/2;
#else
        size_t newcap = vec->len+1;
#endif
        sptScalar *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "Vec Append");
        vec->cap = newcap;
        vec->data = newdata;
    }
    vec->data[vec->len] = value;
    ++vec->len;
    return 0;
}

/**
 * Add a value to the end of a value vector
 *
 * @param vec        a pointer to a valid value vector
 * @param append_vec a pointer to another value vector, containing the values to be appended
 *
 * The values from `append_vec` will be appended to `vec`.
 */
int sptAppendVectorWithVector(sptVector *vec, const sptVector *append_vec) {
    if(vec->cap <= vec->len) {
        size_t newcap = vec->cap + append_vec->cap;
        sptScalar *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "Vec Append Vec");
        vec->cap = newcap;
        vec->data = newdata;
    }
    for(size_t i=0; i<append_vec->len; ++i) {
        vec->data[vec->len + i] = append_vec->data[i];
        ++vec->len;
    }

    return 0;
}

/**
 * Resize a value vector
 *
 * @param vec  the value vector to resize
 * @param size the new size of the value vector
 *
 * If the new size is larger than the current size, new values will be appended
 * but the values of them are undefined. If the new size if smaller than the
 * current size, values at the end will be truncated.
 */
int sptResizeVector(sptVector *vec, size_t size) {
    size_t newcap = size < 2 ? 2 : size;
    if(newcap != vec->cap) {
        sptScalar *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "Vec Resize");
        vec->len = size;
        vec->cap = newcap;
        vec->data = newdata;
    } else {
        vec->len = size;
    }
    return 0;
}

/**
 * Release the memory buffer a value vector is holding
 *
 * @param vec a pointer to a valid value vector
 *
 * By using `sptFreeVector`, a valid value vector would become uninitialized
 * and should not be used anymore prior to another initialization
 */
void sptFreeVector(sptVector *vec) {
    free(vec->data);
}


/**
 * Initialize a new size vector
 *
 * @param vec a valid pointer to an uninitialized sptSizeVector variable,
 * @param len number of values to create
 * @param cap total number of values to reserve
 *
 * Vector is a type of one-dimentional array with dynamic length
 */

int sptNewSizeVector(sptSizeVector *vec, size_t len, size_t cap) {
    if(cap < len) {
        cap = len;
    }
    if(cap < 2) {
        cap = 2;
    }
    vec->len = len;
    vec->cap = cap;
    vec->data = malloc(cap * sizeof *vec->data);
    spt_CheckOSError(!vec->data, "SzVec New");
    return 0;
}


/**
 * Fill an existed dense size vector with a specified constant
 *
 * @param vec   a valid pointer to an existed sptSizeVector variable,
 * @param num   a given value constant
 *
 * Vector is a type of one-dimentional array with dynamic length
 */
int sptConstantSizeVector(sptSizeVector * const vec, size_t const num) {
    for(size_t i=0; i<vec->len; ++i)
        vec->data[i] = num;
    return 0;
}


/**
 * Copy a size vector to an uninitialized size vector
 *
 * @param dest a pointer to an uninitialized size vector
 * @param src  a pointer to an existing valid size vector
 *
 * The contents of `src` will be copied to `dest`.
 */
int sptCopySizeVector(sptSizeVector *dest, const sptSizeVector *src) {
    int result = sptNewSizeVector(dest, src->len, src->len);
    spt_CheckError(result, "SzVec Copy", NULL);
    memcpy(dest->data, src->data, src->len * sizeof *src->data);
    return 0;
}

/**
 * Add a value to the end of a size vector
 *
 * @param vec   a pointer to a valid size vector
 * @param value the value to be appended
 *
 * The length of the size vector will be changed to contain the new value.
 */
int sptAppendSizeVector(sptSizeVector *vec, size_t value) {
    if(vec->cap <= vec->len) {
#ifndef MEMCHECK_MODE
        size_t newcap = vec->cap + vec->cap/2;
#else
        size_t newcap = vec->len+1;
#endif
        size_t *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "SzVec Append");
        vec->cap = newcap;
        vec->data = newdata;
    }
    vec->data[vec->len] = value;
    ++vec->len;
    return 0;
}

/**
 * Add a value to the end of a size vector
 *
 * @param vec        a pointer to a valid size vector
 * @param append_vec a pointer to another size vector, containing the values to be appended
 *
 * The values from `append_vec` will be appended to `vec`.
 */
int sptAppendSizeVectorWithVector(sptSizeVector *vec, const sptSizeVector *append_vec) {
    size_t newlen = vec->len + append_vec->len;
    if(vec->cap <= newlen) {
        size_t newcap = vec->cap + append_vec->cap;
        size_t *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "SzVec Append SzVec");
        vec->cap = newcap;
        vec->data = newdata;
    }
    for(size_t i=0; i<append_vec->len; ++i) {
        vec->data[vec->len + i] = append_vec->data[i];
    }
    vec->len = newlen;

    return 0;
}

/**
 * Resize a size vector
 *
 * @param vec  the size vector to resize
 * @param size the new size of the size vector
 *
 * If the new size is larger than the current size, new values will be appended
 * but the values of them are undefined. If the new size if smaller than the
 * current size, values at the end will be truncated.
 */
int sptResizeSizeVector(sptSizeVector *vec, size_t size) {
    size_t newcap = size < 2 ? 2 : size;
    if(newcap != vec->cap) {
        size_t *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "SzVec Resize");
        vec->len = size;
        vec->cap = newcap;
        vec->data = newdata;
    } else {
        vec->len = size;
    }
    return 0;
}

/**
 * Release the memory buffer a size vector is holding
 *
 * @param vec a pointer to a valid size vector
 *
 * By using `sptFreeSizeVector`, a valid size vector would become uninitialized
 * and should not be used anymore prior to another initialization
 */
void sptFreeSizeVector(sptSizeVector *vec) {
    free(vec->data);
}


/**** New added vectors to support HiCOO ****/
/*
 * Initialize a new sptElementIndexVector vector
 *
 * @param vec a valid pointer to an uninitialized sptElementIndex variable,
 * @param len number of values to create
 * @param cap total number of values to reserve
 *
 * Vector is a type of one-dimentional array with dynamic length
 */

int sptNewElementIndexVector(sptElementIndexVector *vec, uint64_t len, uint64_t cap) {
    if(cap < len) {
        cap = len;
    }
    if(cap < 2) {
        cap = 2;
    }
    vec->len = len;
    vec->cap = cap;
    vec->data = malloc(cap * sizeof *vec->data);
    spt_CheckOSError(!vec->data, "EleIdxVec New");
    memset(vec->data, 0, cap * sizeof *vec->data);
    return 0;
}

/**
 * Add a value to the end of a sptElementIndexVector
 *
 * @param vec   a pointer to a valid size vector
 * @param value the value to be appended
 *
 * The length of the size vector will be changed to contain the new value.
 */
int sptAppendElementIndexVector(sptElementIndexVector *vec, sptElementIndex value) {
    if(vec->cap <= vec->len) {
#ifndef MEMCHECK_MODE
        size_t newcap = vec->cap + vec->cap/2;
#else
        size_t newcap = vec->len+1;
#endif
        sptElementIndex *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "EleIdxVec Append");
        vec->cap = newcap;
        vec->data = newdata;
    }
    vec->data[vec->len] = value;
    ++vec->len;
    return 0;
}


/**
 * Release the memory buffer a sptElementIndexVector is holding
 *
 * @param vec a pointer to a valid size vector
 *
 */
void sptFreeElementIndexVector(sptElementIndexVector *vec) {
    free(vec->data);
    vec->len = 0;
    vec->cap = 0;
}


/*
 * Initialize a new sptBlockIndexVector vector
 *
 * @param vec a valid pointer to an uninitialized sptBlockIndex variable,
 * @param len number of values to create
 * @param cap total number of values to reserve
 *
 * Vector is a type of one-dimentional array with dynamic length
 */

int sptNewBlockIndexVector(sptBlockIndexVector *vec, uint64_t len, uint64_t cap) {
    if(cap < len) {
        cap = len;
    }
    if(cap < 2) {
        cap = 2;
    }
    vec->len = len;
    vec->cap = cap;
    vec->data = malloc(cap * sizeof *vec->data);
    spt_CheckOSError(!vec->data, "BlkIdxVec New");
    memset(vec->data, 0, cap * sizeof *vec->data);
    return 0;
}

/**
 * Add a value to the end of a sptBlockIndexVector
 *
 * @param vec   a pointer to a valid size vector
 * @param value the value to be appended
 *
 * The length of the size vector will be changed to contain the new value.
 */
int sptAppendBlockIndexVector(sptBlockIndexVector *vec, sptBlockIndex value) {
    if(vec->cap <= vec->len) {
#ifndef MEMCHECK_MODE
        size_t newcap = vec->cap + vec->cap/2;
#else
        size_t newcap = vec->len+1;
#endif
        sptBlockIndex *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "BlkIdxVec Append");
        vec->cap = newcap;
        vec->data = newdata;
    }
    vec->data[vec->len] = value;
    ++vec->len;
    return 0;
}


/**
 * Release the memory buffer a sptBlockIndexVector is holding
 *
 * @param vec a pointer to a valid size vector
 *
 */
void sptFreeBlockIndexVector(sptBlockIndexVector *vec) {
    free(vec->data);
    vec->len = 0;
    vec->cap = 0;
}


/*
 * Initialize a new sptIndex vector
 *
 * @param vec a valid pointer to an uninitialized sptIndex variable,
 * @param len number of values to create
 * @param cap total number of values to reserve
 *
 * Vector is a type of one-dimentional array with dynamic length
 */

int sptNewIndexVector(sptIndexVector *vec, uint64_t len, uint64_t cap) {
    if(cap < len) {
        cap = len;
    }
    if(cap < 2) {
        cap = 2;
    }
    vec->len = len;
    vec->cap = cap;
    vec->data = malloc(cap * sizeof *vec->data);
    spt_CheckOSError(!vec->data, "IdxVec New");
    memset(vec->data, 0, cap * sizeof *vec->data);
    return 0;
}

/**
 * Add a value to the end of a sptIndexVector
 *
 * @param vec   a pointer to a valid size vector
 * @param value the value to be appended
 *
 * The length of the size vector will be changed to contain the new value.
 */
int sptAppendIndexVector(sptIndexVector *vec, sptIndex value) {
    if(vec->cap <= vec->len) {
#ifndef MEMCHECK_MODE
        size_t newcap = vec->cap + vec->cap/2;
#else
        size_t newcap = vec->len+1;
#endif
        sptIndex *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "IdxVec Append");
        vec->cap = newcap;
        vec->data = newdata;
    }
    vec->data[vec->len] = value;
    ++vec->len;
    return 0;
}


/**
 * Release the memory buffer a sptIndexVector is holding
 *
 * @param vec a pointer to a valid size vector
 *
 */
void sptFreeIndexVector(sptIndexVector *vec) {
    free(vec->data);
    vec->len = 0;
    vec->cap = 0;
}



/*
 * Initialize a new sptNnzIndexVector vector
 *
 * @param vec a valid pointer to an uninitialized sptNnzIndex variable,
 * @param len number of values to create
 * @param cap total number of values to reserve
 *
 * Vector is a type of one-dimentional array with dynamic length
 */

int sptNewNnzIndexVector(sptNnzIndexVector *vec, uint64_t len, uint64_t cap) {
    if(cap < len) {
        cap = len;
    }
    if(cap < 2) {
        cap = 2;
    }
    vec->len = len;
    vec->cap = cap;
    vec->data = malloc(cap * sizeof *vec->data);
    spt_CheckOSError(!vec->data, "NnzIdxVec New");
    memset(vec->data, 0, cap * sizeof *vec->data);
    return 0;
}

/**
 * Add a value to the end of a sptNnzIndexVector
 *
 * @param vec   a pointer to a valid size vector
 * @param value the value to be appended
 *
 * The length of the size vector will be changed to contain the new value.
 */
int sptAppendNnzIndexVector(sptNnzIndexVector *vec, sptNnzIndex value) {
    if(vec->cap <= vec->len) {
#ifndef MEMCHECK_MODE
        size_t newcap = vec->cap + vec->cap/2;
#else
        size_t newcap = vec->len+1;
#endif
        sptNnzIndex *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "NnzIdxVec Append");
        vec->cap = newcap;
        vec->data = newdata;
    }
    vec->data[vec->len] = value;
    ++vec->len;
    return 0;
}


/**
 * Release the memory buffer a sptNnzIndexVector is holding
 *
 * @param vec a pointer to a valid size vector
 *
 */
void sptFreeNnzIndexVector(sptNnzIndexVector *vec) {
    free(vec->data);
    vec->len = 0;
    vec->cap = 0;
}


/**
 * Initialize a new value vector
 *
 * @param vec a valid pointer to an uninitialized sptValueVector variable,
 * @param len number of values to create
 * @param cap total number of values to reserve
 *
 * Vector is a type of one-dimentional array with dynamic length
 */
int sptNewValueVector(sptValueVector *vec, uint64_t len, uint64_t cap) {
    if(cap < len) {
        cap = len;
    }
    if(cap < 2) {
        cap = 2;
    }
    vec->len = len;
    vec->cap = cap;
    vec->data = malloc(cap * sizeof *vec->data);
    spt_CheckOSError(!vec->data, "ValVec New");
    memset(vec->data, 0, cap * sizeof *vec->data);
    return 0;
}

/**
 * Add a value to the end of a value vector
 *
 * @param vec   a pointer to a valid value vector
 * @param value the value to be appended
 *
 * The length of the value vector will be changed to contain the new value.
 */
int sptAppendValueVector(sptValueVector *vec, sptValue value) {
    if(vec->cap <= vec->len) {
#ifndef MEMCHECK_MODE
        size_t newcap = vec->cap + vec->cap/2;
#else
        size_t newcap = vec->len+1;
#endif
        sptValue *newdata = realloc(vec->data, newcap * sizeof *vec->data);
        spt_CheckOSError(!newdata, "ValVec Append");
        vec->cap = newcap;
        vec->data = newdata;
    }
    vec->data[vec->len] = value;
    ++vec->len;
    return 0;
}

/**
 * Release the memory buffer a value vector is holding
 *
 * @param vec a pointer to a valid value vector
 *
 */
void sptFreeValueVector(sptValueVector *vec) {
    vec->len = 0;
    vec->cap = 0;
    free(vec->data);
}
