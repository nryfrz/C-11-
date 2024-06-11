#include <stdio.h>
#include "threadpool.h"
#include <iostream>
int main()
{
	ThreadPool pool(8);
	for (int i = 1; i <= 8; i++)
	{
		pool.enqueue([i]
			{
				std::cout << "task : " << i << " is running" << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "task : " << i << " is done" << std::endl;
			});
	}
	return 0;
}