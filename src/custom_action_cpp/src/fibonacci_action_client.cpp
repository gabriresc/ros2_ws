#include <functional>
#include <future>
#include <memory>
#include <string>
#include <sstream>
#include <cstdlib>

#include "custom_action_interfaces/action/fibonacci.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using namespace std::chrono_literals;

class FibonacciActionClient : public rclcpp::Node
{
public:
  using Fibonacci = custom_action_interfaces::action::Fibonacci;
  using GoalHandleFibonacci = rclcpp_action::ClientGoalHandle<Fibonacci>;

  // O construtor agora recebe a variável 'order' lida na main
  explicit FibonacciActionClient(int order)
  : Node("fibonacci_action_client"), order_(order)
  {
    this->client_ptr_ = rclcpp_action::create_client<Fibonacci>(this, "fibonacci");
    
    // Usa um timer para iniciar o envio sem bloquear o construtor
    this->timer_ = this->create_wall_timer(
      500ms, std::bind(&FibonacciActionClient::send_goal, this));
  }

private:
  rclcpp_action::Client<Fibonacci>::SharedPtr client_ptr_;
  rclcpp::TimerBase::SharedPtr timer_;
  int order_; // Armazena o valor passado pelo terminal

  void send_goal()
  {
    this->timer_->cancel();

    if (!this->client_ptr_->wait_for_action_server()) {
      RCLCPP_ERROR(this->get_logger(), "Servidor de ação não disponível.");
      rclcpp::shutdown();
      return;
    }

    auto goal_msg = Fibonacci::Goal();
    goal_msg.order = this->order_; // Usa a variável da classe

    RCLCPP_INFO(this->get_logger(), "Enviando goal com order: %d", this->order_);

    auto send_goal_options = rclcpp_action::Client<Fibonacci>::SendGoalOptions();
    
    send_goal_options.goal_response_callback = [this](const GoalHandleFibonacci::SharedPtr & goal_handle) {
      if (!goal_handle) RCLCPP_ERROR(this->get_logger(), "Goal rejeitado pelo servidor");
      else RCLCPP_INFO(this->get_logger(), "Goal aceito, aguardando resultado...");
    };

    send_goal_options.feedback_callback = [this](
      GoalHandleFibonacci::SharedPtr, const std::shared_ptr<const Fibonacci::Feedback> feedback) {
      std::stringstream ss;
      ss << "Feedback recebido: ";
      for (auto number : feedback->partial_sequence) ss << number << " ";
      RCLCPP_INFO(this->get_logger(), ss.str().c_str());
    };

    send_goal_options.result_callback = [this](const GoalHandleFibonacci::WrappedResult & result) {
      if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
        std::stringstream ss;
        ss << "Resultado final recebido: ";
        for (auto number : result.result->sequence) ss << number << " ";
        RCLCPP_INFO(this->get_logger(), ss.str().c_str());
      } else {
        RCLCPP_ERROR(this->get_logger(), "Goal falhou ou foi cancelado.");
      }
      rclcpp::shutdown(); // Encerra o nó do cliente automaticamente
    };

    this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
  }
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  if (argc != 2) {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Uso correto: nome_do_executavel <numero_da_sequencia>");
    return 1;
  }

  int order = atoi(argv[1]);

  // Instancia a classe passando a variável lida do terminal
  rclcpp::spin(std::make_shared<FibonacciActionClient>(order));
  
  return 0;
}