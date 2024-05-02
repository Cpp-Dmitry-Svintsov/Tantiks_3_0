#include "main.h"

int main()
{
    // Providing a seed value
    srand((unsigned)time(NULL));

    AsEngine Engine;

    Engine.Init();

    Engine.Run();
    return 0;
}