#include <iostream>
#include <string>
#include <cppunix/parallel_scheduler.h>
#include <cppunix/channel.h>
#include <cppunix/shell.h>

int main()
{
	cu::parallel_scheduler sch;

	cu::channel<std::string> command(sch);

	command.pipeline(cu::run(), cu::strip());
	
	sch.spawn([&](auto& yield) {
		command(yield, "ls -ltr");
		command.close(yield);
	});
	sch.spawn([&](auto& yield) {
		for(auto& e : cu::range(yield, command))
		{
			std::cout << "line: " << e << std::endl;
		}
	});
	sch.run_until_complete();
}

