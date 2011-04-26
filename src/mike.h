#ifndef _MIKE_H_
#define _MIKE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <v8.h>

#include "mike/env.h"
#include "mike/stdio.h"

namespace mike
{
  void Initialize();
  void Dispose();
}

#endif /* _MIKE_H_ */
