#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"// importa o tipo de mensagem que será publicado
#include <chrono> // necessário para usar std::chrono::milliseconds
#include <memory> // necessário para usar std::shared_ptr
#include <string> // necessário para usar std::string

using namespace std::chrono_literals;// permite escrever 500ms, 1s etc. de forma literal, sem isso você teria que escrever std::chrono::milliseconds(500).
using namespace std;

class No_publisher : public rclcpp::Node
{
  public:
  No_publisher()
  :Node("no_publisher"),count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topico", 10);
    /*
    this->create_publisher<...>(); -> é um método herdado de rclcpp::Node que cria um publisher
    std_msgs::msg::String -> é o tipo de mensagem que será publicada
    "topico" -> é o nome do tópico que será publicado
    10 -> é o tamanho da fila de mensagens que serão publicadas
    */
    timer_ = this->create_wall_timer(1000ms,bind(&No_publisher::callback_timer,this));
    /*
    create_wall_timer() cria um timer que executa uma função repetidamente em intervalos fixos.
    1000ms -> é o intervalo de tempo em milissegundos entre cada execução da função
    std::bind(&MeuNo::callback_timer, this) cria um "ponteiro de função" vinculando o método callback_timer da classe No_publisher
    */
  }

  private:

  void callback_timer()
  {
    auto mensagem = std_msgs::msg::String();//cria uma instância vazia do tipo de mensagem.
    mensagem.data = "Ola, mundo! " + to_string(count_++); //monta o conteúdo da mensagem, concatenando texto fixo com um contador que é incrementado a cada chamada
    RCLCPP_INFO(this->get_logger(), "Publicando: '%s'", mensagem.data.c_str()); //imprime uma mensagem de log no terminal, usando o logger do nó.
    publisher_->publish(mensagem); //efetivamente envia a mensagem para o tópico "topico", para quem estiver inscrito nele.
  }
  rclcpp::TimerBase::SharedPtr timer_; // para o timer  
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;  // para o publisher
  size_t count_;  // para o contador

};


int main(int argc, char ** argv)
{
  rclcpp :: init(argc,argv);
  rclcpp :: spin(make_shared<No_publisher>());
  rclcpp :: shutdown();
  return 0;
}