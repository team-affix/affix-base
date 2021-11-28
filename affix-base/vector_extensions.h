#pragma once
#include "pch.h"
#include "diff.h"

namespace affix_base {
	namespace data {

		template<typename T>
		bool exists(const std::vector<T>& a_vec, std::function<bool(const T&)> a_predicate) {
			return std::find_if(a_vec.begin(), a_vec.end(), a_predicate) != a_vec.end();
		}

		template<typename T>
		bool exists(const std::vector<T>& a_vec, const T& a_val) {
			
			std::function<bool(const T&)> l_predicate = [&](const T& a_b) {
				return a_val == a_b;
			};
			return exists(a_vec, l_predicate);

		}

        template<typename T>
		std::vector<T> range(const std::vector<T>& a_vec, size_t a_start, size_t a_len) {
			std::vector<T> result(a_len);
            for (int i = 0; i < a_len; i++)
                result[i] = a_vec[a_start + i];
            return result;
        }

        template<typename T, typename J>
		std::vector<J> convert(const std::vector<T>& a_vec, std::function<J(const T&)> a_function) {
			std::vector<J> result(a_vec.size());
            for (int i = 0; i < a_vec.size(); i++)
                result[i] = a_function(a_vec[i]);
            return result;
        }

        template<typename T, typename J>
		std::vector<J> cast(const std::vector<T>& a_vec) {
            return convert<T, J>(a_vec, [](const T& a_val) { return (J)a_val; });
        }

		template<typename T>
		bool erase_duplicates(std::vector<T>& a_vec, std::function<bool(const T&, const T&)> a_match) {

			bool result = false;

			for (int i = a_vec.size() - 1; i > 0; i--) {

				auto it = a_vec.begin() + i;

				std::function<bool(const T&)> l_predicate = [&](const T& a_a) {
					return *it == a_a;
				};

				if (std::find_if(a_vec.begin(), it, l_predicate) != it) {
					a_vec.erase(it);
					result = true;
				}

			}

			return result;

		}

		template<typename T>
		bool erase_duplicates(std::vector<T>& a_vec) {
			
			std::function<bool(const T&, const T&)> l_match = [&](const T& a_a, const T& a_b) {
				return a_a == a_b;
			};
			return erase_duplicates(a_vec, l_match);

		}

		template<typename T>
		std::vector<diff<T>> unordered_comparison(const std::vector<T>& a_old, const std::vector<T>& a_new, std::function<bool(const T&, const T&)> a_match) {

			std::vector<diff<T>> result;

			// CHECK FOR SUBTRACTIONS
			for (int i = 0; i < a_old.size(); i++) {

				std::function<bool(const T&)> l_predicate = [&](const T& a_b) {
					return a_match(a_old[i], a_b);
				};

				if (!exists(a_new, l_predicate)) {
					result.push_back(diff<T>{a_old[i], false});
				}

			}

			// CHECK FOR ADDITIONS
			for (int i = 0; i < a_new.size(); i++) {

				std::function<bool(const T&)> l_predicate = [&](const T& a_b) {
					return a_match(a_new[i], a_b);
				};

				if (!exists(a_old, l_predicate)) {
					result.push_back(diff<T>{a_new[i], true});
				}

			}

			return result;

		}

		template<typename T>
		std::vector<diff<T>> unordered_comparison(const std::vector<T>& a_old, const std::vector<T>& a_new) {

			std::function<bool(const T&, const T&)> l_match = [](const T& a_a, const T& a_b) {
				return a_a == a_b;
			};

			return unordered_comparison(a_old, a_new, l_match);

		}

	}
}
