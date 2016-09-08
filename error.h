#ifndef ERROR_H_
# define ERROR_H_

# include <stdlib.h>
# include <stdio.h>

// ignores an unused parameter warning
#define UNUSED(...) (void)(__VA_ARGS__)

/*
** Prints a formatted error message and exits
*/

# define PERROR(FCT)						\
  do								\
  {								\
    fprintf (stderr, "Error at "__FILE__ ":%d => ", __LINE__);	\
    perror(FCT);						\
    exit(EXIT_FAILURE);						\
  } while (0)

# define WPERROR(CALL, NAME)			\
  if ((CALL) == -1)				\
    PERROR(NAME)

# define WNPERROR(CALL, NAME)			\
  if ((CALL) == NULL)				\
    PERROR(NAME)

# define WZPERROR(CALL, NAME)			\
  if ((CALL) == 0)				\
    PERROR(NAME)

# define FAIL(format, ...)						\
  do									\
  {									\
    fprintf (stderr, __FILE__ ":%d\t" format "\n", __LINE__,  ##__VA_ARGS__); \
    exit(EXIT_FAILURE);							\
  } while(0)

# define FAIL0(format)							\
  do									\
  {									\
    fprintf (stderr, __FILE__ ":%d\t" format "\n", __LINE__);		\
    exit(EXIT_FAILURE);							\
  } while(0)

# define LFAIL(...)							\
  do									\
  {									\
    fprintf (stderr, __VA_ARGS__);					\
    exit(EXIT_FAILURE);							\
  } while(0)

# define WARN(format, ...)						\
  fprintf(stderr, "WARNING\t" format "\n", ##__VA_ARGS__)

# define WARN0(...)							\
  fprintf(stderr, "%s", "WARNING\t" format "\n")

#endif
