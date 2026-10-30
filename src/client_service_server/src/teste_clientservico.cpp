#include "rclcpp/rclcpp.hpp"
#include "custom_interface/srv/add_two_ints.hpp" //interface

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  if (argc != 3) { //validacao de ter 3 dados, nome do programa, a e b
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "usage: exemplo_servico_client X Y");
      return 1;
  }

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("exemplo_servico_client");//instancia um nó
  rclcpp::Client<custom_interface::srv::AddTwoInts>::SharedPtr client =
    node->create_client<custom_interface::srv::AddTwoInts>("exemplo_servico");//cria um  cliente para o servico exemplo_servico

  auto request = std::make_shared<custom_interface::srv::AddTwoInts::Request>(); //Montagem do request
  request->a = atoll(argv[1]); //transforma string para long(int64), é passado pelo terminal
  request->b = atoll(argv[2]);//transforma string para long(int64), é passado pelo terminal

  while (!client->wait_for_service(1s)) { // Espera o servico ficar disponivel
    if (!rclcpp::ok()) { // verifica se o ros2 esta rodando
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }

  auto result = client->async_send_request(request); //envia o pedido e espera a resposta de forma assincrona, não trava aqui
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(node, result) ==
    rclcpp::FutureReturnCode::SUCCESS)// fica aguardando o retorno ficar completo -> rclcpp::spin_until_future_complete()
  {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %ld", result.get()->sum);//se deu certo exibe no console
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service exemplo_servico");//se deu errado aparece o erro no console
  }

  rclcpp::shutdown();
  return 0;
}