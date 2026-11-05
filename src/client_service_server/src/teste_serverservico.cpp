#include "rclcpp/rclcpp.hpp"
#include "custom_interface/srv/add_two_ints.hpp"
#include <memory>

using namespace std::placeholders; // Permite o uso de _1 e _2 no bind

class ServicoServer : public rclcpp::Node
{
public:
  ServicoServer() : Node("exemplo_servico_server")
  {
    // Cria o servidor e aponta o callback para o método add_callback da classe
    service_ = this->create_service<custom_interface::srv::AddTwoInts>(
      "exemplo_servico",
      std::bind(&ServicoServer::add_callback, this, _1, _2));
      
    RCLCPP_INFO(this->get_logger(), "Servidor OOP pronto para somar.");
  }

private:
  void add_callback(const std::shared_ptr<custom_interface::srv::AddTwoInts::Request> request,
                    std::shared_ptr<custom_interface::srv::AddTwoInts::Response> response)
  {
    response->sum = request->a + request->b;
    RCLCPP_INFO(this->get_logger(), "Recebido pedido: a: %ld, b: %ld", request->a, request->b);
    RCLCPP_INFO(this->get_logger(), "Enviando resposta: [%ld]", response->sum);
  }

  rclcpp::Service<custom_interface::srv::AddTwoInts>::SharedPtr service_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ServicoServer>()); // Instancia e trava aqui
  rclcpp::shutdown();
  return 0;
}