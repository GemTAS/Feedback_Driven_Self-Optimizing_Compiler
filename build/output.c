#include "profiler_runtime.h"
#include <stdio.h>

int main() {
int i = 0;
int sum = 0;
int max = 2000;
int maxInner = 2000;
prof_hit("L0");
L0:
int t0 = i < max;
if (t0 == 0) goto L1;
int j = 0;
prof_hit("L2");
L2:
int t1 = j < maxInner;
if (t1 == 0) goto L3;
int t2 = sum + 1;
sum = t2;
int t3 = j + 1;
j = t3;
t1 = j < maxInner;
if (t1 == 0) goto L3;
t2 = sum + 1;
sum = t2;
t3 = j + 1;
j = t3;
t1 = j < maxInner;
if (t1 == 0) goto L3;
t2 = sum + 1;
sum = t2;
t3 = j + 1;
j = t3;
t1 = j < maxInner;
if (t1 == 0) goto L3;
t2 = sum + 1;
sum = t2;
t3 = j + 1;
j = t3;
t1 = j < maxInner;
if (t1 == 0) goto L3;
t2 = sum + 1;
sum = t2;
t3 = j + 1;
j = t3;
t1 = j < maxInner;
if (t1 == 0) goto L3;
t2 = sum + 1;
sum = t2;
t3 = j + 1;
j = t3;
t1 = j < maxInner;
if (t1 == 0) goto L3;
t2 = sum + 1;
sum = t2;
t3 = j + 1;
j = t3;
t1 = j < maxInner;
if (t1 == 0) goto L3;
t2 = sum + 1;
sum = t2;
t3 = j + 1;
j = t3;
prof_hit("L2");
goto L2;
prof_hit("L3");
L3:
int t4 = i + 1;
i = t4;
prof_hit("L0");
goto L0;
prof_hit("L1");
L1:
printf("%d\n", sum);
prof_dump();
return 0;
}
