#include "rclcpp/rclcpp.hpp" 
#include "std_msgs/msg/string.hpp" // importa o tipo de mensagem que será publicado
#include <memory> // necessário para usar std::shared_ptr
#include <string> // necessário para usar std::string

using namespace std;

class No_subscriber : public rclcpp::Node
{
  public:
  No_subscriber()
  :Node("no_subscriber")
  {
    subscription_ = this->create_subscription<std_msgs::msg::String>("topico",10,bind(&No_subscriber::callback_subscriber,this,std::placeholders::_1));
    /*
    this->create_subscription<...>() é o método (herdado de rclcpp::Node) que cria uma subscription — ou seja, inscreve o nó em um tópico para receber mensagens.
    std_msgs::msg::String — o tipo de mensagem que ele espera receber. Precisa ser o mesmo tipo usado pelo publisher, senão eles não conseguem se comunicar.
    "topico" — o nome do tópico que ele vai "escutar". Também precisa ser exatamente igual ao nome usado no create_publisher() do outro nó 
    10 — o tamanho da fila (QoS depth), igual explicamos no publisher.
    bind(&No_subscriber::callback_subscriber, this, std::placeholders::_1) 
      ·bind para vincular o método callback_subscriber à instância atual (this).
      ·std::placeholders::_1 é um "espaço reservado" indicando que essa função vai receber um argumento (a mensagem recebida), que será preenchido automaticamente pelo ROS 2 quando a mensagem chegar.
    */
  }
  private:
  void callback_subscriber(const std_msgs::msg::String::SharedPtr msg) const //const std_msgs::msg::String::SharedPtr msg — o parâmetro é a mensagem recebida
  {
    RCLCPP_INFO(this->get_logger(), "Recebido: '%s'", msg->data.c_str());
    /*
      msg->data.c_str() — como msg é um ponteiro, usamos -> (em vez de .) para acessar o campo data (o texto da mensagem).
     .c_str() converte a std::string para const char*, necessário pro %s do RCLCPP_INFO.
    */
  }
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_; //para o subscription
};

int main(int argc, char ** argv)
{
  rclcpp :: init(argc,argv);
  rclcpp :: spin(make_shared<No_subscriber>());
  rclcpp :: shutdown();
  return 0;
}