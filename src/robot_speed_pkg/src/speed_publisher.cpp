#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include <vector>
using namespace std;
using namespace std::chrono_literals;

class SpeedPublisher :public rclcpp::Node
{
public:
    SpeedPublisher() : Node("speed_publisher")
    {
    //创建发布者
    pub_ = this->create_publisher<std_msgs::msg::Int32>("/robot_speed",10);
    //定时器
    timer_ = create_wall_timer(1s,std::bind(&SpeedPublisher::timer_callback,this));
    //速度序列
    speed_seq_ = {2,5,4,8,3,6};
    idx_ = 0;
    RCLCPP_INFO(this->get_logger(),"发布节点启动");
    }

private:
    void timer_callback()
    {
        auto msg = std_msgs::msg::Int32();
    msg.data = speed_seq_[idx_];
    pub_->publish(msg);
    RCLCPP_INFO_STREAM(this->get_logger(), "发布速度：" << msg.data);
    //下标循环
    idx_=(idx_+1)%speed_seq_.size();
    }

    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    vector<int> speed_seq_;
    int idx_;

};
int main(int argc,char** argv)
{
    rclcpp::init(argc,argv);
    auto node = make_shared<SpeedPublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}