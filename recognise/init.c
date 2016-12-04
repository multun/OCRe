#include <stdlib.h>
#include <unistd.h>

#ifdef __APPLE__
#define PATH_MAX 2048
#else
#include <linux/limits.h>
#endif

#include "init.h"
#include "../img_network/network.h"
#include "../neural_network/builder.h"
#include "../img_network/charset.h"
#include "../error.h"

t_net_pool *init_recon(void)
{
  char path[PATH_MAX];
  WNPERROR(getcwd(path, PATH_MAX), "getcwd");
  WPERROR(chdir("img_network/units"), "chdir");
  t_network draft_net = NETWORK(NULL);
  t_net_pool *ret = VECT_ALLOC(w_network, 115);
  for(size_t i = 0; i < cats_bindings_size; i++)
  {
    t_cat_binding cur = cats_bindings[i];
    t_w_network *nelem = malloc(sizeof(t_w_network));
    nelem->target = cur.truename;
    nelem->net = dup_network(&draft_net, cur.dirname);
    if(load_network(nelem->net, true))
      FAIL("cannot find weights for network `%s`", nelem->target);
    VECT_PUSH(ret, nelem);
  }
  chdir(path);
  return ret;
}
