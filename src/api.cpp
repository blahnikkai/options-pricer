#include <json.hpp>
#include <zmq.hpp>
#include <iostream>

#include "pricing.h"

using json = nlohmann::json;

std::string recv_str(zmq::socket_t & sock) {
    zmq::message_t msg;
    zmq::recv_result_t result = sock.recv(msg, zmq::recv_flags::none);
    std::string received_str(static_cast<char*>(msg.data()), msg.size());
    return received_str;
}

json recv_json(zmq::socket_t & sock) {
    std::string str = recv_str(sock);
    return json::parse(str);
}

void send_str(zmq::socket_t & sock, const std::string & str) {
    zmq::message_t msg(str.data(), str.size());
    sock.send(msg, zmq::send_flags::none);
}

void send_json(zmq::socket_t & sock, json & payload) {
    send_str(sock, payload.dump());
}

int main() {
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::rep);

    sock.bind("tcp://*:5555");

    while(true) {
        json req = recv_json(sock);
        double u = req["u"];
        double k = req["k"];
        double t = req["t"];
        double r = req["r"];
        double v = req["v"];

        bool american = req.contains("american") && req["american"];
        bool put = req.contains("put") && req["put"];

        json resp = {};
        if(!american && req["methods"].contains("black-scholes")) {
            double black_scholes_price = pricing::calc_black_scholes(u, k, t, r, v, !put);
            resp["black-scholes"] = black_scholes_price;
        }
        if(req["methods"].contains("binomial")) {
            double binomial_price = pricing::calc_binomial(u, k, t, r, v, req["methods"]["binomial"], !put, !american);
            resp["binomial"] = binomial_price;
        }
        if(!american && req["methods"].contains("monte carlo")) {
            double monte_carlo_price = pricing::calc_monte_carlo(u, k, t, r, v, req["methods"]["monte carlo"][0], req["methods"]["monte carlo"][1], !put);
            resp["monte carlo"] = monte_carlo_price;
        }
        send_json(sock, resp);
    }

}
