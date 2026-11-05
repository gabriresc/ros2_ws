#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "tf2/exceptions.h"
#include "tf2/time.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

using namespace std::chrono_literals;

class PointListener : public rclcpp::Node
{
public:
    PointListener()
        : Node("point_listener")
    {
        // Primeiro cria o armazenamento das transformações.
        buffer_ = std::make_unique<tf2_ros::Buffer>(
            this->get_clock());

        // O listener recebe os TFs e alimenta esse buffer.
        listener_ = std::make_shared<tf2_ros::TransformListener>(
            *buffer_);

        timer_ = this->create_wall_timer(
            1s,
            [this]() {
                transformar_ponto();
            });
    }

private:
    void transformar_ponto()
    {
        // Um ponto fictício expresso no referencial do LiDAR.
        geometry_msgs::msg::PointStamped ponto_lidar;
        ponto_lidar.header.frame_id = "seyond_link";

        // Tempo zero: usamos a transformação mais recente disponível.
        ponto_lidar.header.stamp = rclcpp::Time(0);

        ponto_lidar.point.x = 10.0;
        ponto_lidar.point.y = 0.0;
        ponto_lidar.point.z = 0.0;

        try {
            // Atenção à ordem: DESTINO, ORIGEM, TEMPO.
            const auto transformacao = buffer_->lookupTransform(
                "base_link",
                ponto_lidar.header.frame_id,
                tf2::TimePointZero);

            geometry_msgs::msg::PointStamped ponto_chassi;

            // Aplica rotação e translação ao ponto.
            tf2::doTransform(
                ponto_lidar,
                ponto_chassi,
                transformacao);

            RCLCPP_INFO(
                this->get_logger(),
                "LiDAR: [%.2f, %.2f, %.2f] -> Chassi: [%.2f, %.2f, %.2f]",
                ponto_lidar.point.x,
                ponto_lidar.point.y,
                ponto_lidar.point.z,
                ponto_chassi.point.x,
                ponto_chassi.point.y,
                ponto_chassi.point.z);
        }
        catch (const tf2::TransformException & erro) {
            RCLCPP_WARN(
                this->get_logger(),
                "Ainda nao foi possivel transformar: %s",
                erro.what());
        }
    }

    // O buffer deve continuar existindo enquanto o listener o utiliza.
    std::unique_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<PointListener>());

    rclcpp::shutdown();
    return 0;
}