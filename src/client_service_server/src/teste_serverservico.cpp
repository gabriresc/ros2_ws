#include "rclcpp/rclcpp.hpp"
#include "custom_interface/srv/add_two_ints.hpp" //interface

#include <memory>

void add(const std::shared_ptr<custom_interface::srv::AddTwoInts::Request> request,
          std::shared_ptr<custom_interface::srv::AddTwoInts::Response>      response)
    /*
        void add -> uma funcao que escreve o resuldado dentro de response
        const std::shared_ptr<...> request -> o pedido do cliente
            ·<...> vem da interface que estamos utilizando, tudo que vem antes das ---, no caso a e b
            ·std::shared_ptr -> é o ponteiro inteligente(gerencia a memoria)
            ·const quer dizer que não modificar o valor do resquest
        std::shared_ptr<...> response -> mensagem de resposta ao cliente
            · o que vem depois das ---, no caso o num
    */
{
  response->sum = request->a + request->b; // a logica de servico, soma a e b
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\na: %ld" " b: %ld",
                request->a, request->b); // exibir no terminal o que esta recebendo 
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending back response: [%ld]", (long int)response->sum); // exibir o valor de resposta
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);//inicia o ros2

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("exemplo_servico_server");//instancia um node

  rclcpp::Service<custom_interface::srv::AddTwoInts>::SharedPtr service =
    node->create_service<custom_interface::srv::AddTwoInts>("exemplo_servico", &add);//cria um server para o servico exemplo_servico
    /*
        node->create_service<...>(...) — método do nó que registra um serviço
        <custom_interface::srv::AddTwoInts> — o tipo do serviço (template)
        "exemplo_servico" — o nome do serviço
        &add — ponteiro para a função add
    */

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to add two ints.");//log informativo

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}