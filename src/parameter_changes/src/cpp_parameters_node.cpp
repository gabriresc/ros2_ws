#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <string>
#include <functional>

using namespace std::chrono_literals;

class ParameterNode : public rclcpp::Node
{
public:

    ParameterNode() : Node("parameter_node")
    {
        
        this->declare_parameter<std::string>("my_parameter", "world");// declara o parametro my_parameter com valor world

        timer_ = this->create_wall_timer(
        1s, std::bind(&ParameterNode::check_parameter, this));// cria um timer que chama a função check_parameter a cada 1 segundo
    }
    private:
        void check_parameter()
        {
            // Get the current value of the parameter
            std::string param_value = this->get_parameter("my_parameter").as_string();// pega o valor do parametro my_parameter e armazena na variavel param_value
            RCLCPP_INFO(this->get_logger(), "Current parameter value: %s", param_value.c_str());// imprime o valor do parametro no console
            std::vector<rclcpp::Parameter> all_new_parameters{rclcpp::Parameter("my_parameter", "world")};//redefine o valor do parametro my_parameter para world e coloca dentro de uma lista de parametros
            this->set_parameters(all_new_parameters);// redefine o valor do parametro my_parameter para world
        }
        rclcpp::TimerBase::SharedPtr timer_;// ponteiro para o timer que chama a função check_parameter a cada 1 segundo
    };

int main(int argc, char **argv)
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<ParameterNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}