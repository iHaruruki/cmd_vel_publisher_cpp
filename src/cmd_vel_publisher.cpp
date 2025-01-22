#include <chrono>
#include <memory>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class CmdVelPublisher : public rclcpp::Node
{
public:
  CmdVelPublisher() : Node("cmd_vel_publisher_cpp")
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    start_time_ = this->now();

    // 0.1秒ごとにタイマーコールバックを呼び出す
    timer_ = this->create_wall_timer(100ms, std::bind(&CmdVelPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    rclcpp::Time current_time = this->now();
    double elapsed = (current_time - start_time_).seconds();

    geometry_msgs::msg::Twist msg;

    // 3秒間前進
    if (elapsed < 3.0) {
      msg.linear.x = 1.0;
      msg.angular.z = 0.0;
      RCLCPP_INFO(this->get_logger(), "前進中: 速度 %.2f m/s, 経過時間: %.2f 秒", msg.linear.x, elapsed);
    }
    // 次の2秒間で左回転
    else if (elapsed < 5.0) {
      msg.linear.x = 0.0;
      msg.angular.z = 1.0;  // 左回転（角速度は正方向）
      RCLCPP_INFO(this->get_logger(), "左回転中: 角速度 %.2f rad/s, 経過時間: %.2f 秒", msg.angular.z, elapsed);
    }
    // 次の3秒間で前進
    else if (elapsed < 8.0) {
      msg.linear.x = 1.0;
      msg.angular.z = 0.0;
      RCLCPP_INFO(this->get_logger(), "前進中: 速度 %.2f m/s, 経過時間: %.2f 秒", msg.linear.x, elapsed);
    }
    // 8秒以上経過した場合は停止指令を発信し、ノードを終了
    else {
      msg.linear.x = 0.0;
      msg.angular.z = 0.0;
      publisher_->publish(msg);  // 最終的な停止コマンドを送信
      RCLCPP_INFO(this->get_logger(), "動作終了: 停止命令を送信し、shutdownします。");
      rclcpp::shutdown();
      return;
    }

    publisher_->publish(msg);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time start_time_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CmdVelPublisher>());
  rclcpp::shutdown();
  return 0;
}

