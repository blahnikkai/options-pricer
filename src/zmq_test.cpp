#include <json.hpp>
#include <zmq.hpp>
#include <iostream>
using json = nlohmann::json;

std::string recv_str(zmq::socket_t & sock) {
    zmq::message_t msg;
    zmq::recv_result_t result = sock.recv(msg);
    std::string received_str(static_cast<char*>(msg.data()), msg.size());
    return received_str;
}

int main() {
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::req);

    sock.connect("tcp://localhost:5555");

    const std::string_view m = "whattt this is c++";
    sock.send(zmq::buffer(m), zmq::send_flags::none);

    auto str = recv_str(sock);
    auto a = json::parse(str);
    std::vector<int> b = a;
    std::cout << a << '\n';
}
