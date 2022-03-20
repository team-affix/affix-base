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

struct function_data_transfer_declaration
{
	std::string m_function_identifier;
	std::string m_call_identifier;
	affix_base::data::byte_buffer m_byte_buffer;
};

affix_base::threading::guarded_resource<std::vector<function_data_transfer_declaration>, affix_base::threading::cross_thread_mutex> i_function_data_transfers_to_remote;
affix_base::threading::guarded_resource<std::vector<function_data_transfer_declaration>, affix_base::threading::cross_thread_mutex> i_function_data_transfers_to_local;

void remote_main()
{
	using namespace affix_base::threading;

	affix_base::distributed_computing::remote_invocation_processor<std::string, std::string> l_remote_invocation_processor;

	l_remote_invocation_processor.add_function<double, double, double>(
		"multiply_doubles",
		std::function([&](double a, double b)
		{
			return a * b;
		}));

	while (true)
	{
		locked_resource l_function_data_transfers_to_remote = i_function_data_transfers_to_remote.lock();

		for (int i = l_function_data_transfers_to_remote->size() - 1; i >= 0; i--)
		{
			auto l_it = l_function_data_transfers_to_remote->begin() + i;

			// Extract data from iterator
			function_data_transfer_declaration l_result_data_transfer = *l_it;

			// Erase element from vector
			l_function_data_transfers_to_remote->erase(l_it);

			l_result_data_transfer.m_byte_buffer = l_remote_invocation_processor.process(
				l_result_data_transfer.m_function_identifier, 
				l_result_data_transfer.m_byte_buffer);

			locked_resource l_function_data_transfers_to_local = i_function_data_transfers_to_local.lock();
			
			l_function_data_transfers_to_local->push_back(l_result_data_transfer);

		}

	}
}

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;

	const size_t VECTOR_SIZE = 500000;

	std::vector<double> v1(VECTOR_SIZE);
	std::vector<double> v2(VECTOR_SIZE);
	std::vector<double> product(VECTOR_SIZE);

	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		v1[i] = i;
		v2[i] = 2 * i;
	}

	auto process_one_fifth = 
	[&] (size_t a_fifth_index)
	{
		for (int i = a_fifth_index / 5 * v1.size(); i < (a_fifth_index + 1) / 5 * v1.size(); i++)
			product[i] = v1[i] * v2[i];
	};

	const int MAX_TEST_ITERATIONS = 10000;

	affix_base::timing::stopwatch sw;
	sw.start();
	for (int i = 0; i < MAX_TEST_ITERATIONS; i++)
	{
		std::future l_future_0 = std::async(std::launch::async, process_one_fifth, 0);
		std::future l_future_1 = std::async(std::launch::async, process_one_fifth, 1);
		std::future l_future_2 = std::async(std::launch::async, process_one_fifth, 2);
		std::future l_future_3 = std::async(std::launch::async, process_one_fifth, 3);
		std::future l_future_4 = std::async(std::launch::async, process_one_fifth, 4);
		l_future_0.get();
		l_future_1.get();
		l_future_2.get();
		l_future_3.get();
		l_future_4.get();
	}
	std::cout << "MULTITHREAD PERIOD (ms): " << sw.duration_milliseconds() << std::endl;

	sw.start();
	for (int i = 0; i < MAX_TEST_ITERATIONS; i++)
	{
		process_one_fifth(0);
		process_one_fifth(1);
		process_one_fifth(2);
		process_one_fifth(3);
		process_one_fifth(4);
	}
	std::cout << "SINGLETHREAD PERIOD (ms): " << sw.duration_milliseconds() << std::endl;


	//affix_base::distributed_computing::remote_function_invoker<std::string, std::string> l_remote_function_invoker(
	//	std::function([&](std::string a_function_identifier, affix_base::data::byte_buffer a_byte_buffer)
	//		{
	//			locked_resource l_function_data_transfers_to_remote = i_function_data_transfers_to_remote.lock();

	//			std::string l_call_identifier;
	//			l_call_identifier.resize(25);

	//			CryptoPP::AutoSeededRandomPool l_random;
	//			l_random.GenerateBlock((CryptoPP::byte*)l_call_identifier.data(), l_call_identifier.size());

	//			function_data_transfer_declaration l_func_call_decl{
	//				a_function_identifier,
	//				l_call_identifier,
	//				a_byte_buffer
	//			};

	//			l_function_data_transfers_to_remote->push_back(l_func_call_decl);

	//			return l_call_identifier;

	//		}),
	//	std::function([&](std::string a_call_identifier)
	//		{
	//			while (true)
	//			{
	//				locked_resource l_function_data_transfers_to_local = i_function_data_transfers_to_local.lock();

	//				auto l_it = std::find_if(l_function_data_transfers_to_local->begin(), l_function_data_transfers_to_local->end(),
	//					[&](const function_data_transfer_declaration& a_func_decl)
	//					{
	//						return a_func_decl.m_call_identifier == a_call_identifier;
	//					});

	//				if (l_it != l_function_data_transfers_to_local->end())
	//				{
	//					return l_it->m_byte_buffer;
	//				}

	//			}
	//		})
	//);
	//
	//std::thread l_remote_thd(remote_main);

	//// Wait for thread to start
	//while (!l_remote_thd.joinable());

	//for (int i = 0; i < 100; i++)
	//{
	//	std::future<double> l_future_result_0 = l_remote_function_invoker.invoke<double, double, double>
	//		("multiply_doubles", 1, 1);
	//	double l_result_0 = l_future_result_0.get();
	//}

	//l_remote_thd.join();

 	return EXIT_SUCCESS;

}
