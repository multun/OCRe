#undef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 201610L
#include <dirent.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#ifdef __APPLE__
#define PATH_MAX 2048
#else
#include <linux/limits.h>
#endif

#include "training.h"
#include "../error.h"
#include "../tdefs.h"

ALLOC_IMAGE_DEFINE(nfloat, nfloat_mat)

t_training_case *get_training_case(char *path)
{
  struct stat buf;
  t_training_case *ret = malloc(sizeof(t_training_case));
  WPERROR(ret->fd = open(path, O_RDONLY), "open");
  WPERROR(fstat(ret->fd, &buf), "fstat");
  WNPERROR(ret->data = mmap(
	     NULL,
	     (size_t)buf.st_size,
	     PROT_READ,
	     MAP_SHARED,
	     ret->fd,
	     0),
	   "mmap");
  WPERROR(close(ret->fd), "close");
  return ret;
}

t_training_cat *get_training_cat(char *path, char *dirname, char *name)
{
  struct dirent *ep;
  DIR *dp = opendir(path);
  size_t path_len = strlen(path);
  t_training_cat *ret = malloc(sizeof(t_training_cat));
  ret->cases = VECT_ALLOC(training_case, 16);
  ret->name = name;
  ret->dirname = dirname;

  char casepath[PATH_MAX];
  strcpy(casepath, path);
  strcat(casepath, "/");

  if (dp != NULL)
    {
      while ((ep = readdir(dp)))
      {
	if(ep->d_type != DT_REG)
	  continue;
	casepath[path_len+1] = '\0';
	strcat(casepath, ep->d_name);
        VECT_PUSH(ret->cases, get_training_case(casepath));
      }
      (void)closedir(dp);
    }
  else
    PERROR("opendir");

  if(VECT_SIZE(ret->cases) == 0)
  {
    VECT_FREE(ret->cases);
    return NULL;
  }

  return ret;
}

t_training_set *get_training_set(char *path,
				 size_t cats_count,
				 t_cat_binding *bindings)
{
  size_t path_len = strlen(path);
  t_training_set *ret = malloc(sizeof(t_training_set));
  ret->count = 0;
  ret->cats = VECT_ALLOC(training_cat, cats_count);

  char casepath[PATH_MAX];
  strcpy(casepath, path);
  strcat(casepath, "/");

  for(size_t i = 0; i < cats_count; i++)
  {
    const t_cat_binding *curbind = &bindings[i];
    casepath[path_len+1] = '\0';
    strcat(casepath, curbind->dirname);
    t_training_cat *cat = get_training_cat(casepath,
					   curbind->dirname,
					   curbind->truename);
    if(!cat)
    {
      WARN("category `%s` is empty", curbind->truename);
      continue;
    }
    ret->count += VECT_GET_SIZE(cat->cases);
    VECT_PUSH(ret->cats, cat);
  }

  return ret;
}

t_training_cat *find_cat(t_training_set *set,
			 const char *name,
			 bool is_plaintext)
{
  for(uint i = 0; i < VECT_GET_SIZE(set->cats); i++)
  {
    t_training_cat *cat = VECT_GET(set->cats, i);
    if (!strcmp((is_plaintext) ? cat->name : cat->dirname, name))
      return cat;
  }
  return NULL;
}
