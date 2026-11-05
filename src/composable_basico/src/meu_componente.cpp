#include <chrono>
#include <cstddef>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

namespace composable_basico
{

// Continua sendo um nó ROS 2 comum, com publishers, timers e callbacks.
class MeuComponente : public rclcpp::Node
{
public:
  // O container passa estas opções ao criar o componente.
  explicit MeuComponente(const rclcpp::NodeOptions & options)
  : Node("meu_componente", options)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("mensagem", 10);
    timer_ = create_wall_timer(1s, [this]() {publicar_mensagem();});
    RCLCPP_INFO(get_logger(), "Componente iniciado!");
  }

private:
  void publicar_mensagem()
  {
    std_msgs::msg::String mensagem;
    mensagem.data = "Olá do composable node! Contador: " + std::to_string(contador_++);
    publisher_->publish(mensagem);
    RCLCPP_INFO(get_logger(), "%s", mensagem.data.c_str());
  }

  std::size_t contador_{0};
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace composable_basico

// Expõe a classe para o container. O componente não precisa de uma função main().
RCLCPP_COMPONENTS_REGISTER_NODE(composable_basico::MeuComponente)
