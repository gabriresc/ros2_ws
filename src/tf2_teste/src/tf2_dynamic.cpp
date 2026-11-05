#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"



using namespace std::chrono_literals;

class DynamicBroadcaster : public rclcpp::Node
{
public:
    DynamicBroadcaster()
        : Node("dynamic_broadcaster")
    {
        // Velocidade simulada em metros por segundo.
        velocidade_ =
            this->declare_parameter<double>("velocidade", 1.0);

        broadcaster_ =
            std::make_shared<tf2_ros::TransformBroadcaster>(this);

        inicio_ = this->now();

        // Executa a cada 50 ms: aproximadamente 20 Hz.
        timer_ = this->create_wall_timer(
            50ms,
            std::bind(
                &DynamicBroadcaster::publicar_transformacao,
                this));
    }

private:
    void publicar_transformacao()
    {
        const auto agora = this->now();
        const double tempo = (agora - inicio_).seconds();

        geometry_msgs::msg::TransformStamped transformacao;

        // Timestamp atualizado a cada publicação.
        transformacao.header.stamp = agora;

        transformacao.header.frame_id = "odom";
        transformacao.child_frame_id = "base_link";

        // Movimento retilíneo: posição = velocidade × tempo.
        transformacao.transform.translation.x = velocidade_ * tempo;
        transformacao.transform.translation.y = 0.0;
        transformacao.transform.translation.z = 0.0;

        // Quaternion identidade: sem rotação.
        transformacao.transform.rotation.x = 0.0;
        transformacao.transform.rotation.y = 0.0;
        transformacao.transform.rotation.z = 0.0;
        transformacao.transform.rotation.w = 1.0;

        broadcaster_->sendTransform(transformacao);
    }

    double velocidade_;

    rclcpp::Time inicio_;

    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<DynamicBroadcaster>());

    rclcpp::shutdown();
    return 0;
}