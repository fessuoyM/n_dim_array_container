Conatiner for arrays in C++. Designed to maintain speed and support multiple dimensions. It also will support some basic matrix math.

ToDoList:
 



Done:
-memoryBlock should be initiallized for all the subarrays made in the nDim array
-Addition needs to use the memory block instead of the array so that it is faster for nDim arrays since the memory is already all in one support
-Use memoryBlock instead of arr
-Need to use the protected variables when there is T2 is not defined and the same class would be called for the other variable.
-assign pointer before loop so compilaer can optimize
-make bool specific operstions only availbe for bool class