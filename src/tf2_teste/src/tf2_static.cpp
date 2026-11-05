#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/static_transform_broadcaster.h"

class StaticBroadcaster : public rclcpp::Node
{
public:
    StaticBroadcaster()
        : Node("static_broadcaster")
    {
        // Responsável por publicar transformações estáticas.
        this->declare_parameter<double>("x", 0.0);// declara o parametro my_parameter com valor world
        this->declare_parameter<double>("y", 0.0);// declara o parametro my_parameter com valor world
        this->declare_parameter<double>("z", 0.0);// declara o parametro my_parameter com valor world
        this->declare_parameter<double>("roll", 0.0);// declara o parametro my_parameter com valor world
        this->declare_parameter<double>("pitch", 0.0);// declara o parametro my_parameter
        this->declare_parameter<double>("yaw", 0.0);// declara o parametro my_parameter com valor world
        broadcaster_ =
        std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        publicar_transformacao();
    }

private:
    void publicar_transformacao()
    {
        geometry_msgs::msg::TransformStamped transformacao;

        // Momento da publicação.
        transformacao.header.stamp = this->now();

        // Referenciais pai e filho.
        transformacao.header.frame_id = "base_link"; //pai
        transformacao.child_frame_id = "seyond_link"; //filho

        // Posição do LiDAR no referencial do chassi, em metros.
        transformacao.transform.translation.x = this->get_parameter("x").as_double();
        transformacao.transform.translation.y = this->get_parameter("y").as_double();
        transformacao.transform.translation.z = this->get_parameter("z").as_double();

        // Converte roll, pitch e yaw (radianos) em quaternion.
        tf2::Quaternion rotacao;
        rotacao.setRPY(this->get_parameter("roll").as_double(), this->get_parameter("pitch").as_double(), this->get_parameter("yaw").as_double());

        transformacao.transform.rotation.x = rotacao.x();
        transformacao.transform.rotation.y = rotacao.y();
        transformacao.transform.rotation.z = rotacao.z();
        transformacao.transform.rotation.w = rotacao.w();

        // Envia a transformação uma vez.
        broadcaster_->sendTransform(transformacao);
    }

    // Mantém o broadcaster vivo enquanto o nó existir.
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<StaticBroadcaster>();
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}