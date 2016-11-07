#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 201610L

#include "neurons.h"
#include "../tdefs.h"
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "builder.h"
#include "../error.h"

#ifndef O_DIRECTORY
#define O_DIRECTORY 0
#endif

#define OPEN_FLAGS (O_RDWR)
#define OPEN_MODE  (S_IRUSR | S_IWUSR | S_IRGRP)

static void fill_constant(double *array, size_t size, double constant)
{
  for(size_t i = 0; i < size; i++)
    array[i] = constant;
}

static size_t digits_count(size_t n)
{
  size_t i = 0;
  for(;n;i++)
    n /= 10;
  return i;
}

static void *map_fd(int fd, size_t size)
{
  // UNCHECKED
  WPERROR(ftruncate(fd, (off_t)size), "ftruncate");
  void *ret;
  WZPERROR((ret = mmap(NULL,
		       size,
		       PROT_READ | PROT_WRITE,
		       MAP_SHARED,
		       fd,
		       0)
	     ) != MAP_FAILED,
	   "mmap");
  return ret;
}

int openat_exists(bool *is_new,
		  int dirfd,
		  const char *name)
{
  *is_new = true;
  int fd = openat(dirfd, name, OPEN_FLAGS | O_CREAT | O_EXCL, OPEN_MODE);
  if (fd == -1)
  {
    if(errno == EEXIST)
    {
      *is_new = false;
      WPERROR(fd = openat(dirfd, name, OPEN_FLAGS | O_CREAT, OPEN_MODE),
	      "open");
    }
    else
      PERROR("openat");
  }
  return fd;
}

static inline void alloc_layer(t_layer *layer, int fd, bool is_map)
{
  size_t layer_size = sizeof(*layer->out) * layer->size;
  layer->in	= malloc(layer_size);

  layer->out	= malloc(layer_size);
  layer->delta	= malloc(layer_size);
  if (fd != -1)
  {
    size_t w_size = sizeof(double) * LAYER_WSIZE(layer);
    layer->weights_delta	= malloc(w_size);
    layer->weights	= (is_map) ? map_fd(fd, w_size) : malloc(w_size);
    fill_constant(layer->weights_delta, LAYER_WSIZE(layer), 0.0);
  }
  else
  {
    layer->weights	 = NULL;
    layer->weights_delta = NULL;
  }
}


static void alloc_network(const t_network *net)
{
  for(size_t i = 0; i < net->layers_count; i++)
    alloc_layer(&net->layers[i], (i < (net->layers_count - 1))? 1 : -1, false);
}

bool load_network(const t_network *net)
{
  bool is_new = true;
  const char *name = net->name;
  if(!name)
  {
    alloc_network(net);
    return true;
  }

  const char prefix[] = "layer_";
  size_t prefix_len = sizeof(prefix) - 1;

  int dirfd = open(name, O_DIRECTORY);
  if(dirfd == -1)
    FAIL("no such network data directory: `%s`", name);

  char *namebuf = malloc(prefix_len + digits_count(net->layers_count) + 1);
  strcpy(namebuf, prefix);

  for(size_t i = 0; i < net->layers_count; i++)
  {
    sprintf(namebuf + prefix_len, "%lu", i);
    char has_weights = i < net->layers_count - 1;
    int fd = -1;
    if (has_weights)
      fd = openat_exists(&is_new, dirfd, namebuf);

    alloc_layer(&net->layers[i], fd, has_weights);
    if (has_weights)
      close(fd);
  }
  free(namebuf);
  close(dirfd);
  return is_new;
}


static inline void free_layer(t_layer *layer, const bool is_map)
{
  free(layer->in);
  free(layer->out);
  free(layer->delta);

  if(layer->weights)
  {
    free(layer->weights_delta);
    if(is_map)
      munmap(layer->weights, sizeof(double) * LAYER_WSIZE(layer));
    else
      free(layer->weights);
  }
}

void free_network(const t_network *net)
{
  const bool is_map = net->name != NULL;
  for(size_t i = 0; i < net->layers_count; i++)
    free_layer(&net->layers[i], is_map);
}
