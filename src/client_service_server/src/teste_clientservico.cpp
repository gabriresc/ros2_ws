#include "rclcpp/rclcpp.hpp"
#include "custom_interface/srv/add_two_ints.hpp"
#include <chrono>
#include <memory>
#include <cstdlib>

using namespace std::chrono_literals;

class ServicoClient : public rclcpp::Node
{
public:
  // O construtor agora recebe os valores passados pelo terminal
  ServicoClient(long int a, long int b) : Node("exemplo_servico_client"), a_(a), b_(b)
  {
    client_ = this->create_client<custom_interface::srv::AddTwoInts>("exemplo_servico");
    
    // Dispara o timer para tentar enviar o pedido em 500ms
    timer_ = this->create_wall_timer(500ms, std::bind(&ServicoClient::send_request, this));
  }

private:
  void send_request()
  {
    timer_->cancel(); // Para o timer para enviar o pedido apenas uma vez

    if (!client_->wait_for_service(1s)) {
      RCLCPP_INFO(this->get_logger(), "Servidor indisponível, aguardando...");
      timer_->reset(); // Liga o timer de novo para tentar na próxima iteração
      return;
    }

    auto request = std::make_shared<custom_interface::srv::AddTwoInts::Request>();
    request->a = a_;
    request->b = b_;

    // Função lambda simples para rodar quando a resposta chegar
    auto response_callback = [this](rclcpp::Client<custom_interface::srv::AddTwoInts>::SharedFuture future) {
      RCLCPP_INFO(this->get_logger(), "Soma recebida do servidor: %ld", future.get()->sum);
      rclcpp::shutdown(); // Encerra o programa automaticamente após receber a resposta
    };

    client_->async_send_request(request, response_callback);
  }

  long int a_, b_;
  rclcpp::Client<custom_interface::srv::AddTwoInts>::SharedPtr client_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  if (argc != 3) {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Uso correto: programa X Y");
    return 1;
  }

  long int a = atoll(argv[1]);
  long int b = atoll(argv[2]);

  // Cria a classe passando 'a' e 'b' e trava no spin escutando o ROS 2
  rclcpp::spin(std::make_shared<ServicoClient>(a, b));
  
  // O rclcpp::shutdown() dentro do response_callback fará o código chegar aqui
  return 0;
}