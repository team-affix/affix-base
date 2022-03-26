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

class matrix
{
protected:
	std::vector<std::vector<double>> m_matrix;

public:
	matrix(
		const size_t& a_rows,
		const size_t& a_cols
	)
	{
		m_matrix.resize(a_rows);
		for (int i = 0; i < a_rows; i++)
			m_matrix[i].resize(a_cols);
	}

	matrix(
		const std::vector<std::vector<double>>& a_matrix
	) :
		m_matrix(a_matrix)
	{

	}

public:
	size_t rows(

	) const
	{
		return m_matrix.size();
	}

	size_t cols(

	) const
	{
		return m_matrix.front().size();
	}

	matrix range(
		const size_t& a_row,
		const size_t& a_col,
		const size_t& a_rows,
		const size_t& a_cols
	) const
	{
		matrix l_result(a_rows, a_cols);
		for (int i = 0; i < a_rows; i++)
		{
			const size_t l_row = a_row + i;
			for (int j = 0; j < a_cols; j++)
			{
				const size_t l_col = a_col + j;
				l_result.m_matrix[l_row][l_col] = m_matrix[l_row][l_col];
			}
		}
		return l_result;
	}

	matrix transpose(

	) const
	{
		matrix l_result(cols(), rows());

		for (int i = 0; i < rows(); i++)
		{
			for (int j = 0; j < cols(); j++)
			{
				l_result.m_matrix[j][i] = m_matrix[i][j];
			}
		}

		return l_result;

	}

	matrix tanh(

	) const
	{
		matrix l_result(rows(), cols());

		for (int i = 0; i < rows(); i++)
		{
			for (int j = 0; j < cols(); j++)
			{
				l_result.m_matrix[i][j] = std::tanh(m_matrix[i][j]);
			}
		}
		
		return l_result;

	}

	matrix add(
		const matrix& a_other
	) const
	{
		assert(a_other.rows() == rows() && a_other.cols() == cols());
		matrix l_result(rows(), cols());
		for (int i = 0; i < rows(); i++)
		{
			for (int j = 0; j < cols(); j++)
			{
				l_result.m_matrix[i][j] = m_matrix[i][j] + a_other.m_matrix[i][j];
			}
		}
		return l_result;
	}

	matrix subtract(
		const matrix& a_other
	) const
	{
		assert(a_other.rows() == rows() && a_other.cols() == cols());
		matrix l_result(rows(), cols());
		for (int i = 0; i < rows(); i++)
		{
			for (int j = 0; j < cols(); j++)
			{
				l_result.m_matrix[i][j] = m_matrix[i][j] - a_other.m_matrix[i][j];
			}
		}
		return l_result;
	}

	matrix left_multiply(
		const matrix& a_other
	) const
	{
		return a_other.right_multiply(*this);
	}

	matrix right_multiply(
		const matrix& a_other
	) const
	{
		assert(cols() == a_other.rows());

		matrix l_result(rows(), a_other.cols());

		for (int i = 0; i < rows(); i++)
		{
			for (int j = 0; j < a_other.cols(); j++)
			{
				for (int k = 0; k < cols(); k++)
				{
					l_result.m_matrix[i][j] += m_matrix[i][k] * a_other.m_matrix[k][j];
				}
			}
		}

		return l_result;

	}

	matrix operator +(
		const matrix& a_other
	) const
	{
		return add(a_other);
	}

	matrix operator -(
		const matrix& a_other
	) const
	{
		return subtract(a_other);
	}

	matrix operator *(
		const matrix& a_other
	) const
	{
		return right_multiply(a_other);
	}

};

class differentiable_matrix_function
{
protected:
	std::function<matrix(matrix)> m_forward;
	std::function<matrix(matrix)> m_backward;

public:



};

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;



 	return EXIT_SUCCESS;

}
