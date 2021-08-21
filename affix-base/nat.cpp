#include "pch.h"
#include "nat.h"
#include "cryptopp/randpool.h"
#include "cryptopp/osrng.h"
#include "message.h"

using namespace affix_base;
using affix_base::networking::nat_type;
using namespace CryptoPP;

bool networking::get_external_ip(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, tcp::endpoint& a_output) {
	
	return true;

}

bool networking::get_nat_type(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, nat_type& a_output) {
	return false;
}
