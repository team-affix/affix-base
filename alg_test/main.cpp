#include "affix_base.h"
#include <filesystem>
#include "serializable.h"
#include <tuple>
#include "cryptopp/osrng.h"
#include "guarded_resource.h"

using namespace affix_base::networking;
using namespace affix_base::cryptography;
using std::mutex;
using asio::ip::tcp;
using asio::ip::udp;
using asio::io_service;
using asio::io_context;
using affix_base::data::ptr;
using std::lock_guard;
using std::string;
namespace ip = asio::ip;
using asio::error_code;

int main(

)
{
	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;
	using namespace affix_base::distributed_computing;

	size_t l_test_vector_size = 10000;

	std::vector<double> l_x_0_single_thread(l_test_vector_size);
	std::vector<double> l_x_1_single_thread(l_test_vector_size);
	std::vector<double> l_y_single_thread(l_test_vector_size);

	size_t EXECUTION_ITERATIONS = 100000;

	affix_base::timing::stopwatch l_stopwatch;

	double l_time_for_multithread = 0;

	{
		std::vector<persistent_thread> l_persistent_threads(std::thread::hardware_concurrency() / 2);
	
		std::vector<std::vector<double>> l_x_0_partitioned(l_persistent_threads.size());
		std::vector<std::vector<double>> l_x_1_partitioned(l_persistent_threads.size());
		std::vector<std::vector<double>> l_y_partitioned(l_persistent_threads.size());

		std::vector<std::function<void()>> l_thread_specific_functions(l_persistent_threads.size());

		for (int i = 0; i < l_test_vector_size; i++)
		{
			size_t l_partition_index = i % l_persistent_threads.size();
			l_x_0_partitioned[l_partition_index].push_back(5);
			l_x_1_partitioned[l_partition_index].push_back(7);
			l_y_partitioned[l_partition_index].push_back(0);
			l_x_0_single_thread[i] = 5;
			l_x_1_single_thread[i] = 7;
		}

		for (int i = 0; i < l_thread_specific_functions.size(); i++)
		{
			auto& l_x_0_partition = l_x_0_partitioned[i];
			auto& l_x_1_partition = l_x_1_partitioned[i];
			auto& l_y_partition = l_y_partitioned[i];
			l_thread_specific_functions[i] = [&]
			{
				for (int j = 0; j < l_y_partition.size(); j++)
				{
					l_y_partition[j] = l_x_0_partition[j] * l_x_1_partition[j];
				}
			};
		}

		l_stopwatch.start();

		for (int i = 0; i < EXECUTION_ITERATIONS; i++)
		{
			for (int j = 0; j < l_persistent_threads.size(); j++)
				l_persistent_threads[j].execute(l_thread_specific_functions[j]);

			for (auto& l_persistent_thread : l_persistent_threads)
				l_persistent_thread.join();

		}

		l_time_for_multithread = l_stopwatch.duration_milliseconds();

		std::cout << l_time_for_multithread << std::endl;

	}

	l_stopwatch.start();

	for (int i = 0; i < EXECUTION_ITERATIONS; i++)
	{
		for (int j = 0; j < l_y_single_thread.size(); j++)
		{
			l_y_single_thread[j] = l_x_0_single_thread[j] * l_x_1_single_thread[j];
		}
	}

	double l_time_for_singlethread = l_stopwatch.duration_milliseconds();

	std::cout << l_time_for_singlethread << std::endl;

	std::cout << (l_time_for_multithread / l_time_for_singlethread) << std::endl;

 	return EXIT_SUCCESS;

}
