#pragma once
#include "pch.h"

namespace affix_base {
	namespace data {

        using std::vector;
        using std::function;

        template<typename T>
        vector<T> range(const vector<T>& a_vec, size_t a_start, size_t a_len) {
            vector<T> result(a_len);
            for (int i = 0; i < a_len; i++)
                result[i] = a_vec[a_start + i];
            return result;
        }

        template<typename T, typename J>
        vector<J> convert(const vector<T>& a_vec, function<J(const T&)> a_function) {
            vector<J> result(a_vec.size());
            for (int i = 0; i < a_vec.size(); i++)
                result[i] = a_function(a_vec[i]);
            return result;
        }

        template<typename T, typename J>
        vector<J> cast(const vector<T>& a_vec) {
            return convert<T, J>(a_vec, [](const T& a_val) { return (J)a_val; });
        }

	}
}
