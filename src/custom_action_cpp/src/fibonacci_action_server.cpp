#include <functional>
#include <memory>
#include <thread>

#include "custom_action_interfaces/action/fibonacci.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

class FibonacciActionServer : public rclcpp::Node
{
public:
  using Fibonacci = custom_action_interfaces::action::Fibonacci;
  using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;

  // Construtor limpo, sem NodeOptions ou macros
  FibonacciActionServer() : Node("fibonacci_action_server")
  {
    using namespace std::placeholders;

    auto handle_goal = [this](
      const rclcpp_action::GoalUUID & uuid,
      std::shared_ptr<const Fibonacci::Goal> goal)
    {
      RCLCPP_INFO(this->get_logger(), "Recebido pedido de goal com order %d", goal->order);
      (void)uuid;
      return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    };

    auto handle_cancel = [this](
      const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
      RCLCPP_INFO(this->get_logger(), "Recebido pedido para cancelar o goal");
      (void)goal_handle;
      return rclcpp_action::CancelResponse::ACCEPT;
    };

    auto handle_accepted = [this](
      const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
      // Inicia a execução em uma nova thread para não travar o callback
      auto execute_in_thread = [this, goal_handle](){ return this->execute(goal_handle); };
      std::thread{execute_in_thread}.detach();
    };

    this->action_server_ = rclcpp_action::create_server<Fibonacci>(
      this, "fibonacci", handle_goal, handle_cancel, handle_accepted);
      
    RCLCPP_INFO(this->get_logger(), "Action Server pronto.");
  }

private:
  rclcpp_action::Server<Fibonacci>::SharedPtr action_server_;

  void execute(const std::shared_ptr<GoalHandleFibonacci> goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Executando goal...");
    rclcpp::Rate loop_rate(1);
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<Fibonacci::Feedback>();
    auto & sequence = feedback->partial_sequence;
    sequence.push_back(0);
    sequence.push_back(1);
    auto result = std::make_shared<Fibonacci::Result>();

    for (int i = 1; (i < goal->order) && rclcpp::ok(); ++i) {
      if (goal_handle->is_canceling()) {
        result->sequence = sequence;
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "Goal cancelado");
        return;
      }
      sequence.push_back(sequence[i] + sequence[i - 1]);
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "Publicando feedback");
      loop_rate.sleep();
    }

    if (rclcpp::ok()) {
      result->sequence = sequence;
      goal_handle->succeed(result);
      RCLCPP_INFO(this->get_logger(), "Goal concluído com sucesso");
    }
  }
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FibonacciActionServer>());
  rclcpp::shutdown();
  return 0;
}