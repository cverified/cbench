#if __STDC_VERSION__ < 201100
typedef long double max_align_t;
#define _Alignof(x) (__alignof__(x))
#endif


#include <stddef.h>

#define HEAPSIZE 1000
union {
  unsigned char bytes[HEAPSIZE];
  max_align_t align;
} heap;

struct malloc_chunk {
  size_t mchunk_prev_size;
  size_t mchunk_size;
  struct malloc_chunk *fd;
  struct malloc_chunk *bk;
  struct malloc_chunk *fd_nextsize;
  struct malloc_chunk *bk_nextsize;
};

typedef struct malloc_chunk *mchunkptr;
typedef struct malloc_chunk *mbinptr;

mchunkptr av_top;
struct malloc_chunk av_bin;

#define chunk2mem(p) ((void*) ((char*) (p) + 2*SIZE_SZ))
#define mem2chunk(mem) ((mchunkptr) ((char*) (mem) - 2*SIZE_SZ))

#define SIZE_SZ (sizeof(size_t))
#define MALLOC_ALIGNMENT (2*SIZE_SZ < _Alignof(max_align_t) \
  ? _Alignof(max_align_t) : 2*SIZE_SZ)
#define MALLOC_ALIGN_MASK (MALLOC_ALIGNMENT - 1)

#define MIN_CHUNK_SIZE (sizeof(struct malloc_chunk))
#define MINSIZE (unsigned long) \
  (((MIN_CHUNK_SIZE + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK))

#define PREV_INUSE 0x1
#define SIZE_BITS (PREV_INUSE)

#define chunksize(p) (chunksize_nomask(p) & ~(SIZE_BITS))
#define prev_inuse(p) ((p)->mchunk_size & PREV_INUSE)
#define chunksize_nomask(p) ((p)->mchunk_size)
#define prev_size(p) ((p)->mchunk_prev_size)
#define chunk_at_offset(p, s) ((mchunkptr) (((char *) (p)) + (s)))

#define set_head(p, s) ((p)->mchunk_size = (s))
#define set_foot(p, s) \
  (((mchunkptr) ((char *) (p) + (s)))->mchunk_prev_size = (s))

#define inuse_bit_at_offset(p, s) \
  (((mchunkptr) (((char *) (p)) + (s)))->mchunk_size & PREV_INUSE)
#define set_inuse_bit_at_offset(p, s) \
  (((mchunkptr) (((char *) (p)) + (s)))->mchunk_size |= PREV_INUSE)
#define clear_inuse_bit_at_offset(p, s) \
  (((mchunkptr) (((char *) (p)) + (s)))->mchunk_size &= ~PREV_INUSE)

#define unlink(P, BK, FD) \
  { FD = P->fd; BK = P->bk; FD->bk = BK; BK->fd = FD; }

static void
free_unsorted_chunk(mchunkptr p, size_t size)
{
  mchunkptr fwd, bck;

  set_head(p, size | PREV_INUSE);
  set_foot(p, size);
  bck = &av_bin;
  fwd = bck->fd;
  if (fwd != bck) {
    size |= PREV_INUSE;
    while ((unsigned long) size < chunksize_nomask(fwd))
      fwd = fwd->fd_nextsize;
    if ((unsigned long) size ==
        (unsigned long) chunksize_nomask(fwd))
    {
      p->fd_nextsize = p->bk_nextsize = NULL;
      fwd = fwd->fd;
    } else {
      p->fd_nextsize = fwd;
      p->bk_nextsize = fwd->bk_nextsize;
      fwd->bk_nextsize = p;
      p->bk_nextsize->fd_nextsize = p;
    }
    bck = fwd->bk;
  } else
    p->fd_nextsize = p->bk_nextsize = p;
  p->bk = bck;
  p->fd = fwd;
  fwd->bk = p;
  bck->fd = p;
}

static void
malloc_init_state()
{
  size_t size;

  av_bin.fd = av_bin.bk = &av_bin;
  size = (HEAPSIZE - MIN_CHUNK_SIZE + 2*SIZE_SZ) & ~MALLOC_ALIGN_MASK;
  size -= MALLOC_ALIGNMENT;
  av_top = (mchunkptr) (heap.bytes + MALLOC_ALIGNMENT - 2*SIZE_SZ);
  set_inuse_bit_at_offset(av_top, size);
  free_unsorted_chunk(av_top, size);
}

void *
malloc(size_t bytes)
{
  size_t nb, size;
  mbinptr bin;
  mchunkptr victim, remainder, fwd, bck;
  unsigned long remainder_size;

  nb = bytes + SIZE_SZ + MALLOC_ALIGN_MASK;
  nb = nb < MINSIZE ? MINSIZE : nb & ~MALLOC_ALIGN_MASK;
  if (!(bin = &av_bin))
    malloc_init_state();
  if ((victim = bin->fd) != bin
      && (unsigned long) chunksize_nomask(victim)
         >= (unsigned long) (nb)) {
    victim = victim->bk_nextsize;
    while (((unsigned long) (chunksize(victim))
           < (unsigned long) (nb)))
      victim = victim->bk_nextsize;
    if (victim != bin->bk
        && chunksize_nomask(victim) == chunksize_nomask(victim->fd))
      victim = victim->fd;
    size = chunksize(victim);
    remainder_size = size - nb;
    unlink(victim, bck, fwd);
    if (remainder_size < MINSIZE)
      set_inuse_bit_at_offset(victim, size);
    else {
      remainder = chunk_at_offset(victim, nb);
      set_head(victim, nb | PREV_INUSE);
      free_unsorted_chunk(remainder, remainder_size);
    }
    return chunk2mem(victim);
  }
  victim = av_top;
  size = chunksize(victim);
  if ((unsigned long) (size) >= (unsigned long) (nb + MINSIZE)) {
    remainder_size = size - nb;
    remainder = chunk_at_offset(victim, nb);
    av_top = remainder;
    set_head(victim, nb | PREV_INUSE);
    set_head(remainder, remainder_size | PREV_INUSE);
    return chunk2mem(victim);
  }
  return 0;
}

void
free(void *mem)
{
  mchunkptr p, nextchunk, bck, fwd;
  size_t size, nextsize, prevsize;

  p = mem2chunk(mem);
  size = chunksize(p);
  nextchunk = chunk_at_offset(p, size);
  nextsize = chunksize(nextchunk);
  if (!prev_inuse(p)) {
    prevsize = prev_size(p);
    size += prevsize;
    p = chunk_at_offset(p, -((long) prevsize));
    unlink(p, bck, fwd);
  }
  if (nextchunk != av_top) {
    if (!inuse_bit_at_offset(nextchunk, nextsize)) {
      unlink(nextchunk, bck, fwd);
      size += nextsize;
    } else
      clear_inuse_bit_at_offset(nextchunk, 0);
    free_unsorted_chunk(p, size);
  } else {
    size += nextsize;
    set_head(p, size | PREV_INUSE);
    av_top = p;
  }
}


int
main()
{
  return 0;
}
